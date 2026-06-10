#pragma once

#include <cstddef>
#include <stdexcept>

#include "core/ordinal.hpp"
#include "input_stream.hpp"
#include "lazy_sequence/lazy_sequence.hpp"

template <typename T>
class LazySequenceInputStream : public InputStream<T> {
public:
    explicit LazySequenceInputStream(const LazySequence<T>* source) :
        source_(source),
        position_(),
        is_open_(false)
    {
        if (source_ == nullptr)
            throw std::invalid_argument("LazySequenceInputStream<T>::LazySequenceInputStream: LazySequence source is null");
    }

    explicit LazySequenceInputStream(const LazySequence<T>& source) :
        LazySequenceInputStream(&source)
    {}

    LazySequenceInputStream(const LazySequenceInputStream<T>& other) = delete;

    LazySequenceInputStream<T>& operator=(const LazySequenceInputStream<T>& other) = delete;

    ~LazySequenceInputStream() override {
        close();
    }

    // returns true if stream has no more items
    bool is_end_of_stream() const override {
        ensure_open_();

        return position_ >= source_->get_ordinal_length();
    }

    // reads current item and moves stream position forward
    T input() override {
        ensure_open_();

        if (is_end_of_stream()) {
            throw std::out_of_range("LazySequenceInputStream<T>::input: end of stream");
        }

        T result = source_->get(position_);

        position_ = position_ + Ordinal(1);

        return result;
    }

    // returns number of already read items if position is finite
    std::size_t get_position() const override {
        ensure_open_();

        if (!position_.is_finite()) 
            throw std::logic_error("LazySequenceInputStream<T>::get_position: ordinal position is not finite");

        return position_.get_finite_part();
    }

    Ordinal get_ordinal_position() const {
        ensure_open_();

        return position_;
    }

    bool can_seek() const override {
        return true;
    }

    bool is_open() const override {
        return is_open_;
    }

    // seeks only to a finite index
    std::size_t seek(std::size_t index) override {
        ensure_open_();

        const Ordinal ordinal_index(index);
        seek(ordinal_index);

        return index;
    }

    // seeks to an ordinal index
    Ordinal seek(const Ordinal& index) {
        ensure_open_();

        if (index > source_->get_ordinal_length()) 
            throw std::out_of_range("LazySequenceInputStream<T>::seek: index is out of range");

        position_ = index;

        return position_;
    }

    bool can_go_back() const override {
        return true;
    }

    void open() override {
        if (is_open_) 
            throw std::logic_error("LazySequenceInputStream<T>::open: the stream is already open");

        position_ = Ordinal();
        is_open_ = true;
    }

    void close() override {
        position_ = Ordinal();
        is_open_ = false;
    }

    void reset() override {
        ensure_open_();

        position_ = Ordinal();
    }

private:
    const LazySequence<T>* source_;
    Ordinal position_;  // index of the next item to read
    bool is_open_;

    // checks that stream is open
    void ensure_open_() const {
        if (!is_open_) 
            throw std::logic_error("LazySequenceInputStream<T>::ensure_open_: the stream is closed");
    }
};