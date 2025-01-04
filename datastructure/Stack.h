//
// Created by jaehy on 2024-08-31.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <deque>
#include <stdexcept>

template <typename T, typename Container = std::deque<T>>
struct Stack {
protected:
    Container container;

public:
    explicit Stack(const Container& cont = Container()) : container(cont) {}

    size_t size(){
        return this->container.size();
    }
    size_t max_size(){
        return this->container.max_size();
    }
    bool empty(){
        return this->container.empty();
    }
    T top(){
        if (this->container.empty()) throw std::out_of_range("top() called on empty stack");
        return this->container.front();
    }
    void pop(){
        if (this->container.empty()) throw std::out_of_range("pop() called on empty stack");
        this->container.pop_back();
    }
    void push(T value){
        this->container.push_front(value);
    }

};

#endif //STACK_STACK_H
