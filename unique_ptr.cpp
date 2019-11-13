#include <iostream>
#include <cstddef>
#include <tuple>
#include <algorithm>
#include <memory>

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> data = std::make_tuple(nullptr, Deleter());

public:
    UniquePtr() noexcept {
    }
    UniquePtr(T* t) noexcept {
        std::get<0>(data) = t;
    }
    UniquePtr(UniquePtr&& other) noexcept {
        std::get<0>(data) = std::get<0>(other.data);
        std::get<0>(other.data) = nullptr;
        std::get<1>(data) = std::get<1>(other.data);
    }
    UniquePtr(T* t, Deleter del) noexcept {
        std::get<0>(data) = t;
        std::get<1>(data) = del;
    }
    UniquePtr& operator= (nullptr_t) noexcept {
        get_deleter()(get());
        std::get<0>(data) = nullptr;
        return *this;
    }
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr& operator= (UniquePtr&& other) noexcept {
        get_deleter()(get());
        std::get<0>(data) = std::get<0>(other.data);
        std::get<0>(other.data) = nullptr;
        std::get<1>(data) = std::get<1>(other.data);
        return *this;
    }
    T& operator* () const {
        return *get();
    }
    ~UniquePtr() {
        get_deleter()(get());
    }
    T* operator-> () const noexcept {
        return get();
    }
    T* release() noexcept {
        auto p = get();
        std::get<0>(data) = nullptr;
        return p;
    }
    void reset(T* o_ptr) noexcept {
        get_deleter()(get());
        std::get<0>(data) = o_ptr;
        o_ptr = nullptr;
    }
    void swap(UniquePtr& other) noexcept {
        std::swap(std::get<0>(data), std::get<0>(other.data));
        std::swap(std::get<1>(data), std::get<1>(other.data));
    }
    T* get() const noexcept {
        return std::get<0>(data);
    }
    const Deleter& get_deleter() const {
        return std::get<1>(data);
    }
    Deleter& get_deleter() {
        return std::get<1>(data);
    }
    explicit operator bool() const noexcept {
        return (std::get<0>(data) != nullptr);
    }
};