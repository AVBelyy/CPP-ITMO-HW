#include <iostream>
#include <limits>

template <typename T>
struct numeric_limits;

template <>
struct numeric_limits<double> {
    static double const min;
    static double const max;
};

double const numeric_limits<double>::min = std::numeric_limits<double>::min();
double const numeric_limits<double>::max = std::numeric_limits<double>::max();

// Example 2
template <typename U, typename V>
struct Guesser {
    static int const Ololue = 1;
};

template <typename U, typename V>
struct Guesser<U*, V> {
    static int const Ololue = 3;
};

template <typename U, typename V>
struct Guesser<U, V*> {
    static int const Ololue = 4;
};

// Example 3
template <typename U>
struct vector
{};

template <typename U, typename V>
struct vector<U(*)(V)>
{};

// Example 4
template <unsigned N>
struct fibonacci
{
    static unsigned const result = fibonacci<N - 1>::result + fibonacci<N - 2>::result;
};

template <>
struct fibonacci<0>
{
    static unsigned const result = 0;
};

template <>
struct fibonacci<1>
{
    static unsigned const result = 1;
};

typedef char* string;

int main() {
    Guesser<int, string> G;
    std::cout << G.Ololue << std::endl;

    std::cout << fibonacci<256>::result << std::endl;

    fibonacci < 5 > ololo;
    2 < 3 > 5;
}
