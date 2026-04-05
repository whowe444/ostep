#pragma once

#include <gtest/gtest.h>
#include "concurrent_hash_map.hh"

class ConcurrentHashMapTest : public ::testing::Test {

protected:
    std::unique_ptr<ConcurrentHashMap<int, int>> map;

    ConcurrentHashMapTest() = default;

    void SetUp() override {
        map = std::make_unique<ConcurrentHashMap<int, int>>();
    }

    void TearDown() override {}

};