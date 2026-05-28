#pragma once

#include <stdexcept>

#include "lab_2/option.hpp"
#include "ordinal.hpp"


template <class T>
class Generator {
public:
    virtual ~Generator() = default;
    
    virtual bool has_next() const = 0;
    virtual T get_next() = 0;

    virtual Ordinal length() const = 0;

    // habe to redefine ti in child classes
    virtual bool supports_get_at() const {
        return false;  
    }

    // general logic 'get_at' is set here, in parent class
    virtual T get_at(const Ordinal& index) {
        if (!supports_get_at()) {
            throw std::logic_error("Generator does not support ordinal indexed access");
        }

        if (!(index < length())) {
            throw std::out_of_range("Ordinal index out of range");
        }

        return get_at_impl(index);  // have to redefine it in child classes
    }

    virtual Option<T> try_get_next() {
        if (!has_next())
            return Option<T>();

        return Option<T>(get_next());
    }

    virtual Generator<T>* clone() const = 0;

protected:
    virtual T get_at_impl(const Ordinal& index) {
        throw std::logic_error("get_at_impl is not implemented");
    }
};