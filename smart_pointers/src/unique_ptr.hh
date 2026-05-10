#pragma once

template<typename T>
class UniquePtr {

public:

    // Constructor
    UniquePtr() = delete;

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
    {
        // Reassign 
        this->raw_ptr = other.raw_ptr;

        // Prevent double-delete
        other.raw_ptr = nullptr;
    }

    // Equals Operator
    bool operator==(const UniquePtr<T>& other) const {
        return other.raw_ptr == this->raw_ptr;
    }

    bool operator!=(const UniquePtr<T>& other) const {
        return !(*this == other);
    }

    explicit operator bool() const {
        return this->raw_ptr;
    }

    // Dereference Operator
    T operator*() {
        return *this->raw_ptr;
    }

    // Delete the Copy Ctor
    UniquePtr(const UniquePtr& other) = delete;

    // Delete the Copy Assignment
    UniquePtr& operator=(const UniquePtr& other) = delete;

private:
    T* raw_ptr;
};