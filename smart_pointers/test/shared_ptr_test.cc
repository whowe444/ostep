#include "shared_ptr_test.hh"

TEST_F(SharedPtrTest, TestConstructor) {
    EXPECT_TRUE(my_shared_ptr);
    EXPECT_EQ(*my_shared_ptr, 5);
}

TEST_F(SharedPtrTest, TestCopyAssignment) {
    SharedPtr<int> new_shared_ptr = SharedPtr<int>(new int(10));
    new_shared_ptr = my_shared_ptr;
    EXPECT_EQ(new_shared_ptr, my_shared_ptr);
}

TEST_F(SharedPtrTest, TestCopyConstructor) {
    SharedPtr<int> new_shared_ptr (my_shared_ptr);
    EXPECT_EQ(new_shared_ptr, my_shared_ptr);
}

TEST_F(SharedPtrTest, TestDestructor) {
    int* my_int = new int(4);

    // Construct the object within this scope
    {
        SharedPtr<int> new_shared_ptr;
        new_shared_ptr = SharedPtr<int>(my_int);
    }

    // Since my_int has be deleted, the data will be garbage.
    EXPECT_NE(*my_int, 4);
}

TEST_F(SharedPtrTest, TestMoveAssignment) {
    SharedPtr<int> new_shared_ptr = SharedPtr<int>(new int(10));
    new_shared_ptr = std::move(my_shared_ptr);

    EXPECT_TRUE(new_shared_ptr);
    EXPECT_FALSE(my_shared_ptr);
}

TEST_F(SharedPtrTest, TestMoveConstructor) {
    SharedPtr<int> new_shared_ptr = std::move(my_shared_ptr);
    EXPECT_TRUE(new_shared_ptr);
    EXPECT_FALSE(my_shared_ptr);
}