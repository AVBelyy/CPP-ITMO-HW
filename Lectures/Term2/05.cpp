#include <stdio.h>

int d = 0;

void spacify() {
    for (int i = 0; i < 4 * d; i++) {
        putchar(' ');
    }
}

struct mytype {
    int a;
    int b;

    mytype() {
        spacify();
        printf("Entering\n");
        a = 0;
        d++;
    }

    mytype(int a, int b) {
        spacify();
        printf("Entering with %d %d\n", a, b);
        this->a = a;
        this->b = b;
        d++;
    }
    ~mytype() {
        d--;
        spacify();
        if (a) {
            printf("Leaving from %d %d\n", a, b);
        } else {
            printf("Leaving\n");
        }
    }
};

struct mytype2 {
    mytype a;
    mytype b;
    mytype2() {
        spacify();
        printf("Hello!\n");
    }
};

class A {
    int x, y;
    A(int a, int b) : x(a), y(b) {}
};

class BigInteger {
    int *t;
    BigInteger(size_t sz) {
        t = new int[sz];
    }
    ~BigInteger();

    BigInteger(BigInteger const& other); // copy constructor

    BigInteger operator+(BigInteger const&) const;
    //BigInteger operator+(BigInteger const&, BigInteger const&)
    //-- exactly the same, but must be used outside
    BigInteger& operator+=(BigInteger const&);
    BigInteger& operator-();
    BigInteger& operator++();
    BigInteger operator++(int) const;

    int &operator[](size_t i) {
        return t[i];
    }
};

// ok int * a;
// ok int * const b;
// 1  int const * c;
// 2  int const * const d;
// 1  const int * e;
// 2  const int * const f;
// x  const int const * g;
// x  const int const * const h;
//
// ok int & a;
// x  int & const b;
// ok int const & c;
// x  int const & const d;

void g(mytype, mytype) {
    spacify();
    printf("Hello from g!\n");
    spacify();
    printf("Bye from g!\n");
}

void f() {
    mytype a;
    {
        mytype a;
        {
            mytype(1, 2);
            g(mytype(3, 4), mytype(5, 6));
        }
    }
}

int main() {
    f();
    mytype2 hello;

    int a1, a2;
    
    int *b1 = &a1;
    int &b2 = a2;
}
