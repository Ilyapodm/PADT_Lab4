#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>
#include "lab_2/sequence.hpp"
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

// template <typename T>
// class EmptyGenerator : public Generator<T> {
//     virtual ~EmptyGenerator() = default;

//     bool has_next() const override { return false; }
//     T get_next() override { throw std::logic_error("There is no next element in EmptyGenerator"); }

//     Ordinal length() const override { return Ordinal(); }
// }

// template <class T>
// class GeneratorFactory {
// public:
//     virtual Generator<T>* create() const = 0;
//     virtual Ordinal length() const = 0;

//     virtual ~GeneratorFactory() = default;
// };

// template <typename T>
// class SequenceGenerator : public Generator<T> {
// public:
//     explicit SequenceGenerator(const Sequence<T>* source)
//         : source_(source) {}

//     bool has_next() const override {
//         return index_ < source_->get_size();
//     }

//     T get_next() override {
//         if (!has_next()) {
//             throw std::out_of_range("Generator is exhausted");
//         }

//         return source_->get(index_++);
//     }

//     Ordinal length() const override {
//         return ordinal(source_->get_size());
//     }

//     bool supports_get_at() const override {
//         return true;
//     }

//     T get_at(const Ordinal& index) override {
//         if (!index.is_finite()) {
//             throw std::out_of_range("Infinite index for finite sequence");
//         }

//         return source_->get(index.get_finite_part());
//     }

// private:
//     const Sequence<T>* source_;
//     std::size_t index_ = 0;
// };

