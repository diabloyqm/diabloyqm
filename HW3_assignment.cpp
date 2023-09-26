#include <bits/stdc++.h>
#include <string>
#include <iostream>


using namespace std;

class Car {
protected:
    bool isSedan;
    string seats;
    
public:
    Car(bool isSedan, string seats) {
        this->isSedan = isSedan;
        this->seats = seats;
    }

    bool getIsSedan() {
        return this->isSedan;
    }

    string getSeats() {
        return this->seats;
    }

    virtual string getMileage() = 0;

    void printCar(string name) {
        std::cout << "A " << name << " is " << (this->getIsSedan() ? "" : "not ")
            <<"Sedan, is " << this->getSeats() << "-seater, and has a mileage of around "
            << getMileage() << ".\n"<< std::endl;
    }
};
// Write your code here.

/**
*   WagonR class
**/
class WagonR: virtual public Car{
    int mileage;
public: 
WagonR(int x):
 mileage(x),Car(false, "4")
 {};
 
    std::string getMileage(){
        return std::to_string(mileage) + " kmpl" ; 
    }
};

/**
*   HondaCity class
**/
class HondaCity: virtual public Car{
    int mileage;
public: 
HondaCity(int x):
mileage(x),Car(true, "4")
 {};
 
    std::string getMileage(){
        return std::to_string(mileage) + " kmpl" ; 
    }
};
/**
*   InnovaCrysta class
**/
class InnovaCrysta: virtual public Car{
    int mileage;
public: 
InnovaCrysta(int x):
 mileage(x),Car(false, "6")
 {};
 
    std::string getMileage(){
        return std::to_string(mileage) + " kmpl" ; 
    }
};

int main()
{
    return 0;
}