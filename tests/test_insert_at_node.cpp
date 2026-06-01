#include <gtest/gtest.h>
#include <stdexcept>
#include <functional>

#include "ordinal.hpp"
#include "function_generator.hpp"
#include "source_node.hpp"
#include "concat_node.hpp"
#include "insert_at_node.hpp"

static Ordinal omega() {
    return Ordinal(1, 0);
}

static Ordinal omega_plus(std::size_t finite_part) {
    return Ordinal(1, finite_part);
}

static void expect_ordinal_eq(const Ordinal& value, std::size_t omega_coeff, std::size_t finite_part) {
    EXPECT_EQ(value.get_omega_coeff(), omega_coeff);
    EXPECT_EQ(value.get_finite_part(), finite_part);
}

static int linear_value(std::size_t index) {
    return static_cast<int>(100 + index * 10);
}

static int tail_value(std::size_t index) {
    return static_cast<int>(1000 + index);
}

TEST(InsertAtNodeTest, InsertsIntoMiddleOfFiniteSource) {
    FunctionGenerator<int> gen(linear_value, Ordinal(5));
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(2));

    // source: 100, 110, 120, 130, 140
    // result: 100, 110, 999, 120, 130, 140

    expect_ordinal_eq(inserted.length(), 0, 6);

    EXPECT_EQ(inserted.value_at(Ordinal(0)), 100);
    EXPECT_EQ(inserted.value_at(Ordinal(1)), 110);
    EXPECT_EQ(inserted.value_at(Ordinal(2)), 999);
    EXPECT_EQ(inserted.value_at(Ordinal(3)), 120);
    EXPECT_EQ(inserted.value_at(Ordinal(4)), 130);
    EXPECT_EQ(inserted.value_at(Ordinal(5)), 140);
}

TEST(InsertAtNodeTest, InsertsAtBeginningOfFiniteSource) {
    FunctionGenerator<int> gen(linear_value, Ordinal(4));
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(0));

    // source: 100, 110, 120, 130
    // result: 999, 100, 110, 120, 130

    expect_ordinal_eq(inserted.length(), 0, 5);

    EXPECT_EQ(inserted.value_at(Ordinal(0)), 999);
    EXPECT_EQ(inserted.value_at(Ordinal(1)), 100);
    EXPECT_EQ(inserted.value_at(Ordinal(2)), 110);
    EXPECT_EQ(inserted.value_at(Ordinal(3)), 120);
    EXPECT_EQ(inserted.value_at(Ordinal(4)), 130);
}

TEST(InsertAtNodeTest, InsertsAtEndOfFiniteSource) {
    FunctionGenerator<int> gen(linear_value, Ordinal(4));
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(4));

    // source: 100, 110, 120, 130
    // result: 100, 110, 120, 130, 999

    expect_ordinal_eq(inserted.length(), 0, 5);

    EXPECT_EQ(inserted.value_at(Ordinal(0)), 100);
    EXPECT_EQ(inserted.value_at(Ordinal(1)), 110);
    EXPECT_EQ(inserted.value_at(Ordinal(2)), 120);
    EXPECT_EQ(inserted.value_at(Ordinal(3)), 130);
    EXPECT_EQ(inserted.value_at(Ordinal(4)), 999);
}

TEST(InsertAtNodeTest, ValueAtThrowsWhenIndexIsOutOfRange) {
    FunctionGenerator<int> gen(linear_value, Ordinal(4));
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(2));

    // result length = 5, valid indices: 0..4

    EXPECT_THROW({
        inserted.value_at(Ordinal(5));
    }, std::out_of_range);
}

TEST(InsertAtNodeTest, InsertsIntoEmptySource) {
    FunctionGenerator<int> gen(linear_value, Ordinal(0));
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(0));

    expect_ordinal_eq(inserted.length(), 0, 1);

    EXPECT_EQ(inserted.value_at(Ordinal(0)), 999);

    EXPECT_THROW({
        inserted.value_at(Ordinal(1));
    }, std::out_of_range);
}

TEST(InsertAtNodeTest, InsertsIntoFinitePartOfOmegaSource) {
    FunctionGenerator<int> gen(linear_value, omega());
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(3));

    // source: 100, 110, 120, 130, 140, ...
    // result: 100, 110, 120, 999, 130, 140, ...

    // don't change the length
    expect_ordinal_eq(inserted.length(), 1, 0);

    EXPECT_EQ(inserted.value_at(Ordinal(0)), 100);
    EXPECT_EQ(inserted.value_at(Ordinal(1)), 110);
    EXPECT_EQ(inserted.value_at(Ordinal(2)), 120);
    EXPECT_EQ(inserted.value_at(Ordinal(3)), 999);
    EXPECT_EQ(inserted.value_at(Ordinal(4)), 130);
    EXPECT_EQ(inserted.value_at(Ordinal(5)), 140);
    EXPECT_EQ(inserted.value_at(Ordinal(10)), 190);
}

TEST(InsertAtNodeTest, InsertsAtOmegaBoundary) {
    FunctionGenerator<int> gen(linear_value, omega());
    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, omega());

    // source.length = omega
    // insert_at(omega) gives length = omega + 1
    // inserted[omega] = 999

    expect_ordinal_eq(inserted.length(), 1, 1);

    EXPECT_EQ(inserted.value_at(Ordinal(0)), 100);
    EXPECT_EQ(inserted.value_at(Ordinal(5)), 150);
    EXPECT_EQ(inserted.value_at(omega()), 999);

    EXPECT_THROW({
        inserted.value_at(omega_plus(1));
    }, std::out_of_range);
}

TEST(InsertAtNodeTest, InsertBeforeOmegaBoundaryDoesNotShiftOmegaBlock) {
    FunctionGenerator<int> left_gen(linear_value, omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);
    // source.length = omega + 3
    // source[omega]     = 1000
    // source[omega + 1] = 1001
    // source[omega + 2] = 1002

    InsertAtNode<int> inserted(source, 999, Ordinal(5));

    // don't change the length
    expect_ordinal_eq(inserted.length(), 1, 3);

    EXPECT_EQ(inserted.value_at(Ordinal(4)), 140);
    EXPECT_EQ(inserted.value_at(Ordinal(5)), 999);
    EXPECT_EQ(inserted.value_at(Ordinal(6)), 150);

    // omega-block doesn't change
    EXPECT_EQ(inserted.value_at(omega()), 1000);
    EXPECT_EQ(inserted.value_at(omega_plus(1)), 1001);
    EXPECT_EQ(inserted.value_at(omega_plus(2)), 1002);
}

TEST(InsertAtNodeTest, InsertsInsideFiniteTailAfterOmega) {
    FunctionGenerator<int> left_gen(linear_value, omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);
    // source.length = omega + 3

    InsertAtNode<int> inserted(source, 999, omega_plus(1));

    // source tail: 1000, 1001, 1002
    // result tail: 1000, 999, 1001, 1002
    // length = omega + 4

    expect_ordinal_eq(inserted.length(), 1, 4);

    EXPECT_EQ(inserted.value_at(omega()), 1000);
    EXPECT_EQ(inserted.value_at(omega_plus(1)), 999);
    EXPECT_EQ(inserted.value_at(omega_plus(2)), 1001);
    EXPECT_EQ(inserted.value_at(omega_plus(3)), 1002);
}

TEST(InsertAtNodeTest, InsertsAtEndOfOmegaPlusFiniteSource) {
    FunctionGenerator<int> left_gen(linear_value, omega());
    SourceNode<int> left(left_gen);

    FunctionGenerator<int> right_gen(tail_value, Ordinal(3));
    SourceNode<int> right(right_gen);

    ConcatNode<int> source(left, right);  // source.length = omega + 3

    InsertAtNode<int> inserted(source, 999, omega_plus(3));

    expect_ordinal_eq(inserted.length(), 1, 4);

    EXPECT_EQ(inserted.value_at(omega()), 1000);
    EXPECT_EQ(inserted.value_at(omega_plus(1)), 1001);
    EXPECT_EQ(inserted.value_at(omega_plus(2)), 1002);
    EXPECT_EQ(inserted.value_at(omega_plus(3)), 999);
}

TEST(InsertAtNodeTest, OwnsIndependentCloneOfSource) {
    InsertAtNode<int>* inserted = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(5));
        SourceNode<int> source(gen);

        inserted = new InsertAtNode<int>(source, 999, Ordinal(2));

        EXPECT_EQ(inserted->value_at(Ordinal(2)), 999);
    }

    EXPECT_EQ(inserted->value_at(Ordinal(0)), 100);
    EXPECT_EQ(inserted->value_at(Ordinal(1)), 110);
    EXPECT_EQ(inserted->value_at(Ordinal(2)), 999);
    EXPECT_EQ(inserted->value_at(Ordinal(3)), 120);
    EXPECT_EQ(inserted->value_at(Ordinal(4)), 130);

    delete inserted;
}

TEST(InsertAtNodeTest, CopyConstructorCreatesIndependentCopy) {
    InsertAtNode<int>* copy = nullptr;

    {
        FunctionGenerator<int> gen(linear_value, Ordinal(5));
        SourceNode<int> source(gen);

        InsertAtNode<int> original(source, 999, Ordinal(2));

        EXPECT_EQ(original.value_at(Ordinal(3)), 120);

        copy = new InsertAtNode<int>(original);
    }

    expect_ordinal_eq(copy->length(), 0, 6);

    EXPECT_EQ(copy->value_at(Ordinal(0)), 100);
    EXPECT_EQ(copy->value_at(Ordinal(1)), 110);
    EXPECT_EQ(copy->value_at(Ordinal(2)), 999);
    EXPECT_EQ(copy->value_at(Ordinal(3)), 120);
    EXPECT_EQ(copy->value_at(Ordinal(4)), 130);
    EXPECT_EQ(copy->value_at(Ordinal(5)), 140);

    delete copy;
}

TEST(InsertAtNodeTest, AssignmentOperatorCopiesSourceValueIndexAndLength) {
    FunctionGenerator<int> gen1(linear_value, Ordinal(5));
    SourceNode<int> source1(gen1);

    InsertAtNode<int> first(source1, 999, Ordinal(2));
    // first: 100, 110, 999, 120, 130, 140

    FunctionGenerator<int> gen2(
        [](std::size_t index) {
            return static_cast<int>(2000 + index);
        },
        Ordinal(3)
    );
    SourceNode<int> source2(gen2);

    InsertAtNode<int> second(source2, -1, Ordinal(0));
    // second: -1, 2000, 2001, 2002

    second = first;

    expect_ordinal_eq(second.length(), 0, 6);

    EXPECT_EQ(second.value_at(Ordinal(0)), 100);
    EXPECT_EQ(second.value_at(Ordinal(1)), 110);
    EXPECT_EQ(second.value_at(Ordinal(2)), 999);
    EXPECT_EQ(second.value_at(Ordinal(3)), 120);
    EXPECT_EQ(second.value_at(Ordinal(4)), 130);
    EXPECT_EQ(second.value_at(Ordinal(5)), 140);
}

TEST(InsertAtNodeTest, RepeatedAccessUsesSourceCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(20)
    );

    SourceNode<int> source(gen);

    InsertAtNode<int> inserted(source, 999, Ordinal(3));

    EXPECT_EQ(call_count, 0);

    // inserted[3] inserted, source must not be called
    EXPECT_EQ(inserted.value_at(Ordinal(3)), 999);
    EXPECT_EQ(call_count, 0);

    // inserted[0] -> source[0]
    EXPECT_EQ(inserted.value_at(Ordinal(0)), 100);
    EXPECT_EQ(call_count, 1);

    // repeated call (must use cache)
    EXPECT_EQ(inserted.value_at(Ordinal(0)), 100);
    EXPECT_EQ(call_count, 1);

    // inserted[4] -> source[3].
    // source[0] already exists, have to calculate source[1], source[2], source[3].
    EXPECT_EQ(inserted.value_at(Ordinal(4)), 130);
    EXPECT_EQ(call_count, 4);

    // inserted[2] -> source[2], is in cache
    EXPECT_EQ(inserted.value_at(Ordinal(2)), 120);
    EXPECT_EQ(call_count, 4);
}

TEST(InsertAtNodeTest, ClonePreservesAlreadyMaterializedCache) {
    int call_count = 0;

    FunctionGenerator<int> gen(
        [&call_count](std::size_t index) {
            ++call_count;
            return static_cast<int>(100 + index * 10);
        },
        Ordinal(20)
    );

    SourceNode<int> source(gen);
    InsertAtNode<int> inserted(source, 999, Ordinal(3));

    // inserted[6] -> source[5]
    // source calculate source[0..5], 6 calls at total
    EXPECT_EQ(inserted.value_at(Ordinal(6)), 150);
    EXPECT_EQ(call_count, 6);

    InsertAtNode<int>* cloned = inserted.clone();


    EXPECT_EQ(cloned->value_at(Ordinal(6)), 150);
    EXPECT_EQ(call_count, 6);

    // inserted[7] -> source[6]
    // have to calculate only source[6].
    EXPECT_EQ(cloned->value_at(Ordinal(7)), 160);
    EXPECT_EQ(call_count, 7);

    delete cloned;
}