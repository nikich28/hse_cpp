#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T>
class Polynomial {
private:
    vector<T> data;

public:
    Polynomial(const vector<T>& v) : data(v) {
        balance();
    }
    Polynomial(const T n = T(0)) : data({n}) {}

    template <typename Iter>
    Polynomial(Iter first, Iter last) {
        vector<T> v;
        while (first != last) {
            v.push_back(*first++);
        }
        data = v;
        balance();
    }

    T operator[] (int s) const {
        if (s > Degree())
            return T(0);
        else
            return data[s];
    }

    int Degree() const {
        if (data[0] == T(0) && data.size() == 1)
            return -1;
        return data.size() - 1;
    }

    void balance() {
        auto k = data.size() - 1;
        for (size_t i = 0; i != k + 1; ++i) {
            if (data[k - i] == T(0) && i != k)
                data.pop_back();
            else
                break;
        }
    }

    Polynomial& operator+= (const Polynomial& a) {
        if (a == Polynomial(T(0)))
            return *this;
        int p = max(Degree(), a.Degree()) + 1;
        vector<T> v(p);
        for (int i = 0; i != p; ++i) {
            if (i <= Degree())
                v[i] += data[i];
            if (i <= a.Degree())
                v[i] += a[i];
        }
        auto k = Polynomial(v);
        k.balance();
        *this = k;
        balance();
        return *this;
    }

    Polynomial& operator+= (const T& a) {
        auto p = Polynomial(a);
        if (a == T(0))
            return *this;
        *this += p;
        balance();
        return *this;
    }

    Polynomial& operator-= (const Polynomial& a) {
        if (a == Polynomial(T(0)))
            return *this;
        int p = max(Degree(), a.Degree()) + 1;
        vector<T> v(p);
        for (int i = 0; i != p; ++i) {
            if (i <= Degree())
                v[i] += data[i];
            if (i <= a.Degree())
                v[i] -= a[i];
        }
        auto k = Polynomial(v);
        k.balance();
        *this = k;
        return *this;
    }

    Polynomial& operator-= (const T& a) {
        auto p = Polynomial(a);
        if (a == T(0))
            return *this;
        *this -= p;
        balance();
        return *this;
    }

    Polynomial& operator*= (const Polynomial& a) {
        if (a == Polynomial(T(0))) {
            *this = Polynomial(T(0));
            return *this;
        }
        vector<T> v(a.Degree() + Degree() + 1);
        for (int i = 0; i != Degree() + 1; ++i) {
            for (int j = 0; j != a.Degree() + 1; ++j) {
                v[i + j] += data[i] * a[j];
            }
        }
        auto k = Polynomial(v);
        k.balance();
        *this = k;
        return *this;
    }

    Polynomial& operator*= (const T& a) {
        if (a == T(0)) {
            *this = Polynomial(T(0));
            return *this;
        }
        auto p = Polynomial(a);
        *this *= p;
        balance();
        return *this;
    }

    auto begin() const {
        return data.begin();
    }
    auto end() const {
        return data.end();
    }

    T operator() (const T& x) const {
        auto p = x;
        T res = T(0);
        res += data[0];
        for (int i = 1; i <= Degree(); ++i) {
            res += data[i] * p;
            p *= x;
        }
        return res;
    }

    Polynomial operator & (const Polynomial& g) const {
        auto p = g;
        Polynomial<T> res = Polynomial(T(0));
        res += data[0];
        for (int i = 1; i <= Degree(); ++i) {
            res += data[i] * p;
            p *= g;
        }
        return res;
    }

    Polynomial operator / (const Polynomial& other) const {
        if (*this == Polynomial(T(0)))
            return *this;
        auto p = *this;
        vector<T> ans(p.Degree() - other.Degree() + 1);
        while (p.Degree() >= other.Degree()) {
            vector<T> v(p.Degree() - other.Degree() + 1);
            ans[p.Degree() - other.Degree()] = p[p.Degree()] / other[other.Degree()];
            v[p.Degree() - other.Degree()] = p[p.Degree()] / other[other.Degree()];
            p -= Polynomial(v) * other;
            p.balance();
        }
        auto y = Polynomial(ans);
        y.balance();
        return y;
    }

    Polynomial operator % (const Polynomial& other) const {
        if (*this == Polynomial(T(0)))
            return other;
        auto p = *this / other;
        auto k = *this - p * other;
        k.balance();
        return k;
    }

    Polynomial operator , (const Polynomial& other) const {
        Polynomial a(*this);
        auto b = other;
        while (b != Polynomial(T(0))) {
            auto c = a % b;
            a = b;
            b = c;
            a.balance();
            b.balance();
        }
        if (a[a.Degree()] != T(1)) {
            auto d = a[a.Degree()];
            a = a / Polynomial(d);
        }
        return a;
    }
};

template <typename T>
Polynomial<T> operator+ (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (b == Polynomial<T>(T(0)))
        return a;
    int p = max(b.Degree(), a.Degree()) + 1;
    vector<T> v(p);
    for (int i = 0; i != p; ++i) {
        if (i <= a.Degree())
            v[i] += a[i];
        if (i <= b.Degree())
            v[i] += b[i];
    }
    auto k = Polynomial<T>(v);
    k.balance();
    return k;
}

template <typename T>
Polynomial<T> operator+ (const Polynomial<T>& a, const T& b) {
    if (b == T(0))
        return a;
    auto p = Polynomial<T>{b};
    return a + p;
}

template <typename T>
Polynomial<T> operator+ (const T& b, const Polynomial<T>& a) {
    if (a == Polynomial<T>(T(0)))
            return Polynomial<T>{b};
    auto p = Polynomial<T>{b};
    return a + p;
}

template <typename T>
Polynomial<T> operator- (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (b == Polynomial<T>(T(0)))
        return a;
    int p = max(b.Degree(), a.Degree()) + 1;
    vector<T> v(p);
    for (int i = 0; i != p; ++i) {
        if (i <= a.Degree())
            v[i] += a[i];
        if (i <= b.Degree())
            v[i] -= b[i];
    }
    auto k = Polynomial<T>(v);
    k.balance();
    return k;
}

template <typename T>
Polynomial<T> operator- (const Polynomial<T>& a, const T& b) {
    if (b == T(0))
        return a;
    auto p = Polynomial<T>{b};
    return a - p;
}

template <typename T>
Polynomial<T> operator- (const T& b, const Polynomial<T>& a) {
    if (a == Polynomial<T>(T(0)))
        return Polynomial<T>{b};
    auto p = Polynomial<T>{b};
    return p - a;
}

template <typename T>
bool operator== (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (a.Degree() != b.Degree())
        return false;
    for (int i = 0; i <= a.Degree(); ++i) {
        if (a[i] != b[i])
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
Polynomial<T> operator* (const Polynomial<T>& a, const Polynomial<T>& b) {
    if (b == Polynomial<T>(T(0)) || a == Polynomial<T>(T(0)))
        return Polynomial<T>(T(0));
    vector<T> v(a.Degree() + b.Degree() + 1);
    for (int i = 0; i != a.Degree() + 1; ++i) {
        for (int j = 0; j != b.Degree() + 1; ++j) {
            v[i + j] += a[i] * b[j];
        }
    }
    auto k = Polynomial<T>(v);
    k.balance();
    return k;
}

template <typename T>
Polynomial<T> operator* (const Polynomial<T>& a, const T& b) {
    if (b == T(0))
        return Polynomial<T>(T(0));
    auto p = Polynomial<T>{b};
    return a * p;
}

template <typename T>
Polynomial<T> operator* (const T& b, const Polynomial<T>& a) {
    if (a == Polynomial<T>(T(0)))
        return a;
    auto p = Polynomial<T>{b};
    return a * p;
}

template <typename T>
ostream& operator << (ostream& out, const Polynomial<T>& p) {
    if (p.Degree() == -1) {
        out << T(0);
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
        for (int i = p.Degree(); i >= 0; --i) {
            if (i == p.Degree()) {
                if (p[i] == T(1))
                    out << "x^" << i;
                else if (p[i] == T(-1))
                    out << "-x^" << i;
                else
                    out << p[i] << "*x^" << i;
            } else if (i == 1) {
                if (p[i] == T(1))
                    out << "+x";
                else if (p[i] == T(-1))
                    out << "-x";
                else if (p[i] > T(0))
                    out << "+" << p[i] << "*x";
                else if (p[i] < T(0))
                    out << p[i] << "*x";
            } else if (i == 0) {
                if (p[i] > T(0))
                    out << "+" << p[i];
                else if (p[i] < T(0))
                    out << p[i];
            } else {
                if (p[i] == T(1))
                    out << "+x^" << i;
                else if (p[i] == T(-1))
                    out << "-x^" << i;
                else if (p[i] > T(0))
                    out << "+" << p[i] << "*x^" << i;
                else if (p[i] < T(0))
                    out << p[i] << "*x^" << i;
            }
        }
    }
    return out;
}
