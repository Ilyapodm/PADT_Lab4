#include <gtest/gtest.h>
#include <stdexcept>
#include <functional>

#include "function_generator.hpp"
#include "source_node.hpp"
#include "concat_node.hpp"
#include "remove_at_node.hpp"
#include "ordinal.hpp"

static Ordinal omega_plus(std::size_t finite_part) {
    return Ordinal(1, finite_part);
}

static void expect_ordinal_eq(const Ordinal& value, std::size_t omega_coeff, std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

static int identity_value(std::size_t index) {
    return static_cast<int>(index);
}

static int tail_value(std::size_t index) {
    return static_cast<int>(1000 + index);
}

TEST(RemoveAtNodeTest, RemovesMiddleElementFromFiniteSource) {
    FunctionGenerator<int> gen(identity_value, Ordinal(5));
    SourceNode<int> source(gen);

    RemoveAtNode<int> node(source, Ordinal(2));

    expect_ordinal_eq(node.length(), 0, 4);

    EXPECT_EQ(node.value_at(Ordinal(0)), 0);
    EXPECT_EQ(node.value_at(Ordinal(1)), 1);
    EXPECT_EQ(node.value_at(Ordinal(2)), 3);
    EXPECT_EQ(node.value_at(Ordinal(3)), 4);
}

TEST(RemoveAtNodeTest, RemovesFirstElementFromFiniteSource) {
    FunctionGenerator<int> gen(identity_value, Ordinal(5));
    SourceNode<int> source(gen);

    RemoveAtNode<int> node(source, Ordinal(0));

    expect_ordinal_eq(node.length(), 0, 4);

    EXPECT_EQ(node.value_at(Ordinal(0)), 1);
    EXPECT_EQ(node.value_at(Ordinal(1)), 2);
    EXPECT_EQ(node.value_at(Ordinal(2)), 3);
    EXPECT_EQ(node.value_at(Ordinal(3)), 4);
}

TEST(RemoveAtNodeTest, RemovesLastElementFromFiniteSource) {
    FunctionGenerator<int> gen(identity_value, Ordinal(5));
    SourceNode<int> source(gen);

    RemoveAtNode<int> node(source, Ordinal(4));

    expect_ordinal_eq(node.length(), 0, 4);

    EXPECT_EQ(node.value_at(Ordinal(0)), 0);
    EXPECT_EQ(node.value_at(Ordinal(1)), 1);
    EXPECT_EQ(node.value_at(Ordinal(2)), 2);
    EXPECT_EQ(node.value_at(Ordinal(3)), 3);
}

TEST(RemoveAtNodeTest, ValueAtThrowsWhenIndexIsOutOfResultRange) {
    FunctionGenerator<int> gen(identity_value, Ordinal(5));
    SourceNode<int> source(gen);

    RemoveAtNode<int> node(source, Ordinal(2));

    EXPECT_THROW({
        node.value_at(Ordinal(4));
    }, std::out_of_range);
}

TEST(RemoveAtNodeTest, ConstructorThrowsWhenIndexEqualsFiniteLength) {
    FunctionGenerator<int> gen(identity_value, Ordinal(5));
    SourceNode<int> source(gen);

    EXPECT_THROW({
        RemoveAtNode<int> node(source, Ordinal(5));
    }, std::out_of_range);

    EXPECT_THROW({
        RemoveAtNode<int> node(source, Ordinal(6));
    }, std::out_of_range);
}

TEST(RemoveAtNodeTest, RemovesFiniteIndexFromOmegaSource) {
    FunctionGenerator<int> gen(identity_value, Ordinal::omega());
    SourceNode<int> source(gen);

    RemoveAtNode<int> node(source, Ordinal(5));

    expect_ordinal_eq(node.length(), 1, 0);

    EXPECT_EQ(node.value_at(Ordinal(0)), 0);
    EXPECT_EQ(node.value_at(Ordinal(4)), 4);
    EXPECT_EQ(node.value_at(Ordinal(5)), 6);
    EXPECT_EQ(node.value_at(Ordinal(6)), 7);
    EXPECT_EQ(node.value_at(Ordinal(10)), 11);
}

TEST(RemoveAtNodeTest, ConstructorThrowsWhenRemovingOmegaIndexFromOmegaSource) {
    FunctionGenerator<int> gen(identity_value, Ordinal::omega());
    SourceNode<int> source(gen);

    EXPECT_THROW({
        RemoveAtNode<int> node(source, Ordinal::omega());
    }, std::out_of_range);
}

TEST(RemoveAtNodeTest, RemovesFiniteIndexFromOmegaPlusFiniteSource) {
    FunctionGenerator<int> left_gen(identity_value, Ordinal::omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);
    RemoveAtNode<int> node(source, Ordinal(5));

    expect_ordinal_eq(node.length(), 1, 3);

    EXPECT_EQ(node.value_at(Ordinal(4)), 4);
    EXPECT_EQ(node.value_at(Ordinal(5)), 6);
    EXPECT_EQ(node.value_at(Ordinal(10)), 11);

    
    EXPECT_EQ(node.value_at(omega_plus(0)), 1000);
    EXPECT_EQ(node.value_at(omega_plus(1)), 1001);
    EXPECT_EQ(node.value_at(omega_plus(2)), 1002);
}

TEST(RemoveAtNodeTest, RemovesElementFromFiniteTailAfterOmega) {
    FunctionGenerator<int> left_gen(identity_value, Ordinal::omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);
    RemoveAtNode<int> node(source, omega_plus(1));

    expect_ordinal_eq(node.length(), 1, 2);

    EXPECT_EQ(node.value_at(Ordinal(0)), 0);
    EXPECT_EQ(node.value_at(Ordinal(10)), 10);

    EXPECT_EQ(node.value_at(omega_plus(0)), 1000);
    EXPECT_EQ(node.value_at(omega_plus(1)), 1002);

    EXPECT_THROW({
        node.value_at(omega_plus(2));
    }, std::out_of_range);
}

TEST(RemoveAtNodeTest, RemovesLastElementFromFiniteTailAfterOmega) {
    FunctionGenerator<int> left_gen(identity_value, Ordinal::omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);
    RemoveAtNode<int> node(source, omega_plus(2));

    expect_ordinal_eq(node.length(), 1, 2);

    EXPECT_EQ(node.value_at(omega_plus(0)), 1000);
    EXPECT_EQ(node.value_at(omega_plus(1)), 1001);

    EXPECT_THROW({
        node.value_at(omega_plus(2));
    }, std::out_of_range);
}

TEST(RemoveAtNodeTest, ConstructorThrowsWhenIndexEqualsOmegaPlusFiniteLength) {
    FunctionGenerator<int> left_gen(identity_value, Ordinal::omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);

    EXPECT_THROW({
        RemoveAtNode<int> node(source, omega_plus(3));
    }, std::out_of_range);
}

TEST(RemoveAtNodeTest, OwnsIndependentCloneOfSource) {
    RemoveAtNode<int>* node = nullptr;

    {
        FunctionGenerator<int> gen(identity_value, Ordinal(6));
        SourceNode<int> source(gen);

        node = new RemoveAtNode<int>(source, Ordinal(2));

        EXPECT_EQ(node->value_at(Ordinal(0)), 0);
        EXPECT_EQ(node->value_at(Ordinal(2)), 3);
    }

    // node continues to work
    EXPECT_EQ(node->value_at(Ordinal(0)), 0);
    EXPECT_EQ(node->value_at(Ordinal(1)), 1);
    EXPECT_EQ(node->value_at(Ordinal(2)), 3);
    EXPECT_EQ(node->value_at(Ordinal(3)), 4);

    delete node;
}

TEST(RemoveAtNodeTest, CopyConstructorCreatesIndependentCopy) {
    RemoveAtNode<int>* copy = nullptr;

    {
        FunctionGenerator<int> gen(identity_value, Ordinal(6));
        SourceNode<int> source(gen);

        RemoveAtNode<int> original(source, Ordinal(2));

        EXPECT_EQ(original.value_at(Ordinal(2)), 3);

        copy = new RemoveAtNode<int>(original);
    }

    expect_ordinal_eq(copy->length(), 0, 5);

    EXPECT_EQ(copy->value_at(Ordinal(0)), 0);
    EXPECT_EQ(copy->value_at(Ordinal(1)), 1);
    EXPECT_EQ(copy->value_at(Ordinal(2)), 3);
    EXPECT_EQ(copy->value_at(Ordinal(3)), 4);
    EXPECT_EQ(copy->value_at(Ordinal(4)), 5);

    delete copy;
}

TEST(RemoveAtNodeTest, CloneCreatesIndependentCopy) {
    RemoveAtNode<int>* cloned = nullptr;

    {
        FunctionGenerator<int> gen(identity_value, Ordinal(6));
        SourceNode<int> source(gen);

        RemoveAtNode<int> original(source, Ordinal(2));

        EXPECT_EQ(original.value_at(Ordinal(2)), 3);

        cloned = original.clone();
    }

    expect_ordinal_eq(cloned->length(), 0, 5);

    EXPECT_EQ(cloned->value_at(Ordinal(0)), 0);
    EXPECT_EQ(cloned->value_at(Ordinal(1)), 1);
    EXPECT_EQ(cloned->value_at(Ordinal(2)), 3);
    EXPECT_EQ(cloned->value_at(Ordinal(3)), 4);
    EXPECT_EQ(cloned->value_at(Ordinal(4)), 5);

    delete cloned;
}

TEST(RemoveAtNodeTest, AssignmentOperatorCopiesSourceIndexAndLength) {
    FunctionGenerator<int> gen1(identity_value, Ordinal(6));
    SourceNode<int> source1(gen1);

    RemoveAtNode<int> first(source1, Ordinal(2));
    // first = [0, 1, 3, 4, 5]

    FunctionGenerator<int> gen2(
        [](std::size_t index) {
            return static_cast<int>(1000 + index);
        },
        Ordinal(6)
    );
    SourceNode<int> source2(gen2);

    RemoveAtNode<int> second(source2, Ordinal(0));
    // second = [1001, 1002, 1003, 1004, 1005]

    second = first;

    expect_ordinal_eq(second.length(), 0, 5);

    EXPECT_EQ(second.value_at(Ordinal(0)), 0);
    EXPECT_EQ(second.value_at(Ordinal(1)), 1);
    EXPECT_EQ(second.value_at(Ordinal(2)), 3);
    EXPECT_EQ(second.value_at(Ordinal(3)), 4);
    EXPECT_EQ(second.value_at(Ordinal(4)), 5);
}

TEST(RemoveAtNodeTest, RepeatedAccessUsesSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(20)
    );

    SourceNode<int> source(gen);
    RemoveAtNode<int> node(source, Ordinal(3));

    EXPECT_EQ(call_count, 0);

    // result[3] -> source[4]
    // SourceNode will calculate source[0], source[1], source[2], source[3], source[4]
    EXPECT_EQ(node.value_at(Ordinal(3)), 140);
    EXPECT_EQ(call_count, 5);

    // don't call again
    EXPECT_EQ(node.value_at(Ordinal(3)), 140);
    EXPECT_EQ(call_count, 5);

    // result[2] -> source[2], don't call
    EXPECT_EQ(node.value_at(Ordinal(2)), 120);
    EXPECT_EQ(call_count, 5);

    // result[4] -> source[5], only one call
    EXPECT_EQ(node.value_at(Ordinal(4)), 150);
    EXPECT_EQ(call_count, 6);
}

TEST(RemoveAtNodeTest, ClonePreservesAlreadyMaterializedCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(20)
    );

    SourceNode<int> source(gen);
    RemoveAtNode<int> node(source, Ordinal(3));

    // result[3] -> source[4], calculates source[0..4]
    EXPECT_EQ(node.value_at(Ordinal(3)), 140);
    EXPECT_EQ(call_count, 5);

    RemoveAtNode<int>* cloned = node.clone();

    EXPECT_EQ(cloned->value_at(Ordinal(3)), 140);
    EXPECT_EQ(call_count, 5);

    // result[5] -> source[6].
    // source[0..4] exists, have to calculate source[5], source[6].
    EXPECT_EQ(cloned->value_at(Ordinal(5)), 160);
    EXPECT_EQ(call_count, 7);

    delete cloned;
}