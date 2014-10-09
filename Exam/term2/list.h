#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdint.h>

struct list {
    struct node;
    struct iterator;
    struct const_iterator;

    list();
    list(list const& other);
    ~list();
    
    list& operator=(list other);
    bool empty() const;
    void push_back(int value);
    int& back();
    int const& back() const;
    void pop_back();
    void push_front(int value);
    int& front();
    int const& front() const;
    void pop_front();
    void insert(iterator pos, int value);
    void erase(iterator pos);
    void splice(iterator pos, list& other, iterator first, iterator last);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    private:
    node *root;
};

struct list::node {
    list::node* prev;
    list::node* next;
    int value;
};

struct list::iterator {
    friend class list;

    iterator(node* ptr);

    int& operator*() const;
    iterator& operator++();
    iterator  operator++(int);
    iterator& operator--();
    iterator  operator--(int);

    friend bool operator==(list::iterator const& a, list::iterator const& b);
    friend bool operator!=(list::iterator const& a, list::iterator const& b);

    private:
    node* ptr;
};

bool operator==(list::iterator const& a, list::iterator const& b);
bool operator!=(list::iterator const& a, list::iterator const& b);

struct list::const_iterator {
    friend class list;

    const_iterator(node* ptr);

    int const& operator*() const;
    const_iterator& operator++();
    const_iterator  operator++(int);
    const_iterator& operator--();
    const_iterator  operator--(int);

    friend bool operator==(list::const_iterator const& a, list::const_iterator const& b);
    friend bool operator!=(list::const_iterator const& a, list::const_iterator const& b);

    private:
    node* ptr;
};

bool operator==(list::const_iterator const& a, list::const_iterator const& b);
bool operator!=(list::const_iterator const& a, list::const_iterator const& b);

#endif
