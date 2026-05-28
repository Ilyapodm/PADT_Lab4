#pragma once

#include <stdexcept>

#include "generator.hpp"
#include "ordinal.hpp"


template <typename T>
class EmptyGenerator : public Generator<T> {
public:
    virtual ~EmptyGenerator() = default;

    bool has_next() const override { 
        return false;
    }

    T get_next() override {
        throw std::logic_error("There is no next element in EmptyGenerator"); 
    }

    Ordinal length() const override {
        return Ordinal(); 
    }

    // if fact, it support 'get_at', but there is just no any valid indexes
    bool supports_get_at() const override {
        return true;
    }

    // covariant return type
    EmptyGenerator<T>* clone() const override {
        return new EmptyGenerator<T>(*this);
    }
};