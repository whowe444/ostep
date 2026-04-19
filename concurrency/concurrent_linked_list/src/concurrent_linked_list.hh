#pragma once

#include "node.hh"
#include <vector>
#include <atomic>

template<typename T>
class ConcurrentLinkedList {

public:

    // IteratorTemplate Class Definition.
    template<bool IsConst>
    struct IteratorTemplate {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        using lock_type = std::conditional_t<IsConst, std::shared_lock<std::shared_mutex>,
            std::unique_lock<std::shared_mutex>>;
    public:
    
        // Constructor
        IteratorTemplate(Node<T>* node) 
            : 
                current(node),
                lock(current ? lock_type(current->mtx) : lock_type())
        {
        }

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
            if (current) lock = std::move(lock_type(current->mtx));
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
        lock_type lock;
    };

    using Iterator = IteratorTemplate<false>;
    using ConstIterator = IteratorTemplate<true>;

    // Read-only range — shared lock, multiple threads can hold this concurrently
    class SharedRange {
        const ConcurrentLinkedList<T>* listPointer;
    public:
        SharedRange(const ConcurrentLinkedList<T>* listPointer_)
            : listPointer(listPointer_) {}

        ConstIterator begin() { return listPointer->cbegin(); }
        ConstIterator end()   { return listPointer->cend();   }
    };

    // Mutating range — unique lock, exclusive access
    class UniqueRange {
        ConcurrentLinkedList<T>* listPointer;
    public:
        UniqueRange(ConcurrentLinkedList<T>* listPointer_)
            : listPointer(listPointer_) {}

        Iterator begin() { return listPointer->begin(); }
        Iterator end()   { return listPointer->end();   }
    };

    // Return the SharedRange (read-only)
    SharedRange GetSharedRange() const { return SharedRange(this); }

    // Return the UniqueRange (mutating)
    UniqueRange GetUniqueRange() { return UniqueRange(this); }

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
    ConcurrentLinkedList() {
        sentinel = new Node<T>();
        size = 0;
    }

    // Destructor
    ~ConcurrentLinkedList() {
        this->Clear();
        delete(this->sentinel);
    }

    // Copy Ctor
    ConcurrentLinkedList(const ConcurrentLinkedList& other) = delete;

    // Copy Assignment
    ConcurrentLinkedList& operator=(const ConcurrentLinkedList& other) = delete;

    // Move Ctor
    ConcurrentLinkedList(ConcurrentLinkedList&& other) = delete;

    // Move Assignemnt
    ConcurrentLinkedList& operator=(ConcurrentLinkedList&& other) = delete;

    // GetSize
    size_t GetSize() const {
        return this->size.load();
    }

    // IsEmpty
    bool IsEmpty() const {
        return this->GetSize() == 0;
    }

    // Add Function
    // Returns true upon success
    bool Add(int value) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;

        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            prev_lock = std::move(curr_lock);
            prev = ptr;
            ptr = ptr->next;
        }

        prev->next = new Node<T>(nullptr, value);
        this->size++;
        return true;
    }

    // Value is Contained in the List.
    bool Contains(int value) const {
        std::shared_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::shared_lock<std::shared_mutex> curr_lock(ptr->mtx);
        
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

    // Remove Value at index
    // Returns value removed
    bool Remove(int value) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* prev = this->sentinel;
        Node<T>* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
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

    // Clear out the list so that it is
    // empty.
    void Clear() {
        if (!sentinel) return;
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        Node<T>* ptr = this->sentinel->next;
        while (ptr) {
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            auto temp = ptr->next;
            curr_lock.unlock();
            delete(ptr);
            ptr = temp;
        }

        this->sentinel->next = nullptr;
        this->size = 0;
    }

private:

    Node<T>* sentinel;
    std::atomic<size_t> size;
};
