#include <gtest/gtest.h>

#include "lazy_sequence/lazy_sequence.hpp"
#include "streams/lazy_sequence_output_stream.hpp"
#include "core/ordinal.hpp"

#include <cstddef>
#include <stdexcept>


static int index_times_ten(std::size_t index) {
    return static_cast<int>(index * 10);
}


TEST(LazySequenceOutputStreamTests, ConstructorRejectsNullTarget) {
    EXPECT_THROW(
        {
            LazySequenceOutputStream<int> stream(static_cast<LazySequence<int>*>(nullptr));
        },
        std::invalid_argument
    );
}


TEST(LazySequenceOutputStreamTests, ClosedStreamOperationsThrow) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceOutputStream<int> stream(sequence);

    EXPECT_FALSE(stream.is_open());

    EXPECT_THROW(stream.output(40), std::logic_error);
    EXPECT_THROW(stream.output_ordinal(40), std::logic_error);
    EXPECT_THROW(stream.get_position(), std::logic_error);
    EXPECT_THROW(stream.get_ordinal_position(), std::logic_error);
}


TEST(LazySequenceOutputStreamTests, OpenSetsFinitePositionToSequenceLength) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.is_open());
    EXPECT_EQ(stream.get_position(), 3u);
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(3));
}


TEST(LazySequenceOutputStreamTests, OpenTwiceThrows) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_THROW(stream.open(), std::logic_error);
}


TEST(LazySequenceOutputStreamTests, OutputAppendsToFiniteSequenceAndReturnsFinitePosition) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.output(30), 3u);
    EXPECT_EQ(stream.output(40), 4u);

    LazySequence<int>* result = stream.release_sequence();

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 4);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);
    EXPECT_EQ(result->get(3), 40);

    EXPECT_EQ(sequence.get_size(), 2);
    EXPECT_EQ(sequence.get(0), 10);
    EXPECT_EQ(sequence.get(1), 20);

    delete result;
}


TEST(LazySequenceOutputStreamTests, OutputOrdinalAppendsToFiniteSequenceAndReturnsOrdinalPosition) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.output_ordinal(30) == Ordinal(3));
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(3));

    EXPECT_TRUE(stream.output_ordinal(40) == Ordinal(4));
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(4));

    LazySequence<int>* result = stream.release_sequence();

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 4);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);
    EXPECT_EQ(result->get(3), 40);

    delete result;
}


TEST(LazySequenceOutputStreamTests, OutputOnInfiniteSequenceThrowsWithoutChangingPosition) {
    LazySequence<int> sequence(index_times_ten, Ordinal(1, 0));
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 0));

    EXPECT_THROW(stream.output(100), std::logic_error);

    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 0));
}


TEST(LazySequenceOutputStreamTests, OutputOrdinalAppendsAfterOmegaLength) {
    LazySequence<int> sequence(index_times_ten, Ordinal(1, 0));
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 0));

    EXPECT_TRUE(stream.output_ordinal(100) == Ordinal(1, 1));
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 1));

    EXPECT_TRUE(stream.output_ordinal(200) == Ordinal(1, 2));
    EXPECT_TRUE(stream.get_ordinal_position() == Ordinal(1, 2));

    EXPECT_THROW(stream.get_position(), std::logic_error);

    LazySequence<int>* result = stream.release_sequence();

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get(Ordinal(0)), 0);
    EXPECT_EQ(result->get(Ordinal(5)), 50);
    EXPECT_EQ(result->get(Ordinal(1, 0)), 100);
    EXPECT_EQ(result->get(Ordinal(1, 1)), 200);

    delete result;
}


TEST(LazySequenceOutputStreamTests, ReleaseWithoutOutputReturnsOriginalTargetPointer) {
    int items[] = {10, 20, 30};
    LazySequence<int> sequence(items, 3);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    LazySequence<int>* result = stream.release_sequence();

    EXPECT_EQ(result, &sequence);
    EXPECT_FALSE(stream.is_open());

    EXPECT_THROW(stream.open(), std::logic_error);
    EXPECT_THROW(stream.output(40), std::logic_error);
    EXPECT_THROW(stream.output_ordinal(40), std::logic_error);
}


TEST(LazySequenceOutputStreamTests, ReleaseAfterOutputReturnsResultAndDisablesStream) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.output(30), 3u);

    LazySequence<int>* result = stream.release_sequence();

    ASSERT_NE(result, nullptr);
    EXPECT_NE(result, &sequence);

    EXPECT_FALSE(stream.is_open());

    EXPECT_THROW(stream.open(), std::logic_error);
    EXPECT_THROW(stream.output(40), std::logic_error);
    EXPECT_THROW(stream.output_ordinal(40), std::logic_error);
    EXPECT_THROW(stream.release_sequence(), std::logic_error);

    EXPECT_EQ(result->get_size(), 3);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete result;
}


TEST(LazySequenceOutputStreamTests, CloseAllowsOpeningAgain) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_TRUE(stream.is_open());
    EXPECT_EQ(stream.get_position(), 2u);

    stream.close();

    EXPECT_FALSE(stream.is_open());

    stream.open();

    EXPECT_TRUE(stream.is_open());
    EXPECT_EQ(stream.get_position(), 2u);
}


TEST(LazySequenceOutputStreamTests, CloseDoesNotReleaseResult) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.output(30), 3u);

    stream.close();

    EXPECT_FALSE(stream.is_open());

    LazySequence<int>* result = stream.release_sequence();

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 3);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete result;
}


TEST(LazySequenceOutputStreamTests, ExternalSourceSequenceIsNotModified) {
    int items[] = {10, 20};
    LazySequence<int> sequence(items, 2);
    LazySequenceOutputStream<int> stream(sequence);

    stream.open();

    EXPECT_EQ(stream.output(30), 3u);
    EXPECT_EQ(stream.output(40), 4u);

    EXPECT_EQ(sequence.get_size(), 2);
    EXPECT_EQ(sequence.get(0), 10);
    EXPECT_EQ(sequence.get(1), 20);

    LazySequence<int>* result = stream.release_sequence();

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 4);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);
    EXPECT_EQ(result->get(3), 40);

    delete result;
}