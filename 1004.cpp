#include <string.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;
template <class R> 
class Order {
int a;
public:

    void display(){
    //    stati/c_cast<R*>(this) -> display();
// cout <<"odd"<< endl;
    }
    // void display1(){
    //     cout <<"order" << endl;
    // }
};
class FOKOrder : public Order<FOKOrder>{
public:
    void display(){
        cout<< "fokorder"<<endl;
    }
};

// template <class T>
/*
compiler will recognize different function call. eg, one interger ,
one string
*/
 
// T f(T a, T b){
//     return a + b;
// };

// template <class T1, class T2>

// T1 f(T1 a , T2 b){
//     return true;
// };
// int f(int a, int b){
//     return  a +b;
// }
// string f(string a, string b){
//     return  a + b ;
// };
// #define macrof(A,B)(A+B)
// template <class T2>
// class my_vector{
// T2 tab[20];
// int capacity;
// public:
// my_vector(int capacity_ = 0):capacity(capacity_){};

// void push_back(T2 a ){
//     if (capacity < 20){
//         tab[capacity] = a ;
//         ++ capacity;
    
//     }
//     else if(capacity == 20){
//         std::cout <<"nooooooo"<< std::endl;
//     }
// //
// };
// };
int main(){
    // FOKOrder fok;
    // fok.display();

    Order<FOKOrder> * i2;
    // i2->display();

    // my_vector<int> listinteger;
    // listinteger.push_back(1);
    // listinteger.push_back(2);
    // listinteger.push_back(2);
    // my_vector<string> listinteger2;
    // listinteger2.push_back("1");
    // listinteger2.push_back("2");
    // listinteger2.push_back("2");
    int a =2;
    int b =3;
    std::string aa= "2";
    string bb ="3";
    // std::cout << f<int>(a,b) << std::endl;
    // std::cout << f<string>(aa,bb)<< std::endl;

}