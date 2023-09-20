#include <stdio.h>
#include <iostream>
using std::cout;
using std::endl;
int main()
{
    int sum =0 , val = 1;
    while (val<=10){
        sum +=val;
        ++val;

    }
    std::cout << "Sum of is "<<sum <<std::endl;
    return 0;

}