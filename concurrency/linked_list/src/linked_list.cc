#include "linked_list.hh"

#include <stdexcept>

size_t LinkedList::GetSize() {
    return this->size;
}

bool LinkedList::IsEmpty() {
    return this->GetSize() == 0;
}

bool LinkedList::Add(int value) {
    this->tail->next = new Node(nullptr, value);
    this->size++;
    this->tail = this->tail->next;
    return true;
}

int LinkedList::Get(int index) {
    // If the list is empty throw an exception
    const int n = this->GetSize();
    if (n == 0) throw std::runtime_error("Unable to retrieve from empty list!");

    // If the index is out of bounds throw an exception
    if (index >= n) throw std::runtime_error("Index out of bounds");

    Node* ptr = this->sentinel->next;
    int i = 0;
    while (ptr) {
        if (i == index) return ptr->value;
        ptr = ptr->next;
        i++;
    }

    return -1;
}