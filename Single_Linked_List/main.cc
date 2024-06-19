#include <iostream>
#include "Single_Linked_List.h"
#include "Insert.h"
#include "Search.h"

int main(){
    SingleLinkedList<int> ll;
    ll.push_front(1);
    ll.push_back(3);
    ll.insert(1, 2);
    if(ll.search_iterative(4))
        ll.print();
    return 0;
}