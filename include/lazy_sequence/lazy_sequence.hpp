#pragma once

#include <cstddef>
#include <functional>

#include "core/ordinal.hpp"
#include "adt/ring_buffer.hpp"
#include "adt_lab_2/sequence.hpp"
#include "adt_lab_2/ienumerator.hpp"
#include "adt_lab_2/option.hpp"
#include "nodes/node.hpp"

template <typename T>
class LazySequence : public Sequence<T> {
public:
    // Constructors 
    LazySequence();
    LazySequence(const T* items, int count);

    LazySequence(const Sequence<T>& seq);  // constructor for sequence

    LazySequence(std::function<T(std::size_t)> function, const Ordinal& length);  // constructor for function generators

    // constructor for recurrence generators
    LazySequence(std::function<T(const RingBuffer<T>&)> generator_rule, const T* initial_items, std::size_t count,  const Ordinal& length);
    LazySequence(std::function<T(const T*, std::size_t)> generator_rule, const T* initial_items, std::size_t count,  const Ordinal& length);

    LazySequence(const LazySequence<T>& other);
    LazySequence& operator=(const LazySequence &other);

    ~LazySequence();

    // Getters
    const T& get_first() const override;
    const T& get_last() const override;
    const T& get(int index) const override;  // only finite indexes
    const T& get(Ordinal index) const;  // for Ordinal indexes

    //Option<T> try_get(Ordinal index) const;
    // 'try_get_first', 'try_get_last', 'try_get(int)' with Option<T> return 
    // delegates to 'get_first', 'get_last', 'get' in Sequence, so here need only try_get(Ordinal)

    int get_size() const override;  // for overload, only finite values 
    const Ordinal& get_ordinal_length() const;  
    std::size_t get_materialized_count() const;

    // Operations: immutable, user has to delete
    LazySequence<T>* append(const T &item)  override;
    LazySequence<T>* prepend(const T &item)  override;

    // for overriding, index is finite
    LazySequence<T>* insert_at(const T &item, int index) override;
    LazySequence<T>* set(const T &item, int index) override;
    LazySequence<T>* remove_at(int index) override;
    LazySequence<T>* get_subsequence(int start_index, int end_index) const override;
    LazySequence<T>* concat(const Sequence<T>& list) const override;
    
    // overloading, true lazy seq Ordinal interface 
    LazySequence<T>* insert_at(const T &item, const Ordinal& index);
    LazySequence<T>* set(const T &item, const Ordinal& index);
    LazySequence<T>* remove_at(const Ordinal& index);
    LazySequence<T>* get_subsequence(const Ordinal& start_index, const Ordinal& end_index_exclusive) const;
    LazySequence<T>* concat(const LazySequence<T>& other) const;

    // TODO implement it later
    // map, where, reduce
    Sequence<T>* map(T (*mapper)(const T& element)) override;
    Sequence<T>* where(bool (*predicate)(const T& element)) override;

    T reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) override;

    Sequence<T>* slice(int index, int count, const Sequence<T> &seq) override;

    IEnumerator<T>* get_enumerator() const override;

private:
    Node<T>* root_node_;  

    // for operations
    LazySequence(Node<T>* node);

    // Enumerator
    class LazySequenceEnumerator : public IEnumerator<T> {
    public:
        explicit LazySequenceEnumerator(const Node<T>* root_node);
        bool move_next() override;
        const T& get_current() const override;
        void reset() override;
    private:
        const Node<T>* root_node_;
        long long index_ = -1;
    };
};

#include "lazy_sequence.tpp"