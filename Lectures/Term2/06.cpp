static int a = 5;

// if it is placed in header,
// this will multiply a's and f's
// across the file importing this header
// we shall not do this; this is bad!

// #ifndef A_H
// #define A_H
// ...
// #endif
// -- include guard

inline void f() {
    a = 6;
}

int main() {
}
