#pragma once

#include <cstddef>
#include "stdexcept"

#include "adt/ring_buffer.hpp"


// functions from index
inline int square_function(std::size_t index) {
    return static_cast<int>(index * index);
}

inline int identity_function(std::size_t index) {
    return static_cast<int>(index);
}

inline int multiply_by_10_function(std::size_t index) {
    return static_cast<int>(index * 10);
}

inline int plus_10_function(std::size_t index) {
    return static_cast<int>(index + 10);
}

inline int plus_50_function(std::size_t index) {
    return static_cast<int>(index + 50);
}

inline int plus_100_function(std::size_t index) {
    return static_cast<int>(index + 100);
}

inline int plus_1000_function(std::size_t index) {
    return static_cast<int>(index + 1000);
}

inline int linear_function(std::size_t index) {
    return static_cast<int>(100 + index * 10);
}

inline int odd_linear_function(std::size_t index) {
    return static_cast<int>(2 * index + 1);
}

inline int constant_seven_function(std::size_t index) {
    static_cast<void>(index);
    return 7;
}

// recurrence RingBuffer functions
inline int fibonacci_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) + window.get(1);
}

inline int increment_by_one_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) + 1;
}

inline int double_previous_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) * 2;
}


inline int tribonacci_ring_Rule(const RingBuffer<int>& window) {
    return window.get(0) + window.get(1) + window.get(2);
}

inline int zero_ring_rule(const RingBuffer<int>&) {
    return 0;
}

// recurrence array functions
inline int fibonacci_array_rule(const int* window, std::size_t count) {
    if (count != 2) {
        throw std::invalid_argument("fibonacci_array_rule: count must be 2");
    }

    return window[0] + window[1];
}

inline int tribonacci_array_rule(const int* window, std::size_t count) {
    if (count != 3) {
        throw std::invalid_argument("tribonacci_array_rule: count must be 3");
    }

    return window[0] + window[1] + window[2];
}

inline int zero_array_rule(const int*, std::size_t) {
    return 0;
}