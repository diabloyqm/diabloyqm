#include <iostream>
#include <thread>

using namespace std;

int main()
{
    int balance = 0;

    // Launch a thread for increasing the balance
    thread t1([&balance] { for (int k = 0; k < 1000000; k++) balance++; });

    // Launch a thread for decreasing the balance
    thread t2([&balance] { for (int k = 0; k < 1000000; k++) balance--; });

    // Join the threads with the main thread
    t1.join();
    t2.join();

    cout << "balance=" << balance << endl;

    return 0;
}
