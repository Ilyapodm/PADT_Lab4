#pragma once

#include "core/ordinal.hpp"

// Node, roughly speaking, is Generator + Cache + index' logic
template <typename T>
class Node {
public:
    virtual ~Node() = default;

    virtual const Ordinal& length() const = 0;

    // the number of values stored in caches inside a given Node tree
    virtual std::size_t materialized_count() const = 0;  

    // "Public interface with all checks is not virtual, the realization is specific and thus virtual"
    const T& value_at(const Ordinal& index) const {
        if (index >= length()) 
            throw std::out_of_range("Node index out of range");
        
        return value_at_impl(index);
    }

    virtual Node<T>* clone() const = 0;

protected:
    virtual const T& value_at_impl(const Ordinal& index) const  = 0;
};

