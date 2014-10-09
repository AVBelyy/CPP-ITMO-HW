#include <iostream>
#include <array>

template <typename T>
void f(T*) {}

template <>
void f(int*) {}

void foo() {
}

// const void(*fooPtr)() = foo; // non-compile time constant

template <typename T>
struct eeterator {
    public:
    eeterator(T* ptr) : ptr(ptr) {}

    T operator*() { return *ptr; }
    eeterator<T> & operator++() { ptr += sizeof(T); return *this; }
    
    
    T* ptr;
};

template <typename T>
bool operator==(const eeterator<T> & a, const eeterator<T> & b) {
    return a.ptr == b.ptr;
}

template <typename T>
bool operator!=(const eeterator<T> & a, const eeterator<T> & b) {
    return !(a.ptr == b.ptr);
}

template <typename T, int N>
struct array {
    typedef T value_type;
    typedef eeterator<T> iterator;

    array() {}
    array(int other[N]) { /* ololo */ }

    iterator begin() { return iterator(&data[0]); }
    iterator end() { return iterator(&data[N]); }

    private:
    T data[N];
};

int main() {
    //f(0); // error: cannot deduce T
    f((int*)0); // ok

    int nombres[] = {1,2,3,4,5,6,7,8,9,10};

    array<int, 10> massif(nombres);

    for(int nombre: massif) {
        std::cout << nombre << std::endl;
    }

    std::array<int, 5> a = {1,2,3,4,5};

    return 0;
}

