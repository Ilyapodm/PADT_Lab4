#pragma once

#include "node.hpp"
#include "core/ordinal.hpp"

template <typename T>
class PrependNode : public Node<T> {
public:
    PrependNode(const Node<T>& source, const T& value) : 
        source_(source.clone()),
        value_(value),
        length_(source.length())
    {
        if (length_.is_finite())
            length_ = length_ + Ordinal(1);
    }

    PrependNode(const PrependNode& other) : PrependNode(*other.source_, other.value_) {}

    PrependNode& operator=(const PrependNode& other) {
        if (this == &other)
            return *this;

        Node<T>* new_source = other.source_->clone();
        delete source_;

        source_ = new_source;
        value_ = other.value_;
        length_ = other.length_;

        return *this;
    }

    ~PrependNode() override {
        delete source_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    // prepended value is not a cache actually!!
    std::size_t materialized_count() const override {
        return source_->materialized_count();
    }

    PrependNode* clone() const override {
        return new PrependNode(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) const override {
        if (index == Ordinal()) 
            return value_;
        if (index.is_finite()) 
            return source_->value_at(Ordinal(index.get_finite_part() - 1));
        return source_->value_at(index);
    }

private:
    Node<T>* source_;
    T value_;
    Ordinal length_;
};