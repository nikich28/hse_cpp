#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

template <typename T>
class Matrix;

template <typename T>
class MatrixIterator {
private:
    const Matrix<T>& self;
    size_t row, column;

public:
    MatrixIterator(const Matrix<T>& m, size_t i, size_t j) : self(m), row(i), column(j) {}

    const T& operator * () const {
        return self[row][column];
    }
    MatrixIterator& operator++ () {
        ++column;
        if (column == self.size().second) {
            column = 0;
            ++row;
        }
        return *this;
    }
    bool operator == (const MatrixIterator& other) const {
        return row == other.row && column == other.column;
    }
    bool operator != (const MatrixIterator& other) const {
        return !(*this == other);
    }
};

template <typename T>
class Matrix {
private:
    vector<vector<T>> self;

public:
    Matrix(const vector<vector<T>>& d) : self(d) {}

    const vector<T>& operator[] (size_t i) const {
        return self[i];
    }
    vector<T>& operator[] (size_t i) {
        return self[i];
    }
    const pair<size_t, size_t> size() const {
        return {self.size(), self[0].size()};
    }
    Matrix& operator += (const Matrix& other) {
        for (size_t i = 0; i != self.size(); ++i) {
            for (size_t j = 0; j != self[0].size(); ++j) {
                self[i][j] += other.self[i][j];
            }
        }
        return *this;
    }
    Matrix operator + (const Matrix& other) const {
        Matrix result(*this);
        for (size_t i = 0; i != self.size(); ++i) {
            for (size_t j = 0; j != self[0].size(); ++j) {
                result.self[i][j] += other.self[i][j];
            }
        }
        return result;
    }

    template <typename B>
    Matrix& operator *= (const B& scalar) {
        for (size_t i = 0; i != self.size(); ++i) {
            for (size_t j = 0; j != self[0].size(); ++j) {
                self[i][j] *= scalar;
            }
        }
        return *this;
    }

    template <typename B>
    Matrix operator * (const B& scalar) const {
        Matrix result(*this);
        for (size_t i = 0; i != self.size(); ++i) {
            for (size_t j = 0; j != self[0].size(); ++j) {
                result.self[i][j] *= scalar;
            }
        }
        return result;
    }
    Matrix& transpose() {
        vector<vector<T>> v;
        for (size_t i = 0; i != self[0].size(); ++i) {
            vector<T> h(self.size());
            v.push_back(h);
        }
        for (size_t i = 0; i != self.size(); ++i) {
            for (size_t j = 0; j != self[0].size(); ++j) {
                v[j][i] = self[i][j];
            }
        }
        *this = Matrix(v);
        return *this;
    }
    Matrix transposed() const {
        vector<vector<T>> v;
        for (size_t i = 0; i != self[0].size(); ++i) {
            vector<T> h(self.size());
            v.push_back(h);
        }
        for (size_t i = 0; i != self.size(); ++i) {
            for (size_t j = 0; j != self[0].size(); ++j) {
                v[j][i] = self[i][j];
            }
        }
        return Matrix(v);
    }
    Matrix& operator *= (const Matrix& other) {
        assert(self[0].size() == other.self.size());
        vector<vector<T>> v;
        for (size_t i = 0; i != self.size(); ++i) {
            vector<T> h;
            for (size_t j = 0; j != other.size().second; ++j) {
                T sum = 0;
                for (size_t k = 0; k != self[0].size(); ++k) {
                    sum += self[i][k] * other[k][j];
                }
                h.push_back(sum);
            }
            v.push_back(h);
        }
        *this = Matrix(v);
        return *this;
    }
    Matrix operator * (const Matrix& other) const {
        auto tmp(*this);
        tmp *= other;
        return tmp;
    }
    MatrixIterator<T> begin() {
        return MatrixIterator<T>(*this, 0, 0);
    }
    const MatrixIterator<T> begin() const {
        return MatrixIterator<T>(*this, 0, 0);
    }
    MatrixIterator<T> end() {
        return MatrixIterator<T>(*this, size().first, 0);
    }
    const MatrixIterator<T> end() const {
        return MatrixIterator<T>(*this, size().first, 0);
    }
};

template <typename T>
ostream& operator << (ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i != m.size().first; ++i) {
        for (size_t j = 0; j != m.size().second - 1; ++j) {
            out << m[i][j] << "\t";
        }
        out << m[i][m.size().second - 1];
        if (i != m.size().first - 1)
            out << "\n";
    }
    return out;
}

int main() {
    vector<vector<int>> v = {{1, 0, 0}, {1, 0, 1}};
    Matrix<int> m(v);
    auto p = m.begin();
    ++p;
    *p = 2;
    cout << *m.begin();
    cout << "\n\n";

}