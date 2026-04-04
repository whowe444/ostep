#include "concurrent_linked_list.hh"

#include <stdexcept>

// Copy Ctor
ConcurrentLinkedList::ConcurrentLinkedList(const ConcurrentLinkedList& other) : ConcurrentLinkedList() {
    // Snapshot other's values under its lock
    std::vector<int> values;
    {
        std::unique_lock<std::mutex> other_lock(other.sentinel->mtx);
        Node* ptr = other.sentinel->next;
        while (ptr) {
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
            values.push_back(ptr->value);
            other_lock = std::move(curr_lock);
            ptr = ptr->next;
        }
    }
    // Now add without holding any of other's locks
    for (int v : values) this->Add(v);
}

// Copy Assignment
ConcurrentLinkedList& ConcurrentLinkedList::operator=(const ConcurrentLinkedList& other) {
    // Self assignment guard
    if (this == &other) return *this;

    // Clear out all nodes and set size to 0.
    this->Clear();

    // Snapshot other's values under its lock
    std::vector<int> values;
    {
        std::unique_lock<std::mutex> other_lock(other.sentinel->mtx);
        Node* ptr = other.sentinel->next;
        while (ptr) {
            std::unique_lock<std::mutex> curr_lock(ptr->mtx);
            values.push_back(ptr->value);
            other_lock = std::move(curr_lock);
            ptr = ptr->next;
        }
    }
    // Now add without holding any of other's locks
    for (int v : values) this->Add(v);

    return *this;
}

// Move Ctor
ConcurrentLinkedList::ConcurrentLinkedList(ConcurrentLinkedList&& other) noexcept
    :
        sentinel(other.sentinel),
        size(other.size.load())
{
    other.sentinel = nullptr;
    other.size = 0;
}

// Move Assignment
ConcurrentLinkedList& ConcurrentLinkedList::operator=(ConcurrentLinkedList&& other) noexcept {
    // Self assignment guard
    if (this == &other) return *this;

    // Clear out this list
    this->Clear();

    // Delete old sentinel
    delete(this->sentinel);

    this->sentinel = other.sentinel;
    this->size = other.size.load();

    // Null out other's sentinel
    other.sentinel = nullptr;
    other.size = 0;

    return *this;
}


size_t ConcurrentLinkedList::GetSize() {
    return this->size.load();
}

bool ConcurrentLinkedList::IsEmpty() {
    return this->GetSize() == 0;
}

bool ConcurrentLinkedList::Add(int value) {
    std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
    Node* prev = this->sentinel;
    Node* ptr = prev->next;

    while (ptr) {
        // Grab the next node's lock
        std::unique_lock<std::mutex> curr_lock(ptr->mtx);
        prev_lock = std::move(curr_lock);
        prev = ptr;
        ptr = ptr->next;
    }

    prev->next = new Node(nullptr, value);
    this->size++;
    return true;
}

bool ConcurrentLinkedList::Contains(int value) {
    std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
    Node* prev = this->sentinel;
    Node* ptr = prev->next;
    while (ptr) {
        // Grab the next node's lock
        std::unique_lock<std::mutex> curr_lock(ptr->mtx);
    
        if (ptr->value == value) {
            return true;
        }

        // Release previous
        prev_lock = std::move(curr_lock);

        prev = ptr;
        ptr = ptr->next;
    }
    return false;
}

bool ConcurrentLinkedList::Remove(int value) {
    std::unique_lock<std::mutex> prev_lock(this->sentinel->mtx);
    Node* prev = this->sentinel;
    Node* ptr = prev->next;
    while (ptr) {
        // Grab the next node's lock
        std::unique_lock<std::mutex> curr_lock(ptr->mtx);
        if (ptr->value == value) {
            // Delete this node
            prev->next = ptr->next;
            this->size--;
            curr_lock.unlock();
            delete(ptr);
            return true;
        } else {
            prev_lock = std::move(curr_lock);

            ptr = ptr->next;
            prev = prev->next;
        }
    }
    return false;
}

void ConcurrentLinkedList::Clear() {
    if (!sentinel) return;
    Node* ptr = this->sentinel->next;
    while (ptr) {
        auto temp = ptr->next;
        delete(ptr);
        ptr = temp;

    }

    this->sentinel->next = nullptr;
    this->size = 0;
}