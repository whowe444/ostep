#include "red_black_tree_test.hh"

TEST_F(RedBlackTreeTest, TestConstructor) {
    EXPECT_TRUE(tree);
    
}

TEST_F(RedBlackTreeTest, TestCopyCtor) {
    const int firstKey = 1;
    const int secondKey = 2;
    const int firstValue = 10;
    const int secondValue = 20;
    EXPECT_TRUE(tree->Insert(firstKey, firstValue));
    EXPECT_TRUE(tree->Insert(secondKey, secondValue));

    RedBlackTree<int, int> newTree {*this->tree};

    // Since we made a copy our two lists have the same size.
    EXPECT_EQ(newTree.GetSize(), 2);
    EXPECT_EQ(this->tree->GetSize(), 2); 

    // Mutating newTree doesn't affect the original tree
    newTree.Insert(3, 30);

    // Verify
    EXPECT_EQ(newTree.GetSize(), 3);
    EXPECT_EQ(this->tree->GetSize(), 2);
}
TEST_F(RedBlackTreeTest, TestCopyAssignment) {
    const int firstKey = 1;
    const int secondKey = 2;
    const int firstValue = 10;
    const int secondValue = 20;
    EXPECT_TRUE(tree->Insert(firstKey, firstValue));
    EXPECT_TRUE(tree->Insert(secondKey, secondValue));

    RedBlackTree<int, int> newTree = *this->tree;

    // Since we made a copy our two lists have the same size.
    EXPECT_EQ(newTree.GetSize(), 2);
    EXPECT_EQ(this->tree->GetSize(), 2); 

    // Mutating newTree doesn't affect the original tree
    newTree.Insert(3, 30);

    // Verify
    EXPECT_EQ(newTree.GetSize(), 3);
    EXPECT_EQ(this->tree->GetSize(), 2);
}


TEST_F(RedBlackTreeTest, TestMoveCtor) {
    const int firstKey = 1;
    const int secondKey = 2;
    const int firstValue = 10;
    const int secondValue = 20;
    EXPECT_TRUE(tree->Insert(firstKey, firstValue));
    EXPECT_TRUE(tree->Insert(secondKey, secondValue));

    RedBlackTree<int, int> newTree {std::move(*this->tree)};

    EXPECT_EQ(newTree.GetSize(), 2);
    EXPECT_EQ(this->tree->GetSize(), 0);
}

TEST_F(RedBlackTreeTest, TestMoveAssignment) {
    const int firstKey = 1;
    const int secondKey = 2;
    const int firstValue = 10;
    const int secondValue = 20;
    EXPECT_TRUE(tree->Insert(firstKey, firstValue));
    EXPECT_TRUE(tree->Insert(secondKey, secondValue));

    RedBlackTree<int, int> newTree = std::move(*this->tree);

    EXPECT_EQ(newTree.GetSize(), 2);
    EXPECT_EQ(this->tree->GetSize(), 0);
}

TEST_F(RedBlackTreeTest, TestSize) {
    EXPECT_EQ(tree->GetSize(), 0);
}

TEST_F(RedBlackTreeTest, TestIsEmpty) {
    EXPECT_TRUE(tree->IsEmpty());
}

TEST_F(RedBlackTreeTest, TestInsertAtRoot) {
    const int key = 7;
    const int value = 8;
    EXPECT_TRUE(tree->Insert(key, value));
    EXPECT_EQ(tree->GetSize(), 1);
    EXPECT_EQ(tree->Get(key).value(), value);
}

TEST_F(RedBlackTreeTest, InsertTwoElements) {
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

TEST_F(RedBlackTreeTest, UpdateValue) {
    const int firstKey = 1;
    const int firstValue = 10;
    const int secondValue = 20;

    tree->Insert(firstKey, firstValue);
    EXPECT_EQ(tree->Get(firstKey).value(), firstValue);

    tree->Insert(firstKey, secondValue);
    EXPECT_EQ(tree->Get(firstKey).value(), secondValue);
}

TEST_F(RedBlackTreeTest, DeleteRootNodeTwoChildren) {
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


TEST_F(RedBlackTreeTest, DeleteRootNodeTwoChildrenBalanced) {
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

TEST_F(RedBlackTreeTest, DeleteRootNodeOneChild) {
    const int firstKey = 1;
    const int firstValue = 10;

    const int secondKey = 2;
    const int secondValue = 20;

    tree->Insert(firstKey, firstValue);
    tree->Insert(secondKey, secondValue);

    EXPECT_EQ(tree->Delete(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 1);
}

TEST_F(RedBlackTreeTest, DeleteRootNodeNoChildren) {
    const int firstKey = 1;
    const int firstValue = 10;

    tree->Insert(firstKey, firstValue);

    EXPECT_EQ(tree->Delete(firstKey), firstValue);
    EXPECT_EQ(tree->GetSize(), 0);
}

TEST_F(RedBlackTreeTest, DeleteNonExistingWhenEmpty) {
    EXPECT_FALSE(tree->Delete(67).has_value());
}

TEST_F(RedBlackTreeTest, DeleteNonExisting) {
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

TEST_F(RedBlackTreeTest, DeleteLeft) {
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

TEST_F(RedBlackTreeTest, DeleteRight) {
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

TEST_F(RedBlackTreeTest, MultipleInsertionsBeforeDeleteSeven) {
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

TEST_F(RedBlackTreeTest, MultipleInsertionsBeforeDeleteThree) {
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


TEST_F(RedBlackTreeTest, MultipleInsertionsBeforeDeleteRoot) {
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

TEST_F(RedBlackTreeTest, MultipleInsertionsBeforeDeleteLeaf) {
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

TEST_F(RedBlackTreeTest, DeleteWhereSuccessorIsRightChildsLeftDescendant) {
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

TEST_F(RedBlackTreeTest, DeleteWhereSuccessorIsRightChildsRightDescendant) {
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

TEST_F(RedBlackTreeTest, DeleteWhereSuccessorIsRightChildsLeftDescendantWithRightChild) {
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

TEST_F(RedBlackTreeTest, DeleteNodeWithOnlyLeftChild) {
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

TEST_F(RedBlackTreeTest, InsertManyElements) {
    const int NUM_ELEMENTS = 10000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        tree->Insert(i, i*10);
    }

    EXPECT_EQ(tree->GetSize(), NUM_ELEMENTS);
}

TEST_F(RedBlackTreeTest, TestIterator) {
    std::vector<std::pair<int, int>> pairs = {
        {5, 50}, {3, 30}, {7, 70}, {1, 10}, {4, 40}, {9, 90}, {10, 100}
    };

    // Insert elements into the tree
    for (auto [first, second] : pairs) {
        tree->Insert(first, second);
    }

    // Now iterate through the tree
    int expectedKey = std::numeric_limits<int>::min();
    for (const auto& [key, value] : *tree) {
        // Verify that the keys are monotonically increasing.
        EXPECT_TRUE(key > expectedKey);
        expectedKey = key;

        // Verify the key / value pair.
        EXPECT_EQ(key*10, value);
    }

}