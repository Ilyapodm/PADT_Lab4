#include <gtest/gtest.h>
#include <stdexcept>

#include "empty_generator.hpp"

TEST(EmptyGeneratorTest, basic_behavior) {
    EmptyGenerator<int> generator;

    EXPECT_FALSE(generator.has_next());
    EXPECT_TRUE(generator.supports_get_at());
    EXPECT_EQ(generator.length(), Ordinal(0));

    EXPECT_THROW(generator.get_next(), std::logic_error);

    // empty generator can handle get_at, but there is no valid indexes
    EXPECT_THROW(generator.get_at(Ordinal(0)), std::out_of_range);
}

TEST(EmptyGeneratorTest, clone_preserves_empty_behavior) {
    EmptyGenerator<int> generator;

    Generator<int>* clone = generator.clone();

    ASSERT_NE(clone, nullptr);
    EXPECT_FALSE(clone->has_next());
    EXPECT_TRUE(clone->supports_get_at());
    EXPECT_EQ(clone->length(), Ordinal(0));

    EXPECT_THROW(clone->get_next(), std::logic_error);
    EXPECT_THROW(clone->get_at(Ordinal(0)), std::out_of_range);

    delete clone;
}
