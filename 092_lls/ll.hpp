#ifndef __LL_HPP__
#define __LL_HPP__

#include <assert.h>
#include <cstdlib>
#include <exception>
#include <cstdio>

class LLIndexOutOfBound : public std::exception {
  public:
    virtual const char *what() const throw() {
        return "LLIndexOutOfBound";
    }
    virtual ~LLIndexOutOfBound() throw() {}
};

template<typename T>
class LinkedList {
  private:
    class ListNode {
      public: 
        T val;
        ListNode * next;
        ListNode * prev;
        // Use initializer list for constructors
        ListNode() : val(), next(NULL), prev(NULL) {}
        ListNode(const T & v, ListNode * n, ListNode * p) : val(v), next(n), prev(p) {}
    };
    ListNode * head;
    ListNode * tail;
    int size;

    // Helper function to deep copy a list
    void copyList(const LinkedList & rhs) {
        ListNode * node = rhs.head;
        while (node != NULL) {
            addBack(node->val);
            node = node->next;
        }
    }

  public:
    LinkedList() : head(NULL), tail(NULL), size(0) {}

    LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) {
        copyList(rhs);
    }

    LinkedList & operator=(const LinkedList & rhs) {
        if (this != &rhs) {
            LinkedList<T> temp(rhs);
            swap(temp);
        }
        return *this;
    }

    void swap(LinkedList & other) {
        ListNode * tempHead = head;
        head = other.head;
        other.head = tempHead;

        ListNode * tempTail = tail;
        tail = other.tail;
        other.tail = tempTail;

        int tempSize = size;
        size = other.size;
        other.size = tempSize;
    }

    ~LinkedList() {
        while (head != NULL) {
            ListNode * temp = head->next;
            delete head;
            head = temp;
        }
    }

    void addFront(const T & val) {
        ListNode * newNode = new ListNode(val, head, NULL);
        if (head != NULL) {
            head->prev = newNode;
        }
        head = newNode;
        if (tail == NULL) {
            tail = head;
        }
        size++;
    }

    void addBack(const T & val) {
        ListNode * newNode = new ListNode(val, NULL, tail);
        if (tail != NULL) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == NULL) {
            head = tail;
        }
        size++;
    }

    bool remove(const T & val) {
        ListNode * cur = head;
        while (cur != NULL) {
            if (cur->val == val) {
                if (cur->prev != NULL) {
                    cur->prev->next = cur->next;
                } else {
                    head = cur->next;
                }
                if (cur->next != NULL) {
                    cur->next->prev = cur->prev;
                } else {
                    tail = cur->prev;
                }
                delete cur;
                size--;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    T & operator[](int index) {
        if (index < 0 || index >= size) {
            throw LLIndexOutOfBound();
        }
        ListNode * cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->val;
    }

    const T & operator[](int index) const {
        if (index < 0 || index >= size) {
            throw LLIndexOutOfBound();
        }
        ListNode * cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->val;
    }

    int find(const T & val) const {
        ListNode * cur = head;
        for (int index = 0; cur != NULL; cur = cur->next, ++index) {
            if (cur->val == val) {
                return index;
            }
        }
        return -1;
    }

    int getSize() const {
        return size;
    }
};

#endif
