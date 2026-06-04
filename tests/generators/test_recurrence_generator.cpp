#include <gtest/gtest.h>

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>

#include "generators/recurrence_generator.hpp"
#include "adt/ring_buffer.hpp"
#include "core/ordinal.hpp"


// ---------- Rule functions ----------

static int fibonacci_ring_rule(const RingBuffer<int>& window) {
    return window.get(0) + window.get(1);
}

static int tribonacci_ring_Rule(const RingBuffer<int>& window) {
    return window.get(0) + window.get(1) + window.get(2);
}

static int zero_ring_rule(const RingBuffer<int>&) {
    return 0;
}

static int fibonacci_array_rule(const int* window, std::size_t count) {
    if (count != 2) {
        throw std::invalid_argument("fibonacci_array_rule: count must be 2");
    }

    return window[0] + window[1];
}

static int tribonacci_array_rule(const int* window, std::size_t count) {
    if (count != 3) {
        throw std::invalid_argument("tribonacci_array_rule: count must be 3");
    }

    return window[0] + window[1] + window[2];
}

static int zero_array_rule(const int*, std::size_t) {
    return 0;
}

// cheks, that array_window get elements in a right order:
// window[0] — the oldest
// window[count - 1] — the newest
static int ordered_array_rule(const int* window, std::size_t count) {
    if (count != 3) {
        throw std::invalid_argument("ordered_array_rule: count must be 3");
    }

    return 100 * window[0] + 10 * window[1] + window[2];
}

// ---------- Test helpers ----------

static void ExpectGenerated(
    RecurrenceGenerator<int>& gen,
    std::initializer_list<int> expected_values
) {
    std::size_t index = 0;

    for (int expected : expected_values) {
        SCOPED_TRACE(::testing::Message() << "Generated index: " << index);

        ASSERT_TRUE(gen.has_next());
        EXPECT_EQ(gen.get_next(), expected);

        ++index;
    }
}

static void ExpectExhausted(RecurrenceGenerator<int>& gen) {
    EXPECT_FALSE(gen.has_next());
    EXPECT_THROW(gen.get_next(), std::out_of_range);
}

// ---------- Ring-rule tests ----------

TEST(RecurrenceGeneratorTest, GeneratesFibonacciSequenceWithRingRule) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_ring_rule,
        initial,
        2,
        Ordinal(7)
    );

    ExpectGenerated(gen, {0, 1, 1, 2, 3, 5, 8});
    ExpectExhausted(gen);
}

TEST(RecurrenceGeneratorTest, GeneratesTribonacciSequenceWithRingRule) {
    int initial[] = {0, 1, 1};

    RecurrenceGenerator<int> gen(
        tribonacci_ring_Rule,
        initial,
        3,
        Ordinal(8)
    );

    ExpectGenerated(gen, {0, 1, 1, 2, 4, 7, 13, 24});

    EXPECT_FALSE(gen.has_next());
}

TEST(RecurrenceGeneratorTest, ReturnsOnlyInitialValuesIfLengthLessThanWindowSizeWithRingRule) {
    int initial[] = {10, 20, 30};

    RecurrenceGenerator<int> gen(
        tribonacci_ring_Rule,
        initial,
        3,
        Ordinal(2)
    );

    ExpectGenerated(gen, {10, 20});
    ExpectExhausted(gen);
}

TEST(RecurrenceGeneratorTest, SupportsZeroLengthSequenceWithRingRule) {
    int initial[] = {1, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_ring_rule,
        initial,
        2,
        Ordinal(0)
    );

    ExpectExhausted(gen);
}

// ---------- Array-rule tests ----------

TEST(RecurrenceGeneratorTest, GeneratesFibonacciSequenceWithArrayRule) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_array_rule,
        initial,
        2,
        Ordinal(7)
    );

    ExpectGenerated(gen, {0, 1, 1, 2, 3, 5, 8});
    ExpectExhausted(gen);
}

TEST(RecurrenceGeneratorTest, GeneratesTribonacciSequenceWithArrayRule) {
    int initial[] = {0, 1, 1};

    RecurrenceGenerator<int> gen(
        tribonacci_array_rule,
        initial,
        3,
        Ordinal(8)
    );

    ExpectGenerated(gen, {0, 1, 1, 2, 4, 7, 13, 24});

    EXPECT_FALSE(gen.has_next());
}

TEST(RecurrenceGeneratorTest, ReturnsOnlyInitialValuesIfLengthLessThanWindowSizeWithArrayRule) {
    int initial[] = {10, 20, 30};

    RecurrenceGenerator<int> gen(
        tribonacci_array_rule,
        initial,
        3,
        Ordinal(2)
    );

    ExpectGenerated(gen, {10, 20});
    ExpectExhausted(gen);
}

TEST(RecurrenceGeneratorTest, SupportsZeroLengthSequenceWithArrayRule) {
    int initial[] = {1, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_array_rule,
        initial,
        2,
        Ordinal(0)
    );

    ExpectExhausted(gen);
}

TEST(RecurrenceGeneratorTest, ArrayRuleReceivesWindowInLogicalOrder) {
    int initial[] = {1, 2, 3};

    RecurrenceGenerator<int> gen(
        ordered_array_rule,
        initial,
        3,
        Ordinal(5)
    );

    // first 3 values - initial, then
    // window = [1, 2, 3]      -> 123
    // push(123)
    // window = [2, 3, 123]    -> 353
    //
    ExpectGenerated(gen, {1, 2, 3, 123, 353});

    EXPECT_FALSE(gen.has_next());
}

// ---------- Invalid constructor arguments ----------

TEST(RecurrenceGeneratorTest, ThrowsIfRingFunctionIsEmpty) {
    int initial[] = {0, 1};

    std::function<int(const RingBuffer<int>&)> func;

    EXPECT_THROW(
        RecurrenceGenerator<int>(func, initial, 2, Ordinal(5)),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, ThrowsIfArrayFunctionIsEmpty) {
    int initial[] = {0, 1};

    std::function<int(const int*, std::size_t)> func;

    EXPECT_THROW(
        RecurrenceGenerator<int>(func, initial, 2, Ordinal(5)),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, ThrowsIfInitialCountIsZeroWithRingRule) {
    int initial[] = {0};

    EXPECT_THROW(
        RecurrenceGenerator<int>(
            zero_ring_rule,
            initial,
            0,
            Ordinal(5)
        ),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, ThrowsIfInitialCountIsZeroWithArrayRule) {
    int initial[] = {0};

    EXPECT_THROW(
        RecurrenceGenerator<int>(
            zero_array_rule,
            initial,
            0,
            Ordinal(5)
        ),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, ThrowsIfInitialValuesIsNullWithRingRule) {
    EXPECT_THROW(
        RecurrenceGenerator<int>(
            zero_ring_rule,
            nullptr,
            2,
            Ordinal(5)
        ),
        std::invalid_argument
    );
}

TEST(RecurrenceGeneratorTest, ThrowsIfInitialValuesIsNullWithArrayRule) {
    EXPECT_THROW(
        RecurrenceGenerator<int>(
            zero_array_rule,
            nullptr,
            2,
            Ordinal(5)
        ),
        std::invalid_argument
    );
}

// ---------- Clone tests: ring mode ----------

TEST(RecurrenceGeneratorTest, CloneCopiesCurrentStateInRingMode) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_ring_rule,
        initial,
        2,
        Ordinal(7)
    );

    ExpectGenerated(gen, {0, 1, 1});

    RecurrenceGenerator<int>* clone = gen.clone();

    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(clone->get_next(), 2);

    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(clone->get_next(), 3);

    delete clone;
}

TEST(RecurrenceGeneratorTest, CloneIsIndependentAfterCopyInRingMode) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_ring_rule,
        initial,
        2,
        Ordinal(7)
    );

    ExpectGenerated(gen, {0, 1});

    RecurrenceGenerator<int>* clone = gen.clone();

    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);

    EXPECT_EQ(clone->get_next(), 1);
    EXPECT_EQ(clone->get_next(), 2);

    delete clone;
}

// ---------- Clone tests: array mode ----------

TEST(RecurrenceGeneratorTest, CloneCopiesCurrentStateInArrayMode) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_array_rule,
        initial,
        2,
        Ordinal(7)
    );

    ExpectGenerated(gen, {0, 1, 1});

    RecurrenceGenerator<int>* clone = gen.clone();

    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(clone->get_next(), 2);

    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(clone->get_next(), 3);

    delete clone;
}

TEST(RecurrenceGeneratorTest, CloneIsIndependentAfterCopyInArrayMode) {
    int initial[] = {0, 1};

    RecurrenceGenerator<int> gen(
        fibonacci_array_rule,
        initial,
        2,
        Ordinal(7)
    );

    ExpectGenerated(gen, {0, 1});

    RecurrenceGenerator<int>* clone = gen.clone();

    EXPECT_EQ(gen.get_next(), 1);
    EXPECT_EQ(gen.get_next(), 2);

    EXPECT_EQ(clone->get_next(), 1);
    EXPECT_EQ(clone->get_next(), 2);

    delete clone;
}

// ---------- Assignment tests ----------

TEST(RecurrenceGeneratorTest, AssignmentCopiesArrayModeCurrentState) {
    int initial[] = {0, 1};
    int other_initial[] = {10, 20};

    RecurrenceGenerator<int> gen(
        fibonacci_array_rule,
        initial,
        2,
        Ordinal(7)
    );

    RecurrenceGenerator<int> assigned(
        fibonacci_ring_rule,
        other_initial,
        2,
        Ordinal(2)
    );

    ExpectGenerated(gen, {0, 1, 1});

    assigned = gen;

    EXPECT_EQ(gen.get_next(), 2);
    EXPECT_EQ(assigned.get_next(), 2);

    EXPECT_EQ(gen.get_next(), 3);
    EXPECT_EQ(assigned.get_next(), 3);
}

TEST(RecurrenceGeneratorTest, AssignmentFromRingModeClearsArrayMode) {
    int ring_initial[] = {0, 1};
    int array_initial[] = {10, 20};

    RecurrenceGenerator<int> ring_gen(
        fibonacci_ring_rule,
        ring_initial,
        2,
        Ordinal(7)
    );

    RecurrenceGenerator<int> assigned(
        fibonacci_array_rule,
        array_initial,
        2,
        Ordinal(5)
    );

    ExpectGenerated(ring_gen, {0, 1, 1});

    assigned = ring_gen;

    EXPECT_EQ(assigned.get_next(), 2);
    EXPECT_EQ(assigned.get_next(), 3);
}