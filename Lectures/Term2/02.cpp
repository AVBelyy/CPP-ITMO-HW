#include <iostream>

// Variable size
//
// ABI          MS        GNU    DOS
//           | 32  64 | 32  64 | 16
// short     | 2   2  | 2    2 | 2
// int       | 4   4  | 4    4 | 2
// long      | 4   4  | 4    8 | 4
// long long | 8   8  | 8    8 | 4
//
// int, long - different types
// char, signed char, unsigned char - different types
//
// sizeof
// single 4 (C float)
// double 8 (C double)
// extended 10 (C double)

void f() {
    // sub esp, 4
    // mov [esp], 5
    //
    // call g
    //
    // mov eax, [esp]
    // add eax, eax
    // mov [esp], eax
    //
    // add esp, 4
    // ret
    int b = 5;
    g();
    b *= ;
}

int main() {
}
