#pragma once

#include <cstddef>
#include <functional>
#include "lab_2/sequence.hpp"
#include "generator.hpp"
#include "ordinal.hpp"


template <typename T>
class LazySequence {
public:
    // constructors
    LazySequence();
    LazySequence(T* items, int count);
    LazySequence(Sequence<T>* seq);
    LazySequence(T (*generator_rule)(Sequence<T>*), Sequence<T>* initial_items);

    LazySequence(std::function<T(Sequence<T>*)> generator_rule,
                  Sequence<T>* initial_items);

    LazySequence(const LazySequence<T>& list);

    virtual ~LazySequence() = default;

    // getters
    virtual T& get_first();

    virtual T& get_last();

    virtual T& get(int index);

    virtual LazySequence<T>* get_subsequence(int start_index, int end_index);

    virtual Ordinal get_length() const;

    virtual std::size_t get_materialized_count() const;

    // operations
    virtual Sequence<T>* append(T item);

    virtual Sequence<T>* prepend(T item);

    virtual LazySequence<T>* insert_at(T item, int index);

    virtual LazySequence<T>* concat(LazySequence<T>* list);

    // template <typename T2>
    // virtual LazySequence<T2>* map(T2 (*func)(T));

    // template <typename T2>
    // virtual T2 reduce(T2 (*func)(T2, T));

    // virtual LazySequence<T>* where(bool (*predicate)(T));

    // virtual LazySequence<T>* zip(Sequence<T>* seq);

private:
    Generator<T>* generator_;
    Sequence<T>* cache_;
};

#include "lazy_sequence.tpp"