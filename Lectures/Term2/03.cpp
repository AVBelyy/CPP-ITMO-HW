#include <iostream>

using namespace std;

void f1() {
    cout << "I can do this! Can you?" << endl;
}

void f2() {
    cout << "Another one" << endl;
}

int main() {
    int a = 0, b = 0;
    int *c, d;
    void (*g[10])();
    g[0] = f1;
    g[1] = f2;
    f();
    (g[1])();
    int **(***(***A[1][2][3])[4][5][6])[7][8];
    ++a = ++b;
    ++++a;
    //int t = *c;
    //*(c + d) = 3;
    //a++ = b; // invalid
    (++a)++;
    int F = 0x5f3759df;
    float f = *(float*)&F;
    cout << a << " " << b << " " << sizeof(A) << " " << f << endl;
}
