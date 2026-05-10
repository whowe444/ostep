#pragma once

#include <cassert>
#include <atomic>

struct ControlBlock {
    std::atomic<int> ref_count;
    ControlBlock(int ref_count) 
        :
            ref_count(ref_count)
    {
    }
};

template<typename T>
class SharedPtr {
public:
    // Constructor
    SharedPtr() = default;

    // All Args Constructor
    SharedPtr(T* raw_ptr)
        :
            raw_ptr(raw_ptr),
            control_block(new ControlBlock(1))
    {
    }

    // Destructor
    ~SharedPtr() {
        // Delete raw_ptr if reference count hits zero
        if (this->control_block && --this->control_block->ref_count == 0) {
            delete this->raw_ptr;
            delete this->control_block;
        }
    }

    // Non-Const Dereference Operator
    T& operator*() {
        assert(this->raw_ptr && "Dereferencing null SharedPtr!");
        return *this->raw_ptr;
    }

    // Const Dereference Operator
    const T& operator*() const {
        assert(this->raw_ptr && "Dereferencing null SharedPtr!");
        return *this->raw_ptr;
    }

    // Copy Assignment
    SharedPtr& operator=(const SharedPtr& other) {
        // Prevent self-assignment
        if (this != &other) {
            // If the object we are assigning to exists and is the last reference
            // then clean it up.
            if (this->control_block && --this->control_block->ref_count == 0) {
                delete this->raw_ptr;
                delete this->control_block;
            }

            this->raw_ptr = other.raw_ptr;
            this->control_block = other.control_block;
            // If the other object wasn't a nullptr, increment the ref count.
            if (control_block) ++this->control_block->ref_count;
        }
        return *this;
    }

    // Copy Ctor
    SharedPtr(const SharedPtr& other) {
        this->raw_ptr = other.raw_ptr;
        this->control_block = other.control_block;
        // If the other object wasn't a nullptr, increment the ref count.
        if (control_block) ++this->control_block->ref_count;
    }

    // Move Assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {

            // Release current SharedPtr's resources
            if (this->control_block && --this->control_block->ref_count == 0) {
                delete this->raw_ptr;
                delete this->control_block;
            }

            this->raw_ptr = other.raw_ptr;
            other.raw_ptr = nullptr;
            this->control_block = other.control_block;
            other.control_block = nullptr;
        }
        // Don't modify the reference count since we are going from
        // two things down to one, and we already decremented above.
        return *this;
    }

    // Move Constructor
    SharedPtr(SharedPtr&& other) noexcept 
        :
            raw_ptr(other.raw_ptr),
            control_block(other.control_block)
    {
        other.raw_ptr = nullptr;
        other.control_block = nullptr;
    }

    bool operator==(const SharedPtr& other) const {
        return this->raw_ptr == other.raw_ptr;
    }

    bool operator!=(const SharedPtr& other) const {
        return !(*this == other);
    }

    explicit operator bool() const {
        return this->raw_ptr != nullptr;
    }

    // Arrow Operator
    T* operator->() {
        return this->raw_ptr;
    }

    // Const Arrow Operator
    const T* operator->() const {
        return this->raw_ptr;
    }

    T* get() const {
        return this->raw_ptr;
    }

    T* release() {
        auto temp = this->raw_ptr;

        if (this->control_block && --this->control_block->ref_count == 0) {
            delete this->control_block;
        }

        // Null out this object
        this->control_block = nullptr;
        this->raw_ptr = nullptr;

        return temp;
    }

    void reset(T* new_ptr = nullptr) {
        if (this->raw_ptr != new_ptr) {
            if (this->control_block && --this->control_block->ref_count == 0) {
                delete this->raw_ptr;
                delete this->control_block;
            }
            this->raw_ptr = new_ptr;
            this->control_block = new_ptr ? new ControlBlock(1) : nullptr;
        }
    }

private:
    T* raw_ptr = nullptr;
    ControlBlock* control_block = nullptr;
};