#include <gtest/gtest.h>
#include <functional>
#include <stdexcept>

#include "generators/function_generator.hpp"
#include "core/ordinal.hpp"

TEST(FunctionGeneratorTest, FiniteGeneratorProducesValuesInOrder) {
    FunctionGenerator<int> gen(
        [](std::size_t index) {
            return static_cast<int>(index * index);
        },
        Ordinal(5)
    );

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 0);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 1);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 4);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 9);

    EXPECT_TRUE(gen.has_next());
    EXPECT_EQ(gen.get_next(), 16);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(FunctionGeneratorTest, ZeroLengthGeneratorIsImmediatelyExhausted) {
    FunctionGenerator<int> gen(
        [](std::size_t index) {
            return static_cast<int>(index);
        },
        Ordinal(0)
    );

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}



TEST(FunctionGeneratorTest, ConstructorThrowsOnEmptyFunction) {
    std::function<int(std::size_t)> empty_func;

    EXPECT_THROW(
        FunctionGenerator<int> gen(empty_func, Ordinal(10)),
        std::invalid_argument
    );
}

TEST(FunctionGeneratorTest, ConstructorAcceptsFiniteLength) {
    EXPECT_NO_THROW(
        FunctionGenerator<int> gen(
            [](std::size_t index) {
                return static_cast<int>(index);
            },
            Ordinal(10)
        )
    );
}

TEST(FunctionGeneratorTest, ConstructorAcceptsOmegaLength) {
    EXPECT_NO_THROW(
        FunctionGenerator<int> gen(
            [](std::size_t index) {
                return static_cast<int>(index);
            },
            Ordinal::omega()
        )
    );
}

TEST(FunctionGeneratorTest, ConstructorRejectsLengthGreaterThanOmega) {
    EXPECT_THROW(
        FunctionGenerator<int> gen(
            [](std::size_t index) {
                return static_cast<int>(index);
            },
            Ordinal(1, 1)
        ),
        std::invalid_argument
    );

    EXPECT_THROW(
        FunctionGenerator<int> gen(
            [](std::size_t index) {
                return static_cast<int>(index);
            },
            Ordinal(2, 0)
        ),
        std::invalid_argument
    );

    EXPECT_THROW(
        FunctionGenerator<int> gen(
            [](std::size_t index) {
                return static_cast<int>(index);
            },
            Ordinal(2, 5)
        ),
        std::invalid_argument
    );
}

TEST(FunctionGeneratorTest, LengthReturnsConstructorLength) {
    FunctionGenerator<int> finite_gen(
        [](std::size_t index) {
            return static_cast<int>(index);
        },
        Ordinal(42)
    );

    EXPECT_EQ(finite_gen.length().get_omega_coeff(), 0);
    EXPECT_EQ(finite_gen.length().get_finite_part(), 42);

    FunctionGenerator<int> infinite_gen(
        [](std::size_t index) {
            return static_cast<int>(index);
        },
        Ordinal::omega()
    );

    EXPECT_EQ(infinite_gen.length().get_omega_coeff(), 1);
    EXPECT_EQ(infinite_gen.length().get_finite_part(), 0);
}

TEST(FunctionGeneratorTest, CloneCopiesCurrentState) {
    FunctionGenerator<int> gen(
        [](std::size_t index) {
            return static_cast<int>(index * 10);
        },
        Ordinal(10)
    );

    EXPECT_EQ(gen.get_next(), 0);
    EXPECT_EQ(gen.get_next(), 10);

    FunctionGenerator<int>* cloned = gen.clone();

    EXPECT_EQ(gen.get_next(), 20);
    EXPECT_EQ(cloned->get_next(), 20);

    EXPECT_EQ(gen.get_next(), 30);
    EXPECT_EQ(cloned->get_next(), 30);

    delete cloned;
}