#include <gtest/gtest.h>
#include <stdexcept>

#include "empty_generator.hpp"

TEST(EmptyGeneratorTest, BasicBehavior) {
    EmptyGenerator<int> generator;

    EXPECT_FALSE(generator.has_next());
    EXPECT_EQ(generator.length(), Ordinal(0));

    EXPECT_THROW(generator.get_next(), std::logic_error);
}

TEST(EmptyGeneratorTest, ClonePreservesEmptyBehavior) {
    EmptyGenerator<int> generator;

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);
    EXPECT_FALSE(clone->has_next());
    EXPECT_EQ(clone->length(), Ordinal(0));

    EXPECT_THROW(clone->get_next(), std::logic_error);
    delete clone;
}
