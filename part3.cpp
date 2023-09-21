#include <stdio.h>
#include <iostream>


int main(){
class student{
std::string name;
    
public:
   
    student(std::string stname): name(stname){}
    void set(std::string name2){
        name = name2;
    
    }
    std::string get(){
        return name;
    }
};
    student stdu("haha");
    stdu.set("haha2");
    std::cout << stdu.get()<< std::endl;
    return 0;

}




