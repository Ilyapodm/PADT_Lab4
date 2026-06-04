#include <gtest/gtest.h>
#include <stdexcept>

#include "streams/sequence_output_stream.hpp"
#include "adt_lab_2/array_sequence.hpp"

TEST(SequenceOutputStreamTest, WritesToMutableArraySequence) {
    int items[] = {1, 2, 3};

    Sequence<int>* sequence = new MutableArraySequence<int>(items, 3);
    Sequence<int>* original = sequence;

    SequenceOutputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.output(4), 4);
    EXPECT_EQ(stream.output(5), 5);

    EXPECT_EQ(sequence, original);
    EXPECT_EQ(stream.get_sequence(), sequence);

    EXPECT_EQ(sequence->get_size(), 5);
    EXPECT_EQ(sequence->get(0), 1);
    EXPECT_EQ(sequence->get(1), 2);
    EXPECT_EQ(sequence->get(2), 3);
    EXPECT_EQ(sequence->get(3), 4);
    EXPECT_EQ(sequence->get(4), 5);

    delete sequence;
}

TEST(SequenceOutputStreamTest, MutableArraySequencePositionStartsFromCurrentSize) {
    int items[] = {10, 20};

    Sequence<int>* sequence = new MutableArraySequence<int>(items, 2);

    SequenceOutputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 2);

    stream.output(30);

    EXPECT_EQ(stream.get_position(), 3);
    EXPECT_EQ(sequence->get_size(), 3);
    EXPECT_EQ(sequence->get(2), 30);

    delete sequence;
}

TEST(SequenceOutputStreamTest, WritesToImmutableArraySequenceAndUpdatesPointer) {
    int items[] = {1, 2, 3};

    Sequence<int>* sequence = new ImmutableArraySequence<int>(items, 3);
    Sequence<int>* original = sequence;

    SequenceOutputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 3);

    EXPECT_EQ(stream.output(4), 4);

    Sequence<int>* after_first_output = sequence;

    EXPECT_NE(after_first_output, original);
    EXPECT_EQ(stream.get_sequence(), sequence);

    EXPECT_EQ(original->get_size(), 3);
    EXPECT_EQ(original->get(0), 1);
    EXPECT_EQ(original->get(1), 2);
    EXPECT_EQ(original->get(2), 3);

    EXPECT_EQ(sequence->get_size(), 4);
    EXPECT_EQ(sequence->get(0), 1);
    EXPECT_EQ(sequence->get(1), 2);
    EXPECT_EQ(sequence->get(2), 3);
    EXPECT_EQ(sequence->get(3), 4);

    delete original;
    delete after_first_output;
}

TEST(SequenceOutputStreamTest, MultipleWritesToImmutableArraySequenceUpdatePointerEachTime) {
    int items[] = {5, 6};

    Sequence<int>* sequence = new ImmutableArraySequence<int>(items, 2);
    Sequence<int>* original = sequence;

    SequenceOutputStream<int> stream(sequence);

    EXPECT_EQ(stream.output(7), 3);
    Sequence<int>* after_first_output = sequence;

    EXPECT_EQ(stream.output(8), 4);
    Sequence<int>* after_second_output = sequence;

    EXPECT_NE(after_first_output, original);
    EXPECT_NE(after_second_output, after_first_output);

    EXPECT_EQ(sequence->get_size(), 4);
    EXPECT_EQ(sequence->get(0), 5);
    EXPECT_EQ(sequence->get(1), 6);
    EXPECT_EQ(sequence->get(2), 7);
    EXPECT_EQ(sequence->get(3), 8);

    EXPECT_EQ(original->get_size(), 2);
    EXPECT_EQ(original->get(0), 5);
    EXPECT_EQ(original->get(1), 6);

    EXPECT_EQ(after_first_output->get_size(), 3);
    EXPECT_EQ(after_first_output->get(0), 5);
    EXPECT_EQ(after_first_output->get(1), 6);
    EXPECT_EQ(after_first_output->get(2), 7);

    delete original;
    delete after_first_output;
    delete after_second_output;
}

TEST(SequenceOutputStreamTest, WritesToEmptyMutableArraySequence) {
    Sequence<int>* sequence = new MutableArraySequence<int>();

    SequenceOutputStream<int> stream(sequence);

    EXPECT_EQ(stream.get_position(), 0);

    EXPECT_EQ(stream.output(10), 1);
    EXPECT_EQ(stream.output(20), 2);

    EXPECT_EQ(sequence->get_size(), 2);
    EXPECT_EQ(sequence->get(0), 10);
    EXPECT_EQ(sequence->get(1), 20);

    delete sequence;
}

TEST(SequenceOutputStreamTest, ConstructorThrowsOnNullTarget) {
    Sequence<int>* sequence = nullptr;

    EXPECT_THROW(
        SequenceOutputStream<int> stream(sequence),
        std::invalid_argument
    );
}

TEST(SequenceOutputStreamTest, OutputAfterCloseThrows) {
    int items[] = {1, 2};

    Sequence<int>* sequence = new MutableArraySequence<int>(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.close();

    EXPECT_THROW(stream.output(3), std::logic_error);

    delete sequence;
}

TEST(SequenceOutputStreamTest, OpenAfterCloseAllowsWritingAgain) {
    int items[] = {1, 2};

    Sequence<int>* sequence = new MutableArraySequence<int>(items, 2);

    SequenceOutputStream<int> stream(sequence);

    stream.close();
    stream.open();

    EXPECT_EQ(stream.get_position(), 2);

    EXPECT_EQ(stream.output(3), 3);

    EXPECT_EQ(sequence->get_size(), 3);
    EXPECT_EQ(sequence->get(0), 1);
    EXPECT_EQ(sequence->get(1), 2);
    EXPECT_EQ(sequence->get(2), 3);

    delete sequence;
}