#pragma once

#include <cstddef>
#include <stdexcept>

#include "input_stream.hpp"
#include "adt_lab_2/sequence.hpp"
#include "adt_lab_2/ienumerator.hpp"
#include "adt_lab_2/option.hpp"

template <typename T>
class SequenceInputStream : public InputStream<T> {
public:
    explicit SequenceInputStream(const Sequence<T>* source) :
        source_(source),
        iterator_(nullptr),
        position_(0),
        is_open_(false)
    {
        if (source_ == nullptr) 
            throw std::invalid_argument("SequenceInputStream<T>::SequenceInputStream: Sequence source is null");
    }

    explicit SequenceInputStream(const Sequence<T>& source) : SequenceInputStream(&source) {}

    SequenceInputStream(const SequenceInputStream<T>& other) = delete;

    SequenceInputStream<T>& operator=(const SequenceInputStream<T>& other) = delete;

    ~SequenceInputStream() override {
        close();
    }

    // returns true if stream has no more items
    bool is_end_of_stream() const override {
        ensure_open_();
        return !buffered_item_.has_value();
    }

    // reads current item and moves stream position forward
    T input() override {
        ensure_open_();

        if (is_end_of_stream()) {
            throw std::out_of_range("SequenceInputStream<T>::input: End of stream");
        }

        T result = buffered_item_.get_value();

        //clear_buffer();
        ++position_;
        load_next_();

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

    bool is_open() const override {
        return is_open_;
    }

    // moves stream to given position
    std::size_t seek(std::size_t index) override {
        ensure_open_();

        // don't have to reset, can move forward
        if (index < position_) {
            iterator_->reset();
            position_ = 0;
            load_next_();
        }


        while (position_ < index) {
            if (!buffered_item_.has_value()) {
                throw std::out_of_range("SequenceInputStream<T>::seek: position is out of stream");
            }

            ++position_;
            load_next_();
        }

        return position_;
    }

    // returns true because sequence enumerator supports reset
    bool can_go_back() const override {
        return true;
    }

    // NOTE: user have to open the stream, it doesn't open automatically
    void open() override {
        if (is_open_)
            throw std::logic_error("SequenceInputStream<T>::open(): The stream is already open");

        iterator_ = source_->get_enumerator();

        if (iterator_ == nullptr) 
            throw std::runtime_error("SequenceInputStream<T>::open(): Sequence enumerator is null");
        
        position_ = 0;
        is_open_ = true;

        load_next_();
    }

    void close() override {
        delete iterator_;
        iterator_ = nullptr;

        buffered_item_ = Option<T>();

        is_open_ = false;
    }

    void reset() override {
        ensure_open_();

        iterator_->reset();
        position_ = 0;

        buffered_item_ = Option<T>();

        load_next_();
    }

private:
    const Sequence<T>* source_;
    IEnumerator<T>* iterator_;

    // need buffered item, because work with iterator with 'move_next', so can't understand, when it is the end of stream
    Option<T> buffered_item_;  
    std::size_t position_;  // count of already read elements
    bool is_open_;

    // checks that stream is open
    void ensure_open_() const {
        if (!is_open_) 
            throw std::logic_error("SequenceInputStream<T>::ensure_open_: the stream is closed");
    }

    // loads next item from enumerator to buffer
    void load_next_() {
        if (iterator_ == nullptr) {
            throw std::logic_error("SequenceInputStream<T>::load_next_: stream iterator is null");
        }

        if (iterator_->move_next()) 
            buffered_item_ = Option<T>(iterator_->get_current());
        
        else 
            buffered_item_ = Option<T>();     
    }
};