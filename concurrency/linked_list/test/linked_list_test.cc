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

TEST_F(LinkedListTest, AddMultipleElements) {
    const int NUM_ELEMENTS = 100;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        list->Add(i);
    }

    EXPECT_EQ(list->GetSize(), NUM_ELEMENTS);

    // Remove an elemnent
    EXPECT_EQ(list->Remove(50), 50);
    EXPECT_EQ(list->GetSize(), NUM_ELEMENTS - 1);

    // Verify first and last element
    EXPECT_EQ(list->Get(0), 0);
    EXPECT_EQ(list->Get(98), 99);
}

TEST_F(LinkedListTest, CopyAssignmentSelf) {
    LinkedList list1 = LinkedList<int>();
    list1.Add(1);
    list1 = list1;
    
    EXPECT_EQ(list1.GetSize(), 1);
}

TEST_F(LinkedListTest, CopyAssignment) {
    LinkedList list1 = LinkedList<int>();
    list1.Add(1);
    list1.Add(2);
    LinkedList list2 = list1;

    EXPECT_EQ(list1.GetSize(), list2.GetSize());
    EXPECT_EQ(list1.Get(0), list2.Get(0));
    EXPECT_EQ(list1.Get(1), list2.Get(1));

    list2.Add(3);
    EXPECT_NE(list1.GetSize(), list2.GetSize());
}

TEST_F(LinkedListTest, CopyConstructor) {
    LinkedList list1 = LinkedList<int>();
    list1.Add(1);
    list1.Add(2);
    LinkedList list2 (list1);

    EXPECT_EQ(list1.GetSize(), list2.GetSize());
    EXPECT_EQ(list1.Get(0), list2.Get(0));
    EXPECT_EQ(list1.Get(1), list2.Get(1));

    // Mutating list2 doesn't affect list 1.
    list2.Add(3);
    EXPECT_NE(list1.GetSize(), list2.GetSize());
}

TEST_F(LinkedListTest, MoveAssignmentSelf) {
    LinkedList list1 = LinkedList<int>();
    list1.Add(1);
    list1 = std::move(list1);
    
    EXPECT_EQ(list1.GetSize(), 1); 
}

TEST_F(LinkedListTest, MoveAssignment) {
    LinkedList list1 = LinkedList<int>();
    list1.Add(1);
    list1.Add(2);
    LinkedList list2 = std::move(list1);

    EXPECT_EQ(list2.Get(0), 1);
    EXPECT_EQ(list2.Get(1), 2);
    EXPECT_EQ(list2.GetSize(), 2);
    EXPECT_NE(list1.GetSize(), list2.GetSize());

    list2.Add(3);
    EXPECT_NE(list1.GetSize(), list2.GetSize());

    // The old list should thrown an exception
    EXPECT_THROW(list1.Get(0), std::runtime_error);
}

TEST_F(LinkedListTest, MoveConstructor) {
    LinkedList list1 = LinkedList<int>();
    list1.Add(1);
    list1.Add(2);
    LinkedList list2 (std::move(list1));

    EXPECT_EQ(list2.Get(0), 1);
    EXPECT_EQ(list2.Get(1), 2);
    EXPECT_EQ(list2.GetSize(), 2);
    EXPECT_NE(list1.GetSize(), list2.GetSize());

    // Mutating list2 doesn't affect list 1
    list2.Add(3);
    EXPECT_NE(list1.GetSize(), list2.GetSize());

    // The old list should thrown an exception
    EXPECT_THROW(list1.Get(0), std::runtime_error);
}

TEST_F(LinkedListTest, RemoveLastThenAdd) {
    EXPECT_TRUE(list->Add(1));
    EXPECT_EQ(list->GetSize(), 1);
    EXPECT_EQ(list->Remove(0), 1);

    EXPECT_EQ(list->GetSize(), 0);

    // would segfault if tail isn't updated
    EXPECT_TRUE(list->Add(2));

    EXPECT_EQ(list->GetSize(), 1);
    EXPECT_EQ(list->Get(0), 2);
}

TEST_F(LinkedListTest, BenchmarkAdd) {
    auto start = std::chrono::high_resolution_clock::now();
    const int NUM_ELEMENTS = 10000000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        this->list->Add(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "BenchmarkAdd took: " << std::to_string(duration.count() / (double)NUM_ELEMENTS)
        << " nanoseconds per Add operation." << std::endl;
}

TEST_F(LinkedListTest, BenchmarkRemove) {
    const int NUM_ELEMENTS = 10000000;
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        this->list->Add(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        this->list->Remove(0);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "BenchmarkAdd took: " << std::to_string(duration.count() / (double)NUM_ELEMENTS)
        << " nanoseconds per Remove operation." << std::endl;
}
