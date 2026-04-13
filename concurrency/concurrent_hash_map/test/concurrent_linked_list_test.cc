#include "concurrent_linked_list_test.hh"
#include <thread>

TEST_F(ConcurrentLinkedListTest, TestConstructor) {
    EXPECT_TRUE(list);
}

TEST_F(ConcurrentLinkedListTest, GetSize) {
    EXPECT_EQ(list->GetSize(), 0);
}

TEST_F(ConcurrentLinkedListTest, IsEmpty) {
    EXPECT_TRUE(list->IsEmpty());
}

TEST_F(ConcurrentLinkedListTest, Add) {
    const int new_value = 1;
    EXPECT_TRUE(list->Add(new_value, 0));
    EXPECT_EQ(list->GetSize(), 1);
    EXPECT_TRUE(list->Contains(new_value));
}

TEST_F(ConcurrentLinkedListTest, AddTwoElements) {
    const int new_value = 2;
    const int second_new_value = 3;

    // Add two new values
    list->Add(new_value, 0);
    list->Add(second_new_value, 0);

    EXPECT_EQ(list->GetSize(), 2);
    EXPECT_TRUE(list->Contains(new_value));
    EXPECT_TRUE(list->Contains(second_new_value));
}

TEST_F(ConcurrentLinkedListTest, RemoveEmpty) {
    EXPECT_FALSE(list->Remove(50));
}

TEST_F(ConcurrentLinkedListTest, TestRemove) {
    // Add Elements
    const int first_value = 99;
    list->Add(first_value, 0);
    const int second_value = 100;
    list->Add(second_value, 0);

    // Now Remove
    EXPECT_TRUE(list->Remove(first_value));
    
    // Verify
    EXPECT_TRUE(list->Contains(second_value));
    EXPECT_EQ(list->GetSize(), 1);
}

TEST_F(ConcurrentLinkedListTest, AddMultipleElements) {
    const int NUM_ELEMENTS = 100;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        list->Add(i, 0);
    }

    EXPECT_EQ(list->GetSize(), NUM_ELEMENTS);

    // Remove an element
    EXPECT_TRUE(list->Remove(50));
    EXPECT_EQ(list->GetSize(), NUM_ELEMENTS - 1);

    // Verify first and last element
    EXPECT_TRUE(list->Contains(0));
    EXPECT_TRUE(list->Contains(98));
}

TEST_F(ConcurrentLinkedListTest, RemoveLastThenAdd) {
    EXPECT_TRUE(list->Add(1, 0));
    EXPECT_EQ(list->GetSize(), 1);
    EXPECT_TRUE(list->Remove(1));

    EXPECT_EQ(list->GetSize(), 0);

    // would segfault if tail isn't updated
    EXPECT_TRUE(list->Add(2, 0));

    EXPECT_EQ(list->GetSize(), 1);
    EXPECT_TRUE(list->Contains(2));
}

TEST_F(ConcurrentLinkedListTest, ConcurrentAdd) {
    const int NUM_THREADS = 10;
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([this, i]() {
            for (int j = 0; j < NUM_ELEMENTS; j++) {
                this->list->Add(i * NUM_ELEMENTS + j, 0);
            }
        });
    }
    for (auto& t : threads) t.join();
    EXPECT_EQ(list->GetSize(), NUM_THREADS * NUM_ELEMENTS);
}

TEST_F(ConcurrentLinkedListTest, ConcurrentDelete) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_ELEMENTS; i++) list->Add(i, 0);

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        threads.emplace_back([this, i]() {
            this->list->Remove(i);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(list->GetSize(), 0);
}

TEST_F(ConcurrentLinkedListTest, ConcurrentAddAndDelete) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> adders;
    std::vector<std::thread> removers;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        adders.emplace_back([this, i]() {
            this->list->Add(i, 0);
        });
    }

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        removers.emplace_back([this, i]() {
            this->list->Remove(i);
        });
    }

    for (auto& adder : adders) adder.join();
    for (auto& remover : removers) remover.join();
}

TEST_F(ConcurrentLinkedListTest, ConcurrentContains) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> contains;

    // Add elements
    for (int i = 0; i < NUM_ELEMENTS; i++) list->Add(i, 0);

    // Add contains threads
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        contains.emplace_back([this, i]() {
            EXPECT_TRUE(this->list->Contains(i));
        });
    }

    for (auto& contain : contains) contain.join();
}

TEST_F(ConcurrentLinkedListTest, BenchmarkAdd) {
    auto start = std::chrono::high_resolution_clock::now();
    const int NUM_ELEMENTS = 1000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        this->list->Add(i, 0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "BenchmarkAdd took: " << std::to_string(duration.count() / (double)NUM_ELEMENTS)
        << " milliseconds per Add operation." << std::endl;
}