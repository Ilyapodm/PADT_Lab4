#pragma once

#include <stdexcept>

#include "node.hpp"
#include "core/ordinal.hpp"

template <typename T>
class SetNode : public Node<T> {
public:
    SetNode(const Node<T>& source, const T& value, const Ordinal& index) :
        source_(nullptr),
        value_(value),
        index_(index)
    {
        if (index >= source.length())
            throw std::out_of_range("SetNode index out of range");
        
        source_ = source.clone();
    }

    SetNode(const SetNode& other) : SetNode(*other.source_, other.value_, other.index_) {}

    SetNode& operator=(const SetNode& other) {
        if (this == &other)
            return *this;

        Node<T>* new_source = other.source_->clone();
        delete source_;

        source_ = new_source;
        value_ = other.value_;
        index_ = other.index_;

        return *this;
    }

    ~SetNode() override {
        delete source_;
    }

    const Ordinal& length() const override {
        return source_->length();
    }

    // set value is not a cache actually!!
    std::size_t materialized_count() const override {
        return source_->materialized_count();
    }

    SetNode<T>* clone() const override {
        return new SetNode<T>(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) const override {
        if (index == index_) 
            return value_;

        return source_->value_at(index);
    }

private:
    Node<T>* source_;
    T value_;
    Ordinal index_;
};

