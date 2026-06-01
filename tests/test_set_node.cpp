#include <gtest/gtest.h>
#include <stdexcept>
#include <functional>

#include "ordinal.hpp"
#include "function_generator.hpp"
#include "source_node.hpp"
#include "concat_node.hpp"
#include "set_node.hpp"

static void expect_ordinal_eq(const Ordinal& value, std::size_t omega_coeff, std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

static int linear_value(std::size_t index) {
    return static_cast<int>(100 + index * 10);
}

TEST(SetNodeTest, LengthIsEqualToSourceLength) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SetNode<int> node(source, 999, Ordinal(4));

    expect_ordinal_eq(node.length(), 0, 10);
}

TEST(SetNodeTest, ReplacesOnlySelectedIndex) {
    FunctionGenerator<int> gen(linear_value, Ordinal(10));
    SourceNode<int> source(gen);

    SetNode<int> node(source, 999, Ordinal(3));

    EXPECT_EQ(node.value_at(Ordinal(0)), 100);
    EXPECT_EQ(node.value_at(Ordinal(1)), 110);
    EXPECT_EQ(node.value_at(Ordinal(2)), 120);

    EXPECT_EQ(node.value_at(Ordinal(3)), 999);

    EXPECT_EQ(node.value_at(Ordinal(4)), 140);
    EXPECT_EQ(node.value_at(Ordinal(5)), 150);
}

TEST(SetNodeTest, CanReplaceFirstElement) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    SetNode<int> node(source, -1, Ordinal(0));

    EXPECT_EQ(node.value_at(Ordinal(0)), -1);
    EXPECT_EQ(node.value_at(Ordinal(1)), 110);
    EXPECT_EQ(node.value_at(Ordinal(2)), 120);
}

TEST(SetNodeTest, CanReplaceLastFiniteElement) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    SetNode<int> node(source, 777, Ordinal(4));

    EXPECT_EQ(node.value_at(Ordinal(0)), 100);
    EXPECT_EQ(node.value_at(Ordinal(3)), 130);
    EXPECT_EQ(node.value_at(Ordinal(4)), 777);
}

TEST(SetNodeTest, ValueAtThrowsWhenIndexIsOutOfRange) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    SetNode<int> node(source, 999, Ordinal(2));

    EXPECT_THROW({
        node.value_at(Ordinal(5));
    }, std::out_of_range);
}

TEST(SetNodeTest, ConstructorThrowsWhenSetIndexEqualsLength) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    EXPECT_THROW({
        SetNode<int> node(source, 999, Ordinal(5));
    }, std::invalid_argument);
}

TEST(SetNodeTest, ConstructorThrowsWhenSetIndexGreaterThanLength) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    EXPECT_THROW({
        SetNode<int> node(source, 999, Ordinal(10));
    }, std::invalid_argument);
}

TEST(SetNodeTest, AccessToReplacedIndexDoesNotCallSourceFunction) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(10)
    );

    SourceNode<int> source(gen);
    SetNode<int> node(source, 999, Ordinal(4));

    EXPECT_EQ(call_count, 0);

    EXPECT_EQ(node.value_at(Ordinal(4)), 999);

    EXPECT_EQ(call_count, 0);

    EXPECT_EQ(node.value_at(Ordinal(4)), 999);
    EXPECT_EQ(call_count, 0);
}

TEST(SetNodeTest, NonReplacedIndexUsesSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(10)
    );

    SourceNode<int> source(gen);
    SetNode<int> node(source, 999, Ordinal(4));

    EXPECT_EQ(call_count, 0);

    // node[2] -> source[2].
    // SourceNode will compute source[0], source[1], source[2].
    EXPECT_EQ(node.value_at(Ordinal(2)), 120);
    EXPECT_EQ(call_count, 3);

    // don't have to call (use cache)
    EXPECT_EQ(node.value_at(Ordinal(2)), 120);
    EXPECT_EQ(call_count, 3);

    // node[3] -> source[3]
    // have to call only once
    EXPECT_EQ(node.value_at(Ordinal(3)), 130);
    EXPECT_EQ(call_count, 4);

    // node[4] is replaced, source isn't called
    EXPECT_EQ(node.value_at(Ordinal(4)), 999);
    EXPECT_EQ(call_count, 4);
}

TEST(SetNodeTest, OwnsIndependentCloneOfSource) {
    SetNode<int>* node = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(10));
        SourceNode<int> source(gen);

        node = new SetNode<int>(source, 999, Ordinal(3));

        EXPECT_EQ(node->value_at(Ordinal(3)), 999);
    }

    // source and gen have already been deleted
    EXPECT_EQ(node->value_at(Ordinal(0)), 100);
    EXPECT_EQ(node->value_at(Ordinal(3)), 999);
    EXPECT_EQ(node->value_at(Ordinal(4)), 140);

    delete node;
}

TEST(SetNodeTest, CopyConstructorCreatesIndependentCopy) {
    SetNode<int>* copy = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(10));
        SourceNode<int> source(gen);

        SetNode<int> original(source, 999, Ordinal(3));

        EXPECT_EQ(original.value_at(Ordinal(0)), 100);
        EXPECT_EQ(original.value_at(Ordinal(3)), 999);

        copy = new SetNode<int>(original);
    }

    expect_ordinal_eq(copy->length(), 0, 10);

    EXPECT_EQ(copy->value_at(Ordinal(0)), 100);
    EXPECT_EQ(copy->value_at(Ordinal(3)), 999);
    EXPECT_EQ(copy->value_at(Ordinal(4)), 140);

    delete copy;
}

TEST(SetNodeTest, AssignmentOperatorCopiesSourceValueAndIndex) {
    FunctionGenerator<int> gen1(linear_value, Ordinal(10));
    SourceNode<int> source1(gen1);

    SetNode<int> first(source1, 999, Ordinal(3));

    FunctionGenerator<int> gen2(
        [](std::size_t index) {
            return static_cast<int>(1000 + index);
        },
        Ordinal(10)
    );
    SourceNode<int> source2(gen2);

    SetNode<int> second(source2, -1, Ordinal(0));

    EXPECT_EQ(second.value_at(Ordinal(0)), -1);
    EXPECT_EQ(second.value_at(Ordinal(1)), 1001);

    second = first;

    expect_ordinal_eq(second.length(), 0, 10);

    EXPECT_EQ(second.value_at(Ordinal(0)), 100);
    EXPECT_EQ(second.value_at(Ordinal(1)), 110);
    EXPECT_EQ(second.value_at(Ordinal(2)), 120);
    EXPECT_EQ(second.value_at(Ordinal(3)), 999);
    EXPECT_EQ(second.value_at(Ordinal(4)), 140);
}

TEST(SetNodeTest, WorksWithOmegaLengthAndFiniteSetIndex) {
    FunctionGenerator<int> gen(linear_value, Ordinal::omega());
    SourceNode<int> source(gen);

    SetNode<int> node(source, 999, Ordinal(5));

    expect_ordinal_eq(node.length(), 1, 0);

    EXPECT_EQ(node.value_at(Ordinal(0)), 100);
    EXPECT_EQ(node.value_at(Ordinal(4)), 140);
    EXPECT_EQ(node.value_at(Ordinal(5)), 999);
    EXPECT_EQ(node.value_at(Ordinal(6)), 160);
    EXPECT_EQ(node.value_at(Ordinal(20)), 300);
}

TEST(SetNodeTest, WorksWithOrdinalIndexAfterOmega) {
    FunctionGenerator<int> left_gen(linear_value, Ordinal::omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(
        [](std::size_t index) {
            return static_cast<int>(1000 + index);
        },
        Ordinal(3)
    );
    SourceNode<int> right(right_gen);

    ConcatNode<int> concat(left, right);

    // concat.length     = omega + 3
    // concat[omega]     = right[0] = 1000
    // concat[omega + 1] = right[1] = 1001
    // concat[omega + 2] = right[2] = 1002

    SetNode<int> node(concat, 7777, Ordinal::omega() + Ordinal(1));

    expect_ordinal_eq(node.length(), 1, 3);

    EXPECT_EQ(node.value_at(Ordinal(0)), 100);
    EXPECT_EQ(node.value_at(Ordinal(10)), 200);

    EXPECT_EQ(node.value_at(Ordinal::omega()), 1000);
    EXPECT_EQ(node.value_at(Ordinal::omega() + Ordinal(1)), 7777);
    EXPECT_EQ(node.value_at(Ordinal::omega() + Ordinal(2)), 1002);
}