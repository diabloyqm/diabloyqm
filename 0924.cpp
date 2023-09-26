
#include <string.h>
#include <stdio.h>
// chain list
#include <iostream>
struct ListNode{
    int val;
    ListNode *next; //下一个节点的指针
    ListNode(int x ): val(x), next(NULL){}//节点的构造函数
};
ListNode* removeElements(ListNode* head, int val){

    while (head != NULL && head -> val == val) {
        ListNode* tempo = head;
        head = head -> next;
        delete tempo;
    };
    ListNode* current = head;
    while(current != NULL && current ->next != NULL){
        if (current -> next ->val ==val){
            ListNode* tempo =current -> next;
            current -> next = current->next->next;
            delete tempo;

        }
        else{
            current = current -> next; 
        }
    };
    return head;
};
int main()
{
return 0;

}
// using namespace std;

// enum side_t {BUY, SELL};

// int count_number_of_copy_constructor;

// class Trader{
// public:
//     char name[20];
//     Trader(const char *n)
//     {
//         strcpy(name,n);
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
//        tab[offset]='0'+count_number_of_copy_constructor;
//        tab[offset+1]='\0';
//        trader=new Trader(tab);
//    }
// };



// int main() {
//     count_number_of_copy_constructor=0;
//     Trader t1("seb");
//     Order o1(10,100,side_t::BUY,&t1);
//     Order o2(o1); // Call the copy constructor
//     Order o3 = o1; // Call the copy constructor
//     cout << "order1 trader:" << o1.trader->name << endl;
//     cout << "order2 trader:" << o2.trader->name << endl;
//     cout << "order3 trader:" << o3.trader->name << endl;

//     return 0;
// }