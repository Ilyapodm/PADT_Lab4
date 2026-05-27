#pragma once
#include <stdexcept>

// infinite_count * omega + value
struct Ordinal {
    bool is_infinite;
    size_t infinite_count;
    size_t value;        

    explicit Ordinal(size_t val) : is_infinite(false), infinite_count(0), value(val) {}
    Ordinal() : is_infinite(true), infinite_count(1), value(0) {} 
    Ordinal(size_t infinite_count, size_t val) : is_infinite(infinite_count > 0), infinite_count(infinite_count), value(val) {}

    static Ordinal infinity() {
        return {1, 0};
    }

    bool operator==(const Ordinal& other) const {
        return infinite_count == other.infinite_count && value == other.value;
    }

    bool operator!=(const Ordinal& other) const {
        return !(*this == other);
    }

    bool operator<(const Ordinal& other) const {
        if (infinite_count < other.infinite_count) 
            return true;
        if (infinite_count > other.infinite_count) 
            return false;
        
        return value < other.value;
    }

    Ordinal operator+(const Ordinal& other) const {
        // omega * c1 + v1 + omega * c2 + v2 = omega * (c1 + c2) + v2
        if (other.is_infinite) {
            return {infinite_count + other.infinite_count, other.value};
        }
        return {infinite_count, value + other.value};
    }

    Ordinal operator-(const Ordinal& other) const {
        if (other.infinite_count > infinite_count) {
            return {0, 0};
        }
        
        if (other.infinite_count == infinite_count) {
            if (is_infinite) {
                throw std::invalid_argument("Infinity minus infinity is undefined");
            }
            if (value > other.value) {
                return {0, value - other.value};
            }

            return {0, 0};
        }

        return {infinite_count - other.infinite_count, value};
    }
};