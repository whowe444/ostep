#include "concurrent_red_black_tree_test.hh"

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
    EXPECT_TRUE(tree->Insert(key, value));
    EXPECT_EQ(tree->GetSize(), 1);
    EXPECT_EQ(tree->Get(key).value(), value);
}

TEST_F(ConcurrentRedBlackTreeTest, InsertTwoElements) {
    const int firstKey = 1;
    const int secondKey = 2;
    const int firstValue = 10;
    const int secondValue = 20;
    EXPECT_TRUE(tree->Insert(firstKey, firstValue));
    EXPECT_TRUE(tree->Insert(secondKey, secondValue));
    EXPECT_EQ(tree->GetSize(), 2);

    // Retrieve values
    EXPECT_EQ(tree->Get(firstKey).value(), firstValue);
    EXPECT_EQ(tree->Get(secondKey).value(), secondValue);
}

TEST_F(ConcurrentRedBlackTreeTest, UpdateValue) {
    const int firstKey = 1;
    const int firstValue = 10;
    const int secondValue = 20;

    tree->Insert(firstKey, firstValue);
    EXPECT_EQ(tree->Get(firstKey).value(), firstValue);

    tree->Insert(firstKey, secondValue);
    EXPECT_EQ(tree->Get(firstKey).value(), secondValue);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeTwoChildren) {
    const int firstKey = 1;
    const int firstValue = 10;

    const int secondKey = 2;
    const int secondValue = 20;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);
    tree->Insert(thirdKey, thirdValue);

    EXPECT_EQ(tree->GetSize(), 3);

    EXPECT_EQ(tree->Delete(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 2);   
}


TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeTwoChildrenBalanced) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);
    tree->Insert(thirdKey, thirdValue);

    EXPECT_EQ(tree->GetSize(), 3);

    EXPECT_EQ(tree->Delete(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 2);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeOneChild) {
    const int firstKey = 1;
    const int firstValue = 10;

    const int secondKey = 2;
    const int secondValue = 20;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);

    EXPECT_EQ(tree->Delete(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 1);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRootNodeNoChildren) {
    const int firstKey = 1;
    const int firstValue = 10;

    tree->Insert(firstKey, firstValue);

    EXPECT_EQ(tree->Delete(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 0);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteNonExistingWhenEmpty) {
    EXPECT_FALSE(tree->Delete(67).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteNonExisting) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);
    tree->Insert(thirdKey, thirdValue);

    EXPECT_FALSE(tree->Delete(67).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteLeft) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);
    tree->Insert(thirdKey, thirdValue);

    EXPECT_EQ(tree->Delete(secondKey).value(), secondValue);
    EXPECT_EQ(tree->GetSize(), 2);
}

TEST_F(ConcurrentRedBlackTreeTest, DeleteRight) {
    const int firstKey = 2;
    const int firstValue = 20;

    const int secondKey = 1;
    const int secondValue = 10;

    const int thirdKey = 3;
    const int thirdValue = 30;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);
    tree->Insert(thirdKey, thirdValue);

    EXPECT_EQ(tree->Delete(thirdKey).value(), thirdValue);
    EXPECT_EQ(tree->GetSize(), 2);
}

TEST_F(ConcurrentRedBlackTreeTest, MultipleInsertionsBeforeDeleteSeven) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {6, 60}, {9, 90}
    };

    for (auto [first, second] : pairs) {
        tree->Insert(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Delete(7).value(), 70);
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
        tree->Insert(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Delete(3).value(), 30);
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
        tree->Insert(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Delete(5).value(), 50);
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
        tree->Insert(first, second);
    }

    EXPECT_EQ(tree->GetSize(), pairs.size());

    EXPECT_EQ(tree->Delete(6).value(), 60);
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
        tree->Insert(first, second);
    }
    EXPECT_EQ(tree->GetSize(), pairs.size());
    EXPECT_EQ(tree->Delete(7).value(), 70);
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
        tree->Insert(first, second);
    }
    EXPECT_EQ(tree->GetSize(), pairs.size());
    EXPECT_EQ(tree->Delete(7).value(), 70);
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
        tree->Insert(first, second);
    }
    EXPECT_EQ(tree->GetSize(), pairs.size());
    EXPECT_EQ(tree->Delete(7).value(), 70);
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
        tree->Insert(first, second);
    }
    EXPECT_EQ(tree->Delete(3).value(), 30);
    EXPECT_EQ(tree->GetSize(), pairs.size() - 1);
    EXPECT_TRUE(tree->Get(2).has_value());
    EXPECT_FALSE(tree->Get(3).has_value());
}

TEST_F(ConcurrentRedBlackTreeTest, InsertManyElements) {
    const int NUM_ELEMENTS = 10000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        tree->Insert(i, i*10);
    }

    EXPECT_EQ(tree->GetSize(), NUM_ELEMENTS);
}