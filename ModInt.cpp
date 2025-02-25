#include <iostream>
#include <tuple>
#include <stdexcept>

template <typename T>
std::tuple<T, T, T> exgcd(const T& a, const T& b) {
    if (b == T(0)) {
        return {a, T(1), T(0)};
    }
    auto [g, x, y] = exgcd(b, a % b);
    return {g, y, x - a / b * y};
}

template <long long mod>
class ModInt {
    static_assert(mod >= 1LL, "The modulus must be a positive integer.");
private:
    long long value;
public:
    ModInt() : value(0LL) {}

    ModInt(const ModInt& v) : value(v.value) {}

    template <typename T>
    ModInt(const T& v): value(((long long)(v % mod) + mod) % mod) {}

    long long get_value() const {
        return value;
    }

    static long long get_mod() {
        return mod;
    }

    ModInt inv() const {
        auto [g, x, y] = exgcd<long long>(value, mod);
        if (g != 1LL) {
            throw std::runtime_error("The modular inverse does not exist.");
        }
        ModInt res;
        res.value = x;
        if (res.value < 0LL) {
            res.value += mod;
        }
        return res;
    }

    template <typename T>
    ModInt pow(T b) const {
        long long a = ((b >= T(0)) ? value : inv().value);
        ModInt res;
        res.value = ((mod >= 2LL) ? 1LL : 0LL);
        while (b != T(0)) {
            if (b % T(2) != T(0)) {
                res.value = res.value * a % mod;
            }
            a = a * a % mod;
            b = b / T(2);
        }
        return res;
    }

    ModInt& operator+=(const ModInt& b) {
        value += b.value;
        if (value >= mod) {
            value -= mod;
        }
        return *this;
    }

    ModInt& operator-=(const ModInt& b) {
        value -= b.value;
        if (value < 0LL) {
            value += mod;
        }
        return *this;
    }

    ModInt& operator*=(const ModInt& b) {
        value *= b.value;
        value %= mod;
        return *this;
    }

    ModInt& operator/=(const ModInt& b) {
        value *= b.inv().value;
        value %= mod;
        return *this;
    }

    ModInt& operator=(const ModInt& b) {
        value = b.value;
        return *this;
    }

    ModInt operator+() const {
        return *this;
    }

    ModInt operator-() const {
        ModInt res;
        res.value = -value;
        if (res.value < 0LL) {
            res.value += mod;
        }
        return res;
    }

    ModInt& operator++() {
        ++value;
        if (value >= mod) {
            value -= mod;
        }
        return *this;
    }

    ModInt operator++(int) {
        ModInt temp(*this);
        ++value;
        if (value >= mod) {
            value -= mod;
        }
        return temp;
    }

    ModInt& operator--() {
        --value;
        if (value < 0LL) {
            value += mod;
        }
        return *this;
    }

    ModInt operator--(int) {
        ModInt temp(*this);
        --value;
        if (value < 0LL) {
            value += mod;
        }
        return temp;
    }

    friend ModInt operator+(const ModInt& a, const ModInt& b) {
        ModInt res;
        res.value = a.value + b.value;
        if (res.value >= mod) {
            res.value -= mod;
        }
        return res;
    }

    friend ModInt operator-(const ModInt& a, const ModInt& b) {
        ModInt res;
        res.value = a.value - b.value;
        if (res.value < 0LL) {
            res.value += mod;
        }
        return res; 
    }

    friend ModInt operator*(const ModInt& a, const ModInt& b) {
        ModInt res;
        res.value = a.value * b.value % mod;
        return res;
    }

    friend ModInt operator/(const ModInt& a, const ModInt& b) {
        ModInt res;
        res.value = a.value * b.inv().value % mod;
        return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const ModInt& x) {
        os << x.value;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, ModInt& x) {
        if (is >> x.value) {
            x.value = (x.value % mod + mod) % mod;
        }
        return is;
    }
};

int main() {
    return 0;
}
