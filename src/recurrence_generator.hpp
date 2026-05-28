#pragma once

#include <stdexcept>
#include <cstddef>
#include <functional>

#include "generator.hpp"
#include "ordinal.hpp"
#include "ring_buffer.hpp"

template <typename T>
class RecurrenceGenerator : public Generator<T> {
public:
    RecurrenceGenerator(std::function<T(const RingBuffer<T>&)> func, const T* initial_values, std::size_t count,  Ordinal length) : 
        window_(initial_values, count), 
        length_(length), 
        rule_(func), 
        count_(count)
    {
        if (!rule_)
            throw std::invalid_argument("RecurrenceGenerator's function is null");
        if (length.get_omega_coeff() > 1 || (length.get_omega_coeff() == 1 && length.get_finite_part() != 0)) 
            throw std::invalid_argument("RecurrenceGenerator length must be finite or omega");
        if (count == 0) 
            throw std::invalid_argument("RecurrenceGenerator: initial values' count can't be 0");
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

        // need to return the beginning (initial values) of the seq
        if (index_ < count_)
            return window_.get(index_++);

        T result = rule_(window_);
        window_.push(result);
        ++index_;
        
        return result;
    }

    Ordinal length() const override {
        return length_;
    }

    // do not support 'get_at', it's written in Generator

    RecurrenceGenerator<T>* clone() const override {
        return new RecurrenceGenerator<T>(*this);
    }

private:
    RingBuffer<T> window_;
    Ordinal length_;
    // function have to work with buffer's 'get' or we have to do array appends, removes or copies of buffer into T*
    std::function<T(const RingBuffer<T>&)> rule_;  
    std::size_t count_;
    std::size_t index_ = 0;
};