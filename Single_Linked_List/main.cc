#include <iostream>
#include "Single_Linked_List.h"
#include "Insert.h"
#include "Search.h"
#include "Get_Size.h"

int main(){
    SingleLinkedList<int> ll;
    ll.push_front(1);
    ll.push_back(3);
    ll.insert(1, 2);
    if(ll.search_iterative(2))
        ll.print();
    std::cout << ll.get_size() << " " << ll.get_size_iterative() << " "
              << ll.get_size_recursive(ll.head) << " " << ll.get_size_tail_recursive(ll.head) << '\n';
    return 0;
}