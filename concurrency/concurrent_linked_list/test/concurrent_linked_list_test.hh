#pragma once

#include <gtest/gtest.h>
#include "concurrent_linked_list.hh"

class ConcurrentLinkedListTest : public ::testing::Test {

protected:
    std::unique_ptr<ConcurrentLinkedList> list;

    ConcurrentLinkedListTest() = default;

    void SetUp() override {
        list = std::make_unique<ConcurrentLinkedList>();
    }

    void TearDown() override {}

};
