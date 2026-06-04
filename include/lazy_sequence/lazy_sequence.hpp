#pragma once

#include <cstddef>
#include <functional>
#include <stdexcept>

#include "core/ordinal.hpp"
#include "adt/ring_buffer.hpp"
#include "adt_lab_2/sequence.hpp"

#include "generators/function_generator.hpp"
#include "generators/empty_generator.hpp"
#include "generators/sequence_generator.hpp"
#include "generators/recurrence_generator.hpp"

#include "nodes/node.hpp"
#include "nodes/source_node.hpp"
#include "nodes/append_node.hpp"
#include "nodes/prepend_node.hpp"
#include "nodes/insert_at_node.hpp"
#include "nodes/remove_at_node.hpp"
#include "nodes/set_node.hpp"
#include "nodes/concat_node.hpp"
#include "nodes/subsequence_node.hpp"

template <typename T>
class LazySequence : public Sequence<T> {
public:
    // default constructor
    LazySequence() {
        root_node_ = new SourceNode<T>(EmptyGenerator<T>());  // inside SourceNode there is a gen.clone
    }

    LazySequence(const T* items, int count) {
        if (count < 0)
            throw std::invalid_argument("LazySequence<T>::LazySequence() count of elements must be non-negative");
        if (count > 0 && items == nullptr)
            throw std::invalid_argument("LazySequence<T>::LazySequence(): count is positive and items is nullptr");
        if (count == 0) {
            root_node_ = new SourceNode<T>(EmptyGenerator<T>());
            return;
        }

        root_node_ = new SourceNode<T>(SequenceGenerator<T>(items, count));
    }
    
    LazySequence(const Sequence<T>& seq) {
        if (const auto* lazy = dynamic_cast<const LazySequence<T>*>(&seq)) {
            root_node_ = lazy->root_node_->clone();
        } else {
            root_node_ = new SourceNode<T>(SequenceGenerator<T>(seq));
        }
    }

    // constructor for function generators
    LazySequence(std::function<T(std::size_t)> function, const Ordinal& length) {
        root_node_ = new SourceNode<T>(FunctionGenerator<T>(function, length));
    }

    // constructor for recurrence generators
    LazySequence(std::function<T(const RingBuffer<T>&)> generator_rule, const T* initial_items, std::size_t count,  const Ordinal& length) {
        root_node_ = new SourceNode<T>(RecurrenceGenerator<T>(generator_rule, initial_items, count, length));
    }

    LazySequence(std::function<T(const T*, std::size_t)> generator_rule, const T* initial_items, std::size_t count,  const Ordinal& length) {
        root_node_ = new SourceNode<T>(RecurrenceGenerator<T>(generator_rule, initial_items, count, length));
    }

    LazySequence(const LazySequence<T>& other) {
        root_node_ = other.root_node_->clone();
    }
    LazySequence& operator=(const LazySequence &other) {
        if (this == &other)
            return *this;

        Node<T>* new_root_node = other.root_node_->clone();
        delete root_node_;
        root_node_ = new_root_node;

        return *this;
    }

    ~LazySequence() {
        delete root_node_;
    }

    // NOTE: all the 'index out of range' responsibility lies with value_at
    const T& get_first() const override {
        return root_node_->value_at(Ordinal(0));
    }

    // have to insure, that seq has the end
    const T& get_last() const override {
        if (root_node_->length().get_finite_part() <= 0)
            throw std::logic_error("The sequence does't have the end, can't return last item");
        return root_node_->value_at(root_node_->length().get_last_index());
    }

    // only finite indexes
    const T& get(int index) const override {
        if (index < 0)
            throw std::out_of_range("LazySequence: index must be positive");
        
        return get_at(Ordinal(index));
    }

    const T& get_at(Ordinal index) const {
        return root_node_->value_at(index);
    }

    // try_... with Option<T> return delegates to get_first, get_last, get

    // for overload, only finite values 
    int get_size() const override {
        if (root_node_->length().is_finite())
            return root_node_->length().get_finite_part();
        if (root_node_->length().get_finite_part() > static_cast<std::size_t>(std::numeric_limits<int>::max())) 
            throw std::overflow_error("LazySequence size is too large for int");
        
        throw std::logic_error("The sequence is infinite, can't express the size through 'int' value");
    }

    const Ordinal& get_ordinal_length() const {
        return root_node_->length();
    }

    std::size_t get_materialized_count() const {
        return root_node_->materialized_count();
    }


    // operations: immutable, user has to delete
    LazySequence<T>* append(const T &item)  override {
        return new LazySequence(new AppendNode<T>(*root_node_, item));
    }
    LazySequence<T>* prepend(const T &item)  override {
        return new LazySequence(new PrependNode<T>(*root_node_, item));
    }


    // for overload, index is finite
    LazySequence<T>* insert_at(const T &item, int index)  override {
        if (index < 0)
            throw std::out_of_range("LazySequence<T>::insert_at: index must be positive");
        return insert_at(item, Ordinal(index));
    }

    LazySequence<T>* set(const T &item, int index)  override {
        if (index < 0)
            throw std::out_of_range("LazySequence<T>::set: index must be positive");
        return set(item, Ordinal(index));
    }

    LazySequence<T>* remove_at(int index) override {
        if (index < 0)
            throw std::out_of_range("LazySequence<T>::remove_at: index must be positive");
        return remove_at(Ordinal(index));
    }

    LazySequence<T>* get_subsequence(int start_index, int end_index) const override {
        if (start_index < 0 || end_index < 0)
            throw std::out_of_range("LazySequence<T>::get_subsequence: index must be non-negative");

        if (start_index > end_index)
            throw std::out_of_range("LazySequence<T>::get_subsequence: start index is greater than end index");

        if (end_index == std::numeric_limits<int>::max())
            throw std::overflow_error("LazySequence<T>::get_subsequence: end_index + 1 overflows");

        return get_subsequence(Ordinal(start_index), Ordinal(end_index + 1));
        // use end_index + 1 because subsequence_node reads
    }

    LazySequence<T>* concat(const Sequence<T>& list) const override {
        if (const auto* lazy = dynamic_cast<const LazySequence<T>*>(&list))
            return concat(*lazy);  
        
        LazySequence<T> right(list);  // uses SeqGen, doesn't work with LazySequences!
        return concat(right);
    }
    

    // true lazy seq interface, index is Ordinal
    LazySequence<T>* insert_at(const T &item, const Ordinal& index) {
        return new LazySequence(new InsertAtNode<T>(*root_node_, item, index));
    }

    LazySequence<T>* set(const T &item, const Ordinal& index) {
        return new LazySequence(new SetNode<T>(*root_node_, item, index));
    }
    
    LazySequence<T>* remove_at(const Ordinal& index) {
        return new LazySequence(new RemoveAtNode<T>(*root_node_, index));
    }

    LazySequence<T>* get_subsequence(const Ordinal& start_index, const Ordinal& end_index_exclusive) const {
        if (start_index > end_index_exclusive)
            throw std::out_of_range("start index is bigger than end index");
        if (end_index_exclusive > root_node_->length())
            throw std::invalid_argument("end index is biggeer than the length of the source");
        return new LazySequence(new SubsequenceNode<T>(*root_node_, start_index, end_index_exclusive));
    }

    LazySequence<T>* concat(const LazySequence<T>& other) const {
        return new LazySequence<T>(new ConcatNode<T>(*root_node_, *other.root_node_));
    }

    // TODO implement it later
    // map, where, reduce
    Sequence<T>* map(T (*mapper)(const T& element)) override {
        throw std::logic_error("Not implemented");
    }
    Sequence<T>* where(bool (*predicate)(const T& element)) override {
        throw std::logic_error("Not implemented");
    }

    T reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) override {
        throw std::logic_error("Not implemented");
    }

    Sequence<T>* slice(int index, int count, const Sequence<T> &seq) override {
        throw std::logic_error("Not implemented");
    }

    IEnumerator<T>* get_enumerator() const override {
        throw std::logic_error("Not implemented");
    }

private:
    Node<T>* root_node_;

    LazySequence(Node<T>* node) {
        if (node == nullptr)
            throw std::invalid_argument("LazySequence node is nullptr");
        root_node_ = node;
    }
};