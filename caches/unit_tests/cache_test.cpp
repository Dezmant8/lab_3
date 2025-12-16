#include <gtest/gtest.h>
#include "../CacheImp.h"
#include <string>

class CacheTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(CacheTest, ExampleCacheBasic) {
    ExampleCache<int, int> cache;
    EXPECT_EQ(cache.size(), 0);
}

// создание кэша с нулевой ёмкостью должно бросать исключение
TEST_F(CacheTest, ClockCacheZeroCapacityThrows) {
    EXPECT_THROW((ClockCache<int, int>(0)), std::invalid_argument);
}

// операции set/get
TEST_F(CacheTest, ClockCacheSetAndGet) {
    ClockCache<int, std::string> cache(3);
    
    cache.set(1, "one");
    cache.set(2, "two");
    
    EXPECT_EQ(cache.size(), 2);
    EXPECT_EQ(cache.get(1), "one");
    EXPECT_EQ(cache.get(2), "two");
}

// проверка contains
TEST_F(CacheTest, ClockCacheContains) {
    ClockCache<int, int> cache(5);
    
    EXPECT_FALSE(cache.contains(1));
    
    cache.set(1, 100);
    EXPECT_TRUE(cache.contains(1));
    EXPECT_FALSE(cache.contains(2));
}

// обновление существующего ключа
TEST_F(CacheTest, ClockCacheUpdateExistingKey) {
    ClockCache<std::string, int> cache(3);
    
    cache.set("key", 100);
    EXPECT_EQ(cache.get("key"), 100);
    
    cache.set("key", 200);
    EXPECT_EQ(cache.get("key"), 200);
    EXPECT_EQ(cache.size(), 1);
}

// get несуществующего ключа
TEST_F(CacheTest, ClockCacheGetNonExistentThrows) {
    ClockCache<int, int> cache(3);
    
    EXPECT_THROW(cache.get(999), std::runtime_error);
}

// вытеснение при переполнении
TEST_F(CacheTest, ClockCacheEviction) {
    ClockCache<int, int> cache(3);
    
    cache.set(1, 10);
    cache.set(2, 20);
    cache.set(3, 30);
    
    EXPECT_EQ(cache.size(), 3);
    
    cache.set(4, 40);
    
    EXPECT_EQ(cache.size(), 3);
    EXPECT_TRUE(cache.contains(4));
}

// элемент с обращением не вытесняется сразу
TEST_F(CacheTest, ClockCacheSecondChance) {
    ClockCache<int, int> cache(3);
    
    cache.set(1, 10);
    cache.set(2, 20);
    cache.set(3, 30);
    
    cache.get(1);
    
    cache.set(4, 40);
    
    EXPECT_TRUE(cache.contains(1));
    EXPECT_TRUE(cache.contains(4));
    EXPECT_EQ(cache.size(), 3);
}

// очистка кэша
TEST_F(CacheTest, ClockCacheClear) {
    ClockCache<int, int> cache(5);
    
    cache.set(1, 10);
    cache.set(2, 20);
    cache.set(3, 30);
    
    EXPECT_EQ(cache.size(), 3);
    
    cache.clear();
    
    EXPECT_EQ(cache.size(), 0);
    EXPECT_FALSE(cache.contains(1));
    EXPECT_FALSE(cache.contains(2));
    EXPECT_FALSE(cache.contains(3));
}

// работа после очистки
TEST_F(CacheTest, ClockCacheWorkAfterClear) {
    ClockCache<int, int> cache(3);
    
    cache.set(1, 10);
    cache.set(2, 20);
    cache.clear();
    
    cache.set(3, 30);
    cache.set(4, 40);
    
    EXPECT_EQ(cache.size(), 2);
    EXPECT_EQ(cache.get(3), 30);
    EXPECT_EQ(cache.get(4), 40);
}

// кэш с ёмкостью 1
TEST_F(CacheTest, ClockCacheCapacityOne) {
    ClockCache<int, int> cache(1);
    
    cache.set(1, 10);
    EXPECT_EQ(cache.get(1), 10);
    
    cache.set(2, 20);
    EXPECT_EQ(cache.size(), 1);
    EXPECT_FALSE(cache.contains(1));
    EXPECT_EQ(cache.get(2), 20);
}

// множественные вытеснения
TEST_F(CacheTest, ClockCacheMultipleEvictions) {
    ClockCache<int, int> cache(3);
    
    for (int i = 0; i < 10; ++i) {
        cache.set(i, i * 10);
    }
    
    EXPECT_EQ(cache.size(), 3);
    
    EXPECT_TRUE(cache.contains(9));
}

// строковые ключи и значения
TEST_F(CacheTest, ClockCacheStringKeysAndValues) {
    ClockCache<std::string, std::string> cache(2);
    
    cache.set("hello", "world");
    cache.set("one", "two");
    
    EXPECT_EQ(cache.get("hello"), "world");
    EXPECT_EQ(cache.get("one"), "two");
    
    cache.set("new", "value");
    
    EXPECT_EQ(cache.size(), 2);
    EXPECT_TRUE(cache.contains("new"));
}

