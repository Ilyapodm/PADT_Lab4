#pragma once

#include <stdexcept>
#include <cstddef>
#include <functional>

#include "generator.hpp"
#include "core/ordinal.hpp"
#include "adt/ring_buffer.hpp"

// TODO доделать до конца эту хуйню
// Запушить, доделать тесты с рек геном для лэйзи
// Быстренько потоки бам бум
// быстро свой генератор

template <typename T>
class RecurrenceGenerator : public Generator<T> {
private:
    enum class RuleMode {
        Ring,
        Array
    };

public:
    RecurrenceGenerator(
        std::function<T(const RingBuffer<T>&)> func,
        const T* initial_values,
        std::size_t count,
        Ordinal length
    ) : 
        ring_rule_(func),
        ring_window_(validate_initial_values_(initial_values, count), count),
        window_size_(count),
        length_(length),
        rule_mode_(RuleMode::Ring)
    {
        if (!ring_rule_) {
            throw std::invalid_argument(
                "RecurrenceGenerator<T>::RecurrenceGenerator: function is null"
            );
        }

        validate_length_(length_);
    }

    // NOTE: here we copy all the elements from RingBuffer, instead this is possible to implement RingBuffer's logic here,
    // it would be the fastest way, but in order to devide the logic, I don't follow this path
    RecurrenceGenerator(
        std::function<T(const T*, std::size_t)> func,
        const T* initial_values,
        std::size_t count,
        Ordinal length
    ) : 
        array_rule_(func),
        ring_window_(validate_initial_values_(initial_values, count), count),
        window_size_(count),
        length_(length),
        rule_mode_(RuleMode::Array)
    {
        if (!array_rule_) 
            throw std::invalid_argument("RecurrenceGenerator<T>::RecurrenceGenerator: function is null");
        

        validate_length_(length_);

        // Scratch-buffer. It does not need to contain initial values,
        // because before every array-rule call we copy ring_window_ into it.
        array_window_ = new T[window_size_];
    }

    RecurrenceGenerator(const RecurrenceGenerator& other) : 
        ring_rule_(other.ring_rule_),
        array_rule_(other.array_rule_),
        ring_window_(other.ring_window_),
        array_window_(nullptr),
        window_size_(other.window_size_),
        length_(other.length_),
        index_(other.index_),
        rule_mode_(other.rule_mode_)
    {
        if (other.rule_mode_ == RuleMode::Array) 
            array_window_ = new T[window_size_];
        
    }

    RecurrenceGenerator& operator=(const RecurrenceGenerator& other) {
        if (this == &other) 
            return *this;
        
        T* new_array_window = nullptr;

        try {
            if (other.rule_mode_ == RuleMode::Array) 
                new_array_window = new T[other.window_size_];
            
            // prepare copies first
            // if something throws here, the current object remains unchanged
            RingBuffer<T> new_ring_window(other.ring_window_);
            std::function<T(const RingBuffer<T>&)> new_ring_rule(other.ring_rule_);
            std::function<T(const T*, std::size_t)> new_array_rule(other.array_rule_);

            ring_window_ = new_ring_window;
            ring_rule_ = new_ring_rule;
            array_rule_ = new_array_rule;

            delete[] array_window_;
            array_window_ = new_array_window;
            new_array_window = nullptr;

            window_size_ = other.window_size_;
            length_ = other.length_;
            index_ = other.index_;
            rule_mode_ = other.rule_mode_;
        }
        catch (...) {
            delete[] new_array_window;
            throw;
        }

        return *this;
    }

    ~RecurrenceGenerator() override {
        delete[] array_window_;
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
        if (index_ < window_size_)
            return ring_window_.get(index_++);

        T result = compute_next_value_();
        
        ring_window_.push(result);
        ++index_;
        
        return result;
    }

    Ordinal length() const override {
        return length_;
    }

    RecurrenceGenerator<T>* clone() const override {
        return new RecurrenceGenerator<T>(*this);
    }

private:
    std::function<T(const RingBuffer<T>&)> ring_rule_;  
    std::function<T(const T*, std::size_t)> array_rule_; 

    RingBuffer<T> ring_window_;  // for const RingBuffer<T>& functions - the fastest way 
    T* array_window_ = nullptr;  // for const T* functions

    std::size_t window_size_;  // size of window
    
    Ordinal length_;
    
    std::size_t index_ = 0;

    RuleMode rule_mode_;

    T compute_next_value_() {
        if (rule_mode_ == RuleMode::Array) {
            if (array_window_ == nullptr) 
                throw std::logic_error("RecurrenceGenerator<T>::compute_next_value_: array window is null");
            

            if (!array_rule_) 
                throw std::logic_error("RecurrenceGenerator<T>::compute_next_value_: array rule is null");
            

            ring_window_.copy_to(array_window_);
            return array_rule_(array_window_, window_size_);
        }

        if (!ring_rule_) 
            throw std::logic_error("RecurrenceGenerator<T>::compute_next_value_: ring rule is null");
        

        return ring_rule_(ring_window_);
    }

    static const T* validate_initial_values_(const T* initial_values, std::size_t count) {
        if (count == 0) 
            throw std::invalid_argument("RecurrenceGenerator<T>::RecurrenceGenerator: initial values' count can't be 0");
        

        if (initial_values == nullptr) 
            throw std::invalid_argument("RecurrenceGenerator<T>::RecurrenceGenerator: initial values is nullptr");
        

        return initial_values;
    }

    static void validate_length_(const Ordinal& length) {
        if (length.get_omega_coeff() > 1 || (length.get_omega_coeff() == 1 && length.get_finite_part() != 0)) 
            throw std::invalid_argument("RecurrenceGenerator<T>::RecurrenceGenerator: length must be finite or omega");
    }
};