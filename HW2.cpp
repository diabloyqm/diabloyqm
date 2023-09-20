#include <stdio.h>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

/*
 * Complete the 'countPowerNumbers' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER l
 *  2. INTEGER r
 */

int countPowerNumbers(int l, int r) {

   vector<long long> pows ;

    // Push 1 and 0 in it
    pows.push_back(0);
    pows.push_back(1);
 
    // Iterate over all the exponents
    for (int p = 2; p < 25; p++) {
 
        // Iterate over all possible numbers
        long long int num = 2;
 
        // This loop will run for a
        // maximum of sqrt(R) times
        while ((long long int)(pow(num, p) + 0.5) <= r) {
 
            // Push this power in
            // the array pows[]
            pows.push_back(
                (long long int)(pow(num, p) + 0.5));
 
            // Increase the number
            num++;
        }
    }
 
    // Stores if i can be expressed as
    // the sum of perfect power or not
    int ok[r + 1];
    memset(ok, 0, sizeof(ok));
 
    // Iterate over all possible pairs
    // of the array pows[]
    for (int i = 0;
         i < pows.size(); i++) {
 
        for (int j = 0;
             j < pows.size(); j++) {
 
            if (pows[i] + pows[j] <= r
                and pows[i] + pows[j] >= l) {
 
                // The number is valid
                ok[pows[i] + pows[j]] = 1;
            }
        }
    }
 
    // Find the prefix sum of the
    // array ok[]
    for (int i = 0; i <= r; i++) {
        ok[i] += ok[i - 1];
    }
 
    // Return the count of required number
    return ok[r] - ok[l - 1];
    // return 0;
    
}
int main(){
return 0;
}