#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

template <typename T>
class Polynomial {
private:
    map<int, T> data;

public:
    Polynomial(const vector<T>& v) {
        for (size_t i = 0; i != v.size(); ++i) {
            data[i] = v[i];
        }
        if (data.size() == 0)
            data[0] = T{0};
        balance();
    }
    Polynomial(const T n = T{0}) {
        data[0] = n;
    }

    template <typename Iter>
    Polynomial(Iter first, Iter last) {
        if (first == last) {
            data[0] = T{0};
        } else {
            for (size_t i = 0; next(first, i) != last; ++i) {
                data[i] = *next(first, i);
            }
        }
        balance();
    }

    T operator[] (size_t s) const {
        auto it = data.find(s);
        if (it == data.end())
            return T{0};
        return it->second;
    }

    int Degree() const {
        if ((--data.end())->second == T{0})
            return -1;
        return (--data.end())->first;
    }

    void balance() {
        for (auto it = data.begin(); it != data.end();) {
            if (it->second == T {0})
                it = data.erase(it);
            else
                ++it;
        }
        if (data.size() == 0)
            data[0] = T{0};
    }

    auto begin() const {
        return data.begin();
    }
    auto end() const {
        return data.end();
    }

    Polynomial& operator+= (const Polynomial& a) {
        if (a == Polynomial{T{0}})
            return *this;
        for (auto it = a.begin(); it != a.end(); ++it) {
            data[it->first] += it->second;
        }
        balance();
        return *this;
    }

    Polynomial& operator+= (const T& a) {
        if (a == T{0})
            return *this;
        auto p = Polynomial{a};
        *this += p;
        balance();
        return *this;
    }

    Polynomial& operator-= (const Polynomial& a) {
        if (a == Polynomial{T{0}})
            return *this;
        for (auto it = a.begin(); it != a.end(); ++it) {
            data[it->first] -= it->second;
        }
        balance();
        return *this;
    }

    Polynomial& operator-= (const T& a) {
        if (a == T{0})
            return *this;
        auto p = Polynomial{a};
        *this -= p;
        balance();
        return *this;
    }

    Polynomial& operator*= (const Polynomial& a) {
        if (a == Polynomial{T{0}}) {
            *this = Polynomial{T{0}};
            return *this;
        }
        auto tmp = Polynomial<T>{};
        for (auto it = begin(); it != end(); ++it) {
            for (auto jt = a.begin(); jt != a.end(); ++jt) {
                tmp.data[it->first + jt->first] += it->second * jt->second;
            }
        }
        tmp.balance();
        *this = tmp;
        return *this;
    }

    Polynomial& operator*= (const T& a) {
        if (a == T{0}) {
            *this = Polynomial{T{0}};
            return *this;
        }
        auto p = Polynomial{a};
        *this *= p;
        balance();
        return *this;
    }

    T operator() (const T& x) const {
        T res = (*this)[0];
        auto p = x;
        for (int i = 1; i <= Degree(); ++i) {
            res += (*this)[i] * p;
            p *= x;
        }
        return res;
    }

    Polynomial operator & (const Polynomial& g) const {
        Polynomial<T> res = Polynomial{(*this)[0]};
        auto p = g;
        for (int i = 1; i <= Degree(); ++i) {
            res += (*this)[i] * p;
            p *= g;
        }
        return res;
    }

    Polynomial operator / (const Polynomial& other) const {
        if (*this == Polynomial{T{0}})
            return *this;
        auto p(*this);
        Polynomial<T> ans;
        while (p.Degree() >= other.Degree()) {
            Polynomial<T> v;
            ans.data[p.Degree() - other.Degree()] = p[p.Degree()] / other[other.Degree()];
            v.data[p.Degree() - other.Degree()] = p[p.Degree()] / other[other.Degree()];
            p -= v * other;
            p.balance();
        }
        ans.balance();
        return ans;
    }

    Polynomial operator % (const Polynomial& other) const {
        if (*this == Polynomial{T{0}})
            return other;
        auto p = *this / other;
        auto k = *this - p * other;
        k.balance();
        return k;
    }

    Polynomial operator , (const Polynomial& other) const {
        auto a(*this);
        auto b(other);
        while (b != Polynomial{T{0}}) {
            auto c = a % b;
            a = b;
            b = c;
            a.balance();
            b.balance();
        }
        if (a[a.Degree()] != T{1}) {
            auto d = a[a.Degree()];
            a = a / Polynomial(d);
        }
        return a;
    }
};

template <typename T>
bool operator== (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (a.Degree() != b.Degree())
        return false;
    for (auto it = a.begin(); it != a.end(); ++it) {
        if (it->second != b[it->first])
            return false;
    }
    for (auto it = b.begin(); it != b.end(); ++it) {
        if (it->second != a[it->first])
            return false;
    }
    return true;
}

template <typename T>
bool operator== (const Polynomial<T>& a, const T& b) {
        auto p = Polynomial<T>{b};
        return (a == p);
}

template <typename T>
bool operator== (const T& b, const Polynomial<T>& a) {
    auto p = Polynomial<T>{b};
    return (a == p);
}

template <typename T>
bool operator!= (const Polynomial<T>& a, const T& b) {
    auto p = Polynomial<T>{b};
    return !(a == p);
}

template <typename T>
bool operator!= (const T& b, const Polynomial<T>& a) {
    auto p = Polynomial<T>{b};
    return !(a == p);
}

template <typename T>
bool operator!= (const Polynomial<T>& b, const Polynomial<T>& a) {
    return !(a == b);
}

template <typename T>
Polynomial<T> operator+ (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (b == Polynomial<T>{T{0}})
        return a;
    auto k = Polynomial<T>{};
    k += a;
    k += b;
    k.balance();
    return k;
}

template <typename T>
Polynomial<T> operator+ (const Polynomial<T>& a, const T& b) {
    if (b == T{0})
        return a;
    auto p = Polynomial<T>{b};
    return a + p;
}

template <typename T>
Polynomial<T> operator+ (const T& b, const Polynomial<T>& a) {
    auto p = Polynomial<T>{b};
    if (a == Polynomial<T>{T{0}})
        return p;
    return a + p;
}

template <typename T>
Polynomial<T> operator- (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (b == Polynomial<T>{T{0}})
        return a;
    auto k(a);
    k -= b;
    k.balance();
    return k;
}

template <typename T>
Polynomial<T> operator- (const Polynomial<T>& a, const T& b) {
    if (b == T{0})
        return a;
    auto p = Polynomial<T>{b};
    return a - p;
}

template <typename T>
Polynomial<T> operator- (const T& b, const Polynomial<T>& a) {
    auto p = Polynomial<T>{b};
    if (a == Polynomial<T>{T(0)})
        return p;
    return p - a;
}

template <typename T>
Polynomial<T> operator* (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (b == Polynomial<T>{T{0}} || a == Polynomial<T>{T{0}})
        return Polynomial<T>{T{0}};
    auto tmp(a);
    tmp *= b;
    tmp.balance();
    return tmp;
}

template <typename T>
Polynomial<T> operator* (const Polynomial<T>& a, const T& b) {
    if (b == T{0})
        return Polynomial<T>{T{0}};
    auto p = Polynomial<T>{b};
    return a * p;
}

template <typename T>
Polynomial<T> operator* (const T& b, const Polynomial<T>& a) {
    if (a == Polynomial<T>{T{0}})
        return a;
    auto p = Polynomial<T>{b};
    return a * p;
}

template <typename T>
ostream& operator << (ostream& out, const Polynomial<T>& p) {
    if (p.Degree() == -1) {
        out << T{0};
    } else if (p.Degree() == 0) {
        out << p[0];
    } else if (p.Degree() == 1) {
        for (int i = 1; i >= 0; --i) {
            if (i == 1) {
                if (p[i] == T(1))
                    out << "x";
                else if (p[i] == T(-1))
                    out << "-x";
                else
                    out << p[i] << "*x";
            }
            if (i == 0) {
                if (p[i] > T(0))
                    out << "+" << p[i];
                else if (p[i] < T(0))
                    out << p[i];
            }
        }
    } else {
        for (auto it = p.end(); it != p.begin();) {
            --it;
            auto i = it->first;
            auto j = it->second;
            if (i == p.Degree()) {
                if (j == T(1))
                    out << "x^" << i;
                else if (j == T(-1))
                    out << "-x^" << i;
                else
                    out << j << "*x^" << i;
            } else if (i == 1) {
                if (j == T(1))
                    out << "+x";
                else if (j == T(-1))
                    out << "-x";
                else if (j > T(0))
                    out << "+" << j << "*x";
                else if (j < T(0))
                    out << j << "*x";
            } else if (i == 0) {
                if (j > T(0))
                    out << "+" << j;
                else if (j < T(0))
                    out << j;
            } else {
                if (j == T(1))
                    out << "+x^" << i;
                else if (j == T(-1))
                    out << "-x^" << i;
                else if (j > T(0))
                    out << "+" << j << "*x^" << i;
                else if (j < T(0))
                    out << j << "*x^" << i;
            }
        }
    }
    return out;
}
