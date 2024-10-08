#include <iostream>

template<class T>
class SmartPointer {
private:
    T* ptr{};
    size_t* refCount{};

public:
    SmartPointer(T* p = nullptr) : ptr{ p }, refCount{ new size_t(1) } {}

    SmartPointer(const SmartPointer& other) {
        ptr = other.ptr;
        refCount = other.refCount;
        (*refCount)++;
    }

    SmartPointer& operator=(const SmartPointer& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            (*refCount)++;
        }
        return *this;
    }

    SmartPointer(SmartPointer&& other) noexcept {
        ptr = other.ptr;
        refCount = other.refCount;
        other.ptr = nullptr;
        other.refCount = nullptr;
    }

    SmartPointer& operator=(SmartPointer&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
        }
        return *this;
    }

    ~SmartPointer() {
        release();
    }

    void release() {
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }
    }

    T& operator* () {
        return *ptr;
    }

    T* operator-> () {
        return ptr;
    }

    size_t use_count() const {
        return refCount ? *refCount : 0;
    }
};

class Test {
public:
    void sayHello() {
        std::cout << "Hello, World!" << std::endl;
    }
};

int main() {
    SmartPointer<Test> sp1(new Test());
    sp1->sayHello();

    std::cout << "Use count: " << sp1.use_count() << std::endl;

    {
        SmartPointer<Test> sp2 = sp1; 
        std::cout << "Use count after copy: " << sp1.use_count() << std::endl;

        sp2->sayHello();
    } 

    std::cout << "Use count after sp2 goes out of scope: " << sp1.use_count() << std::endl;

    return 0;
}
