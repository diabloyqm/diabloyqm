
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <stdio.h>
#include <map>

using namespace std;


long maximumProfit(vector<int> price) {
    
int size = price.size();


long int sum_profit = 0;

int currentMax = price.back(); // Initialize to the last value in maxs
    for (int k =size - 1; k >= 0; k--) {
        
        if (price[k] > currentMax) {
            currentMax = price[k];
        }
        sum_profit += (currentMax - price[k]);
        
    }

return sum_profit;
};

int main(){
    int arr[] = {1, 10, 2, 11,20};
std::vector<int> c(arr, arr + sizeof(arr) / sizeof(int));

    // for (int i =1;i<4; i ++){
    //     c.push_back(i);
    //     std::cout<< i <<std::endl;
    // }
    int a = maximumProfit(c);

    std::cout << a <<std::endl;
    return 0;
}