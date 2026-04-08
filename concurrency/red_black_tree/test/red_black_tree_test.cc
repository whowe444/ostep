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