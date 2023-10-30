#include <iostream>

template <typename T>
class myptr {
private:
    T* ptr;
public:
    // Constructor
    myptr(T* p) : ptr(p) {}

    // Destructor
    ~myptr() {
        delete ptr;
        std::cout << "myptr destructor called, memory deallocated" << std::endl;
    }

    // Overload dereference operator
    T& operator*() {
        return *ptr;
    }

    // Overload arrow operator
    T* operator->() {
        return ptr;
    }
};

class Order {
private:
    int id;
    int price;
    int quantity;
public:
    Order(int id_, int price_, int quantity_) : id(id_), price(price_), quantity(quantity_) {
        std::cout << "constructor called for id=" << id << std::endl;
    }
    ~Order() {
        std::cout << "destructor called for id=" << id << std::endl;
    }
};

int main() {
    {
        myptr<Order> o1(new Order(1, 10, 100));  // Automatically deallocated
    }
    return 0;
}

