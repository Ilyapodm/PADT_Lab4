#pragma once

#include <cstddef>
#include <stdexcept>

#include "node.hpp"
#include "ordinal.hpp"
#include "generator.hpp"
#include "lab_2/array_sequence.hpp"

template <typename T>
class SourceNode : public Node<T> {
public:
    SourceNode(const Generator<T>& generator) : 
    generator_(generator.clone()), 
    cache_(), 
    length_(generator.length()) 
    {}

    SourceNode(const SourceNode<T>& other) : 
    generator_(other.generator_->clone()), 
    cache_(other.cache_), 
    length_(other.length_) {}

    SourceNode<T>& operator=(const SourceNode<T>& other) {
        if (this == &other) {
            return *this;
        }

        Generator<T>* new_generator = other.generator_->clone();

        delete generator_;

        generator_ = new_generator;
        cache_ = other.cache_;
        length_ = other.length_;

        return *this;
    }

    ~SourceNode() override {
        delete generator_;
    }

    const Ordinal& length() const override {
        return length_;
    }

    SourceNode<T>* clone() const override {
        return new SourceNode<T>(*this);
    }

protected:
    const T& value_at_impl(const Ordinal& index) override {
        if (!index.is_finite()) {
            throw std::logic_error("SourceNode cannot materialize transfinite local index");
        }

        std::size_t finite_index = index.get_finite_part();

        while (cache_.get_size() <= finite_index) {
            if (!generator_->has_next()) {
                throw std::out_of_range("Generator is exhausted");
            }

            cache_.append(generator_->get_next());
        }

        return cache_[finite_index];
    }

private:
    // use pointer because of the polymorphism (don't know the exact type of Gen)
    Generator<T>* generator_;  // generator_'s state is right after the last element in cache_
    MutableArraySequence<T> cache_;  // use as field, less pain with memory
    Ordinal length_;
};