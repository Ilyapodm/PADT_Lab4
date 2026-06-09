#pragma once

#include <cstddef>
#include <stdexcept>

#include "output_stream.hpp"
#include "adt_lab_2/sequence.hpp"

template <typename T>
class SequenceOutputStream : public OutputStream<T> {
public:
    explicit SequenceOutputStream(Sequence<T>* target) :
        target_(target),
        owns_target_(false),  // starts with false, we can't delete the sequence from outside
        position_(0),
        is_open_(false)
    {
        if (target_ == nullptr)
            throw std::invalid_argument("SequenceOutputStream<T>::constructor: Target sequence is null");
    }

    explicit SequenceOutputStream(Sequence<T>& target) :
        SequenceOutputStream(&target)
    {}

    SequenceOutputStream(const SequenceOutputStream<T>& other) = delete;

    SequenceOutputStream<T>& operator=(const SequenceOutputStream<T>& other) = delete;

    ~SequenceOutputStream() override {
        close();

        if (owns_target_)
            delete target_;
    }

    // writes item to sequence and moves position forward
    std::size_t output(const T& item) override {
        ensure_target_();
        ensure_open_();

        // for mutable 'append' returns this, so target doesn't change
        // for immutable 'append' returns new sequence, so target updates

        Sequence<T>* old_target = target_;  // what was the target before append
        bool old_owns_target = owns_target_;  // could stream delete (before append)

        Sequence<T>* result = target_->append(item);  // result may be the new seq or the old one (just updated)

        if (result == nullptr)
            throw std::runtime_error("SequenceOutputStream<T>::output: Sequence append returned null");

        // if immutable
        if (result != old_target) {
            target_ = result;
            owns_target_ = true;

            if (old_owns_target)
                delete old_target;
        }

        ++position_;
        return position_;
    }

    // returns number of already written items
    std::size_t get_position() const override {
        ensure_open_();
        return position_;
    }

    void open() override {
        if (is_open_)
            throw std::logic_error("SequenceOutputStream<T>::open: The stream is already open");

        ensure_target_();

        position_ = static_cast<std::size_t>(target_->get_size());
        is_open_ = true;
    }

    void close() override {
        is_open_ = false;
    }

    // releases current sequence and disables stream, gives the responsibility for owning the target outside, final point
    Sequence<T>* release_sequence() {
        if (target_ == nullptr)
            throw std::logic_error("SequenceOutputStream<T>::release_sequence: The sequence is already released");

        Sequence<T>* result = target_;

        target_ = nullptr;
        owns_target_ = false;
        is_open_ = false;

        return result;
    }

private:
    Sequence<T>* target_;  // current target sequence
    bool owns_target_;
    std::size_t position_;  // how many elements are already written
    bool is_open_;

    // checks that stream is open
    void ensure_open_() const {
        if (!is_open_)
            throw std::logic_error("SequenceOutputStream<T>::ensure_open_: The stream is closed");
    }

    void ensure_target_() const {
        if (target_ == nullptr)
            throw std::logic_error("SequenceOutputStream<T>::ensure_target_: The sequence is released");
    }
};