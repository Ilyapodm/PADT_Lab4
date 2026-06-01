#pragma once

#include <stdexcept>

#include "node.hpp"
#include "ordinal.hpp"

template <typename T>
class RemoveAtNode : public Node<T> {
public:
    RemoveAtNode(const Node<T>& source, const Ordinal& index) : 
        source_(nullptr),
        index_(index)
    {
        if (index >= source.length())
            throw std::out_of_range("RemoveAtNode index out of range");

        source_ = source.clone();

        // if remove from the last finite part, decrease the length
        if (index_.get_omega_coeff() == source_->length().get_omega_coeff())
            length_ = Ordinal(source_->length().get_omega_coeff(), source_->length().get_finite_part() - 1);  
        else
            length_ = source_->length();
    }

    RemoveAtNode(const RemoveAtNode& other) : RemoveAtNode(*other.source_, other.index_) {}

    RemoveAtNode& operator=(const RemoveAtNode& other) {
        if (this == &other)
            return *this;

        Node<T>* new_source = other.source_->clone();

        delete source_;

        source_ = new_source;
        index_ = other.index_;
        length_ = other.length_;

        return *this;
    }

    ~RemoveAtNode() override {
        delete source_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    RemoveAtNode* clone() const override {
        return new RemoveAtNode(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) override {
        if (index < index_)
            return source_->value_at(index);

        if (index.get_omega_coeff() == index_.get_omega_coeff()) {
            Ordinal source_index(index.get_omega_coeff(), index.get_finite_part() + 1);
            return source_->value_at(source_index);
        }

        return source_->value_at(index);  // index's element is in the next infinite part, no correction
    }

private:
    Node<T>* source_;
    Ordinal index_;
    Ordinal length_;
};