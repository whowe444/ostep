#pragma once

#include "./node.hh"
#include <vector>
#include <atomic>

template<typename K, typename V>
class ConcurrentLinkedList {

using LinkedListNode = linked_list::Node<std::pair<K, V>>;

public:

    // IteratorTemplate Class Definition.
    template<bool IsConst>
    struct IteratorTemplate {
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<K, V>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const std::pair<K, V>*, std::pair<K, V>*>;
        using reference = std::conditional_t<IsConst, const std::pair<K, V>&, std::pair<K, V>&>;

        using lock_type = std::conditional_t<IsConst, std::shared_lock<std::shared_mutex>,
            std::unique_lock<std::shared_mutex>>;
    public:
    
        // Constructor
        IteratorTemplate(LinkedListNode* node) 
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
            auto ptr = current->next;
            if (ptr) {
                // First acquire the lock
                lock_type next_lock(ptr->mtx);
                lock = std::move(next_lock);
            } else {
                lock = lock_type(); // Release current lock if we're moving to end()
                current = nullptr;
            }
            current = ptr;
            return *this;
        }

        // Define the post-increment operator.
        IteratorTemplate<IsConst> operator++(int) requires IsConst {
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
        LinkedListNode* current;
        lock_type lock;
    };

    using Iterator = IteratorTemplate<false>;
    using ConstIterator = IteratorTemplate<true>;

    // Read-only range — shared lock, multiple threads can hold this concurrently
    class SharedRange {
        const ConcurrentLinkedList<K, V>* listPointer;
    public:
        SharedRange(const ConcurrentLinkedList<K, V>* listPointer_)
            : listPointer(listPointer_) {}

        ConstIterator begin() { return listPointer->cbegin(); }
        ConstIterator end()   { return listPointer->cend();   }
    };

    // Mutating range — unique lock, exclusive access
    class UniqueRange {
        ConcurrentLinkedList<K, V>* listPointer;
    public:
        UniqueRange(ConcurrentLinkedList<K, V>* listPointer_)
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
        sentinel = new LinkedListNode();
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
    bool Add(const K& key, const V& value) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        LinkedListNode* prev = this->sentinel;
        LinkedListNode* ptr = prev->next;

        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            prev_lock = std::move(curr_lock);

            // Updating an existing key
            if (ptr->value.first == key) {
                ptr->value = {key, value};
                return true;
            }

            prev = ptr;
            ptr = ptr->next;
        }

        prev->next = new LinkedListNode(nullptr, {key, value});
        this->size++;
        return true;
    }

    // Value is Contained in the List.
    bool Contains(const K& key) const {
        std::shared_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        LinkedListNode* prev = this->sentinel;
        LinkedListNode* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::shared_lock<std::shared_mutex> curr_lock(ptr->mtx);
        
            if (ptr->value.first == key) {
                return true;
            }

            // Release previous
            prev_lock = std::move(curr_lock);

            prev = ptr;
            ptr = ptr->next;
        }
        return false;
    }

    // Value stored at the Key.
    std::optional<V> Get(const K& key) const {
        std::shared_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        LinkedListNode* prev = this->sentinel;
        LinkedListNode* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::shared_lock<std::shared_mutex> curr_lock(ptr->mtx);
        
            if (ptr->value.first == key) {
                return ptr->value.second;
            }

            // Release previous
            prev_lock = std::move(curr_lock);

            prev = ptr;
            ptr = ptr->next;
        }
        return std::nullopt;
    }

    // Remove Value at index
    // Returns value removed
    std::optional<V> Remove(const K& key) {
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        LinkedListNode* prev = this->sentinel;
        LinkedListNode* ptr = prev->next;
        while (ptr) {
            // Grab the next node's lock
            std::unique_lock<std::shared_mutex> curr_lock(ptr->mtx);
            if (ptr->value.first == key) {
                // Delete this node
                auto return_value = ptr->value.second;
                prev->next = ptr->next;
                this->size--;
                curr_lock.unlock();
                delete(ptr);
                return return_value;
            } else {
                prev_lock = std::move(curr_lock);

                ptr = ptr->next;
                prev = prev->next;
            }
        }
        return std::nullopt;
    }

    // Clear out the list so that it is
    // empty.
    void Clear() {
        if (!sentinel) return;
        std::unique_lock<std::shared_mutex> prev_lock(this->sentinel->mtx);
        LinkedListNode* ptr = this->sentinel->next;
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
    LinkedListNode* sentinel;
    std::atomic<size_t> size;
};
