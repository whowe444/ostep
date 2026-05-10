#pragma once

#include <cassert>

struct ControlBlock {
    int ref_count;
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
            control_block(new ControlBlock())
    {
        // Start the reference count at 1.
        this->control_block->ref_count = 1;
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
            this->raw_ptr = other.raw_ptr;
            this->control_block = other.control_block;
            ++this->control_block->ref_count;
        }
        return *this;
    }

    // Copy Ctor
    SharedPtr(const SharedPtr& other) noexcept {
        this->raw_ptr = other.raw_ptr;
        this->control_block = other.control_block;
        ++this->control_block->ref_count;
    }

    // Move Assignment
    SharedPtr& operator=(SharedPtr&& other) {
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

private:
    T* raw_ptr = nullptr;
    ControlBlock* control_block = nullptr;
};