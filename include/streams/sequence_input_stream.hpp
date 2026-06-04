#pragma once

#include <cstddef>
#include <stdexcept>

#include "input_stream.hpp"
#include "adt_lab_2/sequence.hpp"
#include "adt_lab_2/ienumerator.hpp"

template <typename T>
class SequenceInputStream : public InputStream<T> {
public:
    explicit SequenceInputStream(Sequence<T>* source) :
        source_(source),
        iterator_(nullptr),
        buffered_item_(nullptr),
        position_(0),
        is_open_(false)
    {
        if (source_ == nullptr) 
            throw std::invalid_argument("sequence input stream source is null");
        
        open();
    }

    explicit SequenceInputStream(Sequence<T>& source) : SequenceInputStream(&source) {}

    SequenceInputStream(const SequenceInputStream<T>& other) = delete;

    SequenceInputStream<T>& operator=(const SequenceInputStream<T>& other) = delete;

    ~SequenceInputStream() override {
        close();
    }

    // returns true if stream has no more items
    bool is_end_of_stream() const override {
        ensure_open();
        return buffered_item_ == nullptr;
    }

    // reads current item and moves stream position forward
    T input() override {
        ensure_open();

        if (is_end_of_stream()) {
            throw std::out_of_range("end of stream");
        }

        T result = *buffered_item_;

        clear_buffer();
        ++position_;
        load_next();

        return result;
    }

    // returns number of already read items
    std::size_t get_position() const override {
        return position_;
    }

    // returns true because sequence enumerator supports reset
    bool can_seek() const override {
        return true;
    }

    // moves stream to given position
    std::size_t seek(std::size_t index) override {
        ensure_open();

        iterator_->reset();
        clear_buffer();

        position_ = 0;
        load_next();

        while (position_ < index) {
            if (buffered_item_ == nullptr) {
                throw std::out_of_range("seek position is out of stream");
            }

            clear_buffer();
            ++position_;
            load_next();
        }

        return position_;
    }

    // returns true because sequence enumerator supports reset
    bool can_go_back() const override {
        return true;
    }

    // opens stream and resets position to start
    void open() override {
        close();

        iterator_ = source_->get_enumerator();

        if (iterator_ == nullptr) {
            throw std::runtime_error("sequence enumerator is null");
        }

        position_ = 0;
        is_open_ = true;

        load_next();
    }

    // closes stream and releases enumerator
    void close() override {
        clear_buffer();

        delete iterator_;
        iterator_ = nullptr;

        is_open_ = false;
    }

private:
    Sequence<T>* source_;
    IEnumerator<T>* iterator_;
    T* buffered_item_;
    std::size_t position_;
    bool is_open_;

    // checks that stream is open
    void ensure_open() const {
        if (!is_open_) {
            throw std::logic_error("stream is closed");
        }
    }

    // deletes buffered item
    void clear_buffer() {
        delete buffered_item_;
        buffered_item_ = nullptr;
    }

    // loads next item from enumerator to buffer
    void load_next() {
        if (iterator_ == nullptr) {
            throw std::logic_error("stream iterator is null");
        }

        if (iterator_->move_next()) {
            buffered_item_ = new T(iterator_->get_current());
        } else {
            buffered_item_ = nullptr;
        }
    }
};