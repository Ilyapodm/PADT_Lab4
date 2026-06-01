#pragma once

#include <stdexcept>

#include "generator.hpp"
#include "core/ordinal.hpp"


template <typename T>
class EmptyGenerator : public Generator<T> {
public:
    bool has_next() const override { 
        return false;
    }

    T get_next() override {
        throw std::logic_error("There is no next element in EmptyGenerator"); 
    }

    Ordinal length() const override {
        return Ordinal(); 
    }

    // covariant return type
    EmptyGenerator<T>* clone() const override {
        return new EmptyGenerator<T>(*this);
    }
};