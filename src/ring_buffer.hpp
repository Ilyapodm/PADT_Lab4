#pragma once

#include <stdexcept>
#include <cstddef>

template <typename T>
class RingBuffer {
public:
    RingBuffer(const T* items, std::size_t size) : size_{size} {

        if (items == nullptr && size_ > 0)
            throw std::invalid_argument("RingBuffer<T>::RingBuffer: items is nullptr");

        data_ = new T[size_]();  // do the initialization of items

        // fallback if the '=' operator for T will fall 
        try{
            for (std::size_t i = 0; i < size_; i++) {
                data_[i] = items[i];
            }
        } catch (...) {
            delete []data_;
            throw;
        }
    }

    RingBuffer(const RingBuffer<T> &other) : size_{other.size_}, start_{other.start_} {
        data_ = new T[size_]{};  // do the initialization of items

        // fallback if the '=' operator for T will fall 
        try {
            for (std::size_t i = 0; i < size_; i++) {
                data_[i] = other.data_[i];
            }
        } catch (...) {
            delete[] data_;
            throw;
        }
    }

    RingBuffer<T>& operator=(const RingBuffer<T> &other) {
        if (&other == this)  // no self assignment
            return *this;

        // do not need 'try', 'new' is smart, if bad_alloc it will clear everything itself
        T *new_data = new T[other.size_]{};  // do the initialization of items

        // at first try to create new data and then only 'delete' the old
        try {
            for (std::size_t i = 0; i < other.size_; i++) {
                new_data[i] = other.data_[i];  // here can occur a bad alloc or smth
            }
        }
        catch (...) {
            delete []new_data;
            throw;
        }
        
        delete[] data_;
        size_ = other.size_;
        start_ = other.start_;
        data_ = new_data;

        return *this;
    }

    ~RingBuffer() {
        delete[] data_;
    }

    // replace the oldest value to the new one
    void push(const T& new_value) {
        if (size_ == 0)
            throw std::out_of_range("Out of range: zero size buffer");

        data_[start_] = new_value;
        start_ = (start_ + 1) % size_;
    }

    // returns value counting from the 'start'
    const T& get(std::size_t index) const {
        if (index >= size_ || index < 0) 
            throw std::out_of_range("RingBuffer<T>::get: Index out of range");

        return data_[(start_ + index) % size_];
    }

    std::size_t size() const {
        return size_;
    }

private:
    T* data_;
    std::size_t start_ = 0;  // the oldest value 
    std::size_t size_;
};