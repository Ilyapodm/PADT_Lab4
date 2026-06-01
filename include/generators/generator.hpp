#pragma once

#include "adt_lab_2/option.hpp"
#include "core/ordinal.hpp"


template <class T>
class Generator {
public:
    virtual ~Generator() = default;
    
    virtual bool has_next() const = 0;
    virtual T get_next() = 0;

    virtual Ordinal length() const = 0;

    virtual Option<T> try_get_next() {
        if (!has_next())
            return Option<T>();

        return Option<T>(get_next());
    }

    virtual Generator<T>* clone() const = 0;
};