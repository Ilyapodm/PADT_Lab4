#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include "lab_2/sequence.hpp"

template <class T>
class LazySequence;

template <class T>
class Generator {
public:
    // Создание объекта
    Generator(LazySequence<T>* owner,
              std::function<T(Sequence<T>*)> generator_rule);

    Generator(LazySequence<T>* owner,
              std::size_t index,
              Sequence<T>* items);

    Generator(const Generator<T>& other);

    virtual ~Generator() = default;

    // Декомпозиция
    virtual T get_next();

    virtual bool has_next() const;

    virtual std::optional<T> try_get_next();

    // Операции

    virtual Generator<T>* append(T item) const;

    virtual Generator<T>* append(Sequence<T>* items) const;

    virtual Generator<T>* insert(T item) const;

    virtual Generator<T>* insert(Sequence<T>* items) const;

    virtual Generator<T>* remove(T item) const;

    virtual Generator<T>* remove(Sequence<T>* items) const;
};