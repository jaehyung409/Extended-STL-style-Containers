/*
 * @ Created by jaehyung409 on 25. 1. 24.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <limits>
#include <memory>
#include <iterator>
#include <initializer_list>

export module j.list;

namespace j {
    export template<class T, class Allocator = std::allocator <T>>
    class list {
    public:
        class iterator;
        class const_iterator;

        using value_type = T;
        using allocator_type = std::allocator<T>;
        using size_type = size_t;
        using different_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        struct _list_node;
        using Node = _list_node;

        using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        Node *_head;
        Node *_tail;         // _tail is a dummy node (sentinel)
        node_allocator _node_alloc;
        size_type _size;

        // helper function (getter and setter)
        Node* get_head() noexcept {
            return _head;
        }
        Node* get_tail() noexcept {
            return _tail;
        }
        void set_head(Node *new_head) noexcept {
            _head = new_head;
        }
        void set_tail(Node *new_tail) noexcept {
            _tail = new_tail;
        }
        void set_size(size_type new_size) noexcept {
            _size = new_size;
        }

        // helper function (sort)
        template<class Compare>
        void merge_sort(list& x, Compare comp);

    public:
        // constructor and destructor
        list() : list(Allocator()) {}
        explicit list(const Allocator& alloc);
        explicit list(size_type n, const Allocator& alloc = Allocator());
        list(size_type n, const T& value, const Allocator& alloc = Allocator());
        //template <class InputIter>
        //list(InputIter first, InputIter last, const Allocator& alloc = Allocator());
        //template <container-compatible-range<T> R>
        //list(from_range_t, R&& range, const Allocator & = Allocator());
        list(const list& x);
        list(list&& x);
        list(const list& x, const std::type_identity_t<Allocator>& alloc);
        list(list&& x, const std::type_identity_t<Allocator>& alloc);
        list(std::initializer_list<T> il, const Allocator& alloc = Allocator());
        ~list();
        list& operator=(const list& x);
        list& operator=(list&& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
        list& operator=(std::initializer_list<T> il);
        //template<class InputIter>
        //void assign(InputIter first, InputIter last);
        //template<container-compatible-range<T> R>
        //void assign_range(R&& rg);
        void assign(size_type n, const T& t);
        //void assign(initializer_list<T>);
        allocator_type get_allocator() const noexcept;

        // iterators
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        // capacity
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;
        void resize(size_type sz);
        void resize(size_type sz, const T& c);

        // element access
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        // modifiers
        template<class... Args>
        reference emplace_front(Args&&... args);
        template<class... Args>
        reference emplace_back(Args&&... args);
        void push_front(const T& x);
        void push_front(T&& x);
        //template<container-compatible-range<T> R>
        //void prepend_range(R&& rg);
        void pop_front();
        void push_back(const T& x);
        void push_back(T&& x);
        //template<container-compatible-range<T> R>
        //void append_range(R&& rg);
        void pop_back();

        template<class... Args>
        iterator emplace(const_iterator position, Args&&... args);
        iterator insert(const_iterator position, const T& x);
        iterator insert(const_iterator position, T&& x);
        iterator insert(const_iterator position, size_type n, const T& x);
        //template<class InputIter>
        //iterator insert(const_iterator position, InputIter first, InputIter last);
        //template<container-compatible-range<T> R>
        //iterator insert_range(const_iterator position, R&& rg);
        //iterator insert(const_iterator position, initializer_list<T> il);

        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);
        void swap(list& x) noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
            std::swap(_head, x._head);
            std::swap(_tail, x._tail);
            std::swap(_node_alloc, x._node_alloc);
            std::swap(_size, x._size);
        }
        void clear() noexcept;

        // list operations
        void splice(const_iterator position, list& x);
        void splice(const_iterator position, list&& x);
        void splice(const_iterator position, list& x, const_iterator i);
        void splice(const_iterator position, list&& x, const_iterator i);
        void splice(const_iterator position,
                    list& x,
                    const_iterator first,
                    const_iterator last);
        void splice(const_iterator position,
                    list&& x,
                    const_iterator first,
                    const_iterator last);

        size_type remove(const T& value);
        //template<class Predicate>
        //size_type remove_if(Predicate pred);

        size_type unique();
        //template<class BinaryPredicate>
        //size_type unique(BinaryPredicate binary_pred);

        void merge(list& x);
        void merge(list&& x);
        template<class Compare>
        void merge(list& x, Compare comp);
        template<class Compare>
        void merge(list&& x, Compare comp);

        void sort();
        template<class Compare>
        void sort(Compare comp);

        void reverse() noexcept;
    };

    export template<class T, class Allocator>
    void swap(list<T, Allocator>& x, list<T, Allocator>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

    template <class T, class Allocator>
    struct list<T, Allocator>::_list_node {
        friend class list;
        friend class iterator;
        friend class const_iterator;

    private:
        value_type _value;
        _list_node* _next;
        _list_node* _prev;

    public:
        _list_node() : _value(T()), _next(nullptr), _prev(nullptr) {}
        explicit _list_node(const T &value) : _value(value), _next(nullptr), _prev(nullptr) {}
        explicit _list_node(T &&value) : _value(std::move(value)), _next(nullptr), _prev(nullptr) {}
        template <class... Args>
        requires std::is_constructible_v<T, Args...>
        explicit _list_node(Args&&... args) : _value(std::forward<Args>(args)...), _next(nullptr), _prev(nullptr) {}
        ~_list_node() {
            _value.~value_type();
            _next = nullptr;
            _prev = nullptr;
        }

        T& operator*() { return _value; }
        const T& operator*() const { return _value; }
    };

    template <class T, class Allocator>
    class list<T, Allocator>::iterator {
        friend class list;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    private:
        using node_pointer = Node*;
        node_pointer _ptr;

    public:
        explicit iterator(node_pointer ptr = nullptr) : _ptr(ptr) {}
        explicit iterator(const const_iterator& other)
            : _ptr(other._ptr) {}
        iterator& operator=(const const_iterator& other) {
            _ptr = other._ptr;
            return *this;
        }

        reference operator*() { return (**_ptr); }
        pointer operator->() { return &(**_ptr); }

        iterator &operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        iterator &operator--() {
            _ptr = _ptr->_prev;
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const iterator &other) const { return _ptr == other._ptr; }
        bool operator!=(const iterator &other) const { return _ptr != other._ptr; }
        operator const_iterator() const { return const_iterator(_ptr); }
    };

    template <class T, class Allocator>
    class list<T, Allocator>::const_iterator  {
        friend class list;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
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
        explicit const_iterator(node_pointer ptr = nullptr) : _ptr(ptr) {}
        explicit const_iterator(const iterator& other)
            : _ptr(other._ptr) {}
        const_iterator& operator=(const const_iterator& other) {
            _ptr = other._ptr;
            return *this;
        }

        const_reference operator*() const { return **_ptr; }
        const_pointer operator->() const { return &(**_ptr); }

        const_iterator& operator++() {
            _ptr = _ptr->_next;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        const_iterator& operator--() {
            _ptr = _ptr->_prev;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const const_iterator& other) const { return _ptr == other._ptr; }
        bool operator!=(const const_iterator& other) const { return _ptr != other._ptr; }
    };
}

namespace j {
    template <class T, class Allocator>
    list<T, Allocator>::list(const Allocator& alloc) : _head(nullptr), _tail(nullptr), _node_alloc(alloc), _size(0) {
        _tail = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(_tail);
        _tail->_next = _tail;
        _tail->_prev = _tail;
        _head = _tail;
    }

    template <class T, class Allocator>
    list<T, Allocator>::list(size_type n, const Allocator& alloc) : list(n, T(), alloc) {}

    template <class T, class Allocator>
    list<T, Allocator>::list(size_type n, const T& value, const Allocator& alloc) : list(alloc) {
        for (size_type i = 0; i < n; ++i) {
            emplace_front(value);
        }
    }

    template <class T, class Allocator>
    list<T, Allocator>::list(const list& x)
        : list(std::allocator_traits<Allocator>::select_on_container_copy_construction(x.get_allocator())) {
        for (const T& t : x) {
            emplace_back(t);
        }
    }

    template <class T, class Allocator>
    list<T, Allocator>::list(list&& x)
        : _head(x.get_head()), _tail(x.get_tail()),
          _node_alloc(std::move(x.get_allocator())), _size(std::move(x.size()))  {
        x.set_head(nullptr);
        x.set_tail(nullptr);
        x.set_size(0);
    }

    template <class T, class Allocator>
    list<T, Allocator>::list(const list& x, const std::type_identity_t<Allocator>& alloc) : list(alloc) {
        auto xit = x.begin();
        for (const T& t : x) {
            emplace_back(t);
        }
    }

    template <class T, class Allocator>
    list<T, Allocator>::list(list&& x, const std::type_identity_t<Allocator>& alloc)
        : _head(x.get_head()), _tail(x.get_tail()), _node_alloc(alloc), _size(std::move(x.size())) {
        auto new_tail = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_tail);
        x.set_tail(new_tail);
        x._tail->_next = x._tail;
        x._tail->_prev = x._tail;
        x.set_head(x.get_tail());
        x.set_size(0);
    }

    template <class T, class Allocator>
    list<T, Allocator>::list(std::initializer_list<T> il, const Allocator& alloc) : list(alloc) {
        for (const T& t : il) {
            emplace_back(t);
        }
    }

    template <class T, class Allocator>
    list<T, Allocator>::~list() {
        clear();
        std::destroy_at(_tail);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, _tail, 1);
    }

    template <class T, class Allocator>
    list<T, Allocator>& list<T, Allocator>::operator=(const list& x) {
        if (this != &x){
            clear();
            for (const T& t : x) {
                emplace_back(t);
            }
        }
        return *this;
    }

    template <class T, class Allocator>
    list<T, Allocator>& list<T, Allocator>::operator=(list&& x)
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
        if (this != &x){
            clear();
            std::destroy_at(_tail);
            std::allocator_traits<node_allocator>::deallocate(_node_alloc, _tail, 1);
            _head = x.get_head();
            _tail = x.get_tail();
            _node_alloc = std::move(x.get_allocator());
            _size = std::move(x.size());
            auto new_tail = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
            std::construct_at(new_tail);
            x.set_tail(new_tail);
            x._tail->_next = x._tail;
            x._tail->_prev = x._tail;
            x.set_head(x.get_tail());
            x.set_size(0);
        }
        return *this;
    }

    template <class T, class Allocator>
    list<T, Allocator>& list<T, Allocator>::operator=(std::initializer_list<T> il) {
        clear();
        for (const T& t : il) {
            emplace_back(t);
        }
        return *this;
    }


    template <class T, class Allocator>
    void list<T, Allocator>::assign(size_type n, const T& t) {
        clear();
        for (size_type i = 0; i < n; ++i) {
            emplace_front(t);
        }
    }

    template <class T, class Allocator>
    std::allocator<T> list<T, Allocator>::get_allocator() const noexcept{
        return std::allocator<T>(_node_alloc);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator list<T, Allocator>::begin() noexcept {
        return iterator(_head);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_iterator list<T, Allocator>::begin() const noexcept {
        return const_iterator(_head);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator list<T, Allocator>::end() noexcept {
        return iterator(_tail);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_iterator list<T, Allocator>::end() const noexcept {
        return const_iterator(_tail);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::reverse_iterator list<T, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::reverse_iterator list<T, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_iterator list<T, Allocator>::cbegin() const noexcept {
        return const_iterator(_head);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_iterator list<T, Allocator>::cend() const noexcept {
        return const_iterator(_tail);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template <class T, class Allocator>
    bool list<T, Allocator>::empty() const noexcept {
        return _size == 0;
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::size_type list<T, Allocator>::size() const noexcept {
        return _size;
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::size_type list<T, Allocator>::max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(Node) - 1; // -1 for _tail
    }

    template <class T, class Allocator>
    void list<T, Allocator>::resize(size_type sz) {
        resize(sz, T());
    }

    template <class T, class Allocator>
    void list<T, Allocator>::resize(size_type sz, const T& c) {
        if (sz < _size) {
            for (size_type i = 0; i < _size - sz; ++i) {
                pop_back();
            }
        } else if (sz > _size) {
            for (size_type i = 0; i < sz - _size; ++i) {
                emplace_back(c);
            }
        }
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::reference list<T, Allocator>::front() {
        return _head->_value;
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_reference list<T, Allocator>::front() const {
        return _head->_value;
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::reference list<T, Allocator>::back() {
        return _tail->_prev->_value;
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::const_reference list<T, Allocator>::back() const {
        return _tail->_prev->_value;
    }

    template <class T, class Allocator>
    template<class... Args>
    typename list<T, Allocator>::reference list<T, Allocator>::emplace_front(Args&&... args) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);
        if (empty()) {
            new_node->_next = _tail;
            _tail->_prev = new_node;
        } else {
            new_node->_next = _head;
            _head->_prev = new_node;
        }
        _head = new_node;
        new_node->_prev = new_node;
        _size++;
        return new_node->_value;
    }

    template <class T, class Allocator>
    template<class... Args>
    typename list<T, Allocator>::reference list<T, Allocator>::emplace_back(Args&&... args) {
        Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);
        if (empty()) {
            _head = new_node;
            new_node->_prev = new_node;
        } else {
            new_node->_prev = _tail->_prev;
            new_node->_prev->_next = new_node;
        }
        _tail->_prev = new_node;
        new_node->_next = _tail;
        _size++;
        return new_node->_value;
    }

    template <class T, class Allocator>
    void list<T, Allocator>::push_front(const T& x) {
        emplace_front(x);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::push_front(T&& x) {
        emplace_front(std::move(x));
    }

    template <class T, class Allocator>
    void list<T, Allocator>::pop_front() {
        Node *del_node = _head;
        _head = _head->_next;
        _head->_prev = _head;
        std::destroy_at(del_node);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
        _size--;
    }

    template <class T, class Allocator>
    void list<T, Allocator>::push_back(const T& x) {
        emplace_back(x);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::push_back(T&& x) {
        emplace_back(std::move(x));
    }

    template <class T, class Allocator>
    void list<T, Allocator>::pop_back() {
        Node *del_node = _tail->_prev;
        _tail->_prev = del_node->_prev;
        _tail->_prev->_next = _tail;
        std::destroy_at(del_node);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
        _size--;
    }

    template <class T, class Allocator>
    template<class... Args>
    typename list<T, Allocator>::iterator
        list<T, Allocator>::emplace(typename list<T, Allocator>::const_iterator position, Args&&... args) {
        if (position == begin()) {
            emplace_front(std::forward<Args>(args)...);
            return begin();
        }
        Node *new_node = std::allocator_traits<node_allocator>::allocate(_node_alloc, 1);
        std::construct_at(new_node, std::forward<Args>(args)...);
        new_node->_prev = position._ptr->_prev;
        new_node->_next = position._ptr;
        position._ptr->_prev->_next = new_node;
        position._ptr->_prev = new_node;
        _size++;
        return iterator(new_node);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator
        list<T, Allocator>::insert(typename list<T, Allocator>::const_iterator position, const T& x) {
        return emplace(position, x);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator
        list<T, Allocator>::insert(typename list<T, Allocator>::const_iterator position, T&& x) {
        return emplace(position, std::move(x));
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator
        list<T, Allocator>::insert(typename list<T, Allocator>::const_iterator position, size_type n, const T& x) {
        for (size_type i = 0; i < n; ++i) {
            position = emplace(position, x);
        }
        return iterator(position._ptr);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator
        list<T, Allocator>::erase(typename list<T, Allocator>::const_iterator position) {
        if (position == begin()) {
            pop_front();
            return begin();
        }
        Node *del_node = position._ptr;
        position._ptr->_prev->_next = position._ptr->_next;
        position._ptr->_next->_prev = position._ptr->_prev;
        auto next = position._ptr->_next;
        std::destroy_at(del_node);
        std::allocator_traits<node_allocator>::deallocate(_node_alloc, del_node, 1);
        _size--;
        return iterator(next);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::iterator
        list<T, Allocator>::erase(typename list<T, Allocator>::const_iterator first,
                                  typename list<T, Allocator>::const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(first._ptr);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::clear() noexcept {
        for (auto it = begin(); it != end();) {
            auto temp = it;
            ++it;
            std::destroy_at(temp._ptr);
            std::allocator_traits<node_allocator>::deallocate(_node_alloc, temp._ptr, 1);
        }
        _head = _tail;
        _tail->_prev = _tail;
        _size = 0;
    }

    template <class T, class Allocator>
    void list<T, Allocator>::splice(typename list<T, Allocator>::const_iterator position, list& x) {
        if (!x.empty()) {
            if (position == begin()){
                x.get_tail()->_prev->_next = _head;
                _head->_prev = x.get_tail()->_prev;
                _head = x.get_head();
                _head->_prev = _head;
                x.set_head(nullptr);
                _size += x.size();
                x.set_size(0);
            } else {
                x.get_tail()->_prev->_next = position._ptr;
                position._ptr->_prev->_next = x.get_head();
                x.get_head()->_prev = position._ptr->_prev;
                position._ptr->_prev = x.get_tail()->_prev;
                x.set_head(nullptr);
                _size += x.size();
                x.set_size(0);
            }
        }
    }

    template <class T, class Allocator>
    void list<T, Allocator>::splice(typename list<T, Allocator>::const_iterator position, list&& x) {
        splice(position, x);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::splice(typename list<T, Allocator>::const_iterator position, list& x,
                                    typename list<T, Allocator>::const_iterator i) {
        if (position == begin()) {
            if (i == x.begin()) {
                x.set_head(i._ptr->_next);
                x.get_head()->_prev = x.get_head();
            } else {
                i._ptr->_prev->_next = i._ptr->_next;
                i._ptr->_next->_prev = i._ptr->_prev;
            }
            i._ptr->_next = _head;
            _head->_prev = i._ptr;
            _head = i._ptr;
            _head->_prev = _head;
        } else {
            if (i == x.begin()) {
                x.set_head(i._ptr->_next);
                x.get_head()->_prev = x.get_head();
            } else {
                i._ptr->_prev->_next = i._ptr->_next;
                i._ptr->_next->_prev = i._ptr->_prev;
            }
            i._ptr->_next = position._ptr;
            i._ptr->_prev = position._ptr->_prev;
            position._ptr->_prev->_next = i._ptr;
            position._ptr->_prev = i._ptr;
        }
        _size++;
        x.set_size(x.size() - 1);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::splice(typename list<T, Allocator>::const_iterator position, list&& x,
                                    typename list<T, Allocator>::const_iterator i) {
        splice(position, x, i);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::splice(typename list<T, Allocator>::const_iterator position,
                                    list& x,
                                    typename list<T, Allocator>::const_iterator first,
                                    typename list<T, Allocator>::const_iterator last) {
        size_type distance = std::distance(first, last);
        if (position == begin()) {
            last._ptr->_prev->_next = _head;
            _head->_prev = last._ptr->_prev;
            if (first == x.begin()) {
                x.set_head(last._ptr);
                x.get_head()->_prev = x.get_head();
            } else {
                first._ptr->_prev->_next = last._ptr;
                last._ptr->_prev = first._ptr->_prev;
            }
            _head = first._ptr;
            _head->_prev = _head;
        } else {
            position._ptr->_prev->_next = first._ptr;
            last._ptr->_prev->_next = position._ptr;
            position._ptr->_prev = last._ptr->_prev;
            if (first == x.begin()) {
                x.set_head(last._ptr);
                x.get_head()->_prev = x.get_head();
            } else {
                first._ptr->_prev->_next = last._ptr;
                last._ptr->_prev = first._ptr->_prev;
            }
            first._ptr->_prev = position._ptr->_prev;
        }
        _size += distance;
        x.set_size(x.size() - distance);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::splice(typename list<T, Allocator>::const_iterator position,
                                    list&& x,
                                    typename list<T, Allocator>::const_iterator first,
                                    typename list<T, Allocator>::const_iterator last) {
        splice(position, x, first, last);
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::size_type list<T, Allocator>::remove(const T& value) {
        for (auto it = begin(); it != end();){
            if (*it == value) {
                it = erase(it);
            } else {
                ++it;
            }
        }
        return size();
    }

    template <class T, class Allocator>
    typename list<T, Allocator>::size_type list<T, Allocator>::unique() {
        if (empty()) {
            return 0;
        }
        auto it = begin();
        auto next = std::next(it);
        while (next != end()) {
            if (*it == *next) {
                next = erase(next);
            } else {
                it = next;
                ++next;
            }
        }
        return size();
    }

    template <class T, class Allocator>
    void list<T, Allocator>::merge(list& x) {
        auto it = begin();
        auto xit = x.begin();
        while (it != end() && xit != x.end()) {
            if (*it < *xit) {
                ++it;
            } else {
                splice(it, x, xit++);
            }
        }
        if (xit != x.end()) {
            splice(end(), x, xit, x.end());
        }
    }

    template <class T, class Allocator>
    void list<T, Allocator>::merge(list&& x) {
        merge(x);
    }

    template <class T, class Allocator>
    template<class Compare>
    void list<T, Allocator>::merge(list& x, Compare comp) {
        auto it = begin();
        auto xit = x.begin();
        while (it != end() && xit != x.end()) {
            if (comp(*it, *xit)) {
                ++it;
            } else {
                splice(it, x, xit++);
            }
        }
        if (xit != x.end()) {
            splice(end(), x, xit, x.end());
        }
    }

    template <class T, class Allocator>
    template<class Compare>
    void list<T, Allocator>::merge(list&& x, Compare comp) {
        merge(x, comp);
    }

    template<class T, class Allocator>
    template<class Compare>
    void list<T, Allocator>::merge_sort(list &x, Compare comp) {
        if (x.size() <= 1) {
            return;
        }
        auto slow = x.begin();
        auto fast = x.begin();
        while (fast != x.end() && std::next(fast) != x.end()) {
            std::advance(slow, 1);
            std::advance(fast, 2);
        }
        list<T> temp;
        temp.splice(temp.begin(), x, slow, x.end());
        merge_sort(x, comp);
        merge_sort(temp, comp);
        x.merge(temp, comp);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::sort() {
        merge_sort(*this, std::less<T>());
    }

    template <class T, class Allocator>
    template<class Compare>
    void list<T, Allocator>::sort(Compare comp) {
        merge_sort(*this, comp);
    }

    template <class T, class Allocator>
    void list<T, Allocator>::reverse() noexcept {
        if (empty()) {
            return;
        }
        auto next = std::next(begin());
        while (next != end()) {
            auto temp = next;
            next = std::next(next);
            splice(begin(), *this, temp);
        }
    }
}
