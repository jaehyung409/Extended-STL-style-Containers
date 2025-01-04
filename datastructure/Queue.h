//
// Created by jaehy on 25. 1. 4.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <deque>
#include <stdexcept>

template <typename T, typename Container = std::deque<T>>
struct Queue {
protected:
    Container container;

public:
    explicit Queue(const Container& cont = Container()) : container(cont) {}

    size_t size(){
        return this->container.size();
    }
    size_t max_size(){
        return this->container.max_size();
    }
    bool empty(){
        return this->container.empty();
    }
    T front(){
        if (this->container.empty()) throw std::out_of_range("front() called on empty queue");
        return this->container.front();
    }
    T back(){
        if (this->container.empty()) throw std::out_of_range("back() called on empty queue");
        return this->container.back();
    }
    void pop(){
        if (this->container.empty()) throw std::out_of_range("pop() called on empty queue");
        this->container.pop_front();
    }
    void push(T value){
        this->container.push_back(value);
    }

};

#endif //QUEUE_H
