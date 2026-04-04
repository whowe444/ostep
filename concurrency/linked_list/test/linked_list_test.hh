#pragma once

#include <gtest/gtest.h>
#include "linked_list.hh"

class LinkedListTest : public ::testing::Test {

protected:
    std::unique_ptr<LinkedList<int>> list;

    LinkedListTest() = default;

    void SetUp() override {
        list = std::make_unique<LinkedList<int>>();
    }

    void TearDown() override {}

};
