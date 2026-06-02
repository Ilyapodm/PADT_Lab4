#pragma once

#include "node.hpp"
#include "core/ordinal.hpp"

template <typename T>
class AppendNode : public Node<T> {
public:
    AppendNode(const Node<T>& source, const T& value) : 
        source_(source.clone()),
        value_(value),
        length_(source.length() + Ordinal(1))
    {}

    AppendNode(const AppendNode& other) : AppendNode(*other.source_, other.value_) {}

    AppendNode& operator=(const AppendNode& other) {
        if (this == &other)
            return *this;

        Node<T>* new_source = other.source_->clone();
        delete source_;

        source_ = new_source;
        value_ = other.value_;
        length_ = other.length_;

        return *this;
    }

    ~AppendNode() override {
        delete source_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    // appended value is not a cache actually!!
    std::size_t materialized_count() const override {
        return source_->materialized_count();
    }

    AppendNode* clone() const override {
        return new AppendNode(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) override {
        if (index == source_->length())
            return value_;
        return source_->value_at(index);
    }

private:
    Node<T>* source_;
    T value_;
    Ordinal length_;
};