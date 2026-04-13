#pragma once

#include <gtest/gtest.h>
#include "concurrent_hash_map.hh"

class ConcurrentLinkedListHashMapTest : public ::testing::Test {

protected:
    std::unique_ptr<ConcurrentHashMap<int, int, ConcurrentLinkedList>> map;

    ConcurrentLinkedListHashMapTest() = default;

    void SetUp() override {
        map = std::make_unique<ConcurrentHashMap<int, int, ConcurrentLinkedList>>();
    }

    void TearDown() override {}

};