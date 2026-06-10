#pragma once

#include <cstddef>

template <typename T>
class InputStream {
public:
    virtual ~InputStream() = default;

    // returns true if stream has no more items
    virtual bool is_end_of_stream() const = 0;

    virtual bool is_open() const = 0;

    // reads current item and moves stream position forward
    virtual T input() = 0;  // don't use const T&, because of possible temprorary items

    // returns number of already read items
    virtual std::size_t get_position() const = 0;

    // returns true if stream supports moving to position
    virtual bool can_seek() const = 0;

    // moves stream to given position
    virtual std::size_t seek(std::size_t index) = 0;

    // returns true if stream can move back to previous positions
    virtual bool can_go_back() const = 0;

    // open the stream for reading
    virtual void open() = 0;

    // closes stream for reading
    virtual void close() = 0;

    // resets the stream
    virtual void reset() = 0;
};
// TODO сделать отдельный output (корректная запись в конец) и input (ординальный сик) потоки для lazy sequence
// TODO сделать файловые потоки