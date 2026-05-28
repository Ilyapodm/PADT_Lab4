#pragma once

#include <stdexcept>

#include "generator.hpp"
#include "ordinal.hpp"

template <typename T>
class SingletonGenerator : public Generator<T> {
public:

    explicit SingletonGenerator(const T& value) : value_(value) {}
    
    bool has_next() const override {
        return !is_exhausted_;
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("Generator is exhausted");
        }

        is_exhausted_ = true;
        return value_;
    }

    Ordinal length() const override {
        return Ordinal(1);
    }

    bool supports_get_at() const override {
        return true;
    }

    SingletonGenerator<T>* clone() const override {
        return new SingletonGenerator<T>(*this);
    }
protected:
    T get_at_impl(const Ordinal&) override {
        return value_;
    }
private:
    bool is_exhausted_ = false;
    T value_;
};