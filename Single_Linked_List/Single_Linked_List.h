//
// Created by jaehy on 2024-06-14.
//

#ifndef SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H
#define SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H

#include <type_traits>

template <typename T>
struct DefaultValue {
    static constexpr T value = std::conditional_t<std::is_arithmetic<T>::value, std::integral_constant<T, 0>, std::integral_constant<T*, nullptr>>::value;
};

template <typename T>
struct Node{
    T data;
    Node* next;
};

template <typename T>
struct SingleLinkedList{
private:
    Node<T>* head; // head->next is the first node
    Node<T>* tail; // tail is the last node
    int size;

public:
    SingleLinkedList(){
        head = new Node<T>;
        head->data = DefaultValue<T>::value;
        tail = nullptr;
        head->next = tail;
        size = 0;
    }
    int get_size(){
        return this->size;
    }
    Node<T>* get_head(){
        return this->head;
    }
    Node<T>* get_tail(){
        return this->tail;
    }
    void print(){
        Node<T>* current = head->next;
        while(current != nullptr){
            std::cout << current->data << ' ';
            current = current->next;
        }
        std::cout << '\n';
    }
    void push_front(T data);
    void push_back(T data);
    void pop_front();
    void pop_back();
    T front();
    T back();
    void insert(int index, T data);
    void erase(int index);

    bool search_iterative(T data);
    bool search_recursive(Node<T>* node, T data);
    int get_size_iterative();
    int get_size_recursive(Node<T>* node);
    int get_size_tail_recursive(Node<T>* node, int get_size = 0);
};

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

template <typename T>
void SingleLinkedList<T>::push_front(T data){
    auto* new_node = new Node<T>;
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
    auto* new_node = new Node<T>;
    new_node->data = data;
    new_node->next = nullptr;
    if (get_size() == 0){
        tail = new_node;
        head->next = tail;
        return;
    } else {
        tail->next = new_node;
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
    auto* new_node = new Node<T>;
    new_node->data = data;
    Node<T>* current = head->next;
    for (int i = 0; i < index-1; i++){
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
    size++;
}//시간복잡도 O(N) 공간복잡도 O(1)

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

#endif //SINGLE_LINKED_LIST_SINGLE_LINKED_LIST_H
