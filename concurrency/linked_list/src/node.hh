#pragma once
#include <memory>

class Node {

public:

    // Constructor
    Node() = default;

    // All Args Constructor
    Node(Node* node, int value)
        : 
        next(node),
        value(value)
    {
    }

    Node* next;
    int value;


};