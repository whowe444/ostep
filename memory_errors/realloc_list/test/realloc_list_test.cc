#include "realloc_list_test.hh"

TEST(ReallocListTest, EmptyList) {
    ReallocList list = {0, NULL};
    EXPECT_EQ(ReallocList_size(&list), 0);
}

TEST(ReallocListTest, RemoveNonExistent) {
    ReallocList list = {0, NULL};
    int return_val;
    EXPECT_EQ(ReallocList_remove(&list, 0, &return_val), -1);
}

TEST(ReallocListTest, AddElement) {
    ReallocList list = {0, NULL};

    // Add an element to the realloc list.
    EXPECT_EQ(ReallocList_add(&list, 5), 0);

    // Verify the size has increased by 1.
    EXPECT_EQ(ReallocList_size(&list), 1);
}

TEST(ReallocListTest, GetNonExistent) {
    ReallocList list = {0, NULL};
    EXPECT_EQ(ReallocList_get(&list, 0), nullptr);
}

TEST(ReallocListTest, GetElement) {
    ReallocList list = {0, NULL};

    int test_value = 5;

    // Add an element to the realloc list.
    ReallocList_add(&list, test_value);

    // Retrieve the element at index 0.
    EXPECT_EQ(*ReallocList_get(&list, 0), test_value);
}

TEST(ReallocListTest, RemoveElement) {
    ReallocList list = {0, NULL};

    int test_value = 5;

    // Add an element to the realloc list.
    ReallocList_add(&list, test_value);

    // Remove the element.
    int return_val;
    EXPECT_EQ(ReallocList_remove(&list, 0, &return_val), 0);
    EXPECT_EQ(return_val, test_value);

    // Size is zero.
    EXPECT_EQ(ReallocList_size(&list), 0);
}

TEST(ReallocListTest, AddTwoElements) {
    ReallocList list = {0, NULL};

    // Add an element to the realloc list.
    EXPECT_EQ(ReallocList_add(&list, 5), 0);

    // Add a second element to the realloc list.
    EXPECT_EQ(ReallocList_add(&list, 10), 0);

    // Verify the size has increased by 2.
    EXPECT_EQ(ReallocList_size(&list), 2);

    // Verify the elements
    EXPECT_EQ(*ReallocList_get(&list, 1), 10);

    // Verify the elements
    EXPECT_EQ(*ReallocList_get(&list, 0), 5);
}

TEST(ReallocListTest, AddTwoRemoveOne) {
    ReallocList list = {0, NULL};

    // Add an element to the realloc list.
    ReallocList_add(&list, 5);

    // Add a second element to the realloc list.
    ReallocList_add(&list, 10);

    // Remove index 0
    int return_val;
    EXPECT_EQ(ReallocList_remove(&list, 0, &return_val), 0);
    EXPECT_EQ(5, return_val);

    // Size has reduced by 1.
    EXPECT_EQ(ReallocList_size(&list), 1);
}

TEST(ReallocListTest, AddTwoRemoveTwoIndexZero) {
    ReallocList list = {0, NULL};

    // Add an element to the realloc list.
    ReallocList_add(&list, 5);

    // Add a second element to the realloc list.
    ReallocList_add(&list, 10);

    // Remove index 0
    int return_val;
    EXPECT_EQ(ReallocList_remove(&list, 0, &return_val), 0);
    EXPECT_EQ(5, return_val);

    // Remove index 0 again
    EXPECT_EQ(ReallocList_remove(&list, 0, &return_val), 0);
    EXPECT_EQ(10, return_val);

    // Size has reduced by 2.
    EXPECT_EQ(ReallocList_size(&list), 0);
}

TEST(ReallocListTest, AddTwoRemoveTwoIndexOne) {
    ReallocList list = {0, NULL};

    // Add an element to the realloc list.
    ReallocList_add(&list, 5);

    // Add a second element to the realloc list.
    ReallocList_add(&list, 10);

    // Remove index 0
    int return_val;
    EXPECT_EQ(ReallocList_remove(&list, 1, &return_val), 0);
    EXPECT_EQ(10, return_val);

    // Remove index 0 again
    EXPECT_EQ(ReallocList_remove(&list, 0, &return_val), 0);
    EXPECT_EQ(5, return_val);

    // Size has reduced by 2.
    EXPECT_EQ(ReallocList_size(&list), 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}