#include <gtest/gtest.h>
#include <stdexcept>

#include "singleton_generator.hpp"

TEST(SingletonGeneratorTest, BasicBehavior) {
    SingletonGenerator<int> generator(42);

    EXPECT_TRUE(generator.has_next());

    EXPECT_EQ(generator.length(), Ordinal(1));


    EXPECT_EQ(generator.get_next(), 42);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}


TEST(SingletonGeneratorTest, ClonePreservesStateBeforeReading) {
    SingletonGenerator<int> generator(42);

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);

    EXPECT_TRUE(clone->has_next());
    EXPECT_EQ(clone->get_next(), 42);
    EXPECT_FALSE(clone->has_next());

    EXPECT_TRUE(generator.has_next());
    EXPECT_EQ(generator.get_next(), 42);

    delete clone;
}

TEST(SingletonGeneratorTest, ClonePreservesStateAfterReading) {
    SingletonGenerator<int> generator(42);

    EXPECT_EQ(generator.get_next(), 42);
    EXPECT_FALSE(generator.has_next());

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);

    EXPECT_FALSE(clone->has_next());
    EXPECT_THROW(clone->get_next(), std::out_of_range);

    delete clone;
}