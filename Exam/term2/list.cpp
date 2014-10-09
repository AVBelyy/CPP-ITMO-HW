#include <iostream>

#include "list.h"

using namespace std;

// list

list::list() {
    root = new list::node;
    root->value = 0xdeadbeef;
    root->prev = root->next = root;
}

list::list(list const& other) {
    root = new list::node;
    root->value = 0xdeadbeef;
    root->prev = root->next = root;
    for (list::const_iterator it = other.begin(); it != other.end(); ++it) {
        push_back(*it);
    }
}

list::~list() {
    list::node *tmp, *cur = root->next;
    while (cur != root) {
        tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    delete root;
}

list& list::operator=(list other) {
    std::swap(other.root, root);
    return *this;
}

bool list::empty() const {
    return root->next == root;
}

void list::push_back(int value) {
    insert(iterator(root), value);
}

int& list::back() {
    return root->prev->value;
}

int const& list::back() const {
    return root->prev->value;
}

void list::pop_back() {
    erase(iterator(root->prev));
}

void list::push_front(int value) {
    insert(iterator(root->next), value);
}

int& list::front() {
    return root->next->value;
}

int const& list::front() const {
    return root->next->value;
}

void list::pop_front() {
    erase(iterator(root->next));
}

void list::insert(iterator pos, int value) {
    list::node* pos_node = pos.ptr;
    list::node* new_node = new list::node;
    new_node->value = value;
    pos_node->prev->next = new_node;
    new_node->prev = pos_node->prev;
    new_node->next = pos_node;
    pos_node->prev = new_node;
}

void list::erase(iterator pos) {
    list::node* pos_node = pos.ptr;
    pos_node->prev->next = pos_node->next;
    pos_node->next->prev = pos_node->prev;
    delete pos_node;
}

void list::splice(iterator pos, list& other, iterator first, iterator last) {
    list::node *pos_node = pos.ptr;
    list::node *first_node = first.ptr, *last_node = last.ptr;
    list::node *tmp = first_node->prev;
    pos_node->prev->next = first_node;
    first_node->prev = pos_node->prev;
    last_node->prev->next = pos_node;
    pos_node->prev = last_node->prev;
    tmp->next = last_node;
    last_node->prev = tmp;
}

list::iterator list::begin() {
    return iterator(root->next);
}

list::iterator list::end() {
    return iterator(root);
}

list::const_iterator list::begin() const {
    return const_iterator(root->next);
}

list::const_iterator list::end() const {
    return const_iterator(root);
}

// list::iterator

list::iterator::iterator(node* ptr) : ptr(ptr) {}

int& list::iterator::operator*() const {
    return ptr->value;
}

list::iterator& list::iterator::operator++() {
    ptr = ptr->next;
    return *this;
}

list::iterator list::iterator::operator++(int) {
    list::node* res = ptr;
    ptr = ptr->next;
    return iterator(res);
}

list::iterator& list::iterator::operator--() {
    ptr = ptr->prev;
    return *this;
}

list::iterator list::iterator::operator--(int) {
    list::node* res = ptr;
    ptr = ptr->prev;
    return iterator(res);
}

bool operator==(list::iterator const& a, list::iterator const& b) {
    return a.ptr == b.ptr;
}

bool operator!=(list::iterator const& a, list::iterator const& b) {
    return a.ptr != b.ptr;
}

// list::const_iterator

list::const_iterator::const_iterator(node* ptr) : ptr(ptr) {}

int const& list::const_iterator::operator*() const {
    return ptr->value;
}

list::const_iterator& list::const_iterator::operator++() {
    ptr = ptr->next;
    return *this;
}

list::const_iterator list::const_iterator::operator++(int) {
    list::node* res = ptr;
    ptr = ptr->next;
    return const_iterator(res);
}

list::const_iterator& list::const_iterator::operator--() {
    ptr = ptr->prev;
    return *this;
}

list::const_iterator list::const_iterator::operator--(int) {
    list::node* res = ptr;
    ptr = ptr->prev;
    return const_iterator(res);
}

bool operator==(list::const_iterator const& a, list::const_iterator const& b) {
    return a.ptr == b.ptr;
}

bool operator!=(list::const_iterator const& a, list::const_iterator const& b) {
    return a.ptr != b.ptr;
}

void test1() {
    list a;
    for (int i = 0; i < 10; i++) {
        a.push_back(i);
    }
    for (list::iterator it = a.begin(); it != a.end(); ++it) {
        cout << *it << endl;
    }
}

void test2() {
    list a;
    list::iterator it = a.end();
    a.push_back(5);
    --it;
    cout << (*it == 5) << endl;
}

void test3() {
    list a;
    for (int i = 0; i < 5; i++) {
        a.push_back(2 * i);
        a.push_front(2 * i + 1);
    }
    while (!a.empty()) {
        cout << a.front() << endl;
        a.pop_front();
    }
}

void test4() {
    list a, b;
    for (int i = 0; i < 10; i++) {
        a.push_back(i);
    }
    list::iterator first = a.begin(), last = a.end();
    ++++first;
    ----last;
    b.splice(b.begin(), a, first, last);
    cout << "contents of a:" << endl;
    while (!a.empty()) {
        cout << a.front() << endl;
        a.pop_front();
    }
    cout << "contents of b:" << endl;
    for (list::iterator it = b.begin(); it != b.end(); ++it) {
        cout << *it << endl;
    }
    list c;
    c = b;
    c = c;
    cout << "contents of c == b:" << endl;
    while (!c.empty()) {
        cout << c.front() << endl;
        c.pop_front();
    }
}

int main() {
    //test1();
}
