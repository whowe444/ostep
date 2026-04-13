#pragma once

#include <gtest/gtest.h>
#include "concurrent_hash_map.hh"
#include "red_black_tree/concurrent_red_black_tree.hh"

class ConcurrentRedBlackHashMapTest : public ::testing::Test {

protected:
    std::unique_ptr<ConcurrentHashMap<int, int, ConcurrentRedBlackTree>> map;

    ConcurrentRedBlackHashMapTest() = default;

    void SetUp() override {
        map = std::make_unique<ConcurrentHashMap<int, int, ConcurrentRedBlackTree>>();
    }

    void TearDown() override {}

};