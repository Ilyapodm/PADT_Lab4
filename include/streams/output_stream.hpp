#pragma once

#include <cstddef>

template <typename T>
class OutputStream {
public:
    virtual ~OutputStream() = default;

    // writes item to stream and moves position forward
    virtual std::size_t output(const T& item) = 0;

    // returns number of already written items
    virtual std::size_t get_position() const = 0;

    virtual bool is_open() const = 0;

    // prepares stream for writing
    virtual void open() = 0;  // NOTE: user have to open the stream, it doesn't open automatically

    // closes stream for writing
    virtual void close() = 0;
};