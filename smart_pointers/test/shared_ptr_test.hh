#pragma once

#include <gtest/gtest.h>
#include "shared_ptr.hh"

class SharedPtrTest : public ::testing::Test {
public:
    SharedPtrTest()
        :
            my_shared_ptr(new int(5))
    {
    }

    void SetUp() override {}

    void TearDown() override {}
protected:
    SharedPtr<int> my_shared_ptr;
};