#include "concurrent_hash_map_test.hh"
#include <thread>

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

TEST_F(ConcurrentHashMapTest, ConcurrentAdd) {
    const int NUM_THREADS = 10;
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([this, i]() {
            for (int j = 0; j < NUM_ELEMENTS; j++) {
                this->map->Insert(i * NUM_ELEMENTS + j, i* NUM_ELEMENTS + j);
            }
        });
    }
    for (auto& t : threads) t.join();
    EXPECT_EQ(map->GetSize(), NUM_THREADS * NUM_ELEMENTS);
}

TEST_F(ConcurrentHashMapTest, ConcurrentDelete) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_ELEMENTS; i++) map->Insert(i, i+1);

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        threads.emplace_back([this, i]() {
            this->map->Remove(i);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(map->GetSize(), 0);
}

TEST_F(ConcurrentHashMapTest, ConcurrentAddAndDelete) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> adders;
    std::vector<std::thread> removers;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        adders.emplace_back([this, i]() {
            this->map->Insert(i, i+1);
        });
    }

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        removers.emplace_back([this, i]() {
            this->map->Remove(i);
        });
    }

    for (auto& adder : adders) adder.join();
    for (auto& remover : removers) remover.join();
}

TEST_F(ConcurrentHashMapTest, ConcurrentContains) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> contains;

    // Add elements
    for (int i = 0; i < NUM_ELEMENTS; i++) map->Insert(i, i+1);

    // Add contains threads
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        contains.emplace_back([this, i]() {
            EXPECT_TRUE(this->map->Contains(i));
        });
    }

    for (auto& contain : contains) contain.join();
}

TEST_F(ConcurrentHashMapTest, BenchmarkConcurrentInsert) {
    const int NUM_ELEMENTS = 100000;
    const int NUM_THREADS = 10;
    const int NUM_PER_THREAD = NUM_ELEMENTS / NUM_THREADS;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([this, i]() {
            for (int j = i * (NUM_PER_THREAD); j < (i * NUM_PER_THREAD) + NUM_PER_THREAD; j++) {
                this->map->Insert(j, j+1);
            }
        });
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto& thread : threads) thread.join();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "BenchmarkConcurrentInsert took: " << std::to_string(duration.count() / (double)NUM_ELEMENTS)
        << " milliseconds per Add operation." << std::endl;
}

