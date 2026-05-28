#include <gtest/gtest.h>
#include <stdexcept>

#include "sequence_generator.hpp"
#include "array_sequence.hpp"

TEST(SequenceGeneratorTest, ReadsSequenceByGetNextAndGetAt) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(&source);

    EXPECT_TRUE(generator.has_next());
    EXPECT_TRUE(generator.supports_get_at());
    EXPECT_EQ(generator.length(), Ordinal(3));

    // get_at don't move inner index_
    EXPECT_EQ(generator.get_at(Ordinal(0)), 10);
    EXPECT_EQ(generator.get_at(Ordinal(1)), 20);
    EXPECT_EQ(generator.get_at(Ordinal(2)), 30);

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 10);

    // after get_next index has changed, but get_at still works independently
    EXPECT_EQ(generator.get_at(Ordinal(0)), 10);
    EXPECT_EQ(generator.get_at(Ordinal(2)), 30);

    EXPECT_EQ(generator.get_next(), 20);
    EXPECT_EQ(generator.get_next(), 30);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SequenceGeneratorTest, GetAtChecksBounds) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(&source);

    EXPECT_THROW(generator.get_at(Ordinal(3)), std::out_of_range);
    EXPECT_THROW(generator.get_at(Ordinal::omega()), std::out_of_range);
}

TEST(SequenceGeneratorTest, ClonePreservesCurrentReadPosition) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> source(items, 3);

    SequenceGenerator<int> generator(&source);

    EXPECT_EQ(generator.get_next(), 10);
    EXPECT_EQ(generator.get_next(), 20);

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);

    // clone's index_ = 2 too
    EXPECT_TRUE(clone->has_next());
    EXPECT_EQ(clone->get_next(), 30);
    EXPECT_FALSE(clone->has_next());

    // original has it's own index_
    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 30);
    EXPECT_FALSE(generator.has_next());

    delete clone;
}