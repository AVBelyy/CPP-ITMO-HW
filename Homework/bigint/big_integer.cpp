#include "big_integer.h"

big_integer::big_integer()
{
    data.push_back(0);
}

big_integer::big_integer(big_integer const& other)
{
    *this = other;
}

big_integer::big_integer(int a)
{
    nonzero = a ? 0 : UINT32_MAX;
    negative = a < 0;
    data.push_back(negative ? -a : a);
}

const big_integer ZERO = big_integer(0);

big_integer::big_integer(std::string const& str)
{
    size_t i;
    *this = ZERO;
    if (str[0] == '-') {
        i = 1;
    } else {
        i = 0;
    }
    for (; i < str.size(); i++) {
        mul(10);
        add(str[i] - '0');
    }
    if (str[0] == '-') {
        negative = true;
    }
}

big_integer::~big_integer()
{
}

big_integer& big_integer::normalize()
{
    int size = data.size();
    for (int i = size - 1; i >= 0; i--) {
        if (data[i] == 0) {
            size--;
        } else {
            break;
        }
    }
    if (size == 0) {
        *this = ZERO;
    } else {
        data.resize(size);
    }
    return *this;
}

big_integer& big_integer::normalize_nonzero()
{
    nonzero = UINT32_MAX;
    size_t size = data.size();
    for (size_t i = 0; i < size; i++) {
        if (data[i] != 0) {
            nonzero = i;
            break;
        }
    }
    return *this;
}

big_integer& big_integer::operator=(big_integer const& other)
{
    nonzero = other.nonzero;
    negative = other.negative;
    data = other.data;
    return *this;
}

big_integer& big_integer::operator=(big_integer&& other)
{
    nonzero = other.nonzero;
    negative = other.negative;
    data = std::move(other.data);
    return *this;
}

big_integer& big_integer::negate()
{
    int size = data.size();
    for (int i = 0; i < size; i++) {
        data[i] = ~data[i];
    }
    --*this;
    return *this;
}

big_integer& big_integer::denegate()
{
    if (data.back() >> 31) {
        negative = true;
        negate();
    }
    return *this;
}

big_integer& big_integer::add(big_integer const& b)
{
    uint64_t carry = 0, value;
    size_t bsize = b.data.size();
    size_t size = std::max(data.size(), bsize);
    data.resize(size);
    for (size_t i = b.nonzero; i < bsize; i++) {
        value = (uint64_t)data[i] + (uint64_t)b.data[i] + carry;
        carry = value >> 32;
        data[i] = value;
    }
    for (size_t i = bsize; i < size; i++) {
        if (carry == 0) break;
        value = (uint64_t)data[i] + carry;
        carry = value >> 32;
        data[i] = value;
    }
    if (carry != 0) {
        data.push_back(1);
    }
    normalize_nonzero();
    return *this;
}

big_integer& big_integer::sub(big_integer const& b, int shift)
{
    negative = false;
    int64_t carry = 0, value;
    size_t size = b.data.size();
    for (size_t i = b.nonzero; i < size; i++) {
        value = (uint64_t)data[i + shift] - (uint64_t)b.data[i] - carry;
        carry = (uint64_t)value >> 63;
        value += carry << 32;
        data[i + shift] = value;
    }
    if (carry != 0) {
        data[size + shift]--;
    }
    normalize_nonzero();
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs)
{
    big_integer right = rhs;
    int sgna = compare(*this, 0), sgnb = compare(rhs, 0), sgn = abs_compare(*this, rhs, 0);
    if (sgna <= 0 && sgnb <= 0) {
        negative = false;
        right.negative = false;
        add(right);
        negative = true;
    } else if (sgna >= 0 && sgnb >= 0) {
        add(rhs);
    } else if (sgn <= 0 && sgna <= 0) {
        *this = right.sub(*this, 0);
    } else if (sgn <= 0 && sgna >= 0) {
        right.negative = false;
        right.sub(*this, 0);
        right.negative = true;
        *this = right;
    } else if (sgn >= 0 && sgna <= 0) {
        sub(rhs, 0);
        negative = false;
    } else /* if (sgn >= 0 && sgna >= 0) */ {
        sub(rhs, 0);
    }
    return normalize();
}

big_integer& big_integer::operator-=(big_integer const& rhs)
{
    return *this += -rhs;
}

big_integer& big_integer::mul(uint32_t b)
{
    uint64_t carry = 0;
    size_t size = data.size();
    for (size_t i = nonzero; i < size; i++) {
        uint64_t result = (uint64_t)data[i] * (uint64_t)b + carry;
        carry = result >> 32;
        data[i] = result;
    }
    if (carry != 0) {
        data.push_back(carry);
    }
    normalize_nonzero();
    return normalize();
}

big_integer& big_integer::operator*=(big_integer const& rhs)
{
    big_integer source = *this;
    unsigned old_nonzero = nonzero;
    *this = ZERO;
    negative = source.negative ^ rhs.negative;
    uint64_t carry, new_data;
    size_t asize = source.data.size(), bsize = rhs.data.size();
    data.resize(asize + bsize);
    for (size_t i = old_nonzero; i < asize; i++) { 
        carry = 0;
        for (size_t j = rhs.nonzero; j < bsize; j++) {
            new_data = (uint64_t)source.data[i] * (uint64_t)rhs.data[j] + (uint64_t)data[i + j] + carry;
            carry = new_data >> 32;
            data[i + j] = new_data;
        }
        data[i + bsize] = carry;
    }
    normalize_nonzero();
    return normalize();
}

big_integer& big_integer::div(uint32_t b)
{
    uint64_t carry = 0;
    int size = data.size();
    int64_t old_nonzero = std::max((int64_t)nonzero - 1, (int64_t)0);
    for (int64_t i = size - 1; i >= old_nonzero; i--) {
        uint64_t new_data = (carry << 32) + data[i];
        data[i] = new_data / b;
        carry = new_data % b;
    }
    normalize_nonzero();
    return normalize();
}

big_integer& big_integer::operator/=(big_integer const& rhs)
{
    if (rhs.data.size() == 1) {
        negative ^= rhs.negative;
        div(rhs.data[0]);
        return *this;
    }
    big_integer value = *this;
    big_integer right = rhs;
    *this = ZERO;
    negative = value.negative ^ right.negative;
    right.negative = value.negative = false;
    int size = value.data.size() - right.data.size() + 1;
    if (size <= 0) {
        *this = ZERO;
        return *this;
    }
    data.resize(size);
    big_integer tmp;
    if (right.data[right.data.size() - 1] < 1LL << 31) {
        uint32_t d = (1LL << 32) / (right.data[right.data.size() - 1] + 1);
        value.mul(d);
        right.mul(d);
    }
    int result = 0;
    for (int i = size - 1; i >= 0; i--) {
        int vsize = value.data.size(), rsize = right.data.size();
        uint32_t q = 0;
        uint32_t u1 = value.data[vsize - 1], u2 = value.data[vsize - 2], u3 = value.data[vsize - 3];
        uint32_t v1 = right.data[rsize - 1], v2 = right.data[rsize - 2];
        uint64_t u = u1 > v1 ? u1 : ((uint64_t)u1 << 32) + u2;
        uint64_t q_guess = std::min(u / v1, (uint64_t)UINT32_MAX), r_guess = u % v1;
        int iter = 0;
        do {
            if (q_guess == 1LL << 32 || q_guess * v2 > (r_guess << 32) + u3) {
                q_guess--;
                r_guess += v1;
            }
        } while(r_guess < 1LL << 32 && ++iter < 2);
        q = q_guess; // Thanks, D.E.K!
        tmp = right;
        tmp.mul(q);
        result = abs_compare(value, tmp, i);
        if (result >= 0) {
            value.sub(tmp, i);
            value.normalize();
            data[i] = q;
            if (result == 0) {
                break;
            }
        } else {
            data[i] = 0;
        }
    }
    normalize_nonzero();
    return normalize();
}

big_integer& big_integer::operator%=(big_integer const& rhs)
{
    return *this -= *this / rhs * rhs;
}

big_integer& big_integer::operator&=(big_integer const& rhs)
{
    bool may_be_negative = false;
    big_integer right = rhs;
    int size = std::max(data.size(), right.data.size());
    data.resize(size);
    right.data.resize(size);
    if (negative) {
        may_be_negative = true;
        negate();
    }
    if (right.negative) {
        may_be_negative = true;
        right.negate();
    }
    for (int i = 0; i < size; i++) {
        data[i] &= right.data[i];
    }
    if (negative) negate();
    if (may_be_negative) denegate();
    normalize_nonzero();
    return normalize();
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    bool may_be_negative = false;
    big_integer right = rhs;
    int size = std::max(data.size(), right.data.size());
    data.resize(size);
    right.data.resize(size);
    if (negative) {
        may_be_negative = true;
        negate();
    }
    if (right.negative) {
        may_be_negative = true;
        right.negate();
    }
    for (int i = 0; i < size; i++) {
        data[i] |= right.data[i];
    }
    if (negative) negate();
    if (may_be_negative) denegate();
    normalize_nonzero();
    return normalize();
}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    bool may_be_negative = false;
    big_integer right = rhs;
    int size = std::max(data.size(), right.data.size());
    data.resize(size);
    right.data.resize(size);
    if (negative) {
        may_be_negative = true;
        negate();
    }
    if (right.negative) {
        may_be_negative = true;
        right.negate();
    }
    for (int i = 0; i < size; i++) {
        data[i] ^= right.data[i];
    }
    if (negative) negate();
    if (may_be_negative) denegate();
    normalize_nonzero();
    return normalize();
}

void big_integer::set_bit(unsigned dst, bool val) {
    if (val) {
        data[dst >> 5] |= 1 << (dst & 0x1f);
    } else {
        data[dst >> 5] &= ~(1 << (dst & 0x1f));
    }
}

big_integer& big_integer::operator<<=(unsigned rhs)
{
    if (rhs % 32 == 0) {
        int shift = rhs / 32;
        data.push_front(shift, 0);
        nonzero += shift;
    } else {
        int size = data.size(), shift = (rhs - 1) / 32 + 1;
        data.resize(size + shift);
        for (int i = 32 * size - 1; i >= 0; i--) {
            set_bit(i + rhs, data[i / 32] & (1 << (i % 32)));
        }
        for (size_t i = 0; i < rhs; i++) {
            set_bit(i, false);
        }
        normalize_nonzero();
    }
    return normalize();
}

big_integer& big_integer::operator>>=(unsigned rhs)
{
    if (negative) {
        negate();
    }
    unsigned size = data.size(), shift = rhs / 32;
    if (size < shift) {
        return *this = ZERO;
    }
    if (rhs % 32 == 0) {
        data.pop_front(shift);
        if (nonzero >= shift) {
            nonzero -= shift;
        } else {
            normalize_nonzero();
        }
    } else {
        for (size_t i = rhs; i < 32 * size; i++) {
            set_bit(i - rhs, data[i / 32] & (1 << (i % 32)));
        }
        for (size_t i = 32 * size - rhs; i < 32 * size; i++) {
            set_bit(i, negative);
        }
        data.resize(size - shift);
        normalize_nonzero();
    }
    if (negative) {
        negate();
    }
    return normalize();
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer ans = *this;
    ans.negative = !negative;
    return ans;
}

big_integer big_integer::operator~() const
{
    big_integer ans = -*this;
    --ans;
    return ans;
}

big_integer& big_integer::operator++()
{
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int)
{
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--()
{
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int)
{
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const& b)
{
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b)
{
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b)
{
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b)
{
    return a /= b;
}

uint32_t big_integer::mod(uint32_t b)
{
    big_integer tmp = *this;
    tmp.div(b);
    tmp.mul(b);
    return (*this - tmp).data[0];
}

big_integer operator%(big_integer a, big_integer const& b)
{
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

big_integer operator<<(big_integer a, int b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
    return a >>= b;
}

int big_integer::abs_compare(big_integer const& a, big_integer const& b, int shift)
{
    if (a.data.size() > b.data.size() + shift) {
        return 1;
    }
    if (a.data.size() < b.data.size() + shift) {
        return -1;
    }
    for (int i = a.data.size() - 1; i >= shift; i--) {
        if (a.data[i] > b.data[i - shift]) {
            return 1;
        }
        if (a.data[i] < b.data[i - shift]) {
            return -1;
        }
    }
    for (int i = shift - 1; i >= 0; i--) {
        if (a.data[i] > 0) {
            return 1;
        }
    }
    return 0;
}

int big_integer::compare(big_integer const& a, big_integer const& b)
{
    int result = abs_compare(a, b, 0);
    if ((a.negative && b.negative) || (result > 0 && a.negative) || (result < 0 && b.negative)) {
        return -result;
    } else {
        return result;
    }
}

bool operator==(big_integer const& a, big_integer const& b)
{
    return big_integer::compare(a, b) == 0;
}

bool operator!=(big_integer const& a, big_integer const& b)
{
    return big_integer::compare(a, b) != 0;
}

bool operator<(big_integer const& a, big_integer const& b)
{
    return big_integer::compare(a, b) < 0;
}

bool operator>(big_integer const& a, big_integer const& b)
{
    return big_integer::compare(a, b) > 0;
}

bool operator<=(big_integer const& a, big_integer const& b)
{
    return big_integer::compare(a, b) <= 0;
}

bool operator>=(big_integer const& a, big_integer const& b)
{
    return big_integer::compare(a, b) >= 0;
}

std::string to_string(big_integer const& a)
{
    big_integer value = a;
    value.negative = false;
    std::string result;
    if (value == 0) {
        return "0";
    }
    while (value != ZERO) {
        result += '0' + value.mod(10);
        value.div(10);
    }
    if (a.negative) {
        result += '-';
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a)
{
    return s << to_string(a);
}
// TODO : (union ptr / int)[int / ptr] -> [ref-counter | .. | .. | ..] (small -> cow)
