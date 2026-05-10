#include "unique_ptr_test.hh"

TEST_F(UniquePtrTest, ConstructorTest) {
    EXPECT_EQ(*unique_ptr, 5);
}

TEST_F(UniquePtrTest, TestMoveAssignment) {
    // Preconditions
    EXPECT_TRUE(unique_ptr);

    // New Unique Ptr
    UniquePtr<int> new_unique_ptr = std::move(unique_ptr);

    // Verify
    EXPECT_TRUE(new_unique_ptr);
    EXPECT_FALSE(unique_ptr);
}

TEST_F(UniquePtrTest, TestMoveConstructor) {
    // Preconditions
    EXPECT_TRUE(unique_ptr);

    // New Unique Pointer
    UniquePtr<int> new_unique_ptr {std::move(unique_ptr)};

    // Verify
    EXPECT_TRUE(new_unique_ptr);
    EXPECT_FALSE(unique_ptr);
}

TEST_F(UniquePtrTest, TestDefaultCtor) {
    EXPECT_FALSE(UniquePtr<int>());
}
