#include <queue>
#include <string.h>
#include <unordered_map>
#include <iostream>
#include <set>
#include <queue>

// change this to false when you are ready to submit
# define SHOW_LINE_NUM false
# define DEBUG_PASS false
# define DEBUG_FAIL false

typedef unsigned int price_t;
typedef unsigned int quantity_t;
typedef unsigned int level_t;



enum ordertype {LIMIT, MARKET};
enum orderstate {OPEN, ACKNOWLEDGED, FILLED, CANCELLED, REJECTED};


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
    }

    const char * getVenue() const { return venue; }
    const char * getSymbol() const{ return symbol; }
    unsigned int getID() const { return id; }
    ordertype getOrderType() const { return type; }
    void setType(ordertype e) { type=e; };
    bool is_valid() const {
        return (quantity>=0 && price >=0 && (venue!=NULL && venue[0]!='\0'));
    }
    unsigned int getQuantity() const { return quantity; }
    unsigned int getPrice() const { return price; }
    void setVenue(const char * venue_) { strcpy(venue,venue_); }
    void setQuantity(unsigned int quantity_) { quantity=quantity_; }
    void setSymbol(const char * symbol_) { strcpy(symbol,symbol_); }
    void setPrice(unsigned int price_) { price=price_; }
    void setSide(bool is_buy_) { is_buy = is_buy_; }
    void setOrderID(unsigned int id_) { id=id_; }
    unsigned int getTimeStamp() const {return timestamp;}
    bool isBuy() const {return is_buy;}

};


class ExecutionOrder : public Order
{
private:
    orderstate state;
    unsigned int execution_id;
public:
    ExecutionOrder():Order(),state(orderstate::OPEN) {}
    ExecutionOrder(const Order &o):Order(o.getTimeStamp(),
            o.isBuy(),o.getID(),o.getPrice(),o.getQuantity(),
            o.getVenue(),o.getSymbol(),o.getOrderType()){};
    orderstate getState() const {return state;};
    void setState(orderstate e){state=e;}
    void setExecutionID(unsigned int id){execution_id=id;}
    unsigned int getExecutionID() const { return execution_id;}
};



class BookUpdate {
private:
    level_t level;
    price_t price;
    quantity_t quantity;
    char venue[20];
    char symbol[20];
    bool is_buy;
public:
    BookUpdate(level_t level_,
            price_t price_,
            quantity_t quantity_,
            const char *venue_,
            bool is_buy_,
            const char *symbol_):
            level(level_),
            price(price_),
            quantity(quantity_),
            is_buy(is_buy_)
    {
        strcpy(venue,venue_);
        strcpy(symbol,symbol_);
    }
    price_t get_price(){return price;}
    quantity_t get_quantity(){return quantity;}
    level_t get_level(){return level;}
    const char * get_venue(){return venue;}
    bool get_is_buy(){return is_buy;}
    const char * get_symbol(){return symbol;}
};



class AppBase
{
protected:
    bool is_working;
    std::queue<Order> &strategy_to_ordermanager;
    std::queue<ExecutionOrder> &ordermanager_to_strategy;
    std::queue<Order> &ordermanager_to_simulator;
    std::queue<ExecutionOrder> &simulator_to_ordermanager;

public:
    AppBase(std::queue<Order> &strategy_to_ordermanager_,
    std::queue<ExecutionOrder> &ordermanager_to_strategy_,
    std::queue<Order> &ordermanager_to_simulator_,
    std::queue<ExecutionOrder> &simulator_to_ordermanager_):
            strategy_to_ordermanager(strategy_to_ordermanager_),
            ordermanager_to_strategy(ordermanager_to_strategy_),
            ordermanager_to_simulator(ordermanager_to_simulator_),
            simulator_to_ordermanager(simulator_to_ordermanager_)
    {
        is_working=false;
    }
    virtual void start() = 0;
    virtual void stop() = 0;

};


class MarketSimulator : public AppBase {
public:
    unsigned execution_id;
    std::set<std::string> list_symbols;
    MarketSimulator(
        std::queue<Order> &strategy_to_ordermanager_,
        std::queue<ExecutionOrder> &ordermanager_to_strategy_,
        std::queue<Order> &ordermanager_to_simulator_,
        std::queue<ExecutionOrder> &simulator_to_ordermanager_
        ):AppBase(strategy_to_ordermanager_,
                ordermanager_to_strategy_,
                ordermanager_to_simulator_,
                 simulator_to_ordermanager_),
                 execution_id(0)
    {
        list_symbols = {"MSFT", "AAPL", "LUV", "SKYW","UAL","HA","RYAAY","DAL"};
    }
    bool handle_order(){
        if(!is_working)
            return false;
        if(ordermanager_to_simulator.empty())
            return true;
        const Order &o = ordermanager_to_simulator.front();
        ordermanager_to_simulator.pop();

        const bool is_tradeable = list_symbols.find(o.getSymbol()) != list_symbols.end();

        ExecutionOrder new_execution(o);
        if(is_tradeable){
            new_execution.setState(o.getQuantity()>1000?orderstate::ACKNOWLEDGED:orderstate::REJECTED);
            new_execution.setExecutionID(execution_id++);
            simulator_to_ordermanager.push(new_execution);
            new_execution.setState(orderstate::FILLED);
            //    std::cout << "simulator push a fill|" <<
            //                new_execution.getPrice() << "|"  <<
            //                new_execution.isBuy()  << "|" <<
            //                new_execution.getQuantity() << "|" <<
            //                std::endl;
            simulator_to_ordermanager.push(new_execution);
        }
        else{
            // std::cout << "simulator push a reject" << std::endl;
            new_execution.setState(orderstate::REJECTED);
            new_execution.setExecutionID(execution_id++);
            simulator_to_ordermanager.push(new_execution);
        }
        return true;
    }

    virtual void start() {is_working=true;}
    virtual void stop() {is_working=false;}

};


class OrderManager : public AppBase{
public:
    unsigned int order_id;
    Order order;
    ExecutionOrder execution_order;
    std::unordered_map<unsigned int, ExecutionOrder> list_orders;
    std::unordered_map<std::string,int> positions;
    OrderManager(
                           std::queue<Order> &strategy_to_ordermanager_,
                           std::queue<ExecutionOrder> &ordermanager_to_strategy_,
                           std::queue<Order> &ordermanager_to_simulator_,
                           std::queue<ExecutionOrder> &simulator_to_ordermanager_
                   ):AppBase(strategy_to_ordermanager_,
            ordermanager_to_strategy_,
            ordermanager_to_simulator_,
            simulator_to_ordermanager_)
            ,order_id(0){};

    virtual void start() {
        is_working=true;
    }
    virtual void stop() {
        list_orders.clear();
        is_working=false;
    }

    void reset_position(){
        positions.clear();
    }

    bool handle_order() {
        if(!is_working)
            return false;
        if (strategy_to_ordermanager.empty())
            return true;
        const Order &e = strategy_to_ordermanager.front();
        
        int side = e.isBuy() ? 1 : -1;
        if((get_position(e.getSymbol()) + static_cast<int>(e.getQuantity())*static_cast<int>(e.getPrice())*side <= 1000000) && (get_position(e.getSymbol()) + static_cast<int>(e.getQuantity())*static_cast<int>(e.getPrice())*side >= -1000000) && get_number_of_non_acknowledged_orders() < 11){
            order = e;
            ordermanager_to_simulator.push(order);
        }
        else{
            // std::cout<< get_number_of_non_acknowledged_orders()<<std::endl;
            ExecutionOrder new_execution(e);
            new_execution.setState(orderstate::REJECTED);
            ordermanager_to_strategy.push(new_execution);
            list_orders[e.getID()] = e;
        }
        strategy_to_ordermanager.pop();
        return true;
    }
    bool handle_execution_order() {
        if(!is_working)
            return false;
        if (simulator_to_ordermanager.empty())
            return true;

        const ExecutionOrder &e = simulator_to_ordermanager.front();
        simulator_to_ordermanager.pop();
        ordermanager_to_strategy.push(e);
        if (e.getState() == orderstate::FILLED){
        int side = e.isBuy() ? 1:-1;
        positions[e.getSymbol()] += side * e.getPrice()* e.getQuantity();
        }
        list_orders[e.getID()] = e;
        return true;
    }
    unsigned int get_number_of_open_orders() { 
        int num = 0;
        for (std::unordered_map<unsigned int, ExecutionOrder>::iterator it = list_orders.begin();it!=list_orders.end();it++){
             if(it->second.getState() == orderstate::ACKNOWLEDGED){
                num++;
        }
        
        }
        num += ordermanager_to_simulator.size();
        return num;
    }
    unsigned int get_number_of_non_acknowledged_orders() { return ordermanager_to_simulator.size(); }
    int get_position(std::string symbol) { return positions[symbol]; }

};


class Signal{
public:
    bool is_tradeable(BookUpdate &bu){return true;}
};

class Execution{
private:
    Order e;
    bool tradeable;
public:

    Execution():tradeable(false){}
    bool insert_order(long timestamp_,
                      bool is_buy_,
                      unsigned int price_,
                      unsigned int quantity_,
                      const char * venue_,
                      const char * symbol_,
                      ordertype type_,
                      unsigned int id_) 
    {
        e.setSide(is_buy_);
        e.setPrice(price_);
        e.setQuantity(quantity_);
        e.setVenue(venue_);
        e.setType(type_);
        e.setSymbol(symbol_);
        e.setOrderID(id_);
        return true;
    }
    bool is_tradeable() {return tradeable;}
    void set_tradeable(bool is_tradable){tradeable=is_tradable;};
    Order & get_order() {return e;}
};

class TradingStrategy : public AppBase {
private:
    Signal signal;
    Execution execution;
    int order_id;
    std::unordered_map<std::string,int> positions;
    unsigned int number_of_rejections;
    unsigned int number_of_fills;

public:
    TradingStrategy(
            std::queue<Order> &strategy_to_ordermanager_,
            std::queue<ExecutionOrder> &ordermanager_to_strategy_,
            std::queue<Order> &ordermanager_to_simulator_,
            std::queue<ExecutionOrder> &simulator_to_ordermanager_
    ):
    AppBase(strategy_to_ordermanager_,
              ordermanager_to_strategy_,
              ordermanager_to_simulator_,
              simulator_to_ordermanager_),
              signal(),
              execution(),
              order_id(1),
              number_of_rejections(0),
              number_of_fills(0){}
    virtual void start() {is_working=true;}
    virtual void stop() {
        positions.clear();
        is_working=false;
    }


    bool process_book_update(BookUpdate &bu) {
        if(!is_working)
            return false;
        if (signal.is_tradeable(bu)) {
            execution.insert_order(0,bu.get_is_buy(),
                                   bu.get_price(),bu.get_quantity(),bu.get_venue(),bu.get_symbol(),ordertype::LIMIT,order_id++);
            execution.set_tradeable(true);
        }
        return process_execution();
    }

    bool process_execution() {
        if(!is_working)
            return false;
        Order order;
        if (execution.is_tradeable()) {
            order.setType(execution.get_order().getOrderType());
            order.setVenue(execution.get_order().getVenue());
            order.setQuantity(execution.get_order().getQuantity());
            order.setPrice(execution.get_order().getPrice());
            order.setOrderID(execution.get_order().getID());
            order.setSymbol(execution.get_order().getSymbol());
            order.setSide(execution.get_order().isBuy());
            execution.set_tradeable(false);
            strategy_to_ordermanager.push(order);
        }
        return true;
    }
    bool process_market_response() {
        if(ordermanager_to_strategy.empty())
            return true;
        ExecutionOrder &order = ordermanager_to_strategy.front();
        ordermanager_to_strategy.pop();
        if(order.getState() == orderstate::FILLED){
        int side = order.isBuy() ? 1:-1;
        positions[order.getSymbol()] += side * order.getQuantity()*order.getPrice();
        number_of_fills++;}
        if(order.getState() == orderstate::REJECTED)
            number_of_rejections++;
        
        return true;
    }
    int get_position(std::string symbol) { return positions[symbol]; }
    unsigned int get_number_of_rejections() { return number_of_rejections; }
    unsigned int get_number_of_fills() { return number_of_fills; }
    void reset_position() { positions.clear(); }

};


int number_of_tests = 0;
int number_of_pass = 0;

#define TEST_FUNCTION(X,Y) {number_of_tests++; \
                            if (X==Y) \
                            { \
                                if (SHOW_LINE_NUM && DEBUG_PASS) { std::cout << "line:" << __LINE__ << " ";}; \
                                if (DEBUG_PASS) { std::cout << "PASS" << std::endl;}; \
                                number_of_pass++; \
                            } \
                            else \
                            { \
                                if (SHOW_LINE_NUM && DEBUG_FAIL) { std::cout << "line:" << __LINE__ << " ";}; \
                                if (DEBUG_FAIL) { std::cout << "FAIL EXPECTED: " << Y << " RECEIVED: " << X << std::endl;} \
                            }}

# define TEST_STRING(X,Y) {number_of_tests++; \
                            if (strcmp(X,Y)==0) \
                            { \
                                if (SHOW_LINE_NUM && DEBUG_PASS) { std::cout << "line:" << __LINE__ << " ";}; \
                                if (DEBUG_PASS) { std::cout << "PASS" << std::endl;}; \
                                number_of_pass++; \
                            } \
                            else \
                            { \
                                if (SHOW_LINE_NUM && DEBUG_FAIL) { std::cout << "line:" << __LINE__ << " ";}; \
                                if (DEBUG_FAIL) { std::cout << "FAIL EXPECTED: " << Y << " RECEIVED: " << X << std::endl;} \
                            }}


# define PRINT_WARNING() { if (DEBUG_FAIL || DEBUG_PASS) { \
        std::cout << "Be sure to turn off DEBUG_PASS and DEBUG_FAIL when you are ready to submit" << std::endl;}}

#define PRINT_RESULTS() {std::cout << "You succeeded to pass " << number_of_pass << "/" << number_of_tests;}





void reset_communication(
        std::queue<Order> &strategy_to_ordermanager,
        std::queue<ExecutionOrder> &ordermanager_to_strategy,
        std::queue<Order> &ordermanager_to_simulator,
        std::queue<ExecutionOrder> &simulator_to_ordermanager
        )
{
    while (!strategy_to_ordermanager.empty()) strategy_to_ordermanager.pop();
    while (!ordermanager_to_strategy.empty()) ordermanager_to_strategy.pop();
    while (!ordermanager_to_simulator.empty()) ordermanager_to_simulator.pop();
    while (!simulator_to_ordermanager.empty()) simulator_to_ordermanager.pop();
}

int main() {
    /* Create an order o1 on the stack with the following parameter
     *
     * (100,true,1,10,1000,"JPM","APPL",ordertype::LIMIT);
     *
     * */
    PRINT_WARNING();

    std::queue<Order> strategy_to_ordermanager;
    std::queue<ExecutionOrder> ordermanager_to_strategy;
    std::queue<Order> ordermanager_to_simulator;
    std::queue<ExecutionOrder> simulator_to_ordermanager;

    Order o1(100,true,1,10,1000,"JPMX","EURUSD",ordertype::LIMIT);

    TEST_FUNCTION(o1.getID(),1);
    TEST_FUNCTION(o1.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1.getPrice(),10);
    TEST_FUNCTION(o1.getQuantity(),1000);
    TEST_STRING(o1.getVenue(),"JPMX");
    TEST_STRING(o1.getSymbol(),"EURUSD");
    TEST_FUNCTION(o1.is_valid(), true);
    o1.setVenue("\0");
    TEST_FUNCTION(o1.is_valid(), false);
    o1.setVenue("BARX");
    TEST_FUNCTION(o1.is_valid(), true);
//9
    BookUpdate bu1(0,10,10000,"BARX",true,"MSFT");
    MarketSimulator simulator(strategy_to_ordermanager,
                              ordermanager_to_strategy,
                              ordermanager_to_simulator,
                              simulator_to_ordermanager);
    OrderManager order_manager(strategy_to_ordermanager,
                               ordermanager_to_strategy,
                               ordermanager_to_simulator,
                               simulator_to_ordermanager);
    ExecutionOrder eo1;
    TradingStrategy ts1(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);
    simulator.start();
    order_manager.start();
    ts1.start();

    TEST_FUNCTION(ts1.process_book_update(bu1),true);
    const Order o1verif=strategy_to_ordermanager.front();
    TEST_FUNCTION(o1verif.getID(),1);
    TEST_FUNCTION(o1verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1verif.getPrice(),10);
    TEST_FUNCTION(o1verif.getQuantity(),10000);
    TEST_STRING(o1verif.getVenue(),"BARX");
    TEST_STRING(o1verif.getSymbol(),"MSFT");

    TEST_FUNCTION(order_manager.handle_order(),true);
//17
    const Order o2verif=ordermanager_to_simulator.front();
    TEST_FUNCTION(o2verif.getID(),1);
    TEST_FUNCTION(o2verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o2verif.getPrice(),10);
    TEST_FUNCTION(o2verif.getQuantity(),10000);
    TEST_STRING(o2verif.getVenue(),"BARX");
    TEST_STRING(o2verif.getSymbol(),"MSFT");

//23
    simulator.handle_order();
    const ExecutionOrder o3verif=simulator_to_ordermanager.front();
    TEST_FUNCTION(o3verif.getID(),1);
    TEST_FUNCTION(o3verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o3verif.getPrice(),10);
    TEST_FUNCTION(o3verif.getQuantity(),10000);
    TEST_STRING(o3verif.getVenue(),"BARX");
    TEST_STRING(o3verif.getSymbol(),"MSFT");
    TEST_FUNCTION(o3verif.getExecutionID(),0);
    TEST_FUNCTION(o3verif.getState(),orderstate::ACKNOWLEDGED);
//31

    /* Simulator sends a ack */
    TEST_FUNCTION(order_manager.handle_execution_order(),true);
    const ExecutionOrder o4verif=ordermanager_to_strategy.front();
    TEST_FUNCTION(o4verif.getID(),1);
    TEST_FUNCTION(o4verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o4verif.getPrice(),10);
    TEST_FUNCTION(o4verif.getQuantity(),10000);
    TEST_STRING(o4verif.getVenue(),"BARX");
    TEST_STRING(o4verif.getSymbol(),"MSFT");
    TEST_FUNCTION(o4verif.getExecutionID(),0);
    TEST_FUNCTION(o4verif.getState(),orderstate::ACKNOWLEDGED);
    ts1.process_market_response();
    /* Simulator sends a fill */
    TEST_FUNCTION(order_manager.handle_execution_order(),true);
    const ExecutionOrder o5verif=ordermanager_to_strategy.front();
    TEST_FUNCTION(o5verif.getID(),1);
    TEST_FUNCTION(o5verif.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o5verif.getPrice(),10);
    TEST_FUNCTION(o5verif.getQuantity(),10000);
    TEST_STRING(o5verif.getVenue(),"BARX");
    TEST_STRING(o5verif.getSymbol(),"MSFT");
    TEST_FUNCTION(o5verif.getExecutionID(),0);
    TEST_FUNCTION(o5verif.getState(),orderstate::FILLED);
    ts1.process_market_response();
    TEST_FUNCTION(ts1.get_position("MSFT"),100000);
//50



    /* New Book Update - When trying to get this one the market simulator
     * doesnt have the liquidity any more
     *
     */

    BookUpdate bu2(0,10,10000,"BARX",true,"AAA");
    TEST_FUNCTION(ts1.process_book_update(bu2),true);
    order_manager.handle_order();
    simulator.handle_order();
    order_manager.handle_execution_order();
    ts1.process_market_response();
    TEST_FUNCTION(ts1.get_number_of_rejections(),1);

    for(int k=0;k<8;k++)
    {
        BookUpdate bu3(0,10,10000,"BARX",true,"MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_number_of_rejections(),1);
    TEST_FUNCTION(ts1.get_number_of_fills(),9);
    TEST_FUNCTION(ts1.get_position("MSFT"),900000);

    for(int k=0;k<3;k++)
    {
        BookUpdate bu3(0,9,2000,"BARX",false,"MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_number_of_fills(),12);
    TEST_FUNCTION(ts1.get_position("MSFT"),846000);

    TEST_FUNCTION(order_manager.get_number_of_non_acknowledged_orders(),0);

    for(int k=0;k<3;k++) {
        BookUpdate bu3(0, 9, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
    }
//59
    TEST_FUNCTION(order_manager.get_number_of_non_acknowledged_orders(),3);
    simulator.handle_order();
    order_manager.handle_execution_order();
    ts1.process_market_response();

    TEST_FUNCTION(order_manager.get_number_of_non_acknowledged_orders(),2);
    TEST_FUNCTION(order_manager.get_number_of_open_orders(),3);

    simulator.stop();
    order_manager.stop();
    ts1.stop();
    reset_communication(strategy_to_ordermanager,
            ordermanager_to_strategy,
            ordermanager_to_simulator,
            simulator_to_ordermanager);
    simulator.start();
    order_manager.start();
    ts1.start();


    for(int k=0;k<12;k++) {
        BookUpdate bu3(0, 1, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        ts1.process_market_response();
    }


    TEST_FUNCTION(ts1.get_number_of_rejections(),2);

    for(int k=0;k<10;k++) {
        BookUpdate bu3(0, 1, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_number_of_rejections(),12);

    for(int k=0;k<10;k++) {
        BookUpdate bu3(0, 1, 2000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(ts1.get_position("MSFT"),-18000);


    simulator.stop();
    order_manager.stop();
    ts1.stop();
    reset_communication(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);
    order_manager.reset_position();
    ts1.reset_position();
    simulator.start();
    order_manager.start();
    ts1.start();


    {
        BookUpdate bu3(0, 1, 10000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }

    TEST_FUNCTION(order_manager.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_number_of_rejections(),13);

    {
        BookUpdate bu3(0, 100, 10000, "BARX", false, "MSFT");
        ts1.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts1.process_market_response();
        ts1.process_market_response();
    }



    TEST_FUNCTION(order_manager.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_position("MSFT"),-10000);
    TEST_FUNCTION(ts1.get_number_of_rejections(),14);

    TradingStrategy ts2(strategy_to_ordermanager,
                        ordermanager_to_strategy,
                        ordermanager_to_simulator,
                        simulator_to_ordermanager);

    ts2.start();

    {
        BookUpdate bu3(0, 1, 20000, "BARX", false, "MSFT");
        ts2.process_book_update(bu3);
        order_manager.handle_order();
        simulator.handle_order();
        order_manager.handle_execution_order();
        order_manager.handle_execution_order();
        ts2.process_market_response();
        ts2.process_market_response();
    }

    TEST_FUNCTION(order_manager.get_position("MSFT"),-30000);
    TEST_FUNCTION(ts2.get_position("MSFT"),-20000);
    TEST_FUNCTION(ts2.get_number_of_rejections(),0);


    /* Write the unit tests for amendment after this part */

    PRINT_RESULTS();
    return 0;
}