#pragma once

#include <stdexcept>

#include "generator.hpp"
#include "core/ordinal.hpp"

// single element generator
// NOTE this generator is not used in this architecture, it's here 'just in case'
template <typename T>
class SingleElementGenerator : public Generator<T> {
public:

    explicit SingleElementGenerator(const T& value) : value_(value) {}
    
    bool has_next() const override {
        return !is_exhausted_;
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("SingleElementGenerator<T>::get_next: Generator is exhausted");
        }

        is_exhausted_ = true;
        return value_;
    }

    Ordinal length() const override {
        return Ordinal(1);
    }

    SingleElementGenerator<T>* clone() const override {
        return new SingleElementGenerator<T>(*this);
    }

private:
    bool is_exhausted_ = false;
    T value_;
};