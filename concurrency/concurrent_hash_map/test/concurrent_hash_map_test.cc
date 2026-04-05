#include "concurrent_hash_map_test.hh"

TEST_F(ConcurrentHashMapTest, TestsConstructor) {
    EXPECT_TRUE(map);
}

TEST_F(ConcurrentHashMapTest, GetSizeEmpty) {
    EXPECT_EQ(map->GetSize(), 0);
}

TEST_F(ConcurrentHashMapTest, IsEmpty) {
    EXPECT_TRUE(map->IsEmpty());
}

TEST_F(ConcurrentHashMapTest, InsertOne) {
    const int KEY = 1;
    const int VALUE = 10;
    EXPECT_TRUE(this->map->Insert(KEY, VALUE));
    EXPECT_TRUE(this->map->Contains(KEY));
    EXPECT_EQ(this->map->Get(KEY).value(), VALUE);
}

TEST_F(ConcurrentHashMapTest, UpdateValue) {
    const int key = 2;
    const int first_value = 11;
    const int second_value = 12;

    this->map->Insert(key, first_value);
    this->map->Insert(key, second_value);
    EXPECT_EQ(this->map->Get(key).value(), second_value);
}

TEST_F(ConcurrentHashMapTest, BenchmarkInsert) {
    auto start = std::chrono::high_resolution_clock::now();
    const int NUM_ELEMENTS = 100000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        this->map->Insert(i, i+1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "BenchmarkInsert took: " << std::to_string(duration.count() / (double)NUM_ELEMENTS)
        << " milliseconds per Add operation." << std::endl;
}

