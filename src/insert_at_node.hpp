#pragma once

#include <stdexcept>

#include "node.hpp"
#include "ordinal.hpp"

template <typename T>
class InsertAtNode : public Node<T> {
public:
    InsertAtNode(const Node<T>& source, const T& value, const Ordinal& index) : 
        source_(nullptr),
        value_(value),
        index_(index)
    {
        if (index > source.length())
            throw std::out_of_range("InsertAtNode index out of range");

        source_ = source.clone();

        if (index_.get_omega_coeff() == source_->length().get_omega_coeff())
            length_ = Ordinal(source_->length().get_omega_coeff(), source_->length().get_finite_part() + 1);  // just increase
        else
            length_ = source_->length();
    }

    InsertAtNode(const InsertAtNode& other) : InsertAtNode(*other.source_, other.value_, other.index_) {}

    InsertAtNode& operator=(const InsertAtNode& other) {
        if (this == &other)
            return *this;

        Node<T>* new_source = other.source_->clone();

        delete source_;

        source_ = new_source;
        value_ = other.value_;
        index_ = other.index_;
        length_ = other.length_;

        return *this;
    }

    ~InsertAtNode() override {
        delete source_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    InsertAtNode<T>* clone() const override {
        return new InsertAtNode<T>(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) override {
        if (index < index_)
            return source_->value_at(index);

        if (index == index_)
            return value_;

        if (index.get_omega_coeff() == index_.get_omega_coeff()) {
            Ordinal source_index(index.get_omega_coeff(), index.get_finite_part() - 1);
            return source_->value_at(source_index);
        }

        return source_->value_at(index);  // index's element is in the next infinite part, no correction
    }

private:
    Node<T>* source_;
    T value_;
    Ordinal index_;
    Ordinal length_;
};