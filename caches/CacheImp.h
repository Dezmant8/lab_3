#pragma once
#include "ICache.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>

template<typename Key, typename Value>
class ExampleCache : public ICache<Key, Value> {
public:
    ExampleCache() {}
    
    void set(const Key& key, const Value& value) override {
        return;
    }
    
    Value get(const Key& key) override {
        return Value();
    }
    
    bool contains(const Key& key) const override {
        return false;
    }
    
    void clear() override {
        return;
    }
    
    size_t size() const override {
        return 0;
    }
};

template<typename Key, typename Value>
class ClockCache : public ICache<Key, Value> {
private:
    struct CacheEntry {
        Key key;
        Value value;
        int reference_bit; // бит обращения
        bool occupied; // занятость ячейки
        
        CacheEntry() : reference_bit(0), occupied(false) {}
    };
    
    std::vector<CacheEntry> buffer; // циклический буфер
    std::unordered_map<Key, size_t> key_to_index; // ключ и позиция в буфере
    size_t capacity_; // максимальный размер кэша
    size_t clock_hand; // стрелка
    size_t current_size; // текущее количество элементов

public:
    explicit ClockCache(size_t capacity) 
        : capacity_(capacity)
        , clock_hand(0)
        , current_size(0) {
        if (capacity == 0) {
            throw std::invalid_argument("Cache capacity must be greater than 0");
        }
        buffer.resize(capacity_);
    }
    
    void set(const Key& key, const Value& value) override {
        auto it = key_to_index.find(key);
        if (it != key_to_index.end()) {
            size_t idx = it->second;
            buffer[idx].value = value;
            buffer[idx].reference_bit++;
            return;
        }
        
        // вытеснение элемента
        if (current_size == capacity_) {
            evict();
        }
        
        size_t idx = findFreeSlot();
        
        buffer[idx].key = key;
        buffer[idx].value = value;
        buffer[idx].reference_bit = 1;
        buffer[idx].occupied = true;
        
        key_to_index[key] = idx;
        current_size++;
    }
    
    Value get(const Key& key) override {
        auto it = key_to_index.find(key);
        if (it == key_to_index.end()) {
            throw std::runtime_error("Key not found in cache");
        }
        
        size_t idx = it->second;
        buffer[idx].reference_bit++;
        return buffer[idx].value;
    }
    
    bool contains(const Key& key) const override {
        return key_to_index.find(key) != key_to_index.end();
    }
    
    void clear() override {
        for (auto& entry : buffer) {
            entry.occupied = false;
            entry.reference_bit = 0;
        }
        key_to_index.clear();
        current_size = 0;
        clock_hand = 0;
    }
    
    size_t size() const override {
        return current_size;
    }
    
    size_t capacity() const {
        return capacity_;
    }

private:
    //Алгоритм вытеснения
    void evict() {
        while (true) {
            CacheEntry& entry = buffer[clock_hand];
            
            if (entry.occupied) {
                if (entry.reference_bit) {
                    // сбрасывание бита
                    entry.reference_bit--;
                } else {
                    // Вытеснение
                    key_to_index.erase(entry.key);
                    entry.occupied = false;
                    current_size--;
                    return;
                }
            }
            
            clock_hand = (clock_hand + 1) % capacity_;
        }
    }

    size_t findFreeSlot() {
        size_t start = clock_hand;
        do {
            if (!buffer[clock_hand].occupied) {
                return clock_hand;
            }
            clock_hand = (clock_hand + 1) % capacity_;
        } while (clock_hand != start);
        
        throw std::runtime_error("No free slot found");
    }
};
