


#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

class Order{
private:
    int quantity;
public:
    Order(int q):quantity(q){}
    int get_quantity(){
        return quantity;
    }
    virtual void display(){
        cout << "Order(" << get_quantity()
             << ")" << endl;
    }


};

class FOKOrder : public Order{
private:
    int price;
public:
    FOKOrder(int q, int p):Order(q),price(p){}
    void display(){
        cout << "FOKOrder(" << get_quantity()
                 << "," << price << ")" << endl;
    }
};


class GTCOrder : public Order{
private:
    int price;
public:
    GTCOrder(int q, int p):Order(q),price(p){}
    void display(){
        cout << "GTCOrder(" << get_quantity()
             << "," << price << ")" << endl;
    }
};
void displayOrders(vector<Order*> & list_orders) {
    for(Order* order : list_orders) {
        if (FOKOrder* fok = dynamic_cast<FOKOrder*>(order)) {
            fok->display();
        } else if (GTCOrder* gtc = dynamic_cast<GTCOrder*>(order)) {
            gtc->display();
        } else {
            order->display();
        }
    }
}

int main() {
    vector<Order *> list_orders;
    for (int k=0;k<10;k++){
        if (k%2==0){
            list_orders.push_back(new GTCOrder(2,3));
        }
        else{
            list_orders.push_back(new FOKOrder(2,3));
        }
    };
    
    //you will create the function which iterates this vector
    //display the type of orders in this vector
    
displayOrders(list_orders);

    // Memory cleanup (important!)
    for(Order* order : list_orders) {
        delete order;
    }
    return 0;
}