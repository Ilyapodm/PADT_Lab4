#include <gtest/gtest.h>
#include <functional>
#include <stdexcept>

#include "core/ordinal.hpp"
#include "generators/function_generator.hpp"
#include "nodes/source_node.hpp"
#include "nodes/append_node.hpp"
#include "helpers/call_counter.hpp"
#include "utils/generator_functions.hpp"

static void expect_ordinal_eq(const Ordinal& value, std::size_t omega_coeff, std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

TEST(AppendNodeTest, ConstructorComputesFiniteLength) {
    FunctionGenerator<int> gen(linear_function, Ordinal(5));
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 999);

    // source.length = 5
    // append.length = 6
    expect_ordinal_eq(app.length(), 0, 6);
}

TEST(AppendNodeTest, ValueAtReturnsSourceValuesAndAppendedValue) {
    FunctionGenerator<int> gen(linear_function, Ordinal(5));
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 999);

    EXPECT_EQ(app.value_at(Ordinal(0)), 100);
    EXPECT_EQ(app.value_at(Ordinal(1)), 110);
    EXPECT_EQ(app.value_at(Ordinal(2)), 120);
    EXPECT_EQ(app.value_at(Ordinal(3)), 130);
    EXPECT_EQ(app.value_at(Ordinal(4)), 140);

    // append_index = source.length = 5
    EXPECT_EQ(app.value_at(Ordinal(5)), 999);
}

TEST(AppendNodeTest, ValueAtThrowsWhenIndexIsOutOfRange) {
    FunctionGenerator<int> gen(linear_function, Ordinal(5));
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 999);

    // length = 6, valid indices: 0..5
    EXPECT_THROW({
        app.value_at(Ordinal(6));
    }, std::out_of_range);
}

TEST(AppendNodeTest, AppendToEmptySourceWorks) {
    FunctionGenerator<int> gen(linear_function, Ordinal(0));
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 777);

    expect_ordinal_eq(app.length(), 0, 1);

    EXPECT_EQ(app.value_at(Ordinal(0)), 777);

    EXPECT_THROW({
        app.value_at(Ordinal(1));
    }, std::out_of_range);
}

TEST(AppendNodeTest, AppendToOmegaSourceHasOmegaPlusOneLength) {
    FunctionGenerator<int> gen(linear_function, Ordinal::omega());
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 999);

    // source.length = omega
    // append.length = omega + 1
    expect_ordinal_eq(app.length(), 1, 1);
}

TEST(AppendNodeTest, ValueAtOmegaReturnsAppendedValue) {
    FunctionGenerator<int> gen(linear_function, Ordinal::omega());
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 999);

    EXPECT_EQ(app.value_at(Ordinal(0)), 100);
    EXPECT_EQ(app.value_at(Ordinal(1)), 110);
    EXPECT_EQ(app.value_at(Ordinal(10)), 200);

    // append_index = source.length = omega
    EXPECT_EQ(app.value_at(Ordinal::omega()), 999);
}

TEST(AppendNodeTest, ValueAtOmegaPlusOneThrows) {
    FunctionGenerator<int> gen(linear_function, Ordinal::omega());
    SourceNode<int> source(gen);

    AppendNode<int> app(source, 999);

    // length = omega + 1, so omega + 1 is out of range
    EXPECT_THROW({
        app.value_at(Ordinal::omega() + Ordinal(1));
    }, std::out_of_range);
}

TEST(AppendNodeTest, AccessingAppendedValueDoesNotEvaluateSource) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        CountingFunction(call_count, linear_function),
        Ordinal(5)
    );

    SourceNode<int> source(gen);
    AppendNode<int> app(source, 999);

    EXPECT_EQ(call_count, 0);

    // append_index = source.length = 5
    EXPECT_EQ(app.value_at(Ordinal(5)), 999);

    // don't touch the source
    EXPECT_EQ(call_count, 0);
}

TEST(AppendNodeTest, RepeatedSourceAccessUsesSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        CountingFunction(call_count, linear_function),
        Ordinal(10)
    );

    SourceNode<int> source(gen);
    AppendNode<int> app(source, 999);

    EXPECT_EQ(call_count, 0);

    // app[3] -> source[3]
    // SourceNode will compute source[0..3]
    EXPECT_EQ(app.value_at(Ordinal(3)), 130);
    EXPECT_EQ(call_count, 4);

    // repeated access to source[3] will not call again
    EXPECT_EQ(app.value_at(Ordinal(3)), 130);
    EXPECT_EQ(call_count, 4);

    // app[5] -> source[5]
    // source[0..3] already exists, have to call twice for source[4], source[5]
    EXPECT_EQ(app.value_at(Ordinal(5)), 150);
    EXPECT_EQ(call_count, 6);

    // app[4] -> source[4], is already in cache
    EXPECT_EQ(app.value_at(Ordinal(4)), 140);
    EXPECT_EQ(call_count, 6);

    // app[10] — append value, don't call the source
    EXPECT_EQ(app.value_at(Ordinal(10)), 999);
    EXPECT_EQ(call_count, 6);
}

TEST(AppendNodeTest, OwnsIndependentCloneOfSource) {
    AppendNode<int>* app = nullptr;

    {
        FunctionGenerator<int> gen(linear_function, Ordinal(5));
        SourceNode<int> source(gen);

        app = new AppendNode<int>(source, 999);

        EXPECT_EQ(app->value_at(Ordinal(0)), 100);
        EXPECT_EQ(app->value_at(Ordinal(5)), 999);
    }

    EXPECT_EQ(app->value_at(Ordinal(0)), 100);
    EXPECT_EQ(app->value_at(Ordinal(1)), 110);
    EXPECT_EQ(app->value_at(Ordinal(5)), 999);

    delete app;
}

TEST(AppendNodeTest, CopyConstructorCreatesIndependentWorkingCopy) {
    AppendNode<int>* copy = nullptr;

    {
        FunctionGenerator<int> gen(linear_function, Ordinal(5));
        SourceNode<int> source(gen);

        AppendNode<int> original(source, 999);

        EXPECT_EQ(original.value_at(Ordinal(2)), 120);
        EXPECT_EQ(original.value_at(Ordinal(5)), 999);

        copy = new AppendNode<int>(original);
    }

    expect_ordinal_eq(copy->length(), 0, 6);

    EXPECT_EQ(copy->value_at(Ordinal(0)), 100);
    EXPECT_EQ(copy->value_at(Ordinal(1)), 110);
    EXPECT_EQ(copy->value_at(Ordinal(2)), 120);
    EXPECT_EQ(copy->value_at(Ordinal(3)), 130);
    EXPECT_EQ(copy->value_at(Ordinal(4)), 140);
    EXPECT_EQ(copy->value_at(Ordinal(5)), 999);

    delete copy;
}

TEST(AppendNodeTest, ClonePreservesMaterializedSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        CountingFunction(call_count, linear_function),
        Ordinal(10)
    );

    SourceNode<int> source(gen);
    AppendNode<int> app(source, 999);

    // app[3] -> source[3]
    EXPECT_EQ(app.value_at(Ordinal(3)), 130);
    EXPECT_EQ(call_count, 4);

    AppendNode<int>* cloned = app.clone();

    // don't call, because clone copy the cache
    EXPECT_EQ(cloned->value_at(Ordinal(3)), 130);
    EXPECT_EQ(call_count, 4);

    // app[5] -> source[5], calculate source[4], source[5]
    EXPECT_EQ(cloned->value_at(Ordinal(5)), 150);
    EXPECT_EQ(call_count, 6);

    // app[10] — append value, don't call
    EXPECT_EQ(cloned->value_at(Ordinal(10)), 999);
    EXPECT_EQ(call_count, 6);

    delete cloned;
}

TEST(AppendNodeTest, AssignmentOperatorCopiesSourceAndValue) {
    FunctionGenerator<int> gen1(linear_function, Ordinal(5));
    SourceNode<int> source1(gen1);

    AppendNode<int> first(source1, 999);
    // first = [100, 110, 120, 130, 140, 999]

    FunctionGenerator<int> gen2(
        plus_1000_function,
        Ordinal(3)
    );

    SourceNode<int> source2(gen2);

    AppendNode<int> second(source2, 777);
    // second = [1000, 1001, 1002, 777]

    second = first;

    expect_ordinal_eq(second.length(), 0, 6);

    EXPECT_EQ(second.value_at(Ordinal(0)), 100);
    EXPECT_EQ(second.value_at(Ordinal(1)), 110);
    EXPECT_EQ(second.value_at(Ordinal(2)), 120);
    EXPECT_EQ(second.value_at(Ordinal(3)), 130);
    EXPECT_EQ(second.value_at(Ordinal(4)), 140);
    EXPECT_EQ(second.value_at(Ordinal(5)), 999);
}