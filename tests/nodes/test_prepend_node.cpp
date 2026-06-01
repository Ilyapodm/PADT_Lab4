#include <gtest/gtest.h>
#include <stdexcept>
#include <functional>

#include "core/ordinal.hpp"
#include "generators/function_generator.hpp"
#include "nodes/source_node.hpp"
#include "nodes/concat_node.hpp"
#include "nodes/prepend_node.hpp"

static void expect_ordinal_eq(const Ordinal& value,
                              std::size_t omega_coeff,
                              std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

static int linear_value(std::size_t index) {
    return static_cast<int>(100 + index * 10);
}


TEST(PrependNodeTest, FiniteLengthIncreasesByOne) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    PrependNode<int> prepended(source, 999);

    expect_ordinal_eq(prepended.length(), 0, 6);
}

TEST(PrependNodeTest, FirstElementIsPrependedValue) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    PrependNode<int> prepended(source, 999);

    EXPECT_EQ(prepended.value_at(Ordinal(0)), 999);
}

TEST(PrependNodeTest, FiniteIndexesAreShiftedToSource) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    PrependNode<int> prepended(source, 999);

    // source = [100, 110, 120, 130, 140]
    // prepended = [999, 100, 110, 120, 130, 140]

    EXPECT_EQ(prepended.value_at(Ordinal(0)), 999);
    EXPECT_EQ(prepended.value_at(Ordinal(1)), 100); // source[0]
    EXPECT_EQ(prepended.value_at(Ordinal(2)), 110); // source[1]
    EXPECT_EQ(prepended.value_at(Ordinal(3)), 120); // source[2]
    EXPECT_EQ(prepended.value_at(Ordinal(4)), 130); // source[3]
    EXPECT_EQ(prepended.value_at(Ordinal(5)), 140); // source[4]
}

TEST(PrependNodeTest, ValueAtThrowsWhenIndexIsOutOfRange) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    PrependNode<int> prepended(source, 999);

    // length = 6, valid indices: 0..5
    EXPECT_THROW({
        prepended.value_at(Ordinal(6));
    }, std::out_of_range);
}

TEST(PrependNodeTest, WorksWithEmptySource) {
    FunctionGenerator<int> gen(linear_value, Ordinal(0));
    SourceNode<int> source(gen);

    PrependNode<int> prepended(source, 999);

    expect_ordinal_eq(prepended.length(), 0, 1);

    EXPECT_EQ(prepended.value_at(Ordinal(0)), 999);

    EXPECT_THROW({
        prepended.value_at(Ordinal(1));
    }, std::out_of_range);
}

TEST(PrependNodeTest, OmegaLengthStaysOmega) {
    FunctionGenerator<int> gen(linear_value, Ordinal::omega());
    SourceNode<int> source(gen);

    PrependNode<int> prepended(source, 999);

    expect_ordinal_eq(prepended.length(), 1, 0);

    EXPECT_EQ(prepended.value_at(Ordinal(0)), 999);
    EXPECT_EQ(prepended.value_at(Ordinal(1)), 100);  // source[0]
    EXPECT_EQ(prepended.value_at(Ordinal(2)), 110);  // source[1]
    EXPECT_EQ(prepended.value_at(Ordinal(10)), 190); // source[9]
}

TEST(PrependNodeTest, RepeatedAccessUsesSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(10)
    );

    SourceNode<int> source(gen);
    PrependNode<int> prepended(source, 999);

    EXPECT_EQ(call_count, 0);

    // don't have to calculate prepended[0] 
    EXPECT_EQ(prepended.value_at(Ordinal(0)), 999);
    EXPECT_EQ(call_count, 0);

    // prepended[1] -> source[0]
    EXPECT_EQ(prepended.value_at(Ordinal(1)), 100);
    EXPECT_EQ(call_count, 1);

    // second to source[0] don't have to calculate
    EXPECT_EQ(prepended.value_at(Ordinal(1)), 100);
    EXPECT_EQ(call_count, 1);

    // prepended[3] -> source[2].
    // have to calculate source[1], source[2].
    EXPECT_EQ(prepended.value_at(Ordinal(3)), 120);
    EXPECT_EQ(call_count, 3);

    // prepended[2] -> source[1]
    EXPECT_EQ(prepended.value_at(Ordinal(2)), 110);
    EXPECT_EQ(call_count, 3);

    
    EXPECT_EQ(prepended.value_at(Ordinal(3)), 120);
    EXPECT_EQ(call_count, 3);
}

TEST(PrependNodeTest, CopyConstructorCreatesWorkingIndependentCopy) {
    PrependNode<int>* copy = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(5));
        SourceNode<int> source(gen);

        PrependNode<int> original(source, 999);

        EXPECT_EQ(original.value_at(Ordinal(0)), 999);
        EXPECT_EQ(original.value_at(Ordinal(1)), 100);
        EXPECT_EQ(original.value_at(Ordinal(5)), 140);

        copy = new PrependNode<int>(original);
    }

    expect_ordinal_eq(copy->length(), 0, 6);

    EXPECT_EQ(copy->value_at(Ordinal(0)), 999);
    EXPECT_EQ(copy->value_at(Ordinal(1)), 100);
    EXPECT_EQ(copy->value_at(Ordinal(2)), 110);
    EXPECT_EQ(copy->value_at(Ordinal(5)), 140);

    delete copy;
}

TEST(PrependNodeTest, ClonePreservesAlreadyMaterializedCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(10)
    );

    SourceNode<int> source(gen);
    PrependNode<int> prepended(source, 999);

    // prepended[3] -> source[2], have to calculate source[0..2]
    EXPECT_EQ(prepended.value_at(Ordinal(3)), 120);
    EXPECT_EQ(call_count, 3);

    PrependNode<int>* cloned = prepended.clone();

    // don't have to call again
    EXPECT_EQ(cloned->value_at(Ordinal(3)), 120);
    EXPECT_EQ(call_count, 3);

    // cloned[5] -> source[4].
    // source[0..2] already exists, need only source[3], source[4].
    EXPECT_EQ(cloned->value_at(Ordinal(5)), 140);
    EXPECT_EQ(call_count, 5);

    delete cloned;
}

TEST(PrependNodeTest, AssignmentOperatorCopiesSourceValueAndLength) {
    FunctionGenerator<int> gen1(linear_value, Ordinal(5));
    SourceNode<int> source1(gen1);

    PrependNode<int> first(source1, 999);
    // first = [999, 100, 110, 120, 130, 140]

    FunctionGenerator<int> gen2(
        [](std::size_t index) {
            return static_cast<int>(1000 + index);
        },
        Ordinal(3)
    );
    SourceNode<int> source2(gen2);

    PrependNode<int> second(source2, -1);
    // second = [-1, 1000, 1001, 1002]

    second = first;

    expect_ordinal_eq(second.length(), 0, 6);

    EXPECT_EQ(second.value_at(Ordinal(0)), 999);
    EXPECT_EQ(second.value_at(Ordinal(1)), 100);
    EXPECT_EQ(second.value_at(Ordinal(2)), 110);
    EXPECT_EQ(second.value_at(Ordinal(5)), 140);
}

TEST(PrependNodeTest, TransfiniteIndexesAreNotShifted) {
    FunctionGenerator<int> left_gen(
        [](std::size_t index) {
            return static_cast<int>(index);
        },
        Ordinal::omega()
    );

    FunctionGenerator<int> right_gen(
        [](std::size_t index) {
            return static_cast<int>(1000 + index);
        },
        Ordinal(3)
    );

    SourceNode<int> left(left_gen);
    SourceNode<int> right(right_gen);

    // source.length = omega + 3
    ConcatNode<int> source(left, right);

    PrependNode<int> prepended(source, -1);

    // 1 + (omega + 3) = omega + 3
    expect_ordinal_eq(prepended.length(), 1, 3);

    EXPECT_EQ(prepended.value_at(Ordinal(0)), -1);

    // finite part is moving:
    // prepended[1] -> source[0]
    // prepended[2] -> source[1]
    EXPECT_EQ(prepended.value_at(Ordinal(1)), 0);
    EXPECT_EQ(prepended.value_at(Ordinal(2)), 1);

    // but infinite part isn't:
    // prepended[omega] -> source[omega] -> right[0]
    // prepended[omega + 1] -> source[omega + 1] -> right[1]
    // prepended[omega + 2] -> source[omega + 2] -> right[2]
    EXPECT_EQ(prepended.value_at(Ordinal::omega()), 1000);
    EXPECT_EQ(prepended.value_at(Ordinal(1, 1)), 1001);
    EXPECT_EQ(prepended.value_at(Ordinal(1, 2)), 1002);
}