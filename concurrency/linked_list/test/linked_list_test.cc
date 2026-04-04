#include "linked_list_test.hh"

TEST_F(LinkedListTest, TestConstructor) {
    EXPECT_TRUE(list);
}

TEST_F(LinkedListTest, GetSize) {
    EXPECT_EQ(list->GetSize(), 0);
}

TEST_F(LinkedListTest, IsEmpty) {
    EXPECT_TRUE(list->IsEmpty());
}

TEST_F(LinkedListTest, Add) {
    const int new_value = 1;
    EXPECT_TRUE(list->Add(new_value));
    EXPECT_EQ(list->GetSize(), 1);
    EXPECT_EQ(list->Get(0), new_value);
}

TEST_F(LinkedListTest, AddTwoElements) {
    const int new_value = 2;
    const int second_new_value = 3;

    // Add two new values
    list->Add(new_value);
    list->Add(second_new_value);

    EXPECT_EQ(list->GetSize(), 2);
    EXPECT_EQ(list->Get(0), new_value);
    EXPECT_EQ(list->Get(1), second_new_value);
}

TEST_F(LinkedListTest, GetWhenEmpty) {
    EXPECT_THROW({
        list->Get(5);
    }, std::runtime_error);
}

TEST_F(LinkedListTest, TestGetIndexOutOfBounds) {
    // First add an element to the list
    list->Add(100);

    EXPECT_THROW({
        list->Get(5);
    }, std::runtime_error);
}

TEST_F(LinkedListTest, RemoveWhenEmpty) {
    EXPECT_THROW({
        list->Remove(5);
    }, std::runtime_error);
}

TEST_F(LinkedListTest, TestRemoveIndexOutOfBounds) {
    // First add an element to the list
    list->Add(100);

    EXPECT_THROW({
        list->Remove(5);
    }, std::runtime_error);
}

TEST_F(LinkedListTest, TestRemove) {
    // Add Elements
    const int first_value = 99;
    list->Add(first_value);
    const int second_value = 100;
    list->Add(second_value);

    // Now Remove
    EXPECT_EQ(list->Remove(0), first_value);
    
    // Verify
    EXPECT_EQ(list->Get(0), second_value);
    EXPECT_EQ(list->GetSize(), 1);

}