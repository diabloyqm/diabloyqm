#include <iostream>
#include <string.h>
#include <stdlib.h>

// # Change this to false when you are ready to submit
#define DEBUG true
// the dynamic cast is run by run time, from base to dynamic.
// static cast is run by compilation time. from dynamic to base.
// re-interprete cast 
enum ordertype {LIMIT, MARKET};


class Order{
private:
    long timestamp; // epoch time: the number of seconds that have elapsed since 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970,.
    bool is_buy; // buy or sell
    unsigned int id; // order id – unique identifier
    unsigned int price;
    unsigned int quantity;
    char venue[20]; // company name where this order comes from
    char symbol[20];
    ordertype type;
public:
    Order(long timestamp_,
          bool is_buy_,
          unsigned int id_,
          unsigned int price_,
          unsigned int quantity_,
          const char * venue_,
          const char * symbol_,
          ordertype type_)
    {
        timestamp=timestamp_;
        id=id_;
        price=price_;
        quantity=quantity_;
        strcpy(venue,venue_);
        type=type_;
        strcpy(symbol,symbol_);
    }

    char * getVenue()
    {
        return venue;
    }

    char * getSymbol()
    {
        return symbol;
    }

    unsigned int getID()
    {
        return id;
    }

    ordertype getOrderType()
    {
        return type;
    }

    unsigned int getQuantity() const
    {
        return quantity;
    }

    unsigned int getPrice()
    {
        return price;
    }

    bool is_valid()
    {
        return (quantity>=0 && price >=0 && (venue!=NULL && venue[0]!='\0'));
    }

    void setVenue(const char * venue_)
    {
        strcpy(venue,venue_);
    }

    void setQuantity(unsigned int quantity_)
    {
        quantity=quantity_;
    }

virtual unsigned int getOutstandingQuantity() const {
        return quantity;  // By default, it returns the quantity for an Order.
    }
};

class ClosedOrder : public Order {
    // Any new private members for ClosedOrder can go here

public:
    // Use the base class constructor to initialize members
    ClosedOrder(long timestamp_,
                bool is_buy_,
                unsigned int id_,
                unsigned int price_,
                unsigned int quantity_,
                const char * venue_,
                const char * symbol_,
                ordertype type_)
        : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {
        // Any additional initialization for ClosedOrder can go here
    }

    // Override the virtual function in the derived class
    unsigned int getOutstandingQuantity() const override {
        return 0;  // Always returns 0 for a ClosedOrder.
    }
};


class OpenOrder : public Order {
public:
    // Use the base class constructor to initialize members
    OpenOrder(long timestamp_,
              bool is_buy_,
              unsigned int id_,
              unsigned int price_,
              unsigned int quantity_,
              const char * venue_,
              const char * symbol_,
              ordertype type_)
        : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {
        // Any additional initialization for OpenOrder can go here
    }

};
class VectorOrders{
private:
    Order **orders;
    unsigned int capacity;
    unsigned int current_new_order_offset;
public:
    VectorOrders(unsigned int capacity_) : capacity(capacity_), current_new_order_offset(0) {
    orders = new Order*[capacity];
    for(unsigned int i = 0; i < capacity; i++) {
        orders[i] = NULL;
    }
}
VectorOrders(const VectorOrders& other) : capacity(other.capacity), current_new_order_offset(other.current_new_order_offset) {
    orders = new Order*[capacity];
    for(unsigned int i = 0; i < current_new_order_offset; i++) {
        // Assuming Order has a proper copy constructor
        orders[i] = new Order(*other.orders[i]);
    }
    for(unsigned int i = current_new_order_offset; i < capacity; i++) {
        orders[i] = NULL;
    }
}


    Order **get_order_list() const
    {
        return orders;
    }

    bool double_list_orders_size() {
    unsigned int new_capacity = capacity * 2;
    Order** new_orders = new Order*[new_capacity];

    for(unsigned int i = 0; i < capacity; i++) {
        new_orders[i] = orders[i];
    }
    for(unsigned int i = capacity; i < new_capacity; i++) {
        new_orders[i] = NULL;
    }

    delete[] orders;
    orders = new_orders;
    capacity = new_capacity;
    // std::cout<<capacity << std::endl;

    return true;
}



    bool add_order(Order *o) {
    // Check for existing order ID
    for(unsigned int i = 0; i < current_new_order_offset; i++) {
        if(orders[i]->getID() == o->getID()) {
            return false;
        }
    }

    // Check if we need to double the capacity
    // std::cout<<current_new_order_offset<<std::endl;
    if(current_new_order_offset == capacity-1) {
        // std::cout<<"bomb"<< std::endl;
        double_list_orders_size();
    }

    // Add the order
    orders[current_new_order_offset++] = o;
    return true;
}

    unsigned int get_size()
    {
        return current_new_order_offset;
    }

    unsigned int get_capacity()
    {
        return capacity;
    }

    void clear() {
    for(unsigned int i = 0; i < current_new_order_offset; i++) {
        delete orders[i];
        orders[i] = NULL;
    }
    current_new_order_offset = 0;
}

    bool delete_order(unsigned int id) {
    for(unsigned int i = 0; i < current_new_order_offset; i++) {
        if(orders[i]->getID() == id) {
            delete orders[i];
            for(unsigned int j = i; j < current_new_order_offset - 1; j++) {
                orders[j] = orders[j+1];
            }
            orders[current_new_order_offset - 1] = NULL;
            current_new_order_offset--;
            return true;
        }
    }
    return false;
}


    int get_total_volume() {
    int total = 0;
    for(unsigned int i = 0; i < current_new_order_offset; i++) {
        if(orders[i]) {
            total += orders[i]->getQuantity();
        }
    }
    return total;
}


    int get_total_outstanding_volume() {
    int total = 0;
    for(unsigned int i = 0; i < current_new_order_offset; i++) {
        if(orders[i]) {
            total += orders[i]->getOutstandingQuantity();
        }
    }
    return total;
}


    void dump_list_orders();
};