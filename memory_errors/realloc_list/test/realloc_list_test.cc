#include "realloc_list_test.hh"

TEST_F(ReallocListTest, EmptyList) {
    EXPECT_EQ(ReallocList_size(list), 0);
}

TEST_F(ReallocListTest, RemoveNonExistent) {
    int return_val;
    EXPECT_EQ(ReallocList_remove(list, 0, &return_val), -1);
}

TEST_F(ReallocListTest, AddElement) {
    // Add an element to the realloc list.
    EXPECT_EQ(ReallocList_add(list, 5), 0);

    // Verify the size has increased by 1.
    EXPECT_EQ(ReallocList_size(list), 1);
}

TEST_F(ReallocListTest, GetNonExistent) {
    EXPECT_EQ(ReallocList_get(list, 0), nullptr);
}

TEST_F(ReallocListTest, GetElement) {
    int test_value = 5;

    // Add an element to the realloc list.
    ReallocList_add(list, test_value);

    // Retrieve the element at index 0.
    EXPECT_EQ(*ReallocList_get(list, 0), test_value);
}

TEST_F(ReallocListTest, RemoveElement) {
    int test_value = 5;

    // Add an element to the realloc list.
    ReallocList_add(list, test_value);

    // Remove the element.
    int return_val;
    EXPECT_EQ(ReallocList_remove(list, 0, &return_val), 0);
    EXPECT_EQ(return_val, test_value);

    // Size is zero.
    EXPECT_EQ(ReallocList_size(list), 0);
}

TEST_F(ReallocListTest, AddTwoElements) {
    // Add an element to the realloc list.
    EXPECT_EQ(ReallocList_add(list, 5), 0);

    // Add a second element to the realloc list.
    EXPECT_EQ(ReallocList_add(list, 10), 0);

    // Verify the size has increased by 2.
    EXPECT_EQ(ReallocList_size(list), 2);

    // Verify the elements
    EXPECT_EQ(*ReallocList_get(list, 1), 10);

    // Verify the elements
    EXPECT_EQ(*ReallocList_get(list, 0), 5);
}

TEST_F(ReallocListTest, AddTwoRemoveOne) {
    // Add an element to the realloc list.
    ReallocList_add(list, 5);

    // Add a second element to the realloc list.
    ReallocList_add(list, 10);

    // Remove index 0
    int return_val;
    EXPECT_EQ(ReallocList_remove(list, 0, &return_val), 0);
    EXPECT_EQ(5, return_val);

    // Size has reduced by 1.
    EXPECT_EQ(ReallocList_size(list), 1);
}

TEST_F(ReallocListTest, AddTwoRemoveTwoIndexZero) {
    // Add an element to the realloc list.
    ReallocList_add(list, 5);

    // Add a second element to the realloc list.
    ReallocList_add(list, 10);

    // Remove index 0
    int return_val;
    EXPECT_EQ(ReallocList_remove(list, 0, &return_val), 0);
    EXPECT_EQ(5, return_val);

    // Remove index 0 again
    EXPECT_EQ(ReallocList_remove(list, 0, &return_val), 0);
    EXPECT_EQ(10, return_val);

    // Size has reduced by 2.
    EXPECT_EQ(ReallocList_size(list), 0);
}

TEST_F(ReallocListTest, AddTwoRemoveTwoIndexOne) {
    // Add an element to the realloc list.
    ReallocList_add(list, 5);

    // Add a second element to the realloc list.
    ReallocList_add(list, 10);

    // Remove index 0
    int return_val;
    EXPECT_EQ(ReallocList_remove(list, 1, &return_val), 0);
    EXPECT_EQ(10, return_val);

    // Remove index 0 again
    EXPECT_EQ(ReallocList_remove(list, 0, &return_val), 0);
    EXPECT_EQ(5, return_val);

    // Size has reduced by 2.
    EXPECT_EQ(ReallocList_size(list), 0);
}

TEST_F(ReallocListTest, Iterator) {
    ReallocList_add(list, 1);
    ReallocList_add(list, 2);
    ReallocList_add(list, 3);

    ReallocListIterator it = ReallocList_iterator(list);

    EXPECT_TRUE(ReallocListIterator_hasNext(&it));
    EXPECT_EQ(ReallocListIterator_next(&it), 1);

    EXPECT_TRUE(ReallocListIterator_hasNext(&it));
    EXPECT_EQ(ReallocListIterator_next(&it), 2);

    EXPECT_TRUE(ReallocListIterator_hasNext(&it));
    EXPECT_EQ(ReallocListIterator_next(&it), 3);

    EXPECT_FALSE(ReallocListIterator_hasNext(&it));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}