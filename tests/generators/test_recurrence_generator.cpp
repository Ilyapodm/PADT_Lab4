#include <gtest/gtest.h>
#include <stdexcept>

#include "generators/recurrence_generator.hpp"
#include "adt/ring_buffer.hpp"
#include "core/ordinal.hpp"

TEST(RecurrenceGeneratorTest, GeneratesFibonacciSequence) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        [](const RingBuffer<int>& window) {
            return window.get(0) + window.get(1);
        },
        initial,
        2,
        Ordinal(7)
    );

    EXPECT_TRUE(gen.has_next());

    EXPECT_EQ(gen.get_next(), 0);
    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(gen.get_next(), 5);
    EXPECT_EQ(gen.get_next(), 8);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(RecurrenceGeneratorTest, GeneratesTribonacciSequence) {
    int initial[] = {0, 1, 1};

    RecurrenceGenerator<int> gen(
        [](const RingBuffer<int>& window) {
            return window.get(0) + window.get(1) + window.get(2);
        },
        initial,
        3,
        Ordinal(8)
    );

    EXPECT_EQ(gen.get_next(), 0);
    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(gen.get_next(), 4);
    EXPECT_EQ(gen.get_next(), 7);
    EXPECT_EQ(gen.get_next(), 13);
    EXPECT_EQ(gen.get_next(), 24);

    EXPECT_FALSE(gen.has_next());
}

TEST(RecurrenceGeneratorTest, ReturnsOnlyInitialValuesIfLengthLessThanCount) {
    int initial[] = {10, 20, 30};

    RecurrenceGenerator<int> gen(
        [](const RingBuffer<int>& window) {
            return window.get(0) + window.get(1) + window.get(2);
        },
        initial,
        3,
        Ordinal(2)
    );

    EXPECT_EQ(gen.get_next(), 10);
    EXPECT_EQ(gen.get_next(), 20);

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(RecurrenceGeneratorTest, SupportsZeroLengthSequence) {
    int initial[] = {1, 1};

    RecurrenceGenerator<int> gen(
        [](const RingBuffer<int>& window) {
            return window.get(0) + window.get(1);
        },
        initial,
        2,
        Ordinal(0)
    );

    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

TEST(RecurrenceGeneratorTest, ThrowsIfFunctionIsEmpty) {
    int initial[] = {0, 1};

    std::function<int(const RingBuffer<int>&)> func;

    EXPECT_THROW(
        RecurrenceGenerator<int>(func, initial, 2, Ordinal(5)),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, ThrowsIfInitialCountIsZero) {
    int initial[] = {0};

    EXPECT_THROW(
        RecurrenceGenerator<int>(
            [](const RingBuffer<int>& window) {
                return 0;
            },
            initial,
            0,
            Ordinal(5)
        ),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, CloneCopiesCurrentState) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        [](const RingBuffer<int>& window) {
            return window.get(0) + window.get(1);
        },
        initial,
        2,
        Ordinal(7)
    );

    EXPECT_EQ(gen.get_next(), 0);
    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 1);

    RecurrenceGenerator<int>* clone = gen.clone();

    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(clone->get_next(), 2);

    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(clone->get_next(), 3);

    delete clone;
}

TEST(RecurrenceGeneratorTest, CloneIsIndependentAfterCopy) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        [](const RingBuffer<int>& window) {
            return window.get(0) + window.get(1);
        },
        initial,
        2,
        Ordinal(7)
    );

    gen.get_next();
    gen.get_next();

    RecurrenceGenerator<int>* clone = gen.clone();

    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);

    EXPECT_EQ(clone->get_next(), 1);
    EXPECT_EQ(clone->get_next(), 2);

    delete clone;
}