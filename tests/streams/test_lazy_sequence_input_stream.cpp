#include <gtest/gtest.h>

#include "lazy_sequence/lazy_sequence.hpp"
#include "streams/lazy_sequence_input_stream.hpp"
#include "core/ordinal.hpp"

#include <cstddef>
#include <stdexcept>


static int index_times_ten(std::size_t index) {
    return static_cast<int>(index * 10);
}


TEST(LazySequenceInputStreamTests, ConstructorRejectsNullSource) {
    EXPECT_THROW(
        {
            LazySequenceInputStream<int> stream(static_cast<LazySequence<int>*>(nullptr));
        },
        std::invalid_argument
    );
}


TEST(LazySequenceInputStreamTests, ClosedStreamOperationsThrow) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    EXPECT_FALSE(stream.is_open());

    EXPECT_THROW(stream.is_end_of_stream(), std::logic_error);
    EXPECT_THROW(stream.input(), std::logic_error);
    EXPECT_THROW(stream.get_position(), std::logic_error);
    EXPECT_THROW(stream.get_ordinal_position(), std::logic_error);
    EXPECT_THROW(stream.seek(1), std::logic_error);
    EXPECT_THROW(stream.seek(Ordinal(1)), std::logic_error);
    EXPECT_THROW(stream.reset(), std::logic_error);
}


TEST(LazySequenceInputStreamTests, OpenSetsFinitePositionToZero) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.is_open());
    EXPECT_FALSE(stream.is_end_of_stream());
    EXPECT_EQ(stream.get_position(), 0u);
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(0));
}


TEST(LazySequenceInputStreamTests, OpenTwiceThrows) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_THROW(stream.open(), std::logic_error);
}


TEST(LazySequenceInputStreamTests, ReadsFiniteSequenceInOrder) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.get_position(), 1u);

    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position(), 2u);

    EXPECT_EQ(stream.input(), 30);
    EXPECT_EQ(stream.get_position(), 3u);

    EXPECT_TRUE(stream.is_end_of_stream());
}


TEST(LazySequenceInputStreamTests, InputThrowsAfterEnd) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.input(), 20);

    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::out_of_range);
}


TEST(LazySequenceInputStreamTests, ResetMovesStreamToBeginning) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.input(), 10);
    EXPECT_EQ(stream.input(), 20);
    EXPECT_EQ(stream.get_position(), 2u);

    stream.reset();

    EXPECT_EQ(stream.get_position(), 0u);
    EXPECT_FALSE(stream.is_end_of_stream());
    EXPECT_EQ(stream.input(), 10);
}


TEST(LazySequenceInputStreamTests, SeekFiniteIndexMovesWithoutReadingPreviousItems) {
    int items[] = {10, 20, 30, 40, 50};
    LazySequence<int> sequence(items, 5);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.seek(3), 3u);
    EXPECT_EQ(stream.get_position(), 3u);

    EXPECT_EQ(stream.input(), 40);
    EXPECT_EQ(stream.get_position(), 4u);

    EXPECT_EQ(stream.input(), 50);
    EXPECT_EQ(stream.get_position(), 5u);

    EXPECT_TRUE(stream.is_end_of_stream());
}


TEST(LazySequenceInputStreamTests, SeekToFiniteEndMakesStreamEnded) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.seek(3), 3u);
    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::out_of_range);
}


TEST(LazySequenceInputStreamTests, SeekFiniteOutOfRangeThrows) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_THROW(stream.seek(4), std::out_of_range);
}


TEST(LazySequenceInputStreamTests, CanSeekAndCanGoBackAreTrue) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    EXPECT_TRUE(stream.can_seek());
    EXPECT_TRUE(stream.can_go_back());
}


TEST(LazySequenceInputStreamTests, OrdinalSeekReadsFiniteTailAfterOmegaPrefix) {
    LazySequence<int> infinite_sequence(index_times_ten, Ordinal(1, 0));

    int tail_items[] = {100, 200, 300};
    LazySequence<int> tail_sequence(tail_items, 3);

    LazySequence<int>* joined_sequence = infinite_sequence.concat(tail_sequence);

    LazySequenceInputStream<int> stream(*joined_sequence);

    stream.open();

    EXPECT_TRUE(stream.seek(Ordinal(1, 0)) == Ordinal(1, 0));
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 0));

    EXPECT_THROW(stream.get_position(), std::logic_error);

    EXPECT_FALSE(stream.is_end_of_stream());
    EXPECT_EQ(stream.input(), 100);
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 1));

    EXPECT_EQ(stream.input(), 200);
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 2));

    EXPECT_EQ(stream.input(), 300);
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 3));

    EXPECT_TRUE(stream.is_end_of_stream());

    delete joined_sequence;
}


TEST(LazySequenceInputStreamTests, OrdinalSeekToEndAfterOmegaTailMakesStreamEnded) {
    LazySequence<int> infinite_sequence(index_times_ten, Ordinal(1, 0));

    int tail_items[] = {100, 200};
    LazySequence<int> tail_sequence(tail_items, 2);

    LazySequence<int>* joined_sequence = infinite_sequence.concat(tail_sequence);

    LazySequenceInputStream<int> stream(*joined_sequence);

    stream.open();

    EXPECT_TRUE(stream.seek(Ordinal(1, 2)) == Ordinal(1, 2));
    EXPECT_TRUE(stream.is_end_of_stream());

    EXPECT_THROW(stream.input(), std::out_of_range);

    delete joined_sequence;
}


TEST(LazySequenceInputStreamTests, OrdinalSeekOutOfRangeThrows) {
    LazySequence<int> infinite_sequence(index_times_ten, Ordinal(1, 0));

    int tail_items[] = {100, 200};
    LazySequence<int> tail_sequence(tail_items, 2);

    LazySequence<int>* joined_sequence = infinite_sequence.concat(tail_sequence);

    LazySequenceInputStream<int> stream(*joined_sequence);

    stream.open();

    EXPECT_THROW(stream.seek(Ordinal(1, 3)), std::out_of_range);

    delete joined_sequence;
}


TEST(LazySequenceInputStreamTests, CloseMakesStreamClosed) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceInputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.is_open());

    stream.close();

    EXPECT_FALSE(stream.is_open());
    EXPECT_THROW(stream.input(), std::logic_error);
}