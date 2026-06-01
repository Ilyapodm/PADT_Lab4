#pragma once

#include "node.hpp"
#include "core/ordinal.hpp"

template <typename T>
class ConcatNode : public Node<T> {
public:
    ConcatNode(const Node<T>& left, const Node<T>& right) : 
        left_(nullptr),
        right_(nullptr),
        length_(left.length() + right.length())
    {
        // here prevent situation, when 'left.clone()' is Ok, but 'right.clone()' falls
        Node<T>* new_left = left.clone();
        Node<T>* new_right = nullptr;

        try {
            new_right = right.clone();
        } catch (...) {
            delete new_left;
            throw;
        }

        left_ = new_left;
        right_ = new_right;
    }

    ConcatNode(const ConcatNode<T>& other) : ConcatNode(*other.left_, *other.right_) {}

    ConcatNode<T>& operator=(const ConcatNode<T>& other) {
        if (this == &other) 
            return *this;
        
        Node<T>* new_left = other.left_->clone();
        Node<T>* new_right = nullptr;  // clangd don't like without it

        try {
            new_right = other.right_->clone();
        } catch (...) {
            delete new_left;
            throw;
        }

        delete left_;
        delete right_;

        left_ = new_left;
        right_ = new_right;
        length_ = other.length_;

        return *this;
    }

    ~ConcatNode() override {
        delete left_;
        delete right_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    ConcatNode<T>* clone() const override {
        return new ConcatNode<T>(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) override {
        const Ordinal& left_length = left_->length();
        if (index < left_length) 
            return left_->value_at(index);
        
        Ordinal local_index = index.subtract_prefix(left_length);
        return right_->value_at(local_index);
    }


private:
    // use pointers because Node is polymorphic;
    Node<T>* left_;
    Node<T>* right_;
    Ordinal length_;
};