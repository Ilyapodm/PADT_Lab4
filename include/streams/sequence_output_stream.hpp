#pragma once

#include <cstddef>
#include <stdexcept>

#include "output_stream.hpp"
#include "../adt_lab_2/sequence.hpp"

template <typename T>
class SequenceOutputStream : public OutputStream<T> {
public:
    explicit SequenceOutputStream(Sequence<T>*& target) : 
        target_(target),
        position_(0),
        is_open_(false)
    {
        if (target_ == nullptr) 
            throw std::invalid_argument("sequence output stream target is null");
        

        open();
    }

    SequenceOutputStream(const SequenceOutputStream<T>& other) = delete;

    SequenceOutputStream<T>& operator=(const SequenceOutputStream<T>& other) = delete;

    ~SequenceOutputStream() override {
        close();
    }

    // writes item to sequence and moves position forward
    std::size_t output(const T& item) override {
        ensure_open();
        ensure_target();

        Sequence<T>* result = target_->append(item);

        if (result == nullptr) {
            throw std::runtime_error("sequence append returned null");
        }

        target_ = result;
        ++position_;

        return position_;
    }

    // returns current stream position
    std::size_t get_position() const override {
        return position_;
    }

    // opens stream for writing
    void open() override {
        ensure_target();

        position_ = static_cast<std::size_t>(target_->get_size());
        is_open_ = true;
    }

    // closes stream for writing
    void close() override {
        is_open_ = false;
    }

    // returns current target sequence
    Sequence<T>* get_sequence() {
        return target_;
    }

    // returns current target sequence
    const Sequence<T>* get_sequence() const {
        return target_;
    }

private:
    Sequence<T>*& target_;
    std::size_t position_;
    bool is_open_;

    // checks that stream is open
    void ensure_open() const {
        if (!is_open_) {
            throw std::logic_error("stream is closed");
        }
    }

    // checks that target sequence exists
    void ensure_target() const {
        if (target_ == nullptr) {
            throw std::logic_error("sequence output stream target is null");
        }
    }
};