#pragma once

#include <gtest/gtest.h>
#include "linked_list/concurrent_linked_list.hh"

class ConcurrentLinkedListTest : public ::testing::Test {

protected:
    std::unique_ptr<ConcurrentLinkedList<int, int>> list;

    ConcurrentLinkedListTest() = default;

    void SetUp() override {
        list = std::make_unique<ConcurrentLinkedList<int, int>>();
    }

    void TearDown() override {}

};
