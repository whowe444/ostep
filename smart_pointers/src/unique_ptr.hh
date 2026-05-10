#pragma once

#include <cassert>

template<typename T>
class UniquePtr {

public:

    // Constructor
    UniquePtr() = default;

    // All Args Constructor
    UniquePtr(T* raw_ptr)
        : 
        raw_ptr(raw_ptr)
    {
    }

    // Destructor
    ~UniquePtr() {
        delete this->raw_ptr;
    }

    // Move Assignment
    UniquePtr& operator=(UniquePtr<T>&& other) {
        // Prevent self assignment by comparing pointers.
        // You want to compare the ptrs for self-assignment
        // rather than the objects' eqs method.
        if (this != &other) {
            delete raw_ptr;
            this->raw_ptr = other.raw_ptr;
            other.raw_ptr = nullptr;
        }
        return *this;
    }

    // Move Constructor
    UniquePtr(UniquePtr&& other) noexcept
        :
            raw_ptr(other.raw_ptr)
    {
        // Prevent double-delete
        other.raw_ptr = nullptr;
    }

    // Equals Operator
    bool operator==(std::nullptr_t) const {
        return this->raw_ptr == nullptr;
    }

    bool operator!=(std::nullptr_t) const {
        return this->raw_ptr != nullptr;
    }

    explicit operator bool() const {
        return this->raw_ptr;
    }

    // Non-Const Dereference Operator
    T& operator*() {
        assert(this->raw_ptr && "Dereferencing null UniquePtr!");
        return *this->raw_ptr;
    }

    // Const Dereference Operator
    const T& operator*() const {
        assert(this->raw_ptr && "Dereferencing null UniquePtr!");
        return *this->raw_ptr;
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
        this->raw_ptr = nullptr;
        return temp;
    }

    void reset(T* new_ptr = nullptr) {
        if (new_ptr != this->raw_ptr) {
            delete this->raw_ptr;
            this->raw_ptr = new_ptr;
        }
    }

    // Delete the Copy Ctor
    UniquePtr(const UniquePtr& other) = delete;

    // Delete the Copy Assignment
    UniquePtr& operator=(const UniquePtr& other) = delete;

private:
    T* raw_ptr= nullptr;
};