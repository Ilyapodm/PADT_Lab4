#include <gtest/gtest.h>
#include <stdexcept>

#include "streams/sequence_input_stream.hpp"
#include "adt_lab_2/array_sequence.hpp"
#include "adt_lab_2/list_sequence.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "core/ordinal.hpp"

TEST(SequenceInputStreamTest, ReadsArraySequenceInOrder) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> sequence(items, 3);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 0);
    EXPECT_FALSE(stream.is_end_of_stream());

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.get_position(), 1);

    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position(), 2);

    EXPECT_EQ(stream.input(), 30);
    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_TRUE(stream.is_end_of_stream());
}

TEST(SequenceInputStreamTest, ThrowsWhenReadingAfterEnd) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.input(), 1);
    EXPECT_EQ(stream.input(), 2);

    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::out_of_range);
}

TEST(SequenceInputStreamTest, EmptySequenceIsEndImmediately) {
    MutableArraySequence<int> sequence;

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 0);
    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::out_of_range);
}

TEST(SequenceInputStreamTest, SeekForwardWorks) {
    int items[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> sequence(items, 5);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.seek(3), 3);
    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.input(), 40);
    EXPECT_EQ(stream.get_position(), 4);
}

TEST(SequenceInputStreamTest, SeekBackwardWorks) {
    int items[] = {10, 20, 30, 40};
    MutableArraySequence<int> sequence(items, 4);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position(), 2);

    EXPECT_EQ(stream.seek(0), 0);
    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.get_position(), 1);
}

TEST(SequenceInputStreamTest, SeekToEndWorks) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.seek(3), 3);
    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_TRUE(stream.is_end_of_stream());
    EXPECT_THROW(stream.input(), std::out_of_range);
}

TEST(SequenceInputStreamTest, SeekAfterEndThrows) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceInputStream<int> stream(sequence);

    EXPECT_THROW(stream.seek(4), std::out_of_range);
}

TEST(SequenceInputStreamTest, OpenResetsStream) {
    int items[] = {5, 6, 7};
    MutableArraySequence<int> sequence(items, 3);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.input(), 5);
    EXPECT_EQ(stream.input(), 6);
    EXPECT_EQ(stream.get_position(), 2);

    stream.open();

    EXPECT_EQ(stream.get_position(), 0);
    EXPECT_FALSE(stream.is_end_of_stream());
    EXPECT_EQ(stream.input(), 5);
}

TEST(SequenceInputStreamTest, InputAfterCloseThrows) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceInputStream<int> stream(sequence);

    stream.close();

    EXPECT_THROW(stream.input(), std::logic_error);
    EXPECT_THROW(stream.is_end_of_stream(), std::logic_error);
}



TEST(SequenceInputStreamTest, ReadsListSequenceInOrder) {
    int items[] = {3, 6, 9};
    MutableListSequence<int> sequence(items, 3);

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.input(), 3);
    EXPECT_EQ(stream.input(), 6);
    EXPECT_EQ(stream.input(), 9);

    EXPECT_TRUE(stream.is_end_of_stream());
}

TEST(SequenceInputStreamTest, ReadsFiniteLazySequenceInOrder) {
    LazySequence<int> sequence(
        [](std::size_t index) {
            return static_cast<int>(index * 10);
        },
        Ordinal(5)
    );

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 0);
    EXPECT_FALSE(stream.is_end_of_stream());

    EXPECT_EQ(stream.input(), 0);
    EXPECT_EQ(stream.get_position(), 1);

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.get_position(), 2);

    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.input(), 30);
    EXPECT_EQ(stream.get_position(), 4);

    EXPECT_EQ(stream.input(), 40);
    EXPECT_EQ(stream.get_position(), 5);

    EXPECT_TRUE(stream.is_end_of_stream());
    EXPECT_THROW(stream.input(), std::out_of_range);
}

TEST(SequenceInputStreamTest, SeekWorksWithLazySequence) {
    LazySequence<int> sequence(
        [](std::size_t index) {
            return static_cast<int>(index + 100);
        },
        Ordinal(6)
    );

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.input(), 100);
    EXPECT_EQ(stream.input(), 101);
    EXPECT_EQ(stream.get_position(), 2);

    EXPECT_EQ(stream.seek(4), 4);
    EXPECT_EQ(stream.get_position(), 4);
    EXPECT_EQ(stream.input(), 104);

    EXPECT_EQ(stream.seek(1), 1);
    EXPECT_EQ(stream.get_position(), 1);
    EXPECT_EQ(stream.input(), 101);
}

TEST(SequenceInputStreamTest, OpenResetsLazySequenceStream) {
    LazySequence<int> sequence(
        [](std::size_t index) {
            return static_cast<int>(index * index);
        },
        Ordinal(4)
    );

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.input(), 0);
    EXPECT_EQ(stream.input(), 1);
    EXPECT_EQ(stream.input(), 4);
    EXPECT_EQ(stream.get_position(), 3);

    stream.open();

    EXPECT_EQ(stream.get_position(), 0);
    EXPECT_FALSE(stream.is_end_of_stream());

    EXPECT_EQ(stream.input(), 0);
    EXPECT_EQ(stream.input(), 1);
}

TEST(SequenceInputStreamTest, SeekToEndWorksWithLazySequence) {
    LazySequence<int> sequence(
        [](std::size_t index) {
            return static_cast<int>(index);
        },
        Ordinal(3)
    );

    SequenceInputStream<int> stream(sequence);

    EXPECT_EQ(stream.seek(3), 3);
    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_TRUE(stream.is_end_of_stream());
    EXPECT_THROW(stream.input(), std::out_of_range);
}