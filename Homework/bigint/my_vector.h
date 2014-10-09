#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <algorithm>
#include <cstring>
#include <memory>

template <class T>
class my_vector {
    static const size_t SMALL_SIZE = 32;

    enum { SMALL, BIG } state;
    size_t left;
    size_t right;
    size_t data_size;
    my_shared_ptr<T> data;
    T small_data[SMALL_SIZE];

    public:
    my_vector<T>();
    my_vector(my_vector<T> const& other);
    ~my_vector<T>();
    void resize(size_t desired_size);
    my_vector<T>& operator=(my_vector<T> const& other);
    inline void cow_fork();
    T& operator[](int i);
    T operator[](int i) const;
    T& front();
    void pop_front(int count);
    void push_front(int count, T x);
    T& back();
    void push_back(T x);
    size_t size() const;
};

template <class T>
my_vector<T>::my_vector() {
    state = SMALL;
    data.set_const(small_data);
    data_size = SMALL_SIZE;
    memset(data.get(), 0, data_size * sizeof(T));
    left = 0;
    right = 0;
}

template <class T>
my_vector<T>::my_vector(my_vector<T> const& other) {
    *this = other;
}

template <class T>
my_vector<T>::~my_vector() {
    data.reset();
}

template <class T>
inline void my_vector<T>::cow_fork() {
    if (data.use_count() > 1) {
        T* forked_data = new T[data_size];
        memcpy(forked_data, data.get(), data_size * sizeof(T));
        data.reset();
        data.set(forked_data);
    }
}

template <class T>
my_vector<T>& my_vector<T>::operator=(my_vector<T> const& other) {
    if (this != &other) {
        left = other.left;
        right = other.right;
        data_size = other.data_size;
        data.reset();
        if (other.state == BIG) {
            data = other.data;
        } else {
            memcpy(small_data, other.small_data, data_size * sizeof(T));
            data.set_const(small_data);
        }
        state = other.state;
    }
    return *this;
}

template <class T>
void my_vector<T>::resize(size_t desired_size) {
    size_t new_size = 0, cur_size = size();
    if (state == SMALL && left + desired_size > SMALL_SIZE) {
        data_size = desired_size << 1;
        data.reset();
        data.set(new T[data_size]);
        memcpy(data.get(), &small_data[left], cur_size * sizeof(T));
        memset(&data[cur_size], 0, (data_size - cur_size) * sizeof(T));
        left = 0;
        right = desired_size;
        state = BIG;
        return;
    } else if (state == BIG && left + desired_size > data_size) {
        new_size = desired_size + (desired_size >> 2) + 1;
        right = cur_size;
    } else if (state == BIG && desired_size <= SMALL_SIZE) {
        data_size = SMALL_SIZE;
        size_t new_size = std::min(cur_size, desired_size);
        memcpy(small_data, &data[left], new_size * sizeof(T));
        memset(&small_data[new_size], 0, (SMALL_SIZE - new_size) * sizeof(T));
        data.reset();
        data.set_const(small_data);
        left = 0;
        right = desired_size;
        state = SMALL;
        return;
    } else if (state == BIG && desired_size << 2 <= data_size) {
        new_size = data_size >> 1;
        right = std::min(cur_size, new_size);
    } else {
        right = left + desired_size;
        return;
    }
    T* new_data = new T[new_size];
    memcpy(new_data, &data[left], right * sizeof(T));
    memset(&new_data[right], 0, (new_size - right) * sizeof(T));
    left = 0;
    right = desired_size;
    data.reset();
    data.set(new_data);
    data_size = new_size;
}

template <class T>
T& my_vector<T>::operator[](int i) {
    if (!(0 <= i && i < (int)size())) {
        //failSomehow();
    }
    cow_fork();
    return data[left + i];
}

template <class T>
T my_vector<T>::operator[](int i) const {
    if (!(0 <= i && i < (int)size())) {
        //failSomehow();
    }
    return data[left + i];
}

template <class T>
T& my_vector<T>::front() {
    return data[left];
}

template <class T>
void my_vector<T>::pop_front(int count) {
    cow_fork();
    left = std::min(left + count, right);
    resize(size());
}

template <class T>
void my_vector<T>::push_front(int count, T x) {
    size_t old_right = right;
    cow_fork();
    resize(size() + count);
    for (int i = old_right - 1; i >= (int)left; i--) {
        data[i + count] = data[i];
    }
    for (int i = 0; i < count; i++) {
        data[i + left] = x;
    }
}

template <class T>
T& my_vector<T>::back() {
    return data[right - 1];
}

template <class T>
void my_vector<T>::push_back(T x) {
    resize(size() + 1);
    back() = x;
}

template <class T>
size_t my_vector<T>::size() const {
    return right - left;
}

#endif // MY_VECTOR_H
