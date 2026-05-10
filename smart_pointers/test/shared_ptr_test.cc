#include "shared_ptr_test.hh"

TEST_F(SharedPtrTest, TestConstructor) {
    EXPECT_EQ(*my_shared_ptr, 5);
}

TEST_F(SharedPtrTest, TestCopyAssignment) {
    SharedPtr<int> new_shared_ptr = my_shared_ptr;
    EXPECT_EQ(new_shared_ptr, my_shared_ptr);
}

TEST_F(SharedPtrTest, TestCopyConstructor) {
    SharedPtr<int> new_shared_ptr (my_shared_ptr);
    EXPECT_EQ(new_shared_ptr, my_shared_ptr);
}