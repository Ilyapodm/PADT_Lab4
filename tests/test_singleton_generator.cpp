#include <gtest/gtest.h>
#include <stdexcept>

#include "singleton_generator.hpp"

TEST(SingletonGeneratorTest, basic_behavior) {
    SingletonGenerator<int> generator(42);

    EXPECT_TRUE(generator.has_next());
    EXPECT_TRUE(generator.supports_get_at());
    EXPECT_EQ(generator.length(), Ordinal(1));

    EXPECT_EQ(generator.get_at(Ordinal(0)), 42);

    EXPECT_EQ(generator.get_next(), 42);

    EXPECT_FALSE(generator.has_next());
    EXPECT_THROW(generator.get_next(), std::out_of_range);
}

TEST(SingletonGeneratorTest, get_at_checks_bounds) {
    SingletonGenerator<int> generator(42);

    EXPECT_EQ(generator.get_at(Ordinal(0)), 42);

    EXPECT_THROW(generator.get_at(Ordinal(1)), std::out_of_range);
    EXPECT_THROW(generator.get_at(Ordinal::omega()), std::out_of_range);
}

TEST(SingletonGeneratorTest, get_at_does_not_depend_on_get_next_state) {
    SingletonGenerator<int> generator(42);

    EXPECT_EQ(generator.get_next(), 42);
    EXPECT_FALSE(generator.has_next());

    // get_at работает независимо от последовательного чтения
    EXPECT_EQ(generator.get_at(Ordinal(0)), 42);
}

TEST(SingletonGeneratorTest, clone_preserves_state_before_reading) {
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

TEST(SingletonGeneratorTest, clone_preserves_state_after_reading) {
    SingletonGenerator<int> generator(42);

    EXPECT_EQ(generator.get_next(), 42);
    EXPECT_FALSE(generator.has_next());

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);

    EXPECT_FALSE(clone->has_next());
    EXPECT_THROW(clone->get_next(), std::out_of_range);

    // get_at still works
    EXPECT_EQ(clone->get_at(Ordinal(0)), 42);

    delete clone;
}