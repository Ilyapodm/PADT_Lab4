#pragma once

#include "ordinal.hpp"

// Node, roughly speaking, is Generator + Cache + index' logic
template <typename T>
class Node {
public:
    virtual ~Node() = default;

    virtual const Ordinal& length() const = 0;

    // "Public interface with all checks is not virtual, the realization is specific and thus virtual"
    const T& value_at(const Ordinal& index) {
        if (index >= length()) 
            throw std::out_of_range("Node index out of range");
        
        return value_at_impl(index);
    }

    virtual Node<T>* clone() const = 0;

protected:
    virtual const T& value_at_impl(const Ordinal& index) = 0;
};

