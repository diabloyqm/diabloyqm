#include <iostream>
#include <string.h>

using namespace std;

enum side_t {BUY, SELL};

int count_number_of_copy_constructor;

class Trader{
public:
    char name[20];
    Trader(const char * n)
    {
        std::cout << & n << std::endl;
        strcpy(name,n);
        std::cout << n[1] << std::endl;
    }
};

class Order{
private:
    int price;
    int quantity;
    side_t side;
public:
    Trader *trader;
    Order(int p, int q, side_t s,Trader *t):price(p),
                                quantity(q),
                                side(s),
                                trader(t){

    };

   Order(const Order& o):price(o.price),
                        quantity(o.quantity),
                        side(o.side){
       count_number_of_copy_constructor++;
       cout << "copy constructor called" << endl;
       char tab[20];
       int offset=0;
       strcpy(tab,o.trader->name);
       offset=strlen(tab);
       tab[offset]='0'+count_number_of_copy_constructor*5;
       tab[offset+1]='\0';
       trader=new Trader(tab);
       
   }
};



int main() {
    count_number_of_copy_constructor=0;
    Trader t1("seb");
    Order o1(10,100,side_t::BUY,&t1);
    Order o2(o1); // Call the copy constructor
    Order o3 = o1; // Call the copy constructor
    cout << "order1 trader:" << o1.trader->name << endl;
    cout << "order2 trader:" << o2.trader->name << endl;
    cout << "order3 trader:" << o3.trader->name << endl;

    return 0;
}

// #include <iostream>
// #include <string>
// #include <stdlib.h>
// #include <unordered_map>
// #include <cstring>

// using namespace std;
// enum ordertype { LIMIT, MARKET };

// class Order{
// private:
//     long timestamp; // epoch time: the number of seconds that have elapsed since 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970,.
//     bool is_buy; // buy or sell
//     unsigned int id; // order id – unique identifier
//     unsigned int price;
//     unsigned int quantity;
//     char venue[20]; // company name where this order comes from
//     char symbol[20];
//     ordertype type;
// public:
// Order(long timestamp_=0,
//           bool is_buy_=true,
//           unsigned int id_=0,
//           unsigned int price_=0,
//           unsigned int quantity_=0,
//           const char * venue_="",
//           const char * symbol_="",
//           ordertype type_=ordertype::MARKET)
//           {
//         is_buy=is_buy_;
//         timestamp=timestamp_;
//         id=id_;
//         price=price_;
//         quantity=quantity_;
//         strcpy(venue,venue_);
//         type=type_;
//         strcpy(symbol,symbol_);
//     }
//     const char * getVenue(){ return venue; }
//     const char * getSymbol() const{ return symbol; }
//     unsigned int getID() const { return id; }
//     ordertype getOrderType() const { return type; }
//     void setType(ordertype e) { type=e; };
//     bool is_valid() const {
//         return (quantity>=0 && price >=0 && (venue!=NULL && venue[0]!='\0'));
//     }
//     unsigned int getQuantity() const { return quantity; }
//     unsigned int getPrice() const { return price; }
//     void setVenue(const char * venue_) { strcpy(venue,venue_); }
//     void setQuantity(unsigned int quantity_) { quantity=quantity_; }
//     void setSymbol(const char * symbol_) { strcpy(symbol,symbol_); }
//     void setPrice(unsigned int price_) { price=price_; }
//     void setSide(bool is_buy_) { is_buy = is_buy_; }
//     void setOrderID(unsigned int id_) { id=id_; }
//     unsigned int getTimeStamp() const {return timestamp;}
//     bool isBuy() const {return is_buy;}
// };