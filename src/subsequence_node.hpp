#pragma once

#include <stdexcept>

#include "node.hpp"
#include "ordinal.hpp"

template <typename T>
class SubsequenceNode : public Node<T> {
public:
    SubsequenceNode(const Node<T>& source, const Ordinal& start, const Ordinal& end_exclusive) : 
        source_(nullptr),
        start_(start),
        end_exclusive_(end_exclusive)
    {
        if (start > end_exclusive || end_exclusive > source.length())
            throw std::invalid_argument("start index is bigger than end index");

        if (end_exclusive > source.length())
            throw std::invalid_argument("end index is biggeer than the length of the source");
    

    length_ = end_exclusive.subtract_prefix(start);
    source_ = source.clone();
    }

    SubsequenceNode(const SubsequenceNode& other) : SubsequenceNode(*other.source_, other.start_, other.end_exclusive_) {}

    SubsequenceNode& operator=(const SubsequenceNode& other) {
        if (this == &other) 
            return *this;

        Node<T>* new_source = other.source_->clone();
        delete source_;

        source_ = new_source;
        start_ = other.start_;
        end_exclusive_ = other.end_exclusive_;
        length_ = other.length_;

        return *this;
    }

    ~SubsequenceNode() override {
        delete source_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    SubsequenceNode<T>* clone() const override {
        return new SubsequenceNode<T>(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& local_index) override {
        Ordinal source_index = start_ + local_index;
        return source_->value_at(source_index);
    }

private:
    Node<T>* source_;
    Ordinal start_;
    Ordinal end_exclusive_;  // do not include the end index: [0, omega) instead of [0, omega]
    Ordinal length_;
};