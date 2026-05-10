#pragma once

#include <gtest/gtest.h>
#include "unique_ptr.hh"

class UniquePtrTest : public ::testing::Test {
public:
    UniquePtrTest()
        :
            unique_ptr(new int(5))
    {
    }

    void SetUp() override {}

    void TearDown() override {}
protected:
    UniquePtr<int> unique_ptr;
};