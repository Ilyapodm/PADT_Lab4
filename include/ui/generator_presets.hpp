#pragma once

#include <cstddef>

#include "adt/ring_buffer.hpp"

// function presets for FunctionGenerator-based LazySequence creation

inline int ui_identity_function(std::size_t index) {
    return static_cast<int>(index);
}

inline int ui_square_function(std::size_t index) {
    return static_cast<int>(index * index);
}

inline int ui_odd_linear_function(std::size_t index) {
    return static_cast<int>(2 * index + 1);
}

inline int ui_constant_seven_function(std::size_t index) {
    static_cast<void>(index);
    return 7;
}

// recurrence presets for RecurrenceGenerator-based LazySequence creation

inline int ui_fibonacci_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) + window.get(1);
}

inline int ui_increment_by_one_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) + 1;
}

inline int ui_double_previous_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) * 2;
}
