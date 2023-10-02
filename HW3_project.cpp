// #include <iostream>
// #include <string.h>

// using namespace std;

// enum side_t {BUY, SELL};

// int count_number_of_copy_constructor;

// class Trader{
// public:
//     char name[20];
//     Trader(const char * n)
//     {
//         std::cout << & n << std::endl;
//         strcpy(name,n);
//         std::cout << n<< std::endl;
//     }
// };

// class Order{
// private:
//     int price;
//     int quantity;
//     side_t side;
// public:
//     Trader *trader;
//     Order(int p, int q, side_t s,Trader *t):price(p),
//                                 quantity(q),
//                                 side(s),
//                                 trader(t){

//     };

//    Order(const Order& o):price(o.price),
//                         quantity(o.quantity),
//                         side(o.side){
//        count_number_of_copy_constructor++;
//        cout << "copy constructor called" << endl;
//        char tab[20];
//        int offset=0;
//        strcpy(tab,o.trader->name);
//        offset=strlen(tab);
//        tab[offset]='0'+count_number_of_copy_constructor*5;
//        tab[offset+1]='\0';
//        trader=new Trader(tab);
       
//    }
// };


#include <iostream>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <cstring>

using namespace std;
enum ordertype { LIMIT, MARKET };

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
    Order(long timestamp_=0,
          bool is_buy_=true,
          unsigned int id_=0,
          unsigned int price_=0,
          unsigned int quantity_=0,
          const char * venue_="",
          const char * symbol_="",
          ordertype type_=ordertype::MARKET)
    {
        is_buy=is_buy_;
        timestamp=timestamp_;
        id=id_;
        price=price_;
        quantity=quantity_;
        strcpy(venue,venue_);
        type=type_;
        strcpy(symbol,symbol_);
        type = type_;
    }

    const char * getVenue() { 
        return venue;
        }
    const char * getSymbol() { 
        return symbol;
        }
    unsigned int getID() {
         return id; 
         }
    ordertype getOrderType()  
    { return type;
    }
    void setType(ordertype e) { type=e; };
    bool is_valid()  {
        return (quantity>=0 && price >=0 && (venue!=NULL && venue[0]!='\0'));
    }
    unsigned int getQuantity() const { return quantity; }
    unsigned int getPrice() const { return price; }
    
    void setVenue(const char * venue_) { strcpy(venue,venue_); }
    void setQuantity(unsigned int quantity_) { quantity=quantity_; }
    
    unsigned int getTimeStamp() const {return timestamp;}


};

class OrderBook {
private:
    Order* order_bids[20]={nullptr} ; // initialize all pointers to nullptr
    Order* order_offers[20]={nullptr}  ;
    int number_of_bids = 0;
    int number_of_offers = 0;

public:

     OrderBook() {}


    ~OrderBook() {
    }
    bool add_order(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
                   unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_);
                   
    bool modify_order(bool is_buy_, unsigned int id_, unsigned int quantity_);
    
    bool remove_order(bool is_buy_, unsigned int id_);
    
    void clearBooks() {
        for(int k = 0; k < 20; k++) {
            if (order_bids[k] != NULL) {
                delete order_bids[k];
                order_bids[k] = NULL;
                number_of_bids--;
            }
            if (order_offers[k] != NULL) {
                delete order_offers[k];
                order_offers[k] = NULL;
                number_of_offers--;
            }
        }
    }

    Order* getOrderBids(const int a) {
        return order_bids[a];
    }

    Order* getOrderOffers(const int a) {
        return order_offers[a];
    }

    int getNumberOfBids() {
        return number_of_bids;
    }

    int getNumberOfOffers() {
        return number_of_offers;
    }
};

bool OrderBook::add_order(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
                          unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_) {

    Order** array = is_buy_ ? order_bids : order_offers;

    // Check if an order with the same ID already exists
    if (array[id_] != nullptr) {
        return false;
    }

    // Add the new order
    array[id_] = new Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_);

    // Update counts
    if (is_buy_) {
        number_of_bids++;
    } else {
        number_of_offers++;
    }
    return true;
}

bool OrderBook::modify_order(bool is_buy_, unsigned int id_, unsigned int quantity_) {
    // Choose the right array based on is_buy_
    Order** array = is_buy_ ? order_bids : order_offers;

    // Check if the order exists
    if (array[id_] == nullptr) {
        return false;
    }

    // Modify its quantity
    array[id_]->setQuantity(quantity_);
    return true;
}

bool OrderBook::remove_order(bool is_buy_, unsigned int id_) {
    // Choose the right array based on is_buy_
    Order** array = is_buy_ ? order_bids : order_offers;

    // Check if the order exists
    if (array[id_] == nullptr) {
        return false;
    }

    // Remove the order
    delete array[id_];
    array[id_] = nullptr;

    // Update counts
    if (is_buy_) {
        number_of_bids--;
    } else {
        number_of_offers--;
    }
    return true;
}