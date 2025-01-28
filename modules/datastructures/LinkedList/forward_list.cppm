/*
 * @ Created by jaehyung409 on 25. 1. 23.
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

import basics;

namespace j {
    export template <class T, class Allocator = std::allocator<T>>
    class Forward_list {
    private:
        struct _Forward_list_node;
        using Node = _Forward_list_node;

    public:
        class _Forward_iterator;
        class _const_Forward_iterator;

        using value_type = T;
        using allocator_type = std::allocator<T>;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = _Forward_iterator;
        using const_iterator = _const_Forward_iterator;

    private:
        using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node* _before_head; // node for before_begin();
        Node* _head;        // first node, nullptr if empty (sentinel)
        node_allocator _node_alloc;
        // size_type size; didn't use for faster performance

        // helper function (getter and setter)
        Node* get_before_head() noexcept {
            return _before_head;
        }
        Node* get_head() noexcept {
            return _head;
        }
        void set_before_head(Node* new_before_head) noexcept {
            _before_head = new_before_head;
        }
        void set_head(Node* new_head) noexcept {
            _head = new_head;
        }

        // helper function (sort)
        template <class Compare>
        void merge_sort(Forward_list& x, Compare comp);

    public:
        // constructor and destructor
        Forward_list() : Forward_list(Allocator()) {}
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
        iterator insert_after(const_iterator position, const T& value);
        iterator insert_after(const_iterator position, T& value);
        iterator insert_after(const_iterator position, size_type count, const T& value);
        template <class... Args>
        iterator emplace_after(const_iterator position, Args&&... args);
        iterator erase_after(const_iterator position);
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
        // merge -> need to implement newly (using splice_after line 197~8)
        void merge(Forward_list& x);
        void merge(Forward_list&& x);
        template <class Compare>
        void merge(Forward_list& x, Compare comp);
        template <class Compare>
        void merge(Forward_list&& x, Compare comp);

        void splice_after(const_iterator position, Forward_list& other);
        void splice_after(const_iterator position, Forward_list&& other);
        //void splice_after(const_iterator position, Forward_list& other, const_iterator it);
        //void splice_after(const_iterator position, Forward_list&& other, const_iterator it);
        void splice_after(const_iterator position, Forward_list& other, const_iterator first, const_iterator last);
        void splice_after(const_iterator position, Forward_list&& other, const_iterator first, const_iterator last);


        void sort();
        template <class Compare>
        void sort(Compare comp);

        void reverse() noexcept;
        void swap(Forward_list& x) noexcept(noexcept(std::allocator_traits<node_allocator>::is_always_equal::value)) {
            using std::swap;
            swap(_before_head, x._before_head);
            swap(_head, x._head);
            swap(_node_alloc, x._node_alloc);
        }
    };

    export template<class T, class Allocator>
    void swap(Forward_list<T, Allocator>& x,
              Forward_list<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

    // erasure
    export template<class T, class Allocator, class U = T>
    typename Forward_list<T, Allocator>::size_type erase(Forward_list<T, Allocator>& c,
                                                         const U& value);
    /*export template<class T, class Allocator, class Predicate>
    typename Forward_list<T, Allocator>::size_type erase_if(Forward_list<T, Allocator>& c,
                                                            Predicate pred);
*/
    template <class T, class Allocator>
    class Forward_list<T, Allocator>::_Forward_list_node {
        friend class Forward_list<T, Allocator>;
        friend class _Forward_iterator;
        friend class _const_Forward_iterator;

    private:
        value_type _value;
        _Forward_list_node* _next;

    public:
        _Forward_list_node() : _value(T()), _next(nullptr) {}
        explicit _Forward_list_node(const T& value) : _value(value), _next(nullptr) {}
        explicit _Forward_list_node(T&& value) : _value(std::move(value)), _next(nullptr) {}
        template <class... Args>
        requires std::is_constructible_v<T, Args...>
        explicit _Forward_list_node(Args&&... args) : _value(std::forward<Args>(args)...), _next(nullptr) {}
        ~_Forward_list_node() = default;

        T& operator*() { return _value; }
        const T& operator*() const { return _value; }
    };

    template <class T, class Allocator>
    class Forward_list<T, Allocator>::_Forward_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    private:
        using node_pointer = Node*;
        node_pointer _ptr;

    public:
        _Forward_iterator(node_pointer ptr = nullptr) : _ptr(ptr) {}
        _Forward_iterator(const _const_Forward_iterator& other) : _ptr(other._ptr) {}

        reference operator*() const { return **_ptr; }
        pointer operator->() const { return &(**_ptr); }

        _Forward_iterator& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        _Forward_iterator operator++(int) {
            _Forward_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const _Forward_iterator& other) const { return _ptr == other._ptr; }
        bool operator!=(const _Forward_iterator& other) const { return _ptr != other._ptr; }
        operator _const_Forward_iterator() const { return _const_Forward_iterator(_ptr); }
    };

    template <class T, class Allocator>
    class Forward_list<T, Allocator>::_const_Forward_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using const_pointer = const T*;
        using const_reference = const T&;

    private:
        using node_pointer = Node*;
        node_pointer _ptr;

    public:
        _const_Forward_iterator(node_pointer ptr = nullptr) : _ptr(ptr) {}

        const_reference operator*() const { return **_ptr; }
        const_pointer operator->() const { return &(**_ptr); }

        _const_Forward_iterator& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        _const_Forward_iterator operator++(int) {
            _const_Forward_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const _const_Forward_iterator& other) const { return _ptr == other._ptr; }
        bool operator!=(const _const_Forward_iterator& other) const { return _ptr != other._ptr; }
    };
}

namespace j {
    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Allocator& alloc) : _node_alloc(alloc) {
        _before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, _before_head, Node());
        _before_head->_next = _head;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const Allocator& alloc) : Forward_list(n, T(), alloc) {}

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const size_type n, const T& value, const Allocator& alloc) : Forward_list(alloc) {
        for (size_type i = 0; i < n; ++i) {
            emplace_front(value);
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(const Forward_list& x) : Forward_list(std::allocator_traits<node_allocator>::select_on_container_copy_construction(x.get_allocator())) {
        auto xit = x.begin();
        for (auto it = before_begin(); xit != x.end(); ++xit) {
            it = emplace_after(it, *xit);
        }
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::Forward_list(Forward_list&& x)  noexcept
        : _before_head(x.get_before_head()), _head(x.get_head()), _node_alloc(std::move(x.get_allocator())) {
        auto new_before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_before_head, Node());
        x.set_before_head(new_before_head);
        x.set_head(nullptr);
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>::~Forward_list() {
        clear();
        std::allocator_traits<node_allocator>::destroy(_node_alloc, _before_head);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, _before_head, 1);
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(const Forward_list& x) {
        if (this != &x) {
            clear();
            auto xit = x.begin();
            for (auto it = before_begin(); xit != x.end(); ++xit) {
                it = emplace_after(it, *xit);
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    Forward_list<T, Allocator>& Forward_list<T, Allocator>::operator=(Forward_list&& x) noexcept {
        if (this != &x) {
            clear();
            std::allocator_traits<node_allocator>::destroy(_node_alloc, _before_head);
            std::allocator_traits<node_allocator>::deallocate(_node_alloc, _before_head, 1);

            _head = x.get_head();
            _before_head = x.get_before_head();
            _node_alloc = std::move(x.get_allocator());

            auto new_before_head = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
            std::allocator_traits<node_allocator>::construct(_node_alloc, new_before_head, Node());

            x.set_head(nullptr);
            x.set_before_head(new_before_head);
        }
        return *this;
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::assign(Forward_list::size_type count, const T &value) {
        clear();
        for (int i = 0; i < count; i++) {
            emplace_front(value);
        }
    }

    template<class T, class Allocator>
    std::allocator<T> Forward_list<T, Allocator>::get_allocator() const noexcept {
        return std::allocator<T>(_node_alloc);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::before_begin() noexcept {
        return iterator(_before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::before_begin() const noexcept {
        return const_iterator(_before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::begin() noexcept {
        return iterator(_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::begin() const noexcept {
        return const_iterator(_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::end() noexcept {
        return iterator(nullptr);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::end() const noexcept {
        return const_iterator(nullptr);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cbegin() const noexcept {
        return const_iterator(_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cbefore_begin() const noexcept {
        return iterator(_before_head);
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::const_iterator Forward_list<T, Allocator>::cend() const noexcept {
        return iterator();
    }

    template <class T, class Allocator>
    bool Forward_list<T, Allocator>::empty() const noexcept {
        return _head == nullptr;
    }

    template <class T, class Allocator>
    typename Forward_list<T, Allocator>::size_type Forward_list<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_t>::max() / sizeof(Node) - 1; // -1 for _before_head
    }

    template <class T, class Allocator>
    T& Forward_list<T, Allocator>::front() {
        if (empty()) {
            throw std::out_of_range("Forward_list::front() : forward_list is empty");
        }
        return _head->value;
    }

    template <class T, class Allocator>
    const T& Forward_list<T, Allocator>::front() const {
        if (empty()) {
            throw std::out_of_range("Forward_list::front() : forward_list is empty");
        }
        return _head->value;
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator position, const T &value) {
        if (position == before_begin()) {
            push_front(value);
            return begin();
        } else if (position == end()) {
            throw std::out_of_range("Forward_list::insert_after() : position is end()");
        }

        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_node, Node(value));
        new_node->_next = position->_next;
        position->_next = new_node;

        return iterator(new_node);
    }


    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator position, T &value) {
        if (position == before_begin()) {
            push_front(value);
            return begin();
        } else if (position == end()) {
            throw std::out_of_range("Forward_list::insert_after() : position is end()");
        }

        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_node, Node(value));

        new_node->_next = position->_next;
        position->_next = new_node;
        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator
    Forward_list<T, Allocator>::insert_after(Forward_list::const_iterator position, Forward_list::size_type count,
                                             const T &value) {
        if (position == end()) {
            throw std::out_of_range("Forward_list::insert_after() : position is end()");
        }

        for (int i = 0; i < count; i++) {
            position = insert_after(position, value);
        }
        return Forward_list::iterator(position._ptr);
    }

    template<class T, class Allocator>
    template<class... Args>
    typename Forward_list<T, Allocator>::iterator
    Forward_list<T, Allocator>::emplace_after(Forward_list::const_iterator position, Args &&... args) {
        if (position == before_begin()) {
            emplace_front(args...);
            return begin();
        } else if (position == end()) {
            throw std::out_of_range("Forward_list::emplace_after() : position is end()");
        }

        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_node, Node(T(std::forward<Args>(args)...)));

        new_node->_next = position->_next;
        position->_next = new_node;

        return iterator(new_node);
    }

    template<class T, class Allocator>
    typename Forward_list<T, Allocator>::iterator Forward_list<T, Allocator>::erase_after(Forward_list::const_iterator position) {
        if (position == before_begin()) {
            pop_front();
            return begin();
        } else if (position == end()) {
            throw std::out_of_range("Forward_list::erase_after() : position is end()");
        } else if (empty()) {
            throw std::out_of_range("Forward_list::erase_after() : forward_list is empty");
        }


        Node* node_to_delete = position->_next;
        position->_next = node_to_delete->_next;
        std::allocator_traits<node_allocator>::destroy(_node_alloc, node_to_delete);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, node_to_delete, 1);
        return position->_next;
    }

    template <class T, class Allocator>
    template <class ... Args>
    T& Forward_list<T, Allocator>::emplace_front(Args&&... args) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_node, Node(T(std::forward<Args>(args)...)));
        if (empty()) {
            new_node->_next = nullptr;
        } else {
            new_node->_next = _head;
        }
        _head = new_node;
        _before_head->_next = _head;
        return _head->value;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(const T& value) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_node, Node(value));
        new_node->_next = _head;
        _head = new_node;
        _before_head->_next = _head;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::push_front(T&& value) {
        Node* new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::allocator_traits<node_allocator>::construct(_node_alloc, new_node, Node(value));
        new_node->_next = _head;
        _head = new_node;
        _before_head->_next = _head;
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::pop_front() {
        if (empty()) {
            throw std::out_of_range("Forward_list::pop_front() : forward_list is empty");
        }
        Node* del_node = _head;
        _head = _head->_next;
        _before_head->_next = _head;
        std::allocator_traits<node_allocator>::destroy(_node_alloc, del_node);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::resize(size_type nsz) {
        resize(nsz, T());
    }

    template <class T, class Allocator>
    void Forward_list<T, Allocator>::resize(size_type nsz, const T& value) {
        size_type size = 0;
        auto prev = before_begin();
        auto it = begin();
        for(; it != end() && size < nsz; ++prev, ++it, ++size) {}
        if (size == nsz){
            if (it == end()) {
                return;
            }
            for(; std::next(prev) != end(); erase_after(prev)) {}
        }
        else {
            for(; size < nsz; size++){
                emplace_after(prev, value);
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
        if (empty()) {
            return 0;
        }
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
        if (empty()) {
            return 0;
        }
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
                it = emplace_after(it, *next_xit);
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
                it = emplace_after(it, std::move(*next_xit));
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
                it = emplace_after(it, *next_xit);
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
                it = emplace_after(it, std::move(*next_xit));
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
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator position, Forward_list &other) {
        if (get_allocator() != other.get_allocator()) {
            throw std::invalid_argument("Forward_list::splice_after() : allocator is different");
        } else if (position == end()) {
            throw std::out_of_range("Forward_list::splice_after() : position is end()");
        } else if (!other.empty()) {
            auto last = other.before_begin();
            while (std::next(last) != other.end()) {
                ++last;
            }
            last->_next = position->_next;
            if (position == before_begin()) {
                _head = other.get_head();
                _before_head->_next = _head;
            } else {
                position->_next = other.get_head();
            }
            other.set_head(nullptr);
        }
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator position, Forward_list &&other) {
        splice_after(position, other);
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator position, Forward_list &other,
                                                  Forward_list::const_iterator first,
                                                  Forward_list::const_iterator last) {
        if (get_allocator() != other.get_allocator()) {
            throw std::invalid_argument("Forward_list::splice_after() : allocator is different");
        } else if (position == end()) {
            throw std::out_of_range("Forward_list::splice_after() : position is end()");
        } else if (first == last) {
            return;
        }
        auto before_last = first;
        while (std::next(before_last) != last) {
            ++before_last;
        }
        auto move_first = first->_next;
        if (first == other.before_begin()) {
            other.set_head(before_last->_next);
            other.get_before_head()->_next = other.get_head();
        } else {
            first->_next = before_last->_next;
        }
        before_last->_next = position->_next;
        if (position == before_begin()) {
            _head = move_first;
            _before_head->_next = _head;
        } else {
            position->_next = move_first;
        }
    }

    template<class T, class Allocator>
    void Forward_list<T, Allocator>::splice_after(Forward_list::const_iterator position, Forward_list &&other,
                                                  Forward_list::const_iterator first,
                                                  Forward_list::const_iterator last) {
        splice_after(position, other, first, last);
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
        if (empty()) {
            return;
        }
        Node* prev = nullptr;
        Node* current = _head;
        Node* next = nullptr;
        while (current != nullptr) {
            next = current->_next;
            current->_next = prev;
            prev = current;
            current = next;
        }
        _head = prev;
        _before_head->_next = _head;
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
