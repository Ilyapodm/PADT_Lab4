#pragma once

#include <stdexcept>
#include <cstddef>
#include <functional>

#include "generator.hpp"
#include "ordinal.hpp"

// creation the seq from scratch; get the value from index
template <typename T>
class FunctionGenerator : public Generator<T> {
public:
    FunctionGenerator(std::function<T(std::size_t)> func, Ordinal length) : length_{length}, func_{func} {
        if (!func_)
            throw std::invalid_argument("FunctionGenerator's function is null");
        if (length.get_omega_coeff() > 1 || (length.get_omega_coeff() == 1 && length.get_finite_part() != 0)) {
            throw std::invalid_argument("FunctionGenerator length must be finite or omega");
        }
    } 

    bool has_next() const override {
        if (length_.get_omega_coeff() > 0)
            return true;
        return index_ < length_.get_finite_part();
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("Generator is exhausted");
        }

        return func_(index_++);
    }

    Ordinal length() const override {
        return length_;
    }

    FunctionGenerator<T>* clone() const override {
        return new FunctionGenerator<T>(*this);
    }

private:
    std::size_t index_ = 0;
    Ordinal length_;
    std::function<T(std::size_t)> func_;
};