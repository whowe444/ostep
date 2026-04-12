#pragma once

#include <gtest/gtest.h>
#include "red_black_tree.hh"

class RedBlackTreeTest : public ::testing::Test {

protected:
    std::unique_ptr<RedBlackTree<int, int>> tree;

    RedBlackTreeTest() = default;

    void SetUp() override {
        tree = std::make_unique<RedBlackTree<int, int>>();
    }

    void TearDown() override {
        tree->Validate();
    }

};
