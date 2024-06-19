//
// Created by jaehy on 2024-06-20.
//

#ifndef SINGLE_LINKED_LIST_SEARCH_H
#define SINGLE_LINKED_LIST_SEARCH_H

#include "Single_Linked_List.h"

template <typename T>
bool SingleLinkedList<T>::search_iterative(T data) {
    Node<T>* temp = head;
    while (temp->next){
        temp = temp->next;
        if (data == temp->data)
            return true;
    }
    return false;
}//시간복잡도 O(N), 공간복잡도 O(1)

template <typename T>
bool SingleLinkedList<T>::search_recursive(Node<T>* node, T data) {
    if (node == nullptr) return false;
    if (node->data == data) return true;
    return search_recursive(node->next, data);
}//시간복잡도 O(N), 공간복잡도 O(N)


#endif //SINGLE_LINKED_LIST_SEARCH_H
