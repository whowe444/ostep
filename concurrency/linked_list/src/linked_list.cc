#include "linked_list.hh"

#include <stdexcept>

// Copy Ctor
LinkedList::LinkedList(const LinkedList& other) : LinkedList() {
    Node* ptr = other.sentinel->next;
    while(ptr) {
        this->Add(ptr->value);
        ptr = ptr->next;
    }
}

// Copy Assignment
LinkedList& LinkedList::operator=(const LinkedList& other) {
    // Self assignment guard
    if (this == &other) return *this;

    // Clear out all nodes and set size to 0.
    this->Clear();

    // Copy other into this one
    Node* ptr = other.sentinel->next;
    while (ptr) {
        this->Add(ptr->value);
        ptr = ptr->next;
    }

    return *this;
}

// Move Ctor
LinkedList::LinkedList(LinkedList&& other) noexcept
    :
        sentinel(other.sentinel),
        size(other.size)
{
    other.sentinel = nullptr;
    other.size = 0;
}

// Move Assignment
LinkedList& LinkedList::operator=(LinkedList&& other) noexcept {
    // Self assignment guard
    if (this == &other) return *this;

    // Clear out this list
    this->Clear();

    // Delete old sentinel
    auto old_sentinel = this->sentinel;
    delete(old_sentinel);

    this->sentinel = other.sentinel;
    this->size = other.size;

    // Null out other's sentinel
    other.sentinel = nullptr;
    other.size = 0;

    return *this;
}


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

    throw std::logic_error("Get: unreachable!");
}

int LinkedList::Remove(int index) {
    // If the list is empty throw an exception
    const int n = this->GetSize();
    if (n == 0) throw std::runtime_error("Unable to retrieve from empty list!");

    // If the index is out of bounds throw an exception
    if (index >= n) throw std::runtime_error("Index out of bounds");

    Node* prev = this->sentinel;
    Node* ptr = prev->next;
    int i = 0;
    while (ptr) {
        if (i == index) {
            // Delete this node
            prev->next = ptr->next;
            int return_val = ptr->value;
            this->size --;
            delete(ptr);
            return return_val;
        } else {
            i++;
            ptr = ptr->next;
        }
    }

    throw std::logic_error(("Remove: unreachable!"));
}

void LinkedList::Clear() {
    Node* ptr = this->sentinel->next;

    while (ptr) {
        auto temp = ptr->next;
        delete(ptr);
        ptr = temp;
    }

    this->sentinel->next = nullptr;
    this->size = 0;
}