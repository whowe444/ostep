#pragma once

#include <gtest/gtest.h>

extern "C" {
#include "realloc_list.h"
#include "realloc_list_iterator.h"
}

class ReallocListTest : public ::testing::Test {

protected:
    ReallocList *list;

    void SetUp() override {
        list = ReallocList_init();
    }

    void TearDown() override {
        ReallocList_free(list);
    }

};