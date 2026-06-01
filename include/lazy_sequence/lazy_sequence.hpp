#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>

#include "adt_lab_2/sequence.hpp"
#include "generators/generator.hpp"
#include "core/ordinal.hpp"


template <typename T>
class LazySequence : public Sequence<T> {
public:
    // constructors
    LazySequence();  
    LazySequence(const T* items, int count);
    LazySequence(const Sequence<T>* seq);
    LazySequence(T (*generator_rule)(Sequence<T>*), const Sequence<T>* initial_items);
    LazySequence(std::function<T(Sequence<T>*)> generator_rule, const Sequence<T>* initial_items);
    LazySequence(const LazySequence<T>& other);
    LazySequence& operator=(const LazySequence &other);
    virtual ~LazySequence() = default;

    // NOTE: all the 'index out of range' responsibility lies with get_at, which is delegated by other element getters
    // only finite indexes
    const T& get_first() const override {
        return get_at(Ordinal(1));
    }

    // have to insure, that seq has the end
    const T& get_last() const override {
        if (!length_.has_end())
            throw std::logic_error("The sequence does't have the end, can't return last item");
        return get_at(Ordinal(length_.get_finite_part() - 1));
    }

    const T& get(int index) const override {
        return get_at(Ordinal(index));
    }

    const T& get_at(Ordinal index) const {

    }


    int get_size() const override {
        if (length_.is_finite())
            return length_.get_finite_part();
        else
            throw std::logic_error("The sequence is infinite, can't express the size through 'int' value");
    }

    const Ordinal& get_ordinal_length() const {
        return length_;
    }

    int get_materialized_count() const {
        return cache_->get_size();
    }


    // try_... with Option<T> return is using get_first, get_last, get

    // operations
    virtual Sequence<T>* append(const T &item) const override;
    virtual Sequence<T>* prepend(const T &item) = 0;
    virtual Sequence<T>* insert_at(const T &item, int index) = 0;
    virtual Sequence<T>* set(const T &item, int index) = 0;
    
    virtual Sequence<T>* remove_at(int index) = 0;

    virtual Sequence<T>* get_subsequence(int start_index, int end_index) const = 0;

    virtual Sequence<T>* concat(const Sequence<T> &list) const = 0;

    // map, where, reduce
    virtual Sequence<T>* map(T (*mapper)(const T& element)) = 0;
    virtual Sequence<T>* where(bool (*predicate)(const T& element)) = 0;
    virtual T reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) = 0;
    virtual Sequence<T>* slice(int index, int count, const Sequence<T> &seq) = 0;

private:
    Generator<T>* generator_;
    Sequence<T>* cache_;
    Ordinal length_;
};

//#include "lazy_sequence.tpp"