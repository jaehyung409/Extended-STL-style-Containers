/*
 * @ Created by jaehyung409 on 25. 1. 16.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <limits>
#include <memory>
//#include <utility>
#include <iterator>
#include <functional>

export module forward_list;

import default_value;
import forward_iterator;


namespace j {
    export template <class T, class Allocator = std::allocator<T>>
    class Forward_list {
    public:
        using value_type = T;
        using allocator_type = std::allocator<T>;
        using size_type = size_t;
        using iterator = Forward_iterator<T>;
        using const_iterator = const iterator;
        using Node = typename iterator::Node;

    private:
        using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node* before_head; // node for before_begin();
        Node* head;
        Node* tail;        // node for end();
        node_allocator node_alloc;
        // size_type size; didn't use for faster performance


        template <class Compare>
        void merge_sort(Forward_list& x, Compare comp);

    public:
        // constructor and destructor
        Forward_list() : head(nullptr), node_alloc(Allocator()) {
            before_head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(node_alloc, before_head, Node());
            tail = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(node_alloc, tail, Node());
            head = tail;
            before_head->next = head;
        }
        explicit Forward_list(const Allocator& alloc);
        explicit Forward_list(size_type n, const Allocator& alloc = Allocator()); // n size forward_list with default value of T.
        Forward_list(size_type n, const T& value, const Allocator& alloc = Allocator()); // n size forward_list with value.
        //template<class InputIter>
        //forward_list(InputIter first, InputIter last, const Allocator& = Allocator());
        Forward_list(const Forward_list& x);
        Forward_list(Forward_list&& x) noexcept;
        //forward_list(const forward_list& x, const std::type_identity_t<Allocator>&);
        //forward_list(forward_list&& x, const std::type_identity_t<Allocator>&);
        //forward_list(std::initializer_list<T>, const Allocator& = Allocator());
        ~Forward_list();

        // member functions
        Forward_list& operator=(const Forward_list& x);
        Forward_list& operator=(Forward_list&& x) noexcept;
        //forward_list& operator=(initializer_list<T>);
        void assign(size_type count, const T& value);
        //template <class InputIt>
        //void assign(InputIt first, InputIt last);
        //void assign(std::initializer_list<T> ilist);
        allocator_type get_allocator() const noexcept;

        // Iterator -> Use std::iterator before I make iterator
        iterator before_begin() noexcept;
        const_iterator before_begin() const noexcept;
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cbefore_begin() const noexcept;
        const_iterator cend() const noexcept;


        // Capacity
        bool empty() const noexcept;
        size_type max_size() const noexcept;

        // Accessor
        T& front();
        const T& front() const;

        // modifiers
        iterator insert_after(const_iterator pos, const T& value);
        iterator insert_after(const_iterator pos, T& value);
        iterator insert_after(const_iterator pos, size_type count, const T& value);
        template <class... Args>
        iterator emplace_after(const_iterator pos, Args&&... args);
        iterator erase_after(const_iterator pos);
        //iterator erase_after(const_iterator first, const_iterator last);
        template <class... Args>
        T& emplace_front(Args&&... args);
        void push_front(const T& value);
        void push_front(T&& value);
        void pop_front();
        void resize(size_type nsz);
        void resize(size_type nsz, const T& value);
        void clear() noexcept;

        size_type remove(const T& value);
        //template <class Predicate>
        //size_type remove_if(Predicate pred);
        size_type unique();

        void merge(Forward_list& x);
        void merge(Forward_list&& x);
        template <class Compare>
        void merge(Forward_list& x, Compare comp);
        template <class Compare>
        void merge(Forward_list&& x, Compare comp);

        void splice_after(const_iterator pos, Forward_list& other);
        void splice_after(const_iterator pos, Forward_list&& other);
        //void splice_after(const_iterator pos, Forward_list& other, const_iterator it);
        //void splice_after(const_iterator pos, Forward_list&& other, const_iterator it);
        void splice_after(const_iterator pos, Forward_list& other, const_iterator first, const_iterator last);
        void splice_after(const_iterator pos, Forward_list&& other, const_iterator first, const_iterator last);


        void sort();
        template <class Compare>
        void sort(Compare comp);

        void reverse() noexcept;
    };

    export template<class T, class Allocator>
    void swap(Forward_list<T, Allocator>& x,
              Forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y)));

    // erasure
    export template<class T, class Allocator, class U = T>
    typename Forward_list<T, Allocator>::size_type erase(Forward_list<T, Allocator>& c,
                                                         const U& value);
    /*export template<class T, class Allocator, class Predicate>
    typename Forward_list<T, Allocator>::size_type erase_if(Forward_list<T, Allocator>& c,
                                                            Predicate pred);
*/
}

namespace j {
    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Allocator& alloc): head(nullptr), node_alloc(alloc) {
        before_head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, before_head, Node());
        tail = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, tail, Node());
        head = tail;
        before_head->next = head;

    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const Allocator& alloc) : head(nullptr), node_alloc(alloc) {
        before_head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, before_head, Node());
        tail = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, tail, Node());
        if (n == 0) {
            head = tail;
            before_head->next = head;
        } else {
            head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(node_alloc, head, Node());
            before_head->next = head;
            Node* prev = head;
            for (int i = 1; i < n; i++) {
                Node* ptr = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
                std::allocator_traits<node_allocator>::construct(node_alloc, ptr, Node());
                prev->next = ptr;
                prev = ptr;
            }
            prev->next = tail;
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const T& value, const Allocator& alloc) : head(nullptr), node_alloc(alloc) {
        before_head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, before_head, Node());
        tail = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, tail, Node());
        if (n == 0) {
            head = tail;
            before_head->next = head;
        } else {
            head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(node_alloc, head, Node(value));

            Node* prev = head;
            for (int i = 1; i < n; i++) {
                Node* ptr = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
                std::allocator_traits<node_allocator>::construct(node_alloc, ptr, Node(value));
                prev->next = ptr;
                prev = ptr;
            }
            prev->next = tail;
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Forward_list& x) : head(nullptr), node_alloc(x.node_alloc) {
        before_head = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, before_head, Node());
        tail = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, tail, Node());
        head = tail;
        before_head->next = head;

        auto xit = x.begin();
        for (auto it = before_begin(); xit != x.end(); ++xit) {
            it = insert_after(it, *xit);
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(Forward_list&& x)  noexcept
        : before_head(x.before_head), head(x.head), node_alloc(std::move(x.node_alloc)), tail(x.tail) {
        x.before_head = std::allocator_traits<node_allocator>::allocate(x.node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(x.node_alloc, x.before_head, Node());
        x.tail = std::allocator_traits<node_allocator>::allocate(x.node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(x.node_alloc, x.tail, Node());
        x.head = x.tail;
        x.before_head->next = x.head;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::~Forward_list() {
        clear();
        std::allocator_traits<node_allocator>::destroy(node_alloc, before_head);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, before_head, 1);
        before_head = nullptr;
        std::allocator_traits<node_allocator>::destroy(node_alloc, tail);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, tail, 1);
        tail = nullptr;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(const Forward_list& x) {
        if (this != &x) {
            clear();
            auto xit = x.begin();
            for (auto it = before_begin(); xit != x.end(); ++xit) {
                it = insert_after(it, *xit);
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(Forward_list&& x) noexcept {
        if (this != &x) {
            clear();
            std::allocator_traits<node_allocator>::destroy(node_alloc, before_head);
            std::allocator_traits<node_allocator>::deallocate(node_alloc, before_head, 1);
            before_head = nullptr;
            std::allocator_traits<node_allocator>::destroy(node_alloc, tail);
            std::allocator_traits<node_allocator>::deallocate(node_alloc, tail, 1);
            tail = nullptr;
            head = x.head;
            before_head = x.before_head;
            tail = x.tail;
            node_alloc = std::move(std::allocator<T>());
            x.node_alloc = Allocator();
            x.before_head = std::allocator_traits<node_allocator>::allocate(x.node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(x.node_alloc, x.before_head, Node());
            x.tail = std::allocator_traits<node_allocator>::allocate(x.node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(x.node_alloc, x.tail, Node());
            x.head = x.tail;
            x.before_head->next = x.head;
        }
        return *this;
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::assign(Forward_list::size_type count, const T &value) {
        clear();
        for (int i = 0; i < count; i++)
            push_front(value);
    }

    template<class T, class Allocator>
    std::allocator<T> Forward_list<T, Allocator>::get_allocator() const noexcept {
        return node_alloc;
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::before_begin() noexcept {
        return iterator(before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::before_begin() const noexcept {
        return iterator(before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::begin() noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::begin() const noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::end() noexcept {
        return iterator(tail);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::end() const noexcept {
        return iterator(tail);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cbegin() const noexcept {
        return iterator(head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cbefore_begin() const noexcept {
        return iterator(before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cend() const noexcept {
        return iterator(tail);
    }

    template <class T, class Allocator>
    bool Forward_list<T, Allocator>::empty() const noexcept {
        return head == tail;
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(Node) - 2;
    }

    template <class T, class Allocator>
    T& Forward_list<T, Allocator>::front() {
        if (empty()) throw std::out_of_range("front() called on empty forward_list");
        return head->value;
    }

    template <class T, class Allocator>
    const T& Forward_list<T, Allocator>::front() const {
        if (empty()) throw std::out_of_range("front() called on empty forward_list");
        return head->value;
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator pos, const T &value) {
        if (pos == before_begin()) {
            push_front(value);
            return begin();
        }
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);

        Node* new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(value));

        new_node->next = non_const_pos->next;
        non_const_pos->next = new_node;

        return iterator(new_node);
    }


    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator pos, T &value) {
        if (pos == before_begin()) {
            push_front(value);
            return begin();
        }
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);

        Node* new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(value));

        new_node->next = non_const_pos->next;
        non_const_pos->next = new_node;
        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator
    Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator pos, Forward_list::size_type count,
                                             const T &value) {
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        for (int i = 0; i < count; i++)
            non_const_pos = insert_after(non_const_pos, value);
        return non_const_pos;
    }

    template<class T, class Allocator>
    template<class... Args>
    typename Forward_list<T, Allocator>::iterator
    Forward_list<T, Allocator>::emplace_after(Forward_list::const_iterator pos, Args &&... args) {
        if (pos == before_begin()) {
            emplace_front(args...);
            return begin();
        }
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        Node* new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(T(std::forward<Args>(args)...)));

        new_node->next = non_const_pos->next;
        non_const_pos->next = new_node;

        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::erase_after(Forward_list::const_iterator pos) {
        if (empty()) throw std::out_of_range("erase_after() called on empty forward_list");
        if (pos == before_begin()) {
            pop_front();
            return begin();
        }
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        Node* node_to_delete = non_const_pos->next;
        non_const_pos->next = node_to_delete->next;
        std::allocator_traits<node_allocator>::destroy(node_alloc, node_to_delete);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, node_to_delete, 1);
        node_to_delete = nullptr;
        return non_const_pos->next;
    }

    template <class T, class Allocator>
    template <class ... Args>
    T& Forward_list<T, Allocator>::emplace_front(Args&&... args) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(T(std::forward<Args>(args)...)));
        if (empty()) new_node->next = tail;
        else new_node->next = head;
        head = new_node;
        before_head->next = head;
        return head->value;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(const T& value) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(value));
        new_node->next = head;
        head = new_node;
        before_head->next = head;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(T&& value) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(node_alloc, new_node, Node(value));
        new_node->next = head;
        head = new_node;
        before_head->next = head;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::pop_front() {
        if (empty()) throw std::out_of_range("pop_front() called on empty forward_list");
        Node* del_node = head;
        head = head->next;
        before_head->next = head;
        std::allocator_traits<node_allocator>::destroy(node_alloc, del_node);
        std::allocator_traits<node_allocator>::deallocate(node_alloc, del_node, 1);
        del_node = nullptr;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::resize(size_type nsz) {
        size_type size = 0;
        auto prev = before_begin();
        auto it = begin();
        for(; it != end() && size < nsz; ++prev, ++it, ++size) {}
        if (size == nsz){
            if (it == end()) return;
            for(; std::next(prev) != end(); erase_after(prev)) {}
        }
        else {
            for(; size < nsz; size++){
                insert_after(prev, Default_value<T>().get().value_or(T()));
            }
        }
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::resize(size_type nsz, const T& value) {
        size_type size = 0;
        auto prev = before_begin();
        auto it = begin();
        for(; it != end() && size < nsz; ++prev, ++it, ++size) {}
        if (size == nsz){
            if (it == end()) return;
            for(; std::next(prev) != end(); erase_after(prev)) {}
        }
        else {
            for(; size < nsz; size++){
                insert_after(prev, value);
            }
        }
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::clear() noexcept {
        while (!empty()){
            pop_front();
        }
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::remove(const T& value) {
        if (empty()) return 0;
        size_type size = 0;
        auto it = before_begin();
        auto next = begin();
        while (next != end()) {
            if (*next == value) {
                erase_after(it);
                next = std::next(it);
            } else {
                ++it;
                ++next;
                ++size;
            }
        }
        return size;
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::unique() {
        if (empty()) return 0;
        size_type size = 1;
        auto it = begin();
        auto next = std::next(begin());
        while (next != end()) {
            if (*next == *it) {
                erase_after(it);
                next = std::next(it);
            } else {
                ++it;
                ++next;
                ++size;
            }
        }
        return std::distance(begin(), end());
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::merge(Forward_list& x) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || std::less<>{}(*next_xit, *next_it)) {
                it = insert_after(it, *next_xit);
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::merge(Forward_list&& x) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || std::less<>{}(*next_xit, *next_it)) {
                it = insert_after(it, std::move(*next_xit));
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template <class T, class Allocator>
    template <class Compare>
    void Forward_list<T, Allocator>::merge(Forward_list& x, Compare comp) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || comp(*next_xit, *next_it)) {
                it = insert_after(it, *next_xit);
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template <class T, class Allocator>
    template <class Compare>
    void Forward_list<T, Allocator>::merge(Forward_list&& x, Compare comp) {
        auto it = before_begin();
        auto next_it = begin();
        auto xit = x.before_begin();
        auto next_xit = x.begin();
        while (next_xit != x.end()){
            if (next_it == end() || comp(*next_xit, *next_it)) {
                it = insert_after(it, std::move(*next_xit));
                next_it = std::next(it);
                x.erase_after(xit);
                next_xit = std::next(xit);
            } else {
                ++it;
                ++next_it;
            }
        }
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator pos, Forward_list &other) {
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        while (!other.empty()) {
            non_const_pos = insert_after(non_const_pos, other.front());
            other.pop_front();
        }
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator pos, Forward_list &&other) {
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        while (!other.empty()) {
            non_const_pos = insert_after(non_const_pos, std::move(other.front()));
            other.pop_front();
        }
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator pos, Forward_list &other,
                                                  Forward_list::const_iterator first,
                                                  Forward_list::const_iterator last) {
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        if (first != last) {
            auto next = const_cast<Forward_list::const_iterator&>(first);
            std::advance(next, 1);
            while (next != last) {
                non_const_pos = insert_after(non_const_pos, *next);
                std::advance(next, 1);
                other.erase_after(first);
            }
        }
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator pos, Forward_list &&other,
                                                  Forward_list::const_iterator first,
                                                  Forward_list::const_iterator last) {
        auto non_const_pos = const_cast<Forward_list::const_iterator&>(pos);
        if (first != last) {
            auto next = const_cast<Forward_list::const_iterator&>(first);
            std::advance(next, 1);
            while (next != last) {
                non_const_pos = insert_after(non_const_pos, std::move(*next));
                std::advance(next, 1);
                other.erase_after(first);
            }
        }
    }


    template<class T, class Allocator>
    template<class Compare>
    void Forward_list<T, Allocator>::merge_sort(Forward_list &x, Compare comp) {
        if (x.empty() || std::next(x.begin()) == x.end()) {
            return;
        }
        auto slow = x.begin();
        auto fast = x.begin();
        while (std::next(fast) != x.end() && std::next(fast, 2) != x.end()){
            std::advance(slow, 1);
            std::advance(fast, 2);
        }
        Forward_list<T> second;
        second.splice_after(second.before_begin(), x, slow, x.end());
        merge_sort(x, comp);
        merge_sort(second, comp);
        x.merge(second, comp);
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::sort() {
        merge_sort(*this, std::less<T>());
    }

    template <class T, class Allocator>
    template <class Compare>
    void Forward_list<T, Allocator>::sort(Compare comp) {
        merge_sort(*this, comp);
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::reverse() noexcept {
        if (empty()) return;
        Node* prev = tail;
        Node* current = head;
        Node* next = nullptr;
        while (current != tail){
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
        before_head->next = head;
    }

    template <class T, class Allocator>
    void swap(Forward_list<T, Allocator>& x, Forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
        using std::swap;
        swap(x.before_head, y.before_head);
        swap(x.head, y.head);
        swap(x.tail, y.tail);
        swap(x.node_alloc, y.node_alloc);
    }

    template <class T, class Allocator, class U>
    typename Forward_list<T, Allocator>::size_type erase(Forward_list<T, Allocator>& c, const U& value) {
        auto it = c.before_begin();
        while (std::next(it) != c.end()) {
            if (*std::next(it) == value) {
                it = erase_after(it);
            } else {
                ++it;
            }
        }
        return std::distance(c.begin(), c.end());
    }
/*
    template <class T, class Allocator, class Predicate>
    typename Forward_list<T, Allocator>::size_type erase_if(Forward_list<T, Allocator>& c, Predicate pred) {
    } */
}
