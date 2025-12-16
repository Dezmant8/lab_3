#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <cstddef>

template<typename T>
std::vector<T> mergeKSortedArrays(const std::vector<std::vector<T>>& arrays);

// k - количество блоков для слияния за один проход
// block_size - размер начальных блоков для сортировки

template<typename T>
std::vector<T> KwayMergeSort(const std::vector<T>& array, std::size_t k = 4, std::size_t block_size = 8) {
    if (array.empty()) {
        return std::vector<T>();
    }
    
    if (array.size() == 1) {
        return array;
    }
    
    // k не меньше 2
    if (k < 2) {
        k = 2;
    }
    
    // Шаг 1: Разбиение на блоки и сортировка
    std::vector<std::vector<T>> sorted_runs;
    
    for (size_t i = 0; i < array.size(); i += block_size) {
        size_t end = std::min(i + block_size, array.size());
        std::vector<T> block(array.begin() + i, array.begin() + end);
        std::sort(block.begin(), block.end());
        sorted_runs.push_back(std::move(block));
    }
    
    // Шаг 2: Слияние
    while (sorted_runs.size() > 1) {
        std::vector<std::vector<T>> new_runs;
        
        // Обрабатываем по k блоков за раз
        for (size_t i = 0; i < sorted_runs.size(); i += k) {
            size_t end = std::min(i + k, sorted_runs.size());
            
            // Собираем блоки для слияния
            std::vector<std::vector<T>> to_merge(sorted_runs.begin() + i, sorted_runs.begin() + end);
            
            // Соединяем k блоков
            std::vector<T> merged = mergeKSortedArrays(to_merge);
            new_runs.push_back(std::move(merged));
        }
        
        sorted_runs = std::move(new_runs);
    }
    
    return sorted_runs.empty() ? std::vector<T>() : sorted_runs[0];
}

// Слияние k отсортированных массивов
template<typename T>
std::vector<T> mergeKSortedArrays(const std::vector<std::vector<T>>& arrays) {
    if (arrays.empty()) {
        return std::vector<T>();
    }
    
    if (arrays.size() == 1) {
        return arrays[0];
    }
    
    using HeapElement = std::tuple<T, size_t, size_t>;
    
    std::priority_queue<HeapElement, std::vector<HeapElement>, std::greater<HeapElement>> min_heap;
    
    size_t total_size = 0;
    
    for (size_t i = 0; i < arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            min_heap.push(std::make_tuple(arrays[i][0], i, 0));
            total_size += arrays[i].size();
        }
    }
    
    std::vector<T> result;
    result.reserve(total_size);
    
    // Извлекаем минимум и добавляем следующий элемент
    while (!min_heap.empty()) {
        auto [value, array_idx, elem_idx] = min_heap.top();
        min_heap.pop();
        
        result.push_back(value);
        
        // Если в массиве есть ещё элементы, добавляем следующий в кучу
        if (elem_idx + 1 < arrays[array_idx].size()) {
            min_heap.push(std::make_tuple(
                arrays[array_idx][elem_idx + 1], 
                array_idx, 
                elem_idx + 1
            ));
        }
    }
    
    return result;
}

