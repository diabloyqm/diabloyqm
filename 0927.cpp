#include <iostream>
#include <sstream>

using namespace std;

class Order {
private:
    int quantity;
public:
    int get_quantity()  const {
        return quantity;
    }

    // Constructor
    Order(int q): quantity(q) {};

    // Overloaded assignment operator
    Order& operator=(const Order &o1) {
        quantity = o1.quantity;
        return *this;
    }

    Order& operator++(){
        quantity++;
       
        return *this;
    }
    // if there is no &, then the order will be in the stack and delete after 

    Order operator++(int){
        int save_value =quantity;

        ++quantity; //<==> quantity ++'
        return Order(save_value);
    }
    Order& operator +=(float a ){
        quantity +=a ;
        return *this;

    }

    // Overloaded addition operator
    Order operator+(const Order &o) const{
        return Order(quantity + o.quantity);
    // Order& opertator(const Order &o){
    //     quantity += o.quantity;
    //     return *this; //no good (because we do not want to change the left class)
    }
};

// Overloaded stream insertion operator

ostream& operator<<(ostream &os, const Order &o)  {
    os << "Order with quantity: " << o.get_quantity();
    return os;
}

int main() {
    Order o1(2);
    Order o2(3);
    Order o3 = o2; // Invokes copy constructor

    o3 = o2 = o1; // Chained assignment

    cout << o1 << endl;  // Using overloaded stream insertion operator
    cout << o2 << endl;  
    cout << o3 << endl; // cout << o1.quantity << endl; <==> operator <<(cout , o1)

    Order o4 = o1 + o2;  // Using overloaded addition operator
    cout << o4 << endl;
    cout << o1 << endl;
    cout << ++o1<<endl;
    cout << o1 << endl;
    cout<< o1++ << endl;
    cout<< o1 <<endl;


    return 0;
}
