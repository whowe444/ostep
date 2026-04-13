#pragma once

#include <gtest/gtest.h>
#include "concurrent_red_black_tree.hh"

class ConcurrentRedBlackTreeTest : public ::testing::Test {

protected:
    std::unique_ptr<ConcurrentRedBlackTree<int, int>> tree;

    ConcurrentRedBlackTreeTest() = default;

    void SetUp() override {
        tree = std::make_unique<ConcurrentRedBlackTree<int, int>>();
    }

    void TearDown() override {
        tree->Validate();
    }

};
