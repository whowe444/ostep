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
        control_block->ref_count = 0;
    }

    // Destructor
    ~SharedPtr() {
        // Delete raw_ptr if reference count hits zero
        if (this->control_block && this->control_block->ref_count == 0) {
            delete this->raw_ptr;
            delete this->control_block;
        } else if (this->control_block) {
            --this->control_block->ref_count;
        }
    }

    // Non-Const Dereference Operator
    T& operator*() {
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
    }

    // Copy Ctor
    SharedPtr(const SharedPtr& other) noexcept {
        this->raw_ptr = other.raw_ptr;
        this->control_block = other.control_block;
        ++this->control_block->ref_count;
    }

    bool operator==(const SharedPtr& other) const {
        return this->raw_ptr == other.raw_ptr;
    }

    bool operator!=(const SharedPtr& other) const {
        return !(*this == other);
    }

private:
    T* raw_ptr = nullptr;
    ControlBlock* control_block = nullptr;
};