#include "concurrent_red_black_tree_test.hh"
#include <thread>

TEST_F(ConcurrentRedBlackTreeTest, TestConstructor) {
    EXPECT_TRUE(tree);
    
}

TEST_F(ConcurrentRedBlackTreeTest, TestSize) {
    EXPECT_EQ(tree->GetSize(), 0);
}

TEST_F(ConcurrentRedBlackTreeTest, TestIsEmpty) {
    EXPECT_TRUE(tree->IsEmpty());
}

TEST_F(ConcurrentRedBlackTreeTest, TestInsertAtRoot) {
    const int key = 7;
    const int value = 8;
    EXPECT_TRUE(tree->Add(key, value));
    EXPECT_EQ(tree->GetSize(), 1);
    EXPECT_EQ(tree->Get(key).value(), value);
}

TEST_F(ConcurrentRedBlackTreeTest, InsertTwoElements) {
    const int firstKey = 1;
    const int secondKey = 2;
    const int firstValue = 10;
    const int secondValue = 20;
    EXPECT_TRUE(tree->Add(firstKey, firstValue));
    EXPECT_TRUE(tree->Add(secondKey, secondValue));
    EXPECT_EQ(tree->GetSize(), 2);

    // Retrieve values
    EXPECT_EQ(tree->Get(firstKey).value(), firstValue);
    EXPECT_EQ(tree->Get(secondKey).value(), secondValue);
}

TEST_F(ConcurrentRedBlackTreeTest, UpdateValue) {
    const int firstKey = 1;
    const int firstValue = 10;
    const int secondValue = 20;

    tree->Add(firstKey, firstValue);
    EXPECT_EQ(tree->Get(firstKey).value(), firstValue);

    tree->Add(firstKey, secondValue);
    EXPECT_EQ(tree->Get(firstKey).value(), secondValue);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeTwoChildren) {
    const int firstKey = 1;
    const int firstValue = 10;

    const int secondKey = 2;
    const int secondValue = 20;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Add(firstKey, firstValue);
    tree->Add(secondKey, secondValue);
    tree->Add(thirdKey, thirdValue);

    EXPECT_EQ(tree->GetSize(), 3);

    EXPECT_EQ(tree->Remove(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 2);   
}


TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeTwoChildrenBalanced) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Add(firstKey, firstValue);
    tree->Add(secondKey, secondValue);
    tree->Add(thirdKey, thirdValue);

    EXPECT_EQ(tree->GetSize(), 3);

    EXPECT_EQ(tree->Remove(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 2);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeOneChild) {
    const int firstKey = 1;
    const int firstValue = 10;

    const int secondKey = 2;
    const int secondValue = 20;

    tree->Add(firstKey, firstValue);
    tree->Add(secondKey, secondValue);

    EXPECT_EQ(tree->Remove(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 1);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeNoChildren) {
    const int firstKey = 1;
    const int firstValue = 10;

    tree->Add(firstKey, firstValue);

    EXPECT_EQ(tree->Remove(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 0);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteNonExistingWhenEmpty) {
    EXPECT_FALSE(tree->Remove(67).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteNonExisting) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Add(firstKey, firstValue);
    tree->Add(secondKey, secondValue);
    tree->Add(thirdKey, thirdValue);

    EXPECT_FALSE(tree->Remove(67).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteLeft) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Add(firstKey, firstValue);
    tree->Add(secondKey, secondValue);
    tree->Add(thirdKey, thirdValue);

    EXPECT_EQ(tree->Remove(secondKey).value(), secondValue);
    EXPECT_EQ(tree->GetSize(), 2);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRight) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Add(firstKey, firstValue);
    tree->Add(secondKey, secondValue);
    tree->Add(thirdKey, thirdValue);

    EXPECT_EQ(tree->Remove(thirdKey).value(), thirdValue);
    EXPECT_EQ(tree->GetSize(), 2);
}

TEST_F(ConcurrentRedBlackTreeTest, MultipleInsertionsBeforeDeleteSeven) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {6, 60}, {9, 90}
    };

    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Remove(7).value(), 70);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    EXPECT_TRUE(tree->Get(5).has_value());
    EXPECT_TRUE(tree->Get(3).has_value());
    EXPECT_FALSE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_TRUE(tree->Get(6).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, MultipleInsertionsBeforeDeleteThree) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {6, 60}, {9, 90}
    };

    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Remove(3).value(), 30);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    EXPECT_TRUE(tree->Get(5).has_value());
    EXPECT_FALSE(tree->Get(3).has_value());
    EXPECT_TRUE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_TRUE(tree->Get(6).has_value());
}


TEST_F(ConcurrentRedBlackTreeTest, MultipleInsertionsBeforeDeleteRoot) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {6, 60}, {9, 90}
    };

    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Remove(5).value(), 50);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    EXPECT_FALSE(tree->Get(5).has_value());
    EXPECT_TRUE(tree->Get(3).has_value());
    EXPECT_TRUE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_TRUE(tree->Get(6).has_value());   
}

TEST_F(ConcurrentRedBlackTreeTest, MultipleInsertionsBeforeDeleteLeaf) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {6, 60}, {9, 90}
    };

    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Remove(6).value(), 60);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    // Verify other nodes are still present
    EXPECT_TRUE(tree->Get(5).has_value());
    EXPECT_TRUE(tree->Get(3).has_value());
    EXPECT_TRUE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_FALSE(tree->Get(6).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteWhereSuccessorIsRightChildsLeftDescendant) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {9, 90}, {8, 80}
    };
    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }
    EXPECT_EQ(tree->GetSize(), pairs.size());
    EXPECT_EQ(tree->Remove(7).value(), 70);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    EXPECT_TRUE(tree->Get(5).has_value());
    EXPECT_TRUE(tree->Get(3).has_value());
    EXPECT_FALSE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_TRUE(tree->Get(8).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteWhereSuccessorIsRightChildsRightDescendant) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {9, 90}, {10, 100}
    };
    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }
    EXPECT_EQ(tree->GetSize(), pairs.size());
    EXPECT_EQ(tree->Remove(7).value(), 70);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    EXPECT_TRUE(tree->Get(5).has_value());
    EXPECT_TRUE(tree->Get(3).has_value());
    EXPECT_FALSE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_TRUE(tree->Get(10).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteWhereSuccessorIsRightChildsLeftDescendantWithRightChild) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {10, 100}, {8, 80}, {9, 90}
    };
    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }
    EXPECT_EQ(tree->GetSize(), pairs.size());
    EXPECT_EQ(tree->Remove(7).value(), 70);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);

    EXPECT_TRUE(tree->Get(5).has_value());
    EXPECT_TRUE(tree->Get(3).has_value());
    EXPECT_FALSE(tree->Get(7).has_value());
    EXPECT_TRUE(tree->Get(1).has_value());
    EXPECT_TRUE(tree->Get(4).has_value());
    EXPECT_TRUE(tree->Get(8).has_value());
    EXPECT_TRUE(tree->Get(9).has_value());
    EXPECT_TRUE(tree->Get(10).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteNodeWithOnlyLeftChild) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {2, 20}
    };
    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }
    EXPECT_EQ(tree->Remove(3).value(), 30);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);
    EXPECT_TRUE(tree->Get(2).has_value());
    EXPECT_FALSE(tree->Get(3).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, InsertManyElements) {
    const int NUM_ELEMENTS = 10000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        tree->Add(i, i*10);
    }

    EXPECT_EQ(tree->GetSize(), NUM_ELEMENTS);
}

TEST_F(ConcurrentRedBlackTreeTest, ConcurrentAdd) {
    const int NUM_THREADS = 10;
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back([this, i]() {
            for (int j = 0; j < NUM_ELEMENTS; j++) {
                this->tree->Add(i * NUM_ELEMENTS + j, 0);
            }
        });
    }
    for (auto& t : threads) t.join();
    EXPECT_EQ(tree->GetSize(), NUM_THREADS * NUM_ELEMENTS);
}

TEST_F(ConcurrentRedBlackTreeTest, ConcurrentDelete) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_ELEMENTS; i++) this->tree->Add(i, 0);

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        threads.emplace_back([this, i]() {
            this->tree->Remove(i);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(tree->GetSize(), 0);
}

TEST_F(ConcurrentRedBlackTreeTest, ConcurrentAddAndDelete) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> adders;
    std::vector<std::thread> removers;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        adders.emplace_back([this, i]() {
            this->tree->Add(i, 0);
        });
    }

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        removers.emplace_back([this, i]() {
            this->tree->Remove(i);
        });
    }

    for (auto& adder : adders) adder.join();
    for (auto& remover : removers) remover.join();
}


TEST_F(ConcurrentRedBlackTreeTest, ConcurrentContains) {
    const int NUM_ELEMENTS = 100;
    std::vector<std::thread> contains;

    // Add elements
    for (int i = 0; i < NUM_ELEMENTS; i++) tree->Add(i, 0);

    // Add contains threads
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        contains.emplace_back([this, i]() {
            EXPECT_TRUE(this->tree->Get(i).has_value());
        });
    }

    for (auto& contain : contains) contain.join();
}

TEST_F(ConcurrentRedBlackTreeTest, BenchmarkAdd) {
    auto start = std::chrono::high_resolution_clock::now();
    const int NUM_ELEMENTS = 100000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        this->tree->Add(i, 0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "BenchmarkAdd took: " << std::to_string(duration.count() / (double)NUM_ELEMENTS)
        << " nanoseconds per Add operation." << std::endl;
}


TEST_F(ConcurrentRedBlackTreeTest, TestWriteIterator) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {9, 90}, {10, 100}
    };

    // Insert elements into the tree
    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }

    // Now iterate through the tree
    int expectedKey = std::numeric_limits<int>::min();
    for (const auto& [key, value] : this->tree->GetUniqueRange()) {
        // Verify that the keys are monotonically increasing.
        EXPECT_TRUE(key > expectedKey);
        expectedKey = key;

        // Verify the key / value pair.
        EXPECT_EQ(key*10, value);
    }
}

TEST_F(ConcurrentRedBlackTreeTest, TestReadIterator) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {9, 90}, {10, 100}
    };

    // Insert elements into the tree
    for (auto [first, second] : pairs) {
        tree->Add(first, second);
    }

    // Now iterate through the tree
    int expectedKey = std::numeric_limits<int>::min();
    for (const auto& [key, value] : this->tree->GetSharedRange()) {
        // Verify that the keys are monotonically increasing.
        EXPECT_TRUE(key > expectedKey);
        expectedKey = key;

        // Verify the key / value pair.
        EXPECT_EQ(key*10, value);
    }
}