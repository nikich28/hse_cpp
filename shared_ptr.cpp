#include <iostream>
#include <utility>

template <typename T>
class SharedPtr {
private:
    T* ptr = nullptr;
    int* cnt = nullptr;

public:
    SharedPtr() noexcept {
    }
    SharedPtr(T* other) noexcept {
        if (other != nullptr) {
            cnt = new int(1);
            ptr = other;
        }
    }
    SharedPtr(const SharedPtr& other) noexcept {
        ptr = other.ptr;
        cnt = other.cnt;
        if (cnt != nullptr)
            ++*cnt;
    }
    SharedPtr(SharedPtr&& other) {
        ptr = std::move(other.ptr);
        cnt = std::move(other.cnt);
        other.ptr = nullptr;
        other.cnt = nullptr;
    }
    SharedPtr& operator= (T* other) {
        if (this != &other) {
            this->~SharedPtr();
            ptr = nullptr;
            cnt = nullptr;
            ptr = other;
            if (ptr != nullptr)
                cnt = new int(1);
        }
        return *this;
    }
    SharedPtr& operator= (const SharedPtr& other) {
        if (this != &other) {
            this->~SharedPtr();
            ptr = nullptr;
            cnt = nullptr;
            ptr = other.ptr;
            cnt = other.cnt;
            if (ptr != nullptr)
                ++*cnt;
        }
        return *this;
    }

    SharedPtr& operator= (SharedPtr&& other) {
        if (this != &other) {
            this->~SharedPtr();
            ptr = std::move(other.ptr);
            cnt = std::move(other.cnt);
            other.ptr = nullptr;
            other.cnt = nullptr;
        }
        return *this;
    }
    void swap(SharedPtr& other) {
        std::swap(ptr, other.ptr);
        std::swap(cnt, other.cnt);
    }
    ~SharedPtr() {
        if (cnt != nullptr && --*cnt == 0) {
            delete ptr;
            delete cnt;
        }
    }
    T* operator-> () const noexcept {
        return ptr;
    }
    T& operator* () const noexcept {
        return *ptr;
    }
    T& operator* () noexcept {
        return *ptr;
    }
    T* get() const noexcept {
        return ptr;
    }
    void reset(T* p) {
        this->~SharedPtr();
        ptr = nullptr;
        cnt = nullptr;
        ptr = p;
        if (ptr == nullptr)
            cnt == nullptr;
        else
            cnt = new int(1);
    }
    explicit operator bool() const noexcept {
        return (ptr != nullptr);
    }
};