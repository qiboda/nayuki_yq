#include <gtest/gtest.h>

#include <Core/Container/Heap.h>

class HeapTest
    : public ::testing::Test
{
protected:
    HeapTest() {}
    ~HeapTest() override {}

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(HeapTest, RemoveMaxHeapElem)
{
    Heap<double> MaxHeap;
    MaxHeap.Push(1);
    MaxHeap.Push(2);
    MaxHeap.Push(3);
    MaxHeap.Push(4);
    MaxHeap.Push(5);
    MaxHeap.Push(6);
    MaxHeap.Push(7);

    MaxHeap.Remove(8);
    ASSERT_EQ(MaxHeap.Top(), 7);
    ASSERT_EQ(MaxHeap.Size(), 7);

    MaxHeap.Remove(7);
    ASSERT_EQ(MaxHeap.Top(), 6);
    ASSERT_EQ(MaxHeap.Size(), 6);

    MaxHeap.Remove(4);
    ASSERT_EQ(MaxHeap.Top(), 6);
    ASSERT_EQ(MaxHeap.Size(), 5);
}

TEST_F(HeapTest, RemoveMinHeapElem)
{
    Heap<double, std::greater<double>> MinHeap;
    MinHeap.Push(1);
    MinHeap.Push(2);
    MinHeap.Push(3);
    MinHeap.Push(4);
    MinHeap.Push(5);
    MinHeap.Push(6);
    MinHeap.Push(7);

    MinHeap.Remove(8);
    ASSERT_EQ(MinHeap.Top(), 1);
    ASSERT_EQ(MinHeap.Size(), 7);

    MinHeap.Remove(1);
    ASSERT_EQ(MinHeap.Top(), 2);
    ASSERT_EQ(MinHeap.Size(), 6);

    MinHeap.Remove(2);
    ASSERT_EQ(MinHeap.Top(), 3);
    ASSERT_EQ(MinHeap.Size(), 5);
}