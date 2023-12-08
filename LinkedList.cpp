#include "LinkedList.h"

// static cast -1 to get the maximum value of unsigned int
const unsigned int LinkedList::invalidPos = static_cast<unsigned int>(-1);

LinkedList::LinkedList() {
   head = nullptr;
   count = 0;
}

LinkedList::~LinkedList() {
    clear();
}

Node* LinkedList::getNode(unsigned int i) const
{  
    if (i >= count)
    {
        //make sure we don't go out of range
        throw std::runtime_error("Cannot get node at that index");
    }

    Node* curNode = head;
    for (unsigned int j = 0; j < i; ++j)
    {
        curNode = curNode->getNext();
    }
    return curNode;
}

Node* LinkedList::getTail() const
{
    if (count == 0)
    {
        //make sure we don't go out of range
        throw std::runtime_error("Cannot get the tail of empty LinkedList");
    }
    else
    {
        return getNode(count - 1);
    }
}

unsigned int LinkedList::size() const
{
    return count;
}

bool LinkedList::empty() const
{
    return count == 0;
}

Node* LinkedList::getHead() const
{
    return head;
}

Stock& LinkedList::at(unsigned int i) const
{
    return getNode(i)->getData();
}

void LinkedList::append(const Stock& data)
{
    Node* newNode = new Node(data, nullptr);

    if (count == 0)
    {
        //if we don't have elements, set the head
        head = newNode;
    }
    else
    {
        //if we have elements, put it at the end
        Node* tail = getTail();
        tail->setNext(newNode);
    }

    ++count;
}

void LinkedList::prepend(const Stock& data)
{
    Node* newNode = new Node(data, nullptr);

    if (count == 0)
    {
        //if we don't have elements, set the head
        head = newNode;
    }
    else
    {
        //if we have elements, put it before the head
        newNode->setNext(head);
        head = newNode;
    }

    ++count;
}

Stock LinkedList::removeBack()
{
    Stock result;

    if (count == 0)
    {
        //make sure we have elements to remove
        throw std::runtime_error("Cannot remove back element of empty LinkedList");
    }
    else if (count == 1)
    {
        //if there is only one element, remove the head and set it to nullptr
        result = Stock(head->getData());
        Node* prevHead = head;
        head = nullptr;
        delete prevHead;
    }
    else
    {
        //if we have more than one element, remove the back element (no need to worry about the head node)
        Node* beforeTail = getNode(count - 2);
        Node* tail = beforeTail->getNext();
        result = Stock(tail->getData());
        beforeTail->setNext(nullptr);
        delete tail;
    }

    --count;
    return result;
}

Stock LinkedList::removeFront()
{
    Stock result;

    if (count == 0)
    {
        //make sure we have elements to remove
        throw std::runtime_error("Cannot remove front element of empty LinkedList");
    }
    else
    {
        //if there is an element, just remove the head
        result = Stock(head->getData());
        Node* prevHead = head;
        head = prevHead->getNext();
        delete prevHead;
    }

    --count;
    return result;
}

void LinkedList::insertBefore(unsigned int index, const Stock& data)
{
    if (index > count)
    {
        //check if we are in bounds
        //count is included if we want to append
        throw std::runtime_error("Cannot insert before that index for LinkedList");
    }
    else if (index == 0)
    {
        //inserting before index 0 means prepend
        prepend(data);
    }
    else if (index == count)
    {
        //insert before index count means append
        append(data);
    }
    else
    {
        //we now know we are actually inserting between two nodes
        Node* insertAfterNode = getNode(index - 1);
        Node* insertBeforeNode = insertAfterNode->getNext();
        Node* newNode = new Node(data, nullptr);
        insertAfterNode->setNext(newNode);
        newNode->setNext(insertBeforeNode);
        ++count;
    }
}

Stock LinkedList::removeAt(unsigned int index)
{
    Stock result;

    if (index >= count)
    {
        //check if we're in bounds
        throw std::runtime_error("Cannot remove at index for LinkedList");
    }
    else if (index == 0)
    {
        //remove at index 0 means remove front
        result = removeFront();
    }
    else if (index == count - 1)
    {
        //remove at index count - 1 means remove back
        result = removeBack();
    }
    else
    {
        //we now know we are not removing from one of the ends
        Node* prevNode = getNode(index - 1);
        Node* removeNode = prevNode->getNext();
        result = Stock(removeNode->getData());
        Node* nextNode = removeNode->getNext();
        prevNode->setNext(nextNode);
        delete removeNode;
        --count;
    }

    return result;
}

void LinkedList::clear()
{
    //loop throug the linked list and release them from the heap
    Node* curNode = head;
    for (unsigned int i = 0; i < count; ++i)
    {
        Node* delNode = curNode;
        curNode = curNode->getNext();
        delete delNode;
    }
    head = nullptr;
    count = 0;
}

std::ostream& operator<<(std::ostream& os, const LinkedList& linkedList)
{
    //used for debugging
    //just prints out the list contents using std::cout
    if (linkedList.size() == 0)
    {
        std::cout << "EMPTY" << std::endl;
    }
    else
    {
        Node* curNode = linkedList.getHead();
        for (unsigned int i = 0; i < linkedList.size(); ++i)
        {
            std::cout << curNode->getData() << std::endl;
            curNode = curNode->getNext();
        }
    }

    return os;
}   

unsigned int LinkedList::findFirst(const std::function<bool(const Stock&)>& predicate) const
{
    //finds the first stock instance where the predicate returns true
    unsigned int foundIndex = LinkedList::invalidPos;

    Node* curNode = head;
    for (unsigned int i = 0; i < count && foundIndex == invalidPos; ++i)
    {
        if (predicate(curNode->getData()))
        {
            foundIndex = i;
        }
        curNode = curNode->getNext();
    }

    return foundIndex;
}

template <typename T>
std::vector<T> LinkedList::getTransformedValues(const std::function<T(const Stock&)>& func) const
{
    //transform the stock instances to another value and store those in a list
    std::vector<T> resultList;
    resultList.reserve(count);

    Node* curNode = head;
    for (unsigned int i = 0; i < count; ++i)
    {
        resultList.push_back(func(curNode->getData()));
        curNode = curNode->getNext();
    }

    return resultList;
}
//wouldn't work unless I do explicit template instantiation
template std::vector<int> LinkedList::getTransformedValues(const std::function<int(const Stock&)>& func) const;

void LinkedList::forEach(const std::function<void(Stock&)>& action)
{
    //Loop through each stock instances and apply an action
    //this is the forEach method where the instances are modifiable
    Node* curNode = head;
    for (unsigned int i = 0; i < count; ++i)
    {
        action(curNode->getData());
        curNode = curNode->getNext();
    }
}

void LinkedList::forEach(const std::function<void(const Stock&)>& action) const
{
    //I don't know how to properly use const_cast
    //so I just copied the code above
    Node* curNode = head;
    for (unsigned int i = 0; i < count; ++i)
    {
        action(curNode->getData());
        curNode = curNode->getNext();
    }
}