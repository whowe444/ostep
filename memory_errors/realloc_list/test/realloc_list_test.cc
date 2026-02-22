#include "realloc_list_test.hh"

TEST(ReallocListTest, EmptyList) {
    ReallocList list = {0};
    EXPECT_EQ(ReallocList_size(&list), 0);
}

TEST(ReallocListTest, RemoveNonExistent) {
    ReallocList list = {0};
    EXPECT_EQ(ReallocList_remove(&list, 0), nullptr);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}