// simple bind

struct F_int {
    typedef int result_type;
};

template <typename F, typename A>
struct bind_t {
    F f;
    A a;

    bind_t(F f, A a) : f(f), a(a) {}
    decltype(f(a)) operator()() const {
        return f(a);
    }
};

template <typename F, typename A>
auto bind(F f, A a) -> decltype(f(a)) {
    return bind_t<F, A>(f, a);
}
