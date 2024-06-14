#include <iostream>
#include "Single_Linked_List.h"
#include "Insert.h"

int main(){
    SingleLinkedList<int> ll;
    ll.push_front(1);
    ll.push_back(3);
    ll.insert(1, 2);
    ll.insert(0, 0);
    ll.print();
    return 0;
}