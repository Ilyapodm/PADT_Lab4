#include <gtest/gtest.h>
#include <stdexcept>
#include <functional>

#include "generators/function_generator.hpp"
#include "nodes/source_node.hpp"
#include "nodes/subsequence_node.hpp"
#include "core/ordinal.hpp"

static void expect_ordinal_eq(const Ordinal& value, std::size_t omega_coeff, std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

static int linear_value(std::size_t index) {
    return static_cast<int>(100 + index * 10);
}

// source: 100, 110, 120, 130, 140, 150, ...
TEST(SubsequenceNodeTest, ConstructorComputesFiniteLength) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(2), Ordinal(7));

    // [2, 7) => indices 2, 3, 4, 5, 6 => length = 5
    expect_ordinal_eq(sub.length(), 0, 5);
}

TEST(SubsequenceNodeTest, ValueAtTranslatesLocalIndexToSourceIndex) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(2), Ordinal(7));

    EXPECT_EQ(sub.value_at(Ordinal(0)), 120); // source[2]
    EXPECT_EQ(sub.value_at(Ordinal(1)), 130); // source[3]
    EXPECT_EQ(sub.value_at(Ordinal(2)), 140); // source[4]
    EXPECT_EQ(sub.value_at(Ordinal(3)), 150); // source[5]
    EXPECT_EQ(sub.value_at(Ordinal(4)), 160); // source[6]
}

TEST(SubsequenceNodeTest, WorksFromBeginning) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(0), Ordinal(4));

    expect_ordinal_eq(sub.length(), 0, 4);

    EXPECT_EQ(sub.value_at(Ordinal(0)), 100);
    EXPECT_EQ(sub.value_at(Ordinal(1)), 110);
    EXPECT_EQ(sub.value_at(Ordinal(2)), 120);
    EXPECT_EQ(sub.value_at(Ordinal(3)), 130);
}

TEST(SubsequenceNodeTest, WorksUntilSourceEnd) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(7), Ordinal(10));

    expect_ordinal_eq(sub.length(), 0, 3);

    EXPECT_EQ(sub.value_at(Ordinal(0)), 170); // source[7]
    EXPECT_EQ(sub.value_at(Ordinal(1)), 180); // source[8]
    EXPECT_EQ(sub.value_at(Ordinal(2)), 190); // source[9]
}

TEST(SubsequenceNodeTest, EmptySubsequenceHasZeroLength) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(5), Ordinal(5));

    expect_ordinal_eq(sub.length(), 0, 0);

    EXPECT_THROW({
        sub.value_at(Ordinal(0));
    }, std::out_of_range);
}

TEST(SubsequenceNodeTest, ValueAtThrowsWhenLocalIndexIsOutOfRange) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(2), Ordinal(7));

    // length = 5, valid local indices: 0, 1, 2, 3, 4
    EXPECT_THROW({
        sub.value_at(Ordinal(5));
    }, std::out_of_range);
}

TEST(SubsequenceNodeTest, ConstructorThrowsWhenStartGreaterThanEnd) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    EXPECT_THROW({
        SubsequenceNode<int> sub(source, Ordinal(7), Ordinal(2));
    }, std::invalid_argument);
}

TEST(SubsequenceNodeTest, ConstructorThrowsWhenEndIsGreaterThanSourceLength) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    EXPECT_THROW({
        SubsequenceNode<int> sub(source, Ordinal(3), Ordinal(11));
    }, std::invalid_argument);
}

TEST(SubsequenceNodeTest, ConstructorAllowsFullSourceRange) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(0), Ordinal(5));

    expect_ordinal_eq(sub.length(), 0, 5);

    EXPECT_EQ(sub.value_at(Ordinal(0)), 100);
    EXPECT_EQ(sub.value_at(Ordinal(1)), 110);
    EXPECT_EQ(sub.value_at(Ordinal(2)), 120);
    EXPECT_EQ(sub.value_at(Ordinal(3)), 130);
    EXPECT_EQ(sub.value_at(Ordinal(4)), 140);
}

TEST(SubsequenceNodeTest, OwnsIndependentCloneOfSource) {
    SubsequenceNode<int>* sub = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(10));
        SourceNode<int> source(gen);

        sub = new SubsequenceNode<int>(source, Ordinal(2), Ordinal(6));

        EXPECT_EQ(sub->value_at(Ordinal(0)), 120);
    }

    // there is no gen and source
    EXPECT_EQ(sub->value_at(Ordinal(0)), 120);
    EXPECT_EQ(sub->value_at(Ordinal(1)), 130);
    EXPECT_EQ(sub->value_at(Ordinal(2)), 140);
    EXPECT_EQ(sub->value_at(Ordinal(3)), 150);

    delete sub;
}

TEST(SubsequenceNodeTest, CopyConstructorCreatesWorkingIndependentCopy) {
    SubsequenceNode<int>* copy = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(10));
        SourceNode<int> source(gen);

        SubsequenceNode<int> original(source, Ordinal(3), Ordinal(8));

        EXPECT_EQ(original.value_at(Ordinal(0)), 130);
        EXPECT_EQ(original.value_at(Ordinal(4)), 170);

        copy = new SubsequenceNode<int>(original);
    }

    // original, source and gen were deleted
    // copy continues work
    expect_ordinal_eq(copy->length(), 0, 5);

    EXPECT_EQ(copy->value_at(Ordinal(0)), 130);
    EXPECT_EQ(copy->value_at(Ordinal(1)), 140);
    EXPECT_EQ(copy->value_at(Ordinal(2)), 150);
    EXPECT_EQ(copy->value_at(Ordinal(3)), 160);
    EXPECT_EQ(copy->value_at(Ordinal(4)), 170);

    delete copy;
}

TEST(SubsequenceNodeTest, CloneCreatesWorkingIndependentCopy) {
    SubsequenceNode<int>* cloned = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(10));
        SourceNode<int> source(gen);

        SubsequenceNode<int> original(source, Ordinal(1), Ordinal(5));

        EXPECT_EQ(original.value_at(Ordinal(0)), 110);
        EXPECT_EQ(original.value_at(Ordinal(3)), 140);

        cloned = original.clone();
    }

    // original, source and gen has already deleted
    // cloned has copy
    expect_ordinal_eq(cloned->length(), 0, 4);

    EXPECT_EQ(cloned->value_at(Ordinal(0)), 110);
    EXPECT_EQ(cloned->value_at(Ordinal(1)), 120);
    EXPECT_EQ(cloned->value_at(Ordinal(2)), 130);
    EXPECT_EQ(cloned->value_at(Ordinal(3)), 140);

    delete cloned;
}

TEST(SubsequenceNodeTest, AssignmentOperatorCopiesSourceAndRange) {
    FunctionGenerator<int> gen1(linear_value, Ordinal(10));
    SourceNode<int> source1(gen1);

    SubsequenceNode<int> first(source1, Ordinal(2), Ordinal(6));
    // first = [120, 130, 140, 150]

    FunctionGenerator<int> gen2(
        [](std::size_t index) {
            return static_cast<int>(1000 + index);
        },
        Ordinal(10)
    );
    SourceNode<int> source2(gen2);

    SubsequenceNode<int> second(source2, Ordinal(0), Ordinal(2));
    // second = [1000, 1001]

    second = first;

    expect_ordinal_eq(second.length(), 0, 4);

    EXPECT_EQ(second.value_at(Ordinal(0)), 120);
    EXPECT_EQ(second.value_at(Ordinal(1)), 130);
    EXPECT_EQ(second.value_at(Ordinal(2)), 140);
    EXPECT_EQ(second.value_at(Ordinal(3)), 150);
}

TEST(SubsequenceNodeTest, SelfAssignmentDoesNotBreakObject) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(2), Ordinal(6));

    sub = sub;

    expect_ordinal_eq(sub.length(), 0, 4);

    EXPECT_EQ(sub.value_at(Ordinal(0)), 120);
    EXPECT_EQ(sub.value_at(Ordinal(1)), 130);
    EXPECT_EQ(sub.value_at(Ordinal(2)), 140);
    EXPECT_EQ(sub.value_at(Ordinal(3)), 150);
}

TEST(SubsequenceNodeTest, WorksWithOmegaEndAndFiniteStart) {
    FunctionGenerator<int> gen(linear_value, Ordinal::omega());
    SourceNode<int> source(gen);

    SubsequenceNode<int> sub(source, Ordinal(5), Ordinal::omega());

    // [5, omega)'s length is omega
    expect_ordinal_eq(sub.length(), 1, 0);

    EXPECT_EQ(sub.value_at(Ordinal(0)), 150);  // source[5]
    EXPECT_EQ(sub.value_at(Ordinal(1)), 160);  // source[6]
    EXPECT_EQ(sub.value_at(Ordinal(10)), 250); // source[15]
}

TEST(SubsequenceNodeTest, RepeatedAccessUsesSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(20)
    );

    SourceNode<int> source(gen);

    // sub = source[3, 8)
    // sub[0] -> source[3]
    // sub[1] -> source[4]
    // sub[2] -> source[5]
    SubsequenceNode<int> sub(source, Ordinal(3), Ordinal(8));

    EXPECT_EQ(call_count, 0);

    // to get source[3], we have to compute: source[0], source[1], source[2], source[3]
    EXPECT_EQ(sub.value_at(Ordinal(0)), 130);
    EXPECT_EQ(call_count, 4);

    // don't have to call the func again
    EXPECT_EQ(sub.value_at(Ordinal(0)), 130);
    EXPECT_EQ(call_count, 4);

    // sub[2] -> source[5].
    // source[0..3] are in cache, have to compute source[4], source[5]
    EXPECT_EQ(sub.value_at(Ordinal(2)), 150);
    EXPECT_EQ(call_count, 6);

    // sub[1] -> source[4], source[4] was computed
    EXPECT_EQ(sub.value_at(Ordinal(1)), 140);
    EXPECT_EQ(call_count, 6);

    EXPECT_EQ(sub.value_at(Ordinal(2)), 150);
    EXPECT_EQ(call_count, 6);
}