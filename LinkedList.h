#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>
#include <functional>
#include <iostream>
#include <vector>
#include "Node.h"

class LinkedList
{
public:
    // constructors and destructors
    LinkedList();
    ~LinkedList();

    // maximum value of unsigned int
    static const unsigned int invalidPos;

    // more functions to be added perhaps...

    /**
     * @brief Get the number of elements in the Linked List
     * @return Number of elements in the Linked List
    */
    unsigned int size() const;

    /**
     * @brief Get whether the Linked List is empty
     * @return Whether the list is empty
    */
    bool empty() const;

    /**
     * @brief Get a modify stock reference at an index
     * @param i The index of the stock to be retrieved
     * @return Editable stock reference
    */
    Stock& at(unsigned int i) const;

    /**
     * @brief Create a copy of the data on the heap and append it to the Linked List
     * @param data The data to append
    */
    void append(const Stock& data);

    /**
     * @brief Create a copy of the data on the heap and prepend it to the Linked List
     * @param data The data to prepend
    */
    void prepend(const Stock& data);

    /**
     * @brief Remove the front item in the Linked List and free it from the heap
     * @return Copy of the data that was removed
    */
    Stock removeFront();

    /**
     * @brief Remove the back item in the Linked List and free it from the heap
     * @return Copy of the data that was removed
    */
    Stock removeBack();

    /**
     * @brief Insert a copy of the data on the heap before a specified index in the Linked List
     * @param index The index to insert the data before
     * @param data The data to insert
    */
    void insertBefore(unsigned int index, const Stock& data);

    /**
     * @brief Remove an item at a specificed index in the Linked List and free it from the heap
     * @return Copy of the data that was removed
    */
    Stock removeAt(unsigned int index);

    /**
     * @brief Remove all items in the Linked List and free them from the heap
    */
    void clear();

    /**
     * @brief Find the index of the first item that satisfies a criteria
     * @param predicate The critera that must be meet
     * @return index of the first item that satisfies the critera or LinkedList::invalidPos
    */
    unsigned int findFirst(const std::function<bool(const Stock&)>& predicate) const;

    /**
     * @brief Loop through each item in the Linked List, they are editable
     * @param action The action to be taken for each item
    */
    void forEach(const std::function<void(Stock&)>& action);

    /**
     * @brief Loop through each item in the Linked List, they are NOT editable
     * @param action The action to be taken for each item
    */
    void forEach(const std::function<void(const Stock&)>& action) const;

    /**
     * @brief Transform the list of items into a list of another type
     * @param func The mapping function
    */
    template <typename T>
    std::vector<T> getTransformedValues(const std::function<T(const Stock&)>& func) const;

    // used with std::cout to print the contents out, just for debugging
    friend std::ostream& operator<<(std::ostream& os, const LinkedList& linkedList);

private:
    // the beginning of the list
    Node* head;
  
    // how many nodes are there in the list?
    unsigned count;

    // get the node at an index
    Node* getNode(unsigned int index) const;

    // get the tail: O(n) because we aren't storing the tail
    Node* getTail() const;

    // I don't know why this is here? to be able to print the list using std::cout?
    Node* getHead() const;
};

#endif  // LINKEDLIST_H


