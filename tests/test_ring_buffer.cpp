#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include "ring_buffer.hpp"

TEST(RingBufferTest, InitializesAndReadsInLogicalOrder) {
    int items[] = {1, 2, 3};

    RingBuffer<int> buffer(items, 3);

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.get(0), 1);
    EXPECT_EQ(buffer.get(1), 2);
    EXPECT_EQ(buffer.get(2), 3);
}

TEST(RingBufferTest, PushOverwritesOldestAndMovesStart) {
    int items[] = {1, 2, 3};

    RingBuffer<int> buffer(items, 3);

    buffer.push(4);

    EXPECT_EQ(buffer.get(0), 2);
    EXPECT_EQ(buffer.get(1), 3);
    EXPECT_EQ(buffer.get(2), 4);
}

TEST(RingBufferTest, MultiplePushsWrapAroundCorrectly) {
    int items[] = {1, 2, 3};

    RingBuffer<int> buffer(items, 3);

    buffer.push(4); // 2, 3, 4
    buffer.push(5); // 3, 4, 5
    buffer.push(6); // 4, 5, 6
    buffer.push(7); // 5, 6, 7

    EXPECT_EQ(buffer.get(0), 5);
    EXPECT_EQ(buffer.get(1), 6);
    EXPECT_EQ(buffer.get(2), 7);
}

TEST(RingBufferTest, ThrowsWhenItemsIsNullptrAndSizeIsPositive) {
    EXPECT_THROW(
        RingBuffer<int> buffer(nullptr, 3),
        std::invalid_argument
    );
}

TEST(RingBufferTest, AllowsZeroSizeWithNullptr) {
    RingBuffer<int> buffer(nullptr, 0);

    EXPECT_EQ(buffer.size(), 0);
    EXPECT_THROW(buffer.get(0), std::out_of_range);
    EXPECT_THROW(buffer.push(10), std::out_of_range);
}

TEST(RingBufferTest, ThrowsOnOutOfRangeGet) {
    int items[] = {1, 2, 3};

    RingBuffer<int> buffer(items, 3);

    EXPECT_THROW(buffer.get(3), std::out_of_range);
    EXPECT_THROW(buffer.get(100), std::out_of_range);
}

TEST(RingBufferTest, CopyConstructorCopiesLogicalState) {
    int items[] = {1, 2, 3};

    RingBuffer<int> original(items, 3);
    original.push(4); // logical: 2, 3, 4

    RingBuffer<int> copy(original);

    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy.get(0), 2);
    EXPECT_EQ(copy.get(1), 3);
    EXPECT_EQ(copy.get(2), 4);

    original.push(5); // original: 3, 4, 5; copy must stay 2, 3, 4

    EXPECT_EQ(copy.get(0), 2);
    EXPECT_EQ(copy.get(1), 3);
    EXPECT_EQ(copy.get(2), 4);
}

TEST(RingBufferTest, CopyAssignmentCopiesLogicalState) {
    int first[] = {1, 2, 3};
    int second[] = {10, 20};

    RingBuffer<int> source(first, 3);
    source.push(4); // logical: 2, 3, 4

    RingBuffer<int> target(second, 2);

    target = source;

    EXPECT_EQ(target.size(), 3);
    EXPECT_EQ(target.get(0), 2);
    EXPECT_EQ(target.get(1), 3);
    EXPECT_EQ(target.get(2), 4);

    source.push(5); // source changes, target must not

    EXPECT_EQ(target.get(0), 2);
    EXPECT_EQ(target.get(1), 3);
    EXPECT_EQ(target.get(2), 4);
}

TEST(RingBufferTest, SelfAssignmentKeepsBufferValid) {
    int items[] = {1, 2, 3};

    RingBuffer<int> buffer(items, 3);
    buffer.push(4); // logical: 2, 3, 4

    buffer = buffer;

    EXPECT_EQ(buffer.size(), 3);
    EXPECT_EQ(buffer.get(0), 2);
    EXPECT_EQ(buffer.get(1), 3);
    EXPECT_EQ(buffer.get(2), 4);
}

TEST(RingBufferTest, WorksWithStrings) {
    std::string items[] = {"a", "b", "c"};

    RingBuffer<std::string> buffer(items, 3);

    buffer.push("d");

    EXPECT_EQ(buffer.get(0), "b");
    EXPECT_EQ(buffer.get(1), "c");
    EXPECT_EQ(buffer.get(2), "d");
}