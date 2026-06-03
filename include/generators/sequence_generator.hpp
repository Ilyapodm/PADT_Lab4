#pragma once

#include <cstddef>
#include <stdexcept>

#include "generator.hpp"
#include "adt_lab_2/sequence.hpp"
#include "adt_lab_2/array_sequence.hpp"
#include "adt_lab_2/ienumerator.hpp"
#include "core/ordinal.hpp"


// make a snapshot of any finite Sequence<T> into an owned MutableArraySequence<T>
template <typename T>
class SequenceGenerator : public Generator<T> {
public:
    explicit SequenceGenerator(const Sequence<T>& source) {
        // enumerator allows not to waste time through 'get',
        // PERF: but we don't have enough access to dynamic array to allocate enough memory at once, and not doing: 'allocate more -> copy -> delete'
        // as a decision: TODO: add 'reserve' to array_seq
        // HACK: enumarator will not work with LazySequence, in 'concat' in LazySequence there is a validation for this case 
        // btw, in general case mad can do: (Generator->Node->LazySequence) => SequenceGenerator (explosion)
        IEnumerator<T>* iter = source.get_enumerator();

        try{
            while (iter->move_next()) {
                source_.append(iter->get_current());
            }
        } catch (...) {
            delete iter;
            throw;
        }

        delete iter;
    }

    explicit SequenceGenerator(const MutableArraySequence<T>& other) : source_(other) {}

    bool has_next() const override {
        return index_ < static_cast<std::size_t>(source_.get_size());
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("Generator is exhausted");
        }

        return source_.get(static_cast<int>(index_++));
    }

    Ordinal length() const override {
        return Ordinal(source_.get_size());  
    }

    SequenceGenerator<T>* clone() const override {
        return new SequenceGenerator<T>(*this);
    }

private:
    MutableArraySequence<T> source_;  // copies the entered sequence
    std::size_t index_ = 0;
};
