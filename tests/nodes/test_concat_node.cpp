#include <gtest/gtest.h>

#include "core/ordinal.hpp"
#include "generators/function_generator.hpp"
#include "nodes/source_node.hpp"
#include "nodes/concat_node.hpp"


// helpers
static Ordinal Omega(std::size_t finite_part = 0) {
    return Ordinal(1, finite_part);
}

static Ordinal Omega2(std::size_t finite_part = 0) {
    return Ordinal(2, finite_part);
}

static void ExpectOrdinalEq(const Ordinal& value,
                            std::size_t omega_coeff,
                            std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

static int identity_function(std::size_t index) {
    return static_cast<int>(index);
}

static int plus_10_function(std::size_t index) {
    return static_cast<int>(index + 10);
}

static int plus_50_function(std::size_t index) {
    return static_cast<int>(index + 50);
}

static int plus_100_function(std::size_t index) {
    return static_cast<int>(index + 100);
}

static int plus_1000_function(std::size_t index) {
    return static_cast<int>(index + 1000);
}

// 1. finite + finite
TEST(ConcatNodeTest, FiniteFiniteComputesCorrectLength) {
    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        plus_100_function,
        Ordinal(2)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    ExpectOrdinalEq(concat.length(), 0, 5);
}

TEST(ConcatNodeTest, FiniteFiniteReturnsValuesFromLeftAndRight) {
    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        plus_100_function,
        Ordinal(2)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Ordinal(0)), 10);
    EXPECT_EQ(concat.value_at(Ordinal(1)), 11);
    EXPECT_EQ(concat.value_at(Ordinal(2)), 12);

    EXPECT_EQ(concat.value_at(Ordinal(3)), 100);
    EXPECT_EQ(concat.value_at(Ordinal(4)), 101);
}

TEST(ConcatNodeTest, FiniteFiniteThrowsWhenIndexEqualsLength) {
    FunctionGenerator<int> left_gen(
        identity_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        identity_function,
        Ordinal(2)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_THROW(concat.value_at(Ordinal(5)), std::out_of_range);
}

TEST(ConcatNodeTest, FiniteFiniteThrowsWhenIndexGreaterThanLength) {
    FunctionGenerator<int> left_gen(
        identity_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        identity_function,
        Ordinal(2)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_THROW(concat.value_at(Ordinal(100)), std::out_of_range);
}

// 2. empty cases
TEST(ConcatNodeTest, EmptyLeftBehavesLikeRight) {
    FunctionGenerator<int> empty_gen(
        identity_function,
        Ordinal(0)
    );

    FunctionGenerator<int> right_gen(
        plus_50_function,
        Ordinal(3)
    );

    SourceNode<int> empty(empty_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(empty, right);

    ExpectOrdinalEq(concat.length(), 0, 3);

    EXPECT_EQ(concat.value_at(Ordinal(0)), 50);
    EXPECT_EQ(concat.value_at(Ordinal(1)), 51);
    EXPECT_EQ(concat.value_at(Ordinal(2)), 52);

    EXPECT_THROW(concat.value_at(Ordinal(3)), std::out_of_range);
}

TEST(ConcatNodeTest, EmptyRightBehavesLikeLeft) {
    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(3)
    );

    FunctionGenerator<int> empty_gen(
        identity_function,
        Ordinal(0)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> empty(empty_gen);

    ConcatNode<int> concat(left, empty);

    ExpectOrdinalEq(concat.length(), 0, 3);

    EXPECT_EQ(concat.value_at(Ordinal(0)), 10);
    EXPECT_EQ(concat.value_at(Ordinal(1)), 11);
    EXPECT_EQ(concat.value_at(Ordinal(2)), 12);

    EXPECT_THROW(concat.value_at(Ordinal(3)), std::out_of_range);
}

// 3. finite + omega
TEST(ConcatNodeTest, FiniteOmegaLengthIsOmega) {
    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(2)
    );

    FunctionGenerator<int> right_gen(
        plus_1000_function,
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    // 2 + omega = omega
    ExpectOrdinalEq(concat.length(), 1, 0);
}

TEST(ConcatNodeTest, FiniteOmegaRoutesFiniteIndexesCorrectly) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(10 + i);
        },
        Ordinal(2)
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(1000 + i);
        },
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Ordinal(0)), 10);
    EXPECT_EQ(concat.value_at(Ordinal(1)), 11);

    EXPECT_EQ(left_calls, 2);
    EXPECT_EQ(right_calls, 0);

    // index 2 is the first element of the right part
    EXPECT_EQ(concat.value_at(Ordinal(2)), 1000);

    EXPECT_EQ(left_calls, 2);
    EXPECT_EQ(right_calls, 1);
}

// NOTE
TEST(ConcatNodeTest, FiniteOmegaRoutesFarFiniteIndexToRightWithoutTouchingMoreLeft) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(10 + i);
        },
        Ordinal(2)
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(1000 + i);
        },
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    // global index 10 -> right local index 8
    EXPECT_EQ(concat.value_at(Ordinal(10)), 1008);

    EXPECT_EQ(left_calls, 0);
    EXPECT_EQ(right_calls, 9);
}

// 4. omega + finite
TEST(ConcatNodeTest, OmegaFiniteLengthIsOmegaPlusFinite) {
    FunctionGenerator<int> left_gen(
        identity_function,
        Omega()
    );

    FunctionGenerator<int> right_gen(
        plus_100_function,
        Ordinal(3)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    ExpectOrdinalEq(concat.length(), 1, 3);
}

TEST(ConcatNodeTest, OmegaFiniteIndexGoesToLeftOnly) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(i);
        },
        Omega()
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(100 + i);
        },
        Ordinal(3)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Ordinal(50)), 50);

    EXPECT_EQ(left_calls, 51);
    EXPECT_EQ(right_calls, 0);
}

TEST(ConcatNodeTest, OmegaFiniteTransfiniteIndexGoesToRight) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(i);
        },
        Omega()
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(100 + i);
        },
        Ordinal(3)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Omega(0)), 100);
    EXPECT_EQ(concat.value_at(Omega(1)), 101);
    EXPECT_EQ(concat.value_at(Omega(2)), 102);

    // left is infinite, but transfinite access must not enumerate left
    EXPECT_EQ(left_calls, 0);

    // SourceNode right has dense prefix cache: to get right[2], it materializes 0,1,2
    EXPECT_EQ(right_calls, 3);
}

TEST(ConcatNodeTest, OmegaFiniteThrowsAtOmegaPlusFiniteLength) {
    FunctionGenerator<int> left_gen(
        identity_function,
        Omega()
    );

    FunctionGenerator<int> right_gen(
        plus_100_function,
        Ordinal(3)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    // length is omega + 3, so index omega + 3 is already out of range
    EXPECT_THROW(concat.value_at(Omega(3)), std::out_of_range);
}

// 5. omega + omega
TEST(ConcatNodeTest, OmegaOmegaLengthIsOmegaTimesTwo) {
    FunctionGenerator<int> left_gen(
        identity_function,
        Omega()
    );

    FunctionGenerator<int> right_gen(
        plus_1000_function,
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    ExpectOrdinalEq(concat.length(), 2, 0);
}

TEST(ConcatNodeTest, OmegaOmegaFiniteIndexGoesToLeft) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(i);
        },
        Omega()
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(1000 + i);
        },
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Ordinal(10)), 10);

    EXPECT_EQ(left_calls, 11);
    EXPECT_EQ(right_calls, 0);
}

TEST(ConcatNodeTest, OmegaOmegaIndexGoesToRightWithoutTouchingLeft) {
    int left_calls = 0;
    int right_calls = 0;

    // lambda is accepted, because have to count the calls
    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(i);
        },
        Omega()
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(1000 + i);
        },
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Omega(10)), 1010);

    // Critical test:
    // concat must not sequentially read the infinite left part.
    EXPECT_EQ(left_calls, 0);

    // right local index is 10, dense cache means 11 generated values.
    EXPECT_EQ(right_calls, 11);
}

TEST(ConcatNodeTest, OmegaOmegaRejectsIndexOmegaTimesTwo) {
    FunctionGenerator<int> left_gen(
        identity_function,
        Omega()
    );

    FunctionGenerator<int> right_gen(
        plus_1000_function,
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    // length is omega * 2, so index omega * 2 is out of range
    EXPECT_THROW(concat.value_at(Omega2(0)), std::out_of_range);
}

// 6. cache behavior
TEST(ConcatNodeTest, RepeatedAccessUsesChildCache) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(i);
        },
        Omega()
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(1000 + i);
        },
        Omega()
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Omega(10)), 1010);
    EXPECT_EQ(left_calls, 0);
    EXPECT_EQ(right_calls, 11);

    // Same index: should be already cached in right child.
    EXPECT_EQ(concat.value_at(Omega(10)), 1010);
    EXPECT_EQ(left_calls, 0);
    EXPECT_EQ(right_calls, 11);

    // Smaller right-local index is also already inside dense prefix cache.
    EXPECT_EQ(concat.value_at(Omega(5)), 1005);
    EXPECT_EQ(left_calls, 0);
    EXPECT_EQ(right_calls, 11);
}

TEST(ConcatNodeTest, ConstructorClonesExistingChildCaches) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(10 + i);
        },
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(100 + i);
        },
        Ordinal(3)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    // Pre-materialize left[0..2] and right[0..1].
    EXPECT_EQ(left.value_at(Ordinal(2)), 12);
    EXPECT_EQ(right.value_at(Ordinal(1)), 101);

    EXPECT_EQ(left_calls, 3);
    EXPECT_EQ(right_calls, 2);

    ConcatNode<int> concat(left, right);

    // These values should be available from cloned caches.
    EXPECT_EQ(concat.value_at(Ordinal(2)), 12);
    EXPECT_EQ(concat.value_at(Ordinal(4)), 101);

    EXPECT_EQ(left_calls, 3);
    EXPECT_EQ(right_calls, 2);
}

// 7. clone
TEST(ConcatNodeTest, CloneCopiesStructureAndCachedValues) {
    int left_calls = 0;
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        [&left_calls](std::size_t i) {
            ++left_calls;
            return static_cast<int>(10 + i);
        },
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(100 + i);
        },
        Ordinal(5)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    // global 4 -> right local 1
    EXPECT_EQ(concat.value_at(Ordinal(4)), 101);
    EXPECT_EQ(right_calls, 2);

    ConcatNode<int>* cloned = concat.clone();
    ASSERT_NE(cloned, nullptr);

    // Must use cloned cache, no new get_next calls.
    EXPECT_EQ(cloned->value_at(Ordinal(4)), 101);
    EXPECT_EQ(right_calls, 2);

    delete cloned;
}

TEST(ConcatNodeTest, CloneHasIndependentGeneratorStateAfterCopiedCache) {
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(100 + i);
        },
        Ordinal(5)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> original(left, right);

    // global 4 -> right local 1
    EXPECT_EQ(original.value_at(Ordinal(4)), 101);
    EXPECT_EQ(right_calls, 2);

    ConcatNode<int>* cloned = original.clone();
    ASSERT_NE(cloned, nullptr);

    // global 5 -> right local 2
    // Clone should generate its own next value.
    EXPECT_EQ(cloned->value_at(Ordinal(5)), 102);
    EXPECT_EQ(right_calls, 3);

    // Original has independent generator state.
    // It still also needs to generate right local 2 for itself.
    EXPECT_EQ(original.value_at(Ordinal(5)), 102);
    EXPECT_EQ(right_calls, 4);

    delete cloned;
}

// 8. assignment
TEST(ConcatNodeTest, AssignmentCopiesStructureAndValues) {
    int right_calls = 0;

    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        [&right_calls](std::size_t i) {
            ++right_calls;
            return static_cast<int>(100 + i);
        },
        Ordinal(5)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);

    // global 4 -> right local 1
    EXPECT_EQ(source.value_at(Ordinal(4)), 101);
    EXPECT_EQ(right_calls, 2);

    FunctionGenerator<int> other_left_gen(
        [](std::size_t i) { return static_cast<int>(-10 - static_cast<int>(i)); },
        Ordinal(1)
    );

    FunctionGenerator<int> other_right_gen(
        [](std::size_t i) { return static_cast<int>(-100 - static_cast<int>(i)); },
        Ordinal(1)
    );

    SourceNode<int> other_left(other_left_gen);
    SourceNode<int> other_right(other_right_gen);

    ConcatNode<int> target(other_left, other_right);

    target = source;

    ExpectOrdinalEq(target.length(), 0, 8);

    // Must use copied cache from source.
    EXPECT_EQ(target.value_at(Ordinal(4)), 101);
    EXPECT_EQ(right_calls, 2);

    // source remains valid too.
    EXPECT_EQ(source.value_at(Ordinal(4)), 101);
    EXPECT_EQ(right_calls, 2);
}

TEST(ConcatNodeTest, SelfAssignmentKeepsObjectValid) {
    FunctionGenerator<int> left_gen(
        plus_10_function,
        Ordinal(3)
    );

    FunctionGenerator<int> right_gen(
        plus_100_function,
        Ordinal(2)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    EXPECT_EQ(concat.value_at(Ordinal(0)), 10);
    EXPECT_EQ(concat.value_at(Ordinal(4)), 101);

    concat = concat;

    ExpectOrdinalEq(concat.length(), 0, 5);

    EXPECT_EQ(concat.value_at(Ordinal(0)), 10);
    EXPECT_EQ(concat.value_at(Ordinal(4)), 101);
}

// 9. nested concat
TEST(ConcatNodeTest, NestedConcatFiniteOmegaFiniteHasOmegaPlusFiniteLength) {
    FunctionGenerator<int> a_gen(
        plus_10_function,
        Ordinal(2)
    );

    FunctionGenerator<int> b_gen(
        plus_1000_function,
        Omega()
    );

    FunctionGenerator<int> c_gen(
        plus_50_function,
        Ordinal(3)
    );

    SourceNode<int> a(a_gen);
    SourceNode<int> b(b_gen);
    SourceNode<int> c(c_gen);

    ConcatNode<int> ab(a, b);
    ConcatNode<int> abc(ab, c);

    // (2 + omega) + 3 = omega + 3
    ExpectOrdinalEq(abc.length(), 1, 3);
}

TEST(ConcatNodeTest, NestedConcatRoutesInsideLeftConcat) {
    FunctionGenerator<int> a_gen(
        plus_10_function,
        Ordinal(2)
    );

    FunctionGenerator<int> b_gen(
        plus_1000_function,
        Omega()
    );

    FunctionGenerator<int> c_gen(
        plus_50_function,
        Ordinal(3)
    );

    SourceNode<int> a(a_gen);
    SourceNode<int> b(b_gen);
    SourceNode<int> c(c_gen);

    ConcatNode<int> ab(a, b);
    ConcatNode<int> abc(ab, c);

    EXPECT_EQ(abc.value_at(Ordinal(0)), 10);
    EXPECT_EQ(abc.value_at(Ordinal(1)), 11);

    // global finite 10:
    // abc -> left ab
    // ab  -> right b
    // local in b = 8
    EXPECT_EQ(abc.value_at(Ordinal(10)), 1008);
}

TEST(ConcatNodeTest, NestedConcatRoutesOmegaTailToFinalRightPart) {
    FunctionGenerator<int> a_gen(
        plus_10_function,
        Ordinal(2)
    );

    FunctionGenerator<int> b_gen(
        plus_1000_function,
        Omega()
    );

    FunctionGenerator<int> c_gen(
        plus_50_function,
        Ordinal(3)
    );

    SourceNode<int> a(a_gen);
    SourceNode<int> b(b_gen);
    SourceNode<int> c(c_gen);

    ConcatNode<int> ab(a, b);
    ConcatNode<int> abc(ab, c);

    EXPECT_EQ(abc.value_at(Omega(0)), 50);
    EXPECT_EQ(abc.value_at(Omega(1)), 51);
    EXPECT_EQ(abc.value_at(Omega(2)), 52);

    EXPECT_THROW(abc.value_at(Omega(3)), std::out_of_range);
}

TEST(ConcatNodeTest, NestedConcatOmegaFiniteOmegaRoutesAfterOmegaPlusFiniteToLastOmega) {
    int a_calls = 0;
    int b_calls = 0;
    int c_calls = 0;

    FunctionGenerator<int> a_gen(
        [&a_calls](std::size_t i) {
            ++a_calls;
            return static_cast<int>(100 + i);
        },
        Omega()
    );

    FunctionGenerator<int> b_gen(
        [&b_calls](std::size_t i) {
            ++b_calls;
            return static_cast<int>(200 + i);
        },
        Ordinal(2)
    );

    FunctionGenerator<int> c_gen(
        [&c_calls](std::size_t i) {
            ++c_calls;
            return static_cast<int>(300 + i);
        },
        Omega()
    );

    SourceNode<int> a(a_gen);
    SourceNode<int> b(b_gen);
    SourceNode<int> c(c_gen);

    ConcatNode<int> ab(a, b);
    ConcatNode<int> abc(ab, c);

    // (omega + 2) + omega = omega * 2
    ExpectOrdinalEq(abc.length(), 2, 0);

    // index omega + 2 is exactly first element after the left block (omega + 2)
    EXPECT_EQ(abc.value_at(Omega(2)), 300);

    EXPECT_EQ(a_calls, 0);
    EXPECT_EQ(b_calls, 0);
    EXPECT_EQ(c_calls, 1);

    // index omega + 10 -> right local index 8
    EXPECT_EQ(abc.value_at(Omega(10)), 308);

    EXPECT_EQ(a_calls, 0);
    EXPECT_EQ(b_calls, 0);
    EXPECT_EQ(c_calls, 9);
}