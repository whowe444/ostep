#pragma once

#include "node.hh"
#include <vector>
#include <stdexcept>

template<typename T>
class LinkedList {

public:

    // IteratorTemplate Class Definition.
    template<bool IsConst>
    struct IteratorTemplate {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

    public:
    
        // Constructor
        IteratorTemplate(Node<T>* node) : current(node) {}

        // Define the dereference operator.
        reference operator*() const { 
            assert(current && "Dereferencing end() iterator is undefined behavior.");
            return current->value;
        }

        // Define the pointer access operator.
        pointer operator->() const { 
            assert(current && "Dereferencing end() iterator is undefined behavior.");
            return &(current->value);
        }

        // Define the pre-increment operator.
        IteratorTemplate<IsConst>& operator++() {
            assert(current && "Incrementing end() iterator is undefined behavior.");
            current = current->next;
            return *this;
        }

        // Define the post-increment operator.
        IteratorTemplate<IsConst> operator++(int) {
            auto tmp = *this;

            // Now pre-increment the current pointer.
            ++(*this);

            // Now return the temporary.
            return tmp;
        }

        // Define the equals operator.
        bool operator==(const IteratorTemplate<IsConst>& other) const {
            return this->current == other.current;
        }

        // Define the not-equals operator.
        bool operator!=(const IteratorTemplate<IsConst>& other) const {
            return !(*this == other);
        }

    private:
        Node<T>* current;
    };

    using Iterator = IteratorTemplate<false>;
    using ConstIterator = IteratorTemplate<true>;

    // Define the begin function.
    Iterator begin() {
        return Iterator(this->sentinel->next);
    }

    // Define the end function.
    Iterator end() {
        return Iterator(nullptr);
    }

    // Define the cbegin function.
    ConstIterator cbegin() const {
        return ConstIterator(this->sentinel->next);
    }

    // Define the cend function.
    ConstIterator cend() const {
        return ConstIterator(nullptr);
    }

    // Constructor 
    LinkedList() {
        sentinel = new Node<T>();
        tail = sentinel;
        size = 0;
    }

    // Destructor
    ~LinkedList() {
        this->Clear();
        delete(this->sentinel);
    }

    // Copy Ctor
    LinkedList(const LinkedList& other) : LinkedList() {
        Node<T>* ptr = other.sentinel->next;
        while(ptr) {
            this->Add(ptr->value);
            ptr = ptr->next;
        }
    }

    // Copy Assignment
    LinkedList& operator=(const LinkedList& other) {
        // Self assignment guard
        if (this == &other) return *this;

        // Clear out all nodes and set size to 0.
        this->Clear();

        // Copy other into this one
        Node<T>* ptr = other.sentinel->next;
        while (ptr) {
            this->Add(ptr->value);
            ptr = ptr->next;
        }

        return *this;
    }

    // Move Ctor
    LinkedList(LinkedList&& other) noexcept
        :
            sentinel(other.sentinel),
            tail(other.tail),
            size(other.size)
    {
        other.sentinel = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    // Move Assignemnt
    LinkedList& operator=(LinkedList&& other) noexcept {
        // Self assignment guard
        if (this == &other) return *this;

        // Clear out this list
        this->Clear();

        // Delete old sentinel
        delete(this->sentinel);

        this->sentinel = other.sentinel;
        this->tail = other.tail;
        this->size = other.size;

        // Null out other's sentinel
        other.sentinel = nullptr;
        other.tail = nullptr;
        other.size = 0;

        return *this;
    }

    // GetSize
    size_t GetSize() const {
        return this->size;
    }

    // IsEmpty
    bool IsEmpty() const {
        return this->GetSize() == 0;
    }

    // Add Function
    // Returns true upon success
    bool Add(int value) {
        this->tail->next = new Node<T>(nullptr, value);
        this->size++;
        this->tail = this->tail->next;
        return true;
    }

    // Get Value at index
    int Get(int index) const {
        // If the list is empty throw an exception
        const int n = this->GetSize();
        if (n == 0) throw std::runtime_error("Unable to retrieve from empty list!");

        // If the index is out of bounds throw an exception
        if (index >= n) throw std::runtime_error("Index out of bounds");

        Node<T>* ptr = this->sentinel->next;
        int i = 0;
        while (ptr) {
            if (i == index) return ptr->value;
            ptr = ptr->next;
            i++;
        }

        throw std::logic_error("Get: unreachable!");
    }

    // Remove Value at index
    // Returns value removed
    int Remove(int index) {
        // If the list is empty throw an exception
        const int n = this->GetSize();
        if (n == 0) throw std::runtime_error("Unable to retrieve from empty list!");

        // If the index is out of bounds throw an exception
        if (index >= n) throw std::runtime_error("Index out of bounds");

        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;
        int i = 0;
        while (ptr) {
            if (i == index) {
                // check if tail
                if (i == n - 1) this->tail = prev;

                // Delete this node
                prev->next = ptr->next;
                int return_val = ptr->value;
                this->size --;
                delete(ptr);

                return return_val;
            } else {
                i++;
                ptr = ptr->next;
                prev = prev->next;
            }
        }

        throw std::logic_error(("Remove: unreachable!"));
    }

    // Clear out the list so that it is
    // empty.
    void Clear() {
        if (!sentinel) return;
        Node<T>* ptr = this->sentinel->next;

        while (ptr) {
            auto temp = ptr->next;
            delete(ptr);
            ptr = temp;
        }

        this->sentinel->next = nullptr;
        this->size = 0;
    }

private:

    Node<T>* sentinel;
    Node<T>* tail;
    size_t size;

};
