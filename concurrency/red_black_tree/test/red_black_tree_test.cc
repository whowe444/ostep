#include "red_black_tree_test.hh"

TEST_F(RedBlackTreeTest, TestConstructor) {
    EXPECT_TRUE(tree);
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
    EXPECT_EQ(tree->Get(key), value);
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
    EXPECT_EQ(tree->Get(firstKey), firstValue);
    EXPECT_EQ(tree->Get(secondKey), secondValue);
}