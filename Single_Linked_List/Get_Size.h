//
// Created by jaehy on 2024-06-20.
//

#ifndef SINGLE_LINKED_LIST_GET_SIZE_H
#define SINGLE_LINKED_LIST_GET_SIZE_H

#include "Single_Linked_List.h"

template <typename T>
int SingleLinkedList<T>::get_size_iterative() {
    Node<T>* temp = head;
    int get_size = 0;
    while(temp->next){
        temp = temp->next;
        get_size++;
    }
    return get_size;
}//시간복잡도 O(N), 공간복잡도 O(1)

template <typename T>
int SingleLinkedList<T>::get_size_recursive(Node<T>* node) {
    if (node == nullptr) return -1; //head 부터 세므로 -1
    return get_size_recursive(node->next) + 1;
}//시간복잡도 O(N), 공간복잡도 O(N)

template <typename T>
int SingleLinkedList<T>::get_size_tail_recursive(Node<T>* node, int get_size) {
    if (node == nullptr) return get_size - 1; //head 부터 세므로 -1
    return get_size_tail_recursive(node->next, get_size + 1);
}//시간복잡도 O(N), 공간복잡도 O(1) when tail call optimization (if not O(N))

#endif //SINGLE_LINKED_LIST_GET_SIZE_H
