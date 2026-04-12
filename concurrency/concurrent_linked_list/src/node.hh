#pragma once

#include <memory>
#include <shared_mutex>

template<typename T>
class Node {

public:

    // Constructor
    Node() = default;

    // All Args Constructor
    Node(Node<T>* node, int value)
        : 
        next(node),
        value(value)
    {
    }

    Node<T>* next;
    int value;
    std::shared_mutex mtx;


};