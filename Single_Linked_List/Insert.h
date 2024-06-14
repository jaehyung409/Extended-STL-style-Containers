//
// Created by jaehy on 2024-06-14.
//

#ifndef SINGLE_LINKED_LIST_INSERT_H
#define SINGLE_LINKED_LIST_INSERT_H

#include "Single_Linked_List.h"

template <typename T>
void SingleLinkedList<T>::push_front(T data){
    Node<T>* new_node = new Node<T>;
    new_node->data = data;
    if (get_size() == 0){
        tail = new_node;
        new_node->next = nullptr;
    } else {
        new_node->next = head->next;
    }
    head->next = new_node;
    size++;
}//시간복잡도 O(1) 공간복잡도 O(1)

template <typename T>
void SingleLinkedList<T>::push_back(T data){
    Node<T>* new_node = new Node<T>;
    new_node->data = data;
    new_node->next = nullptr;
    if (get_size() == 0){
        tail = new_node;
        head->next = tail;
        return;
    } else {
        Node<T>* current = head->next;
        while (current->next != nullptr){
            current = current->next;
        }
        current->next = new_node;
    }
    tail = new_node;
    size++;
}//시간복잡도 O(N) 공간복잡도 O(1)

template <typename T>
void SingleLinkedList<T>::insert(int index, T data){
    if (index < 0 || index > get_size()){
        throw std::out_of_range("index out_of_range");
    }
    if (index == 0){
        push_front(data);
        return;
    }
    if (index == get_size()){
        push_back(data);
        return;
    }
    Node<T>* new_node = new Node<T>;
    new_node->data = data;
    Node<T>* current = head->next;
    for (int i = 0; i < index-1; i++){
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
    size++;
}//시간복잡도 O(N) 공간복잡도 O(1)
#endif //SINGLE_LINKED_LIST_INSERT_H
