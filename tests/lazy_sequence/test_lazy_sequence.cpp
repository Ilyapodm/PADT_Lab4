#include <gtest/gtest.h>
#include <functional>
#include <stdexcept>

#include "lazy_sequence/lazy_sequence.hpp"
#include "adt_lab_2/array_sequence.hpp"
#include "adt_lab_2/list_sequence.hpp"
#include "core/ordinal.hpp"
#include "adt/ring_buffer.hpp"
#include "utils/generator_functions.hpp"

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, DefaultConstructorCreatesEmptySequence) {
    LazySequence<int> seq;

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(0));
    EXPECT_EQ(seq.get_size(), 0);
    EXPECT_EQ(seq.get_materialized_count(), 0);

    EXPECT_THROW(seq.get_first(), std::exception);
    EXPECT_THROW(seq.get_last(), std::exception);
    EXPECT_THROW(seq.get(0), std::exception);
    EXPECT_THROW(seq.get(Ordinal(0)), std::exception);
}

TEST(LazySequenceTest, ConstructsFromArrayAndReadsValues) {
    int items[] = {10, 20, 30};

    LazySequence<int> seq(items, 3);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(3));
    EXPECT_EQ(seq.get_size(), 3);

    EXPECT_EQ(seq.get_first(), 10);
    EXPECT_EQ(seq.get_last(), 30);

    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(1), 20);
    EXPECT_EQ(seq.get(2), 30);

    EXPECT_EQ(seq.get(Ordinal(0)), 10);
    EXPECT_EQ(seq.get(Ordinal(1)), 20);
    EXPECT_EQ(seq.get(Ordinal(2)), 30);
}

TEST(LazySequenceTest, ArrayConstructorAcceptsNullptrWhenCountIsZero) {
    LazySequence<int> seq(nullptr, 0);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(0));
    EXPECT_EQ(seq.get_size(), 0);
    EXPECT_EQ(seq.get_materialized_count(), 0);

    EXPECT_THROW(seq.get_first(), std::exception);
}

TEST(LazySequenceTest, ArrayConstructorRejectsNegativeCount) {
    int items[] = {1, 2, 3};

    EXPECT_THROW(
        LazySequence<int> seq(items, -1),
        std::invalid_argument
    );
}

TEST(LazySequenceTest, ArrayConstructorRejectsNullptrWithPositiveCount) {
    EXPECT_THROW(
        LazySequence<int> seq(nullptr, 3),
        std::invalid_argument
    );
}

TEST(LazySequenceTest, ConstructsFromMutableArraySequence) {
    int items[] = {1, 2, 3};
    MutableArraySequence<int> source(items, 3);

    LazySequence<int> seq(source);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(3));
    EXPECT_EQ(seq.get_size(), 3);

    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(2), 3);
}

TEST(LazySequenceTest, ConstructsFromMutableListSequence) {
    int items[] = {4, 5, 6};
    MutableListSequence<int> source(items, 3);

    LazySequence<int> seq(source);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(3));
    EXPECT_EQ(seq.get_size(), 3);

    EXPECT_EQ(seq.get(0), 4);
    EXPECT_EQ(seq.get(1), 5);
    EXPECT_EQ(seq.get(2), 6);
}

TEST(LazySequenceTest, ConstructorFromSequenceMakesSnapshot) {
    int items[] = {10, 20, 30};
    MutableArraySequence<int> source(items, 3);

    LazySequence<int> seq(source);

    source.set(999, 0);
    source.append(777);

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(3));
    EXPECT_EQ(seq.get_size(), 3);

    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(1), 20);
    EXPECT_EQ(seq.get(2), 30);
}

TEST(LazySequenceTest, ConstructsFromFunctionWithFiniteLength) {
    LazySequence<int> seq(
        square_function,
        Ordinal(5)
    );

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(5));
    EXPECT_EQ(seq.get_size(), 5);

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 4);
    EXPECT_EQ(seq.get(3), 9);
    EXPECT_EQ(seq.get(4), 16);

    EXPECT_THROW(seq.get(5), std::exception);
}

TEST(LazySequenceTest, FunctionSequenceCanBeInfinite) {
    LazySequence<int> seq(
        plus_100_function,
        Ordinal(1, 0)
    );

    EXPECT_EQ(seq.get(Ordinal(0)), 100);
    EXPECT_EQ(seq.get(Ordinal(5)), 105);
    EXPECT_EQ(seq.get(Ordinal(100)), 200);

    EXPECT_THROW(seq.get_size(), std::logic_error);
    EXPECT_THROW(seq.get_last(), std::logic_error);
}

// ----------------------------------------------------------------------------
// Recurrence constructors
// ----------------------------------------------------------------------------

TEST(LazySequenceRecurrenceTest, ConstructsFibonacciUsingRingBufferRule) {
    int initial[] = {0, 1};

    LazySequence<int> seq(fibonacci_ring_rule, initial, 2, Ordinal(8));

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(8));
    EXPECT_EQ(seq.get_size(), 8);

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 2);
    EXPECT_EQ(seq.get(4), 3);
    EXPECT_EQ(seq.get(5), 5);
    EXPECT_EQ(seq.get(6), 8);
    EXPECT_EQ(seq.get(7), 13);

    EXPECT_THROW(seq.get(8), std::exception);
}

TEST(LazySequenceRecurrenceTest, ConstructsTribonacciUsingRingBufferRule) {
    int initial[] = {0, 1, 1};

    LazySequence<int> seq(tribonacci_ring_Rule, initial, 3, Ordinal(8));

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(8));
    EXPECT_EQ(seq.get_size(), 8);

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 2);
    EXPECT_EQ(seq.get(4), 4);
    EXPECT_EQ(seq.get(5), 7);
    EXPECT_EQ(seq.get(6), 13);
    EXPECT_EQ(seq.get(7), 24);
}

TEST(LazySequenceRecurrenceTest, ConstructsFibonacciUsingArrayRule) {
    int initial[] = {0, 1};

    LazySequence<int> seq(fibonacci_array_rule, initial, 2, Ordinal(8));

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(8));
    EXPECT_EQ(seq.get_size(), 8);

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 2);
    EXPECT_EQ(seq.get(4), 3);
    EXPECT_EQ(seq.get(5), 5);
    EXPECT_EQ(seq.get(6), 8);
    EXPECT_EQ(seq.get(7), 13);

    EXPECT_THROW(seq.get(8), std::exception);
}

TEST(LazySequenceRecurrenceTest, ConstructsTribonacciUsingArrayRule) {
    int initial[] = {0, 1, 1};

    LazySequence<int> seq(tribonacci_array_rule, initial, 3, Ordinal(8));

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(8));
    EXPECT_EQ(seq.get_size(), 8);

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 2);
    EXPECT_EQ(seq.get(4), 4);
    EXPECT_EQ(seq.get(5), 7);
    EXPECT_EQ(seq.get(6), 13);
    EXPECT_EQ(seq.get(7), 24);
}

TEST(LazySequenceRecurrenceTest, InfiniteRecurrenceSequenceSupportsFiniteIndexes) {
    int initial[] = {1, 1};

    LazySequence<int> seq(fibonacci_ring_rule, initial, 2, Ordinal(1, 0));

    EXPECT_EQ(seq.get_ordinal_length(), Ordinal(1, 0));

    EXPECT_EQ(seq.get(Ordinal(0)), 1);
    EXPECT_EQ(seq.get(Ordinal(1)), 1);
    EXPECT_EQ(seq.get(Ordinal(2)), 2);
    EXPECT_EQ(seq.get(Ordinal(3)), 3);
    EXPECT_EQ(seq.get(Ordinal(4)), 5);
    EXPECT_EQ(seq.get(Ordinal(5)), 8);
    EXPECT_EQ(seq.get(Ordinal(6)), 13);

    EXPECT_THROW(seq.get_size(), std::logic_error);
    EXPECT_THROW(seq.get_last(), std::logic_error);
}

TEST(LazySequenceRecurrenceTest, MaterializedCountGrowsForRecurrenceSequence) {
    int initial[] = {0, 1};

    LazySequence<int> seq(fibonacci_ring_rule, initial, 2, Ordinal(10));

    EXPECT_EQ(seq.get_materialized_count(), 0);

    EXPECT_EQ(seq.get(5), 5);

    EXPECT_EQ(seq.get_materialized_count(), 6);

    EXPECT_EQ(seq.get(2), 1);

    EXPECT_EQ(seq.get_materialized_count(), 6);
}

TEST(LazySequenceRecurrenceTest, InitialValuesAreCopiedForRingBufferRule) {
    int initial[] = {0, 1};

    LazySequence<int> seq(fibonacci_ring_rule, initial, 2, Ordinal(6));

    initial[0] = 100;
    initial[1] = 200;

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 2);
    EXPECT_EQ(seq.get(4), 3);
    EXPECT_EQ(seq.get(5), 5);
}

TEST(LazySequenceRecurrenceTest, InitialValuesAreCopiedForArrayRule) {
    int initial[] = {0, 1};

    LazySequence<int> seq(fibonacci_array_rule, initial, 2, Ordinal(6));

    initial[0] = 100;
    initial[1] = 200;

    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 1);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 2);
    EXPECT_EQ(seq.get(4), 3);
    EXPECT_EQ(seq.get(5), 5);
}

TEST(LazySequenceRecurrenceTest, OperationResultIsIndependentFromOriginalRecurrenceSequence) {
    int initial[] = {0, 1};

    LazySequence<int>* original = new LazySequence<int>(fibonacci_ring_rule, initial, 2, Ordinal(6));

    EXPECT_EQ(original->get(4), 3);

    LazySequence<int>* result = original->append(999);

    delete original;

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 7);

    EXPECT_EQ(result->get(0), 0);
    EXPECT_EQ(result->get(1), 1);
    EXPECT_EQ(result->get(2), 1);
    EXPECT_EQ(result->get(3), 2);
    EXPECT_EQ(result->get(4), 3);
    EXPECT_EQ(result->get(5), 5);
    EXPECT_EQ(result->get(6), 999);

    delete result;
}

TEST(LazySequenceRecurrenceTest, ConstructorRejectsNullInitialValuesWithPositiveCountForRingBufferRule) {
    EXPECT_THROW(
        LazySequence<int> seq(fibonacci_ring_rule, nullptr, 2, Ordinal(5)),
        std::invalid_argument
    );
}

TEST(LazySequenceRecurrenceTest, ConstructorRejectsNullInitialValuesWithPositiveCountForArrayRule) {
    EXPECT_THROW(
        LazySequence<int> seq(fibonacci_array_rule, nullptr, 2, Ordinal(5)),
        std::invalid_argument
    );
}

TEST(LazySequenceRecurrenceTest, ConstructorRejectsNullRuleForRingBufferRule) {
    int initial[] = {0, 1};

    std::function<int(const RingBuffer<int>&)> rule;

    EXPECT_THROW(
        LazySequence<int> seq(rule, initial, 2, Ordinal(5)),
        std::invalid_argument
    );
}

TEST(LazySequenceRecurrenceTest, ConstructorRejectsNullRuleForArrayRule) {
    int initial[] = {0, 1};

    std::function<int(const int*, std::size_t)> rule;

    EXPECT_THROW(
        LazySequence<int> seq(rule, initial, 2, Ordinal(5)),
        std::invalid_argument
    );
}

// ----------------------------------------------------------------------------
// Materialization
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, MaterializedCountStartsFromZeroForSourceNode) {
    int items[] = {10, 20, 30, 40};
    LazySequence<int> seq(items, 4);

    EXPECT_EQ(seq.get_materialized_count(), 0);
}

TEST(LazySequenceTest, MaterializedCountGrowsAfterAccess) {
    int items[] = {10, 20, 30, 40};
    LazySequence<int> seq(items, 4);

    EXPECT_EQ(seq.get(2), 30);

    EXPECT_EQ(seq.get_materialized_count(), 3);

    EXPECT_EQ(seq.get(1), 20);

    EXPECT_EQ(seq.get_materialized_count(), 3);
}

// ----------------------------------------------------------------------------
// Append
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, AppendAddsItemToFiniteSequence) {
    int items[] = {1, 2, 3};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.append(4);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(4));
    EXPECT_EQ(result->get_size(), 4);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    EXPECT_EQ(seq.get_size(), 3);
    EXPECT_EQ(seq.get_last(), 3);

    delete result;
}

TEST(LazySequenceTest, AppendToInfiniteSequencePlacesItemAtOmegaIndex) {
    LazySequence<int> seq(
        identity_function,
        Ordinal(1, 0)
    );

    LazySequence<int>* result = seq.append(999);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(1, 1));

    EXPECT_EQ(result->get(Ordinal(0)), 0);
    EXPECT_EQ(result->get(Ordinal(10)), 10);
    EXPECT_EQ(result->get(Ordinal(1, 0)), 999);

    delete result;
}

// ----------------------------------------------------------------------------
// Prepend
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, PrependAddsItemToBeginning) {
    int items[] = {2, 3, 4};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.prepend(1);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(4));
    EXPECT_EQ(result->get_size(), 4);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    EXPECT_EQ(seq.get_size(), 3);
    EXPECT_EQ(seq.get_first(), 2);

    delete result;
}

TEST(LazySequenceTest, PrependToInfiniteSequenceDoesNotShiftTransfiniteIndex) {
    LazySequence<int> seq(
        multiply_by_10_function,
        Ordinal(1, 0)
    );

    LazySequence<int>* result = seq.prepend(999);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(1, 0));

    EXPECT_EQ(result->get(Ordinal(0)), 999);
    EXPECT_EQ(result->get(Ordinal(1)), 0);
    EXPECT_EQ(result->get(Ordinal(2)), 10);

    delete result;
}

// ----------------------------------------------------------------------------
// Set
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, SetReplacesOneElement) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.set(999, 1);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 3);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 999);
    EXPECT_EQ(result->get(2), 30);

    EXPECT_EQ(seq.get(1), 20);

    delete result;
}

TEST(LazySequenceTest, SetWithOrdinalIndexWorks) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.set(777, Ordinal(2));

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 777);

    delete result;
}

TEST(LazySequenceTest, SetRejectsNegativeIndex) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    EXPECT_THROW(seq.set(999, -1), std::exception);
}

TEST(LazySequenceTest, SetRejectsOutOfRangeIndex) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    EXPECT_THROW(seq.set(999, 3), std::exception);
}

// ----------------------------------------------------------------------------
// InsertAt
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, InsertAtBeginning) {
    int items[] = {20, 30};
    LazySequence<int> seq(items, 2);

    LazySequence<int>* result = seq.insert_at(10, 0);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 3);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete result;
}

TEST(LazySequenceTest, InsertAtMiddle) {
    int items[] = {10, 30, 40};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.insert_at(20, 1);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 4);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);
    EXPECT_EQ(result->get(3), 40);

    EXPECT_EQ(seq.get_size(), 3);
    EXPECT_EQ(seq.get(1), 30);

    delete result;
}

TEST(LazySequenceTest, InsertAtEndWorksLikeAppendForFiniteSequence) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    LazySequence<int>* result = seq.insert_at(30, 2);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 3);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);
    EXPECT_EQ(result->get(2), 30);

    delete result;
}

TEST(LazySequenceTest, InsertAtRejectsNegativeIndex) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.insert_at(999, -1), std::exception);
}

TEST(LazySequenceTest, InsertAtRejectsIndexGreaterThanLength) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.insert_at(999, 3), std::exception);
}

// ----------------------------------------------------------------------------
// RemoveAt
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, RemoveAtBeginning) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.remove_at(0);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 2);

    EXPECT_EQ(result->get(0), 20);
    EXPECT_EQ(result->get(1), 30);

    EXPECT_EQ(seq.get_size(), 3);
    EXPECT_EQ(seq.get(0), 10);

    delete result;
}

TEST(LazySequenceTest, RemoveAtMiddle) {
    int items[] = {10, 20, 30, 40};
    LazySequence<int> seq(items, 4);

    LazySequence<int>* result = seq.remove_at(1);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 3);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 30);
    EXPECT_EQ(result->get(2), 40);

    delete result;
}

TEST(LazySequenceTest, RemoveAtEnd) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* result = seq.remove_at(2);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 2);

    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(1), 20);

    delete result;
}

TEST(LazySequenceTest, RemoveAtRejectsNegativeIndex) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.remove_at(-1), std::exception);
}

TEST(LazySequenceTest, RemoveAtRejectsOutOfRangeIndex) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.remove_at(2), std::exception);
}

TEST(LazySequenceTest, RemoveAtFromSingleElementSequenceCreatesEmptySequence) {
    int items[] = {10};
    LazySequence<int> seq(items, 1);

    LazySequence<int>* result = seq.remove_at(0);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(0));
    EXPECT_EQ(result->get_size(), 0);

    EXPECT_THROW(result->get_first(), std::exception);

    delete result;
}

// ----------------------------------------------------------------------------
// Subsequence
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, GetSubsequenceWithIntIndexesUsesInclusiveEnd) {
    int items[] = {10, 20, 30, 40, 50};
    LazySequence<int> seq(items, 5);

    LazySequence<int>* sub = seq.get_subsequence(1, 3);

    ASSERT_NE(sub, nullptr);

    EXPECT_EQ(sub->get_size(), 3);

    EXPECT_EQ(sub->get(0), 20);
    EXPECT_EQ(sub->get(1), 30);
    EXPECT_EQ(sub->get(2), 40);

    delete sub;
}

TEST(LazySequenceTest, GetSubsequenceCanTakeWholeSequence) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* sub = seq.get_subsequence(0, 2);

    ASSERT_NE(sub, nullptr);

    EXPECT_EQ(sub->get_size(), 3);

    EXPECT_EQ(sub->get(0), 10);
    EXPECT_EQ(sub->get(1), 20);
    EXPECT_EQ(sub->get(2), 30);

    delete sub;
}

TEST(LazySequenceTest, GetSubsequenceWithOrdinalIndexesUsesEndExclusive) {
    int items[] = {10, 20, 30, 40, 50};
    LazySequence<int> seq(items, 5);

    LazySequence<int>* sub = seq.get_subsequence(Ordinal(1), Ordinal(4));

    ASSERT_NE(sub, nullptr);

    EXPECT_EQ(sub->get_ordinal_length(), Ordinal(3));

    EXPECT_EQ(sub->get(0), 20);
    EXPECT_EQ(sub->get(1), 30);
    EXPECT_EQ(sub->get(2), 40);

    delete sub;
}

TEST(LazySequenceTest, GetSubsequenceRejectsNegativeIndexes) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    EXPECT_THROW(seq.get_subsequence(-1, 1), std::exception);
    EXPECT_THROW(seq.get_subsequence(0, -1), std::exception);
}

TEST(LazySequenceTest, GetSubsequenceRejectsStartGreaterThanEnd) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    EXPECT_THROW(seq.get_subsequence(2, 1), std::exception);
}

TEST(LazySequenceTest, GetSubsequenceRejectsOutOfRangeEnd) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    EXPECT_THROW(seq.get_subsequence(1, 3), std::exception);
}


// ----------------------------------------------------------------------------
// Concat
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, ConcatWithLazySequence) {
    int left_items[] = {1, 2};
    int right_items[] = {3, 4, 5};

    LazySequence<int> left(left_items, 2);
    LazySequence<int> right(right_items, 3);

    LazySequence<int>* result = left.concat(right);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(5));
    EXPECT_EQ(result->get_size(), 5);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);
    EXPECT_EQ(result->get(4), 5);

    delete result;
}

TEST(LazySequenceTest, ConcatWithMutableArraySequence) {
    int left_items[] = {1, 2};
    int right_items[] = {3, 4};

    LazySequence<int> left(left_items, 2);
    MutableArraySequence<int> right(right_items, 2);

    LazySequence<int>* result = left.concat(right);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 4);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    delete result;
}

TEST(LazySequenceTest, ConcatWithMutableListSequence) {
    int left_items[] = {1, 2};
    int right_items[] = {3, 4, 5};

    LazySequence<int> left(left_items, 2);
    MutableListSequence<int> right(right_items, 3);

    LazySequence<int>* result = left.concat(right);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 5);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);
    EXPECT_EQ(result->get(4), 5);

    delete result;
}

TEST(LazySequenceTest, ConcatWithOrdinalsInfinitePlusFinite) {
    LazySequence<int> infinite(
        identity_function,
        Ordinal(1, 0)
    );

    int tail_items[] = {100, 200, 300};
    LazySequence<int> tail(tail_items, 3);

    LazySequence<int>* result = infinite.concat(tail);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_ordinal_length(), Ordinal(1, 3));

    EXPECT_EQ(result->get(Ordinal(0)), 0);
    EXPECT_EQ(result->get(Ordinal(10)), 10);

    EXPECT_EQ(result->get(Ordinal(1, 0)), 100);
    EXPECT_EQ(result->get(Ordinal(1, 1)), 200);
    EXPECT_EQ(result->get(Ordinal(1, 2)), 300);

    delete result;
}

// ----------------------------------------------------------------------------
// Copying and independence
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, CopyConstructorClonesRootNode) {
    int items[] = {10, 20, 30};

    LazySequence<int>* original = new LazySequence<int>(items, 3);

    EXPECT_EQ(original->get(2), 30);

    LazySequence<int> copy(*original);

    delete original;

    EXPECT_EQ(copy.get_size(), 3);

    EXPECT_EQ(copy.get(0), 10);
    EXPECT_EQ(copy.get(1), 20);
    EXPECT_EQ(copy.get(2), 30);
}

TEST(LazySequenceTest, CopyAssignmentClonesRootNode) {
    int first_items[] = {1, 2, 3};
    int second_items[] = {10, 20};

    LazySequence<int> first(first_items, 3);
    LazySequence<int> second(second_items, 2);

    second = first;

    EXPECT_EQ(second.get_size(), 3);

    EXPECT_EQ(second.get(0), 1);
    EXPECT_EQ(second.get(1), 2);
    EXPECT_EQ(second.get(2), 3);
}

TEST(LazySequenceTest, CopyAssignmentHandlesSelfAssignment) {
    int items[] = {1, 2, 3};
    LazySequence<int> seq(items, 3);

    seq = seq;

    EXPECT_EQ(seq.get_size(), 3);

    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(2), 3);
}

TEST(LazySequenceTest, OperationResultIsIndependentFromOriginalSequence) {
    int items[] = {1, 2, 3};
    LazySequence<int>* original = new LazySequence<int>(items, 3);

    LazySequence<int>* result = original->append(4);

    delete original;

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->get_size(), 4);

    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    delete result;
}


// ----------------------------------------------------------------------------
// Option getters
// ----------------------------------------------------------------------------

TEST(LazySequenceOptionTest, TryGetFirstReturnsSomeForNonEmptySequence) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    Option<int> result = seq.try_get_first();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.get_value(), 10);
}

TEST(LazySequenceOptionTest, TryGetFirstReturnsNoneForEmptySequence) {
    LazySequence<int> seq;

    Option<int> result = seq.try_get_first();

    EXPECT_FALSE(result.has_value());
    EXPECT_THROW(result.get_value(), std::runtime_error);
}

TEST(LazySequenceOptionTest, TryGetLastReturnsSomeForNonEmptySequence) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    Option<int> result = seq.try_get_last();

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.get_value(), 30);
}

TEST(LazySequenceOptionTest, TryGetReturnsSomeForValidIntIndex) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    Option<int> result = seq.try_get(Ordinal(1));

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.get_value(), 20);
}

TEST(LazySequenceOptionTest, TryGetReturnsNoneForOutOfRangeIntIndex) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    Option<int> result = seq.try_get(Ordinal(3));

    EXPECT_FALSE(result.has_value());
    EXPECT_THROW(result.get_value(), std::runtime_error);
}

TEST(LazySequenceOptionTest, TryGetReturnsSomeForValidOrdinalIndex) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    Option<int> result = seq.try_get(Ordinal(2));

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.get_value(), 30);
}

TEST(LazySequenceOptionTest, TryGetReturnsNoneForOutOfRangeOrdinalIndex) {
    int items[] = {10, 20, 30};
    LazySequence<int> seq(items, 3);

    Option<int> result = seq.try_get(Ordinal(3));

    EXPECT_FALSE(result.has_value());
    EXPECT_THROW(result.get_value(), std::runtime_error);
}

// ----------------------------------------------------------------------------
// Exceptions
// ----------------------------------------------------------------------------

TEST(LazySequenceTest, GetRejectsNegativeIndex) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.get(-1), std::exception);
}

TEST(LazySequenceTest, GetRejectsIndexEqualToSize) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.get(2), std::exception);
}

TEST(LazySequenceTest, GetAtRejectsOrdinalOutOfRange) {
    int items[] = {10, 20};
    LazySequence<int> seq(items, 2);

    EXPECT_THROW(seq.get(Ordinal(2)), std::exception);
}

TEST(LazySequenceTest, GetLastThrowsForEmptySequence) {
    LazySequence<int> seq;

    EXPECT_THROW(seq.get_last(), std::exception);
}

TEST(LazySequenceTest, GetFirstThrowsForEmptySequence) {
    LazySequence<int> seq;

    EXPECT_THROW(seq.get_first(), std::exception);
}