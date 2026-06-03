#include <gtest/gtest.h>
#include <stdexcept>

#include "generators/sequence_generator.hpp"
#include "adt_lab_2/array_sequence.hpp"
#include "adt_lab_2/list_sequence.hpp"

TEST(SequenceGeneratorTest, ReadsMutableArraySequenceByGetNext) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(source);

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.length(), Ordinal(3));

    EXPECT_EQ(generator.get_next(), 10);
    EXPECT_EQ(generator.get_next(), 20);
    EXPECT_EQ(generator.get_next(), 30);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, ReadsMutableListSequenceByGetNext) {
    int items[] = {5, 15, 25, 35};
    MutableListSequence<int> source(items, 4);

    SequenceGenerator<int> generator(source);

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.length(), Ordinal(4));

    EXPECT_EQ(generator.get_next(), 5);
    EXPECT_EQ(generator.get_next(), 15);
    EXPECT_EQ(generator.get_next(), 25);
    EXPECT_EQ(generator.get_next(), 35);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, CopiesSequenceThroughBaseReference) {
    int items[] = {1, 2, 3, 4};
    MutableListSequence<int> source(items, 4);

    const Sequence<int>& base_source = source;

    SequenceGenerator<int> generator(base_source);

    EXPECT_EQ(generator.length(), Ordinal(4));

    EXPECT_EQ(generator.get_next(), 1);
    EXPECT_EQ(generator.get_next(), 2);
    EXPECT_EQ(generator.get_next(), 3);
    EXPECT_EQ(generator.get_next(), 4);

    EXPECT_FALSE(generator.has_next());
}

TEST(SequenceGeneratorTest, ClonePreservesCurrentReadPosition) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(source);

    EXPECT_EQ(generator.get_next(), 10);
    EXPECT_EQ(generator.get_next(), 20);

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);

    EXPECT_TRUE(clone->has_next());
    EXPECT_EQ(clone->get_next(), 30);
    EXPECT_FALSE(clone->has_next());

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 30);
    EXPECT_FALSE(generator.has_next());

    delete clone;
}

TEST(SequenceGeneratorTest, EmptySequenceHasNoNext) {
    MutableArraySequence<int> source;

    SequenceGenerator<int> generator(source);

    EXPECT_EQ(generator.length(), Ordinal(0));
    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, SnapshotIsIndependentFromMutableArraySequenceAfterAppend) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(source);

    source.append(999);

    EXPECT_EQ(generator.length(), Ordinal(3));

    EXPECT_EQ(generator.get_next(), 1);
    EXPECT_EQ(generator.get_next(), 2);
    EXPECT_EQ(generator.get_next(), 3);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, SnapshotIsIndependentFromMutableListSequenceAfterAppend) {
    int items[] = {10, 20, 30};
    MutableListSequence<int> source(items, 3);

    SequenceGenerator<int> generator(source);

    source.append(999);

    EXPECT_EQ(generator.length(), Ordinal(3));

    EXPECT_EQ(generator.get_next(), 10);
    EXPECT_EQ(generator.get_next(), 20);
    EXPECT_EQ(generator.get_next(), 30);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, SnapshotIsIndependentFromMutableArraySequenceAfterSet) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(source);

    source.set(999, 0);
    source.set(888, 1);
    source.set(777, 2);

    EXPECT_EQ(generator.length(), Ordinal(3));

    EXPECT_EQ(generator.get_next(), 1);
    EXPECT_EQ(generator.get_next(), 2);
    EXPECT_EQ(generator.get_next(), 3);

    EXPECT_FALSE(generator.has_next());
}

TEST(SequenceGeneratorTest, SnapshotIsIndependentFromMutableListSequenceAfterSet) {
    int items[] = {10, 20, 30};
    MutableListSequence<int> source(items, 3);

    SequenceGenerator<int> generator(source);

    source.set(999, 0);
    source.set(888, 1);
    source.set(777, 2);

    EXPECT_EQ(generator.length(), Ordinal(3));

    EXPECT_EQ(generator.get_next(), 10);
    EXPECT_EQ(generator.get_next(), 20);
    EXPECT_EQ(generator.get_next(), 30);

    EXPECT_FALSE(generator.has_next());
}

TEST(SequenceGeneratorTest, SnapshotWorksThroughBaseSequenceReference) {
    int items[] = {4, 5, 6};
    MutableListSequence<int> source(items, 3);

    const Sequence<int>& base_source = source;

    SequenceGenerator<int> generator(base_source);

    source.append(999);

    EXPECT_EQ(generator.length(), Ordinal(3));

    EXPECT_EQ(generator.get_next(), 4);
    EXPECT_EQ(generator.get_next(), 5);
    EXPECT_EQ(generator.get_next(), 6);

    EXPECT_FALSE(generator.has_next());
}