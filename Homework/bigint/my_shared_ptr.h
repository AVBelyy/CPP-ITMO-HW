#ifndef MY_SHARED_PTR_H
#define MY_SHARED_PTR_H

template <class T>
class my_shared_ptr;

template <class T>
class my_shared_frame {
    friend class my_shared_ptr<T>;
    
    T* raw;
    bool cnst;
    size_t count;

    public:
    my_shared_frame(T* raw, bool cnst);

    bool leave();
    size_t use_count();
};

template <class T>
class my_shared_ptr {
    my_shared_frame<T>* frame;

    public:
    my_shared_ptr();
    my_shared_ptr(T* raw);

    void set(T* other);
    void set_const(T* other);
    void reset();
    T* get() const;
    my_shared_ptr<T>& operator=(my_shared_ptr<T> const& other);
    T& operator[](int i);
    T operator[](int i) const;
    size_t use_count();
};

template <class T>
my_shared_frame<T>::my_shared_frame(T* raw, bool cnst) : raw(raw), cnst(cnst), count(1) {}

template <class T>
bool my_shared_frame<T>::leave() {
    --count;
    if (count == 0) {
        if (!cnst) {
            delete[] raw;
        }
        return true;
    } else {
        return false;
    }
}

template <class T>
size_t my_shared_frame<T>::use_count() {
    return count;
}

template <class T>
my_shared_ptr<T>::my_shared_ptr() {
    frame = NULL;
}

template <class T>
my_shared_ptr<T>::my_shared_ptr(T* raw) {
    set(raw);
}

template <class T>
void my_shared_ptr<T>::reset() {
    if (frame && frame->leave()) {
        delete frame;
    } else {
        frame = NULL;
    }
}

template <class T>
void my_shared_ptr<T>::set(T* other) {
    frame = new my_shared_frame<T>(other, false);
}

template <class T>
void my_shared_ptr<T>::set_const(T* other) {
    frame = new my_shared_frame<T>(other, true);
}

template <class T>
T* my_shared_ptr<T>::get() const {
    return frame->raw;
}

template <class T>
my_shared_ptr<T>& my_shared_ptr<T>::operator=(my_shared_ptr<T> const& other) {
    frame = other.frame;
    ++frame->count;
    return *this;
}

template <class T>
T& my_shared_ptr<T>::operator[](int i) {
    return frame->raw[i];
}

template <class T>
T my_shared_ptr<T>::operator[](int i) const {
    return frame->raw[i];
}

template <class T>
size_t my_shared_ptr<T>::use_count() {
    return frame->count;
}

#endif // MY_SHARED_PTR_H
