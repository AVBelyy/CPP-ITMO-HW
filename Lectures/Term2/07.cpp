// Inheritance
#include <iostream>
#include <string>

using namespace std;

struct base {
    int a;
    base() {}
    base(int);
    base(int, int) {}

    void f() { /* a */ }

    virtual void g() { /* a */ }
};

struct derived : base {
    int b;
    derived() : base(1, 2) {}

    void f() { /* b */ }

    /* still virtual */ void g() { }
};

// egg sample
struct logger {
    // pure virtual function
    virtual void log(string const& msg) = 0;
};

struct file_logger : logger {
    // can pass to logger &
    void log(string const*);
};

// another egg sample
struct game_object {
    virtual void explode();
};

struct TNT : game_object {
    void explode();
};

// keyword: RTTI

// Multi inheritance
struct base1 {
    int a;
    void f() {}
};
struct base2 {
    int a;
    void f();
};

struct multiderived : base1, base2 {};

// Virtual inheritance

struct A { int a; };
struct B : virtual A {};
struct C : virtual A {};
struct D : B, C {};

// Egg sample
struct Animal {
    virtual void move() = 0;
    virtual void speak() = 0;
};

struct SwimmingAnimal : virtual Animal {
    void move();
};

struct MuteAnimal : virtual Animal {
    void speak();
};

// There's only one sub-object, it would have been 2 without virtual
// There would also be an error, beacause each struct implements only one virtual function
struct Fish : SwimmingAnimal, MuteAnimal {
};

// Also egg sample
struct Destroyable {};

struct ObjectWithHealth : virtual Destroyable {};

int main() {
    derived d;
    d.a = 5;
    d.b = 6;

    base &b = d;
    b.a = 7;
    base *bb = &d;
    bb->a = 8;

    derived dd;
    dd.f(); // b
    dd.g(); // b

    base &bbb = dd;
    bbb.f(); // a
    bbb.g(); // b
    dd.base::f(); // a

    base bbbb;
    bbbb.g(); // a

    multiderived md;
    //md.a - ERROR
    md.base1::f(); // OK

    D ddd;
    d.a;
}
