#include <iostream>
#include <utility>
#include <memory>

class Vector {
private:
    int* ptr;
    size_t sz;
    size_t cp;

public:
    Vector() {
        ptr = nullptr;
        sz = 0;
        cp = 0;
    }
    Vector(const Vector& v) {
        ptr = new int[v.cp];
        sz = v.sz;
        cp = v.cp;
        for (size_t i = 0; i < v.sz; ++i) {
            ptr[i] = v.ptr[i];
        }
    }
    Vector& operator= (const Vector& v) {
        delete [] ptr;
        ptr = new int[v.cp];
        sz = v.sz;
        cp = v.cp;
        for (size_t i = 0; i < sz; ++i) {
            ptr[i] = v.ptr[i];
        }
        return *this;
    }
    ~Vector() {
        delete [] ptr;
    }
    int operator[] (size_t i) {
        return ptr[i];
    }
    size_t size() {
        return sz;
    }
    void push_back(int n) {
        if (sz < cp) {
            ptr[sz] = n;
        } else {
            if (cp == 0)
                cp = 1;
            cp *= 2;
            int* new_ptr = new int[cp];
            for (size_t i = 0; i < sz; ++i)
                new_ptr[i] = ptr[i];
            new_ptr[sz] = n;
            std::swap(ptr, new_ptr);
            delete [] new_ptr;
        }
        ++sz;
    }
    void pop_back() {
        --sz;
    }
};