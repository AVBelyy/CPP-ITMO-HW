// book "C++ Object Model"

// Case 1: f() is called as a non-virtual function
struct b {
    virtual void f();
    virtual void g() = 0;
    void h() {
        g();
    }

    b() {
        h();
    };
};

struct d : b {
    void f();
    void g();
};

// Case 2: f() is called as a non-virtual function because the "base" init comes prior to "derived"

struct base {
    base() {
        f();
    }

    virtual void f() {
    }
};

struct derived : base {
    void f() {
    }
};

int main() {
    d o;
    o.b::f();
    o.b::g(); // linking error
    derived d;
}



struct B {
    virtual ~B() = 0;
};

struct D : B {
    ~D();
};
