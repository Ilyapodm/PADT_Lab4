#include <gtest/gtest.h>

#include "nodes/source_node.hpp"
#include "generators/function_generator.hpp"
#include "core/ordinal.hpp"
#include "utils/generator_functions.hpp"
#include "helpers/call_counter.hpp"

TEST(SourceNodeTest, ReturnsLength) {
    FunctionGenerator<int> gen(
        identity_function,
        Ordinal(5)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.length(), Ordinal(5));
}

TEST(SourceNodeTest, MaterializesValueByIndex) {
    FunctionGenerator<int> gen(
        square_function,
        Ordinal(10)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.value_at(Ordinal(0)), 0);
    EXPECT_EQ(node.value_at(Ordinal(3)), 9);
    EXPECT_EQ(node.value_at(Ordinal(5)), 25);
}

TEST(SourceNodeTest, ReusesCachedValue) {
    int calls = 0;

    FunctionGenerator<int> gen(
        CountingFunction(calls, identity_function),
        Ordinal(10)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.value_at(Ordinal(4)), 4);

    // to get index 4, have to compute 0, 1, 2, 3, 4.
    EXPECT_EQ(calls, 5);

    EXPECT_EQ(node.value_at(Ordinal(4)), 4);

    // must be from cash
    EXPECT_EQ(calls, 5);
}

TEST(SourceNodeTest, ExtendsCacheOnlyWhenNeeded) {
    int calls = 0;

    FunctionGenerator<int> gen(
        CountingFunction(calls, identity_function),
        Ordinal(10)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.value_at(Ordinal(2)), 2);
    EXPECT_EQ(calls, 3);

    EXPECT_EQ(node.value_at(Ordinal(5)), 5);

    // extra compute 3, 4, 5.
    EXPECT_EQ(calls, 6);
}

TEST(SourceNodeTest, WorksWithOmegaLength) {
    FunctionGenerator<int> gen(
        plus_10_function,
        Ordinal::omega() // omega
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.length(), Ordinal(1, 0));
    EXPECT_EQ(node.value_at(Ordinal(0)), 10);
    EXPECT_EQ(node.value_at(Ordinal(5)), 15);
}

TEST(SourceNodeTest, ThrowsWhenIndexOutOfRange) {
    FunctionGenerator<int> gen(
        identity_function,
        Ordinal(3)
    );

    SourceNode<int> node(gen);

    EXPECT_NO_THROW(node.value_at(Ordinal(2)));
    EXPECT_THROW(node.value_at(Ordinal(3)), std::out_of_range);
}

TEST(SourceNodeTest, ThrowsOnTransfiniteLocalIndex) {
    FunctionGenerator<int> gen(
        identity_function,
        Ordinal(1, 0) // omega
    );

    SourceNode<int> node(gen);

    EXPECT_THROW(node.value_at(Ordinal(1, 0)), std::out_of_range);
}

TEST(SourceNodeTest, CloneCopiesCache) {
    int calls = 0;

    FunctionGenerator<int> gen(
        CountingFunction(calls, identity_function),
        Ordinal(10)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.value_at(Ordinal(3)), 3);
    EXPECT_EQ(calls, 4);

    SourceNode<int>* cloned = node.clone();

    // index 3 must be in copy
    EXPECT_EQ(cloned->value_at(Ordinal(3)), 3);
    EXPECT_EQ(calls, 4);

    delete cloned;
}

TEST(SourceNodeTest, CloneCopiesGeneratorState) {
    int calls = 0;

    FunctionGenerator<int> gen(
        CountingFunction(calls, identity_function),
        Ordinal(10)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.value_at(Ordinal(2)), 2);
    EXPECT_EQ(calls, 3);

    SourceNode<int>* cloned = node.clone();

    EXPECT_EQ(cloned->value_at(Ordinal(4)), 4);

    // clone have to compute only 3 and 4.
    EXPECT_EQ(calls, 5);

    delete cloned;
}

TEST(SourceNodeTest, CloneIsIndependentFromOriginal) {
    int calls = 0;

    FunctionGenerator<int> gen(
        CountingFunction(calls, identity_function),
        Ordinal(10)
    );

    SourceNode<int> node(gen);

    EXPECT_EQ(node.value_at(Ordinal(2)), 2);
    EXPECT_EQ(calls, 3);

    SourceNode<int>* cloned = node.clone();

    EXPECT_EQ(cloned->value_at(Ordinal(4)), 4);
    EXPECT_EQ(calls, 5);

    EXPECT_EQ(node.value_at(Ordinal(4)), 4);

    // original works independently
    EXPECT_EQ(calls, 7);

    delete cloned;
}