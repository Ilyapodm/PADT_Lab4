#include <gtest/gtest.h>
#include <stdexcept>

#include "streams/sequence_output_stream.hpp"
#include "adt_lab_2/array_sequence.hpp"
#include "lazy_sequence/lazy_sequence.hpp"
#include "core/ordinal.hpp"
#include "utils/generator_functions.hpp"

TEST(SequenceOutputStreamTest, ConstructorThrowsOnNullTarget) {
    Sequence<int>* sequence = nullptr;

    EXPECT_THROW(
        SequenceOutputStream<int> stream(sequence),
        std::invalid_argument
    );
}

TEST(SequenceOutputStreamTest, OutputBeforeOpenThrows) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceOutputStream<int> stream(sequence);

    EXPECT_THROW(stream.output(4), std::logic_error);
}

TEST(SequenceOutputStreamTest, OpenSetsPositionToCurrentSize) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> sequence(items, 3);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.get_position(), 3);
}

TEST(SequenceOutputStreamTest, OpenWhenAlreadyOpenThrows) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_THROW(stream.open(), std::logic_error);
}

TEST(SequenceOutputStreamTest, WritesToMutableArraySequence) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.output(4), 4);
    EXPECT_EQ(stream.output(5), 5);

    EXPECT_EQ(sequence.get_size(), 5);
    EXPECT_EQ(sequence.get(0), 1);
    EXPECT_EQ(sequence.get(1), 2);
    EXPECT_EQ(sequence.get(2), 3);
    EXPECT_EQ(sequence.get(3), 4);
    EXPECT_EQ(sequence.get(4), 5);
}

TEST(SequenceOutputStreamTest, ReleaseMutableSequenceReturnsOriginalObject) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.output(3), 3);

    Sequence<int>* released = stream.release_sequence();

    EXPECT_EQ(released, &sequence);
    EXPECT_EQ(released->get_size(), 3);
    EXPECT_EQ(released->get(0), 1);
    EXPECT_EQ(released->get(1), 2);
    EXPECT_EQ(released->get(2), 3);
}

TEST(SequenceOutputStreamTest, OutputAfterCloseThrows) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.open();
    stream.close();

    EXPECT_THROW(stream.output(3), std::logic_error);
}

TEST(SequenceOutputStreamTest, CloseCanBeCalledMoreThanOnce) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_NO_THROW(stream.close());
    EXPECT_NO_THROW(stream.close());

    EXPECT_THROW(stream.output(3), std::logic_error);
}

TEST(SequenceOutputStreamTest, OpenAfterCloseWorks) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.output(3), 3);

    stream.close();
    stream.open();

    EXPECT_EQ(stream.get_position(), 3);
    EXPECT_EQ(stream.output(4), 4);

    EXPECT_EQ(sequence.get_size(), 4);
    EXPECT_EQ(sequence.get(0), 1);
    EXPECT_EQ(sequence.get(1), 2);
    EXPECT_EQ(sequence.get(2), 3);
    EXPECT_EQ(sequence.get(3), 4);
}

TEST(SequenceOutputStreamTest, ReleaseBeforeOpenReturnsOriginalSequenceAndDisablesStream) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    SequenceOutputStream<int> stream(sequence);

    Sequence<int>* released = stream.release_sequence();

    EXPECT_EQ(released, &sequence);
    EXPECT_EQ(released->get_size(), 3);

    EXPECT_THROW(stream.open(), std::logic_error);
    EXPECT_THROW(stream.output(4), std::logic_error);
    EXPECT_THROW(stream.release_sequence(), std::logic_error);
}

TEST(SequenceOutputStreamTest, OutputAfterReleaseThrows) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    Sequence<int>* released = stream.release_sequence();

    EXPECT_EQ(released, &sequence);

    EXPECT_THROW(stream.output(3), std::logic_error);
    EXPECT_THROW(stream.open(), std::logic_error);
}

TEST(SequenceOutputStreamTest, ReleaseTwiceThrows) {
    int items[] = {1, 2};
    MutableArraySequence<int> sequence(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    Sequence<int>* released = stream.release_sequence();

    EXPECT_EQ(released, &sequence);
    EXPECT_THROW(stream.release_sequence(), std::logic_error);
}

TEST(SequenceOutputStreamTest, WritesToImmutableArraySequence) {
    int items[] = {1, 2, 3};

    Sequence<int>* original = new ImmutableArraySequence<int>(items, 3);

    SequenceOutputStream<int> stream(original);

    stream.open();

    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.output(4), 4);
    EXPECT_EQ(stream.output(5), 5);

    Sequence<int>* result = stream.release_sequence();

    EXPECT_NE(result, original);

    EXPECT_EQ(original->get_size(), 3);
    EXPECT_EQ(original->get(0), 1);
    EXPECT_EQ(original->get(1), 2);
    EXPECT_EQ(original->get(2), 3);

    EXPECT_EQ(result->get_size(), 5);
    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);
    EXPECT_EQ(result->get(4), 5);

    delete original;
    delete result;
}

TEST(SequenceOutputStreamTest, ImmutableReleaseAfterSingleWriteReturnsNewSequence) {
    int items[] = {10, 20};

    Sequence<int>* original = new ImmutableArraySequence<int>(items, 2);

    SequenceOutputStream<int> stream(original);

    stream.open();

    EXPECT_EQ(stream.output(30), 3);

    Sequence<int>* result = stream.release_sequence();

    EXPECT_NE(result, original);

    EXPECT_EQ(original->get_size(), 2);
    EXPECT_EQ(original->get(0), 10);
    EXPECT_EQ(original->get(1), 20);

    EXPECT_EQ(result->get_size(), 3);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete original;
    delete result;
}

TEST(SequenceOutputStreamTest, ImmutableReleaseWithoutWritesReturnsOriginalSequence) {
    int items[] = {7, 8, 9};

    Sequence<int>* original = new ImmutableArraySequence<int>(items, 3);

    SequenceOutputStream<int> stream(original);

    stream.open();

    Sequence<int>* result = stream.release_sequence();

    EXPECT_EQ(result, original);

    EXPECT_EQ(result->get_size(), 3);
    EXPECT_EQ(result->get(0), 7);
    EXPECT_EQ(result->get(1), 8);
    EXPECT_EQ(result->get(2), 9);

    delete original;
}

TEST(SequenceOutputStreamTest, WritesToEmptyMutableArraySequence) {
    MutableArraySequence<int> sequence;

    SequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.output(10), 1);
    EXPECT_EQ(stream.output(20), 2);

    EXPECT_EQ(sequence.get_size(), 2);
    EXPECT_EQ(sequence.get(0), 10);
    EXPECT_EQ(sequence.get(1), 20);
}

TEST(SequenceOutputStreamTest, WritesToEmptyImmutableArraySequence) {
    Sequence<int>* original = new ImmutableArraySequence<int>();

    SequenceOutputStream<int> stream(original);

    stream.open();

    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.output(10), 1);
    EXPECT_EQ(stream.output(20), 2);

    Sequence<int>* result = stream.release_sequence();

    EXPECT_NE(result, original);

    EXPECT_EQ(original->get_size(), 0);

    EXPECT_EQ(result->get_size(), 2);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);

    delete original;
    delete result;
}

TEST(SequenceOutputStreamTest, WritesToFiniteLazySequence) {
    Sequence<int>* original = new LazySequence<int>(
        identity_function,
        Ordinal(3)
    );

    SequenceOutputStream<int> stream(original);

    stream.open();

    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.output(100), 4);
    EXPECT_EQ(stream.output(200), 5);

    Sequence<int>* result = stream.release_sequence();

    EXPECT_NE(result, original);

    EXPECT_EQ(original->get_size(), 3);
    EXPECT_EQ(original->get(0), 0);
    EXPECT_EQ(original->get(1), 1);
    EXPECT_EQ(original->get(2), 2);

    EXPECT_EQ(result->get_size(), 5);
    EXPECT_EQ(result->get(0), 0);
    EXPECT_EQ(result->get(1), 1);
    EXPECT_EQ(result->get(2), 2);
    EXPECT_EQ(result->get(3), 100);
    EXPECT_EQ(result->get(4), 200);

    delete original;
    delete result;
}

TEST(SequenceOutputStreamTest, LazySequenceReleaseWithoutWritesReturnsOriginalSequence) {
    Sequence<int>* original = new LazySequence<int>(
        plus_100_function,
        Ordinal(4)
    );

    SequenceOutputStream<int> stream(original);

    stream.open();

    Sequence<int>* result = stream.release_sequence();

    EXPECT_EQ(result, original);

    EXPECT_EQ(result->get_size(), 4);
    EXPECT_EQ(result->get(0), 100);
    EXPECT_EQ(result->get(1), 101);
    EXPECT_EQ(result->get(2), 102);
    EXPECT_EQ(result->get(3), 103);

    delete original;
}

TEST(SequenceOutputStreamTest, WritesToEmptyLazySequence) {
    Sequence<int>* original = new LazySequence<int>();

    SequenceOutputStream<int> stream(original);

    stream.open();

    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.output(10), 1);
    EXPECT_EQ(stream.output(20), 2);

    Sequence<int>* result = stream.release_sequence();

    EXPECT_NE(result, original);

    EXPECT_EQ(original->get_size(), 0);

    EXPECT_EQ(result->get_size(), 2);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);

    delete original;
    delete result;
}