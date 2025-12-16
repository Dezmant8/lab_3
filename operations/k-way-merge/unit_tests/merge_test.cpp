#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include "../KwayMerge.h"

class MergeTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// пустой массив
TEST_F(MergeTest, EmptyArray) {
    std::vector<int> empty;
    auto result = KwayMergeSort(empty);
    EXPECT_TRUE(result.empty());
}

// массив из одного элемента
TEST_F(MergeTest, SingleElement) {
    std::vector<int> single = {42};
    auto result = KwayMergeSort(single);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 42);
}

// уже отсортированный массив
TEST_F(MergeTest, AlreadySorted) {
    std::vector<int> sorted = {1, 2, 3, 4, 5, 6, 7, 8};
    auto result = KwayMergeSort(sorted);
    EXPECT_EQ(result, sorted);
}

// обратно отсортированный массив
TEST_F(MergeTest, ReverseSorted) {
    std::vector<int> reversed = {8, 7, 6, 5, 4, 3, 2, 1};
    auto result = KwayMergeSort(reversed);
    
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_EQ(result, expected);
}

// k=1
TEST_F(MergeTest, KValueOne) {
    std::vector<int> arr = {5, 2, 8, 1};
    auto result = KwayMergeSort(arr, 1, 2);
    
    std::vector<int> expected = {1, 2, 5, 8};
    EXPECT_EQ(result, expected);
}

// k=2
TEST_F(MergeTest, TwoWayMerge) {
    std::vector<int> arr = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    auto result = KwayMergeSort(arr, 2, 3);
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

// k=4
TEST_F(MergeTest, FourWayMerge) {
    std::vector<int> arr = {15, 3, 9, 1, 12, 7, 5, 11, 2, 8, 14, 4, 10, 6, 13};
    auto result = KwayMergeSort(arr, 4, 4);
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

// большой block_size
TEST_F(MergeTest, LargeBlockSize) {
    std::vector<int> arr = {5, 2, 8, 1, 9};
    auto result = KwayMergeSort(arr, 2, 100);
    
    std::vector<int> expected = {1, 2, 5, 8, 9};
    EXPECT_EQ(result, expected);
}

// block_size = 1
TEST_F(MergeTest, BlockSizeOne) {
    std::vector<int> arr = {5, 2, 8, 1, 9, 3};
    auto result = KwayMergeSort(arr, 3, 1);
    
    std::vector<int> expected = {1, 2, 3, 5, 8, 9};
    EXPECT_EQ(result, expected);
}

// массив с дубликатами
TEST_F(MergeTest, WithDuplicates) {
    std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto result = KwayMergeSort(arr, 3, 4);
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

// все элементы одинаковые
TEST_F(MergeTest, AllSameElements) {
    std::vector<int> arr(10, 42);
    auto result = KwayMergeSort(arr, 3, 3);
    
    EXPECT_EQ(result, arr);
}

// отрицательные числа
TEST_F(MergeTest, NegativeNumbers) {
    std::vector<int> arr = {-5, 3, -1, 0, -8, 7, -3};
    auto result = KwayMergeSort(arr, 2, 2);
    
    std::vector<int> expected = {-8, -5, -3, -1, 0, 3, 7};
    EXPECT_EQ(result, expected);
}

// сортировка строк
TEST_F(MergeTest, StringSort) {
    std::vector<std::string> arr = {"banana", "apple", "cherry", "date"};
    auto result = KwayMergeSort(arr, 2, 2);
    
    std::vector<std::string> expected = {"apple", "banana", "cherry", "date"};
    EXPECT_EQ(result, expected);
}

// сортировка дробных чисел
TEST_F(MergeTest, FloatSort) {
    std::vector<double> arr = {3.14, 1.41, 2.71, 1.73, 0.57};
    auto result = KwayMergeSort(arr, 2, 2);
    
    std::vector<double> expected = arr;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

// большой массив со случайными данными
TEST_F(MergeTest, LargeRandomArray) {
    std::vector<int> arr(1000);
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(0, 10000);
    
    for (auto& x : arr) {
        x = dis(gen);
    }
    
    auto result = KwayMergeSort(arr, 4, 32);
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(result, expected);
}

// проверка стабильности размера
TEST_F(MergeTest, PreservesSize) {
    std::vector<int> arr = {9, 1, 5, 3, 7, 2, 8, 4, 6};
    auto result = KwayMergeSort(arr, 3, 3);
    
    EXPECT_EQ(result.size(), arr.size());
}

// количество блоков не кратно k (7 элементов, block_size=2 -> 4 блока, k=3 -> не делится нацело)
TEST_F(MergeTest, NonDivisibleBlocks) {
    std::vector<int> arr = {7, 2, 5, 1, 6, 3, 4};
    auto result = KwayMergeSort(arr, 3, 2);
    
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(result, expected);
}