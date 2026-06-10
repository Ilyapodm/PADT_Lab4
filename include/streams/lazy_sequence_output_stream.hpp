#pragma once

#include <stdexcept>

#include "output_stream.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "core/ordinal.hpp"

template <typename T>
class LazySequenceOutputStream : public OutputStream<T> {
public:
    // don't use const for target, because of sequence append contract, which is not const 
    explicit LazySequenceOutputStream(LazySequence<T>* target) :
        target_(target),
        owns_target_(false),
        position_(),
        is_open_(false)
    {
        if (target_ == nullptr)
            throw std::invalid_argument("LazySequenceOutputStream<T>::LazySequenceOutputStream: Target lazy sequence is null");
    }

    explicit LazySequenceOutputStream(LazySequence<T>& target) :
        LazySequenceOutputStream(&target)
    {}

    LazySequenceOutputStream(const LazySequenceOutputStream<T>& other) = delete;

    LazySequenceOutputStream<T>& operator=(const LazySequenceOutputStream<T>& other) = delete;

    ~LazySequenceOutputStream() override {
        close();

        if (owns_target_)
            delete target_;
    }

    // works only if the sequence if finite
    std::size_t output(const T& item) override {
        ensure_target_();
        ensure_open_();

        if (!position_.is_finite()) 
            throw std::logic_error("LazySequenceOutputStream<T>::output: can't return infinite Ordinal position through std::size_t");

        output_ordinal(item);
        return position_.get_finite_part();
    }

    // can't use "output" name, because only return type differs, this is not overload
    Ordinal output_ordinal(const T& item) {
        ensure_target_();
        ensure_open_();

        // for immutable 'append' returns new sequence, so target updates
        LazySequence<T>* old_target = target_;  // what was the target before append
        bool old_owns_target = owns_target_;  // could stream delete (before append)

        LazySequence<T>* result = target_->append(item);  // for LazySequence always new result

        if (result == nullptr)
            throw std::runtime_error("LazySequenceOutputStream<T>::output: Sequence append returned null");

        target_ = result;
        owns_target_ = true;

        // will not delete in the first output
        if (old_owns_target)
            delete old_target;
        
        position_ = target_->get_ordinal_length();;
        return position_;
    }

    std::size_t get_position() const override {
        ensure_open_();

        if (!position_.is_finite()) 
            throw std::logic_error("LazySequenceOutputStream<T>::get_position: ordinal position is not finite");

        return position_.get_finite_part();
    }

    Ordinal get_ordinal_position() const {
        ensure_open_();

        return position_;
    }

    bool is_open() const override {
        return is_open_;
    }

    LazySequence<T>* release_sequence() {
        if (target_ == nullptr)
            throw std::logic_error("LazySequenceOutputStream<T>::release_sequence: The lazy sequence is already released");

        // if the stream owns the target, ownership is transferred to the caller
        // if the stream does not own the target, the returned pointer is borrowed
        LazySequence<T>* result = target_;

        target_ = nullptr;
        owns_target_ = false;
        is_open_ = false;
        position_ = Ordinal();

        return result;
    }

    // prepares stream for writing
    void open() override {
        if (is_open_)
            throw std::logic_error("LazySequenceOutputStream<T>::open: The stream is already open");

        ensure_target_();

        // if the length if infinite, without finite last part, we will create a new last finite part
        position_ = target_->get_ordinal_length();  
        is_open_ = true;
    }

    // closes stream for writing
    void close() override {
        is_open_ = false;
    }

private:
    LazySequence<T>* target_;
    bool owns_target_;
    Ordinal position_;
    bool is_open_;

    void ensure_open_() const {
        if (!is_open_)
            throw std::logic_error("LazySequenceOutputStream<T>::ensure_open_: The stream is closed");
    }

    void ensure_target_() const {
        if (target_ == nullptr)
            throw std::logic_error("LazySequenceOutputStream<T>::ensure_target_: The lazy sequence is released");
    }
};