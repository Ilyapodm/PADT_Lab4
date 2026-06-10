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
        // PERF: MutableArraySequence has no reserve(), so repeated append() may cause reallocations
        // TODO: add reserve() to MutableArraySequence to allocate source.get_size() elements at once
        
        // HACK: the constructor accepts any Sequence<T>, but this snapshot model is valid only
        // for finite, materializable sequences. Passing an infinite or ordinal LazySequence<T>
        // would try to enumerate it as an ordinary finite sequence and may never finish.
        // LazySequence concat code should validate this moment
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

    SequenceGenerator(const T* items, int count) : source_(make_source_(items, count)) {}

    explicit SequenceGenerator(const MutableArraySequence<T>& other) : source_(other) {}

    bool has_next() const override {
        return index_ < static_cast<std::size_t>(source_.get_size());
    }

    T get_next() override {
        if (!has_next()) {
            throw std::out_of_range("SequenceGenerator<T>::get_next: Generator is exhausted");
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

    // helper to initialize, not transform already existing 'source_'
    static MutableArraySequence<T> make_source_(const T* items, int count) {
        if (count < 0) {
            throw std::invalid_argument("SequenceGenerator<T>::make_source_: size cannot be negative");
        }

        if (items == nullptr && count > 0) {
            throw std::invalid_argument("SequenceGenerator<T>::make_source_: items is nullptr");
        }

        if (count == 0) {
            return MutableArraySequence<T>();
        }

        return MutableArraySequence<T>(items, count);
    }
};
