struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    bool defined = false;
    alignas(T) unsigned char buf[sizeof(T)];

public:
    Optional() = default;
    Optional(const T& elem) {
        defined = true;
        new (buf) T(elem);
    }
    Optional(T&& elem) {
        defined = true;
        new (buf) T(elem);
    }
    Optional(const Optional& other) {
        defined = other.defined;
        if (defined) {
            //new(buf) T(*reinterpret_cast<T*>(other.buf));
            new(buf) T(*other);
        }
    }
    Optional& operator=(const Optional& other) {
        //if (defined)
            //reinterpret_cast<T*>(buf)->~T();
        defined = other.defined;
        if (defined)
            //new(buf) T(*reinterpret_cast<T*>(other.buf));
            new(buf) T(*other);
    }
    Optional& operator=(const T& elem) {
        if (defined)
            reinterpret_cast<T*>(buf)->~T();
        defined = true;
        new (buf) T(elem);
    }
    Optional& operator=(T&& elem) {
        if (defined)
            reinterpret_cast<T*>(buf)->~T();
        defined = true;
        new(buf) T(elem);
    }
    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return *(reinterpret_cast<T*>(buf));
    }
    const T& operator*() const {
        return *(reinterpret_cast<const T*>(buf));
    }

    T* operator->() {
        return reinterpret_cast<T*>(buf);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(buf);
    }

    T& value() {
        if (defined)
            return *(reinterpret_cast<T*>(buf));
        else
            throw BadOptionalAccess();
    }
    const T& value() const {
        if (defined)
            return *(reinterpret_cast<T*>(buf));
        else
            throw BadOptionalAccess();
    }

    void reset() {
        if (defined)
            value().T::~T();
    }
    ~Optional() {
        if (defined) {
            reinterpret_cast<T*>(buf)->~T();
        }
    }
};