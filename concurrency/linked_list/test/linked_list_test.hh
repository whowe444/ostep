#pragma once

#include <gtest/gtest.h>
#include "linked_list.hh"

class LinkedListTest : public ::testing::Test {

protected:
    std::unique_ptr<LinkedList> list;

    LinkedListTest() = default;

    void SetUp() override {
        list = std::make_unique<LinkedList>();
    }

    void TearDown() override {}

};
