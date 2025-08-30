/*
 * @ Created by jaehyung409 on 25. 1. 27.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>

export module j:deque;

namespace j {
export template <class T, class Allocator = std::allocator<T>> class deque {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    class iterator;
    class const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    using map = pointer *;
    using buf = pointer;
    using map_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<buf>;
    using buf_allocator = Allocator;

    static constexpr size_type _buffer_size() {
        return (sizeof(T) < 512) ? 512 / sizeof(T) : 1;
    }
    static constexpr size_type _initial_map_size = 8;

    map _map;                // pointer to the map
    size_type _map_capacity; // capacity of the map
    iterator _start;         // pointer to the first element
    iterator _finish;        // pointer to the end element

    map_allocator _map_alloc;
    buf_allocator _buf_alloc;

    // helper functions (change private variables)
    [[nodiscard]] map _create_map(size_type n) {
        map new_map = std::allocator_traits<map_allocator>::allocate(_map_alloc, n); // T* is trivial type
        std::uninitialized_fill_n(new_map, n, nullptr); // initialize all pointers to nullptr (empty)
        return new_map;
    }

    void _deallocate_map(map &deallocate_map, size_type capacity) noexcept {
        std::allocator_traits<map_allocator>::deallocate(_map_alloc, deallocate_map, capacity);
        deallocate_map = nullptr;
    }

    void _create_buf(buf &node) {
        if (node == nullptr) {
            node = std::allocator_traits<buf_allocator>::allocate(_buf_alloc, _buffer_size());
        }
    }

    void _deallocate_buf(buf &node) noexcept {
        if (node != nullptr) {
            std::allocator_traits<buf_allocator>::deallocate(_buf_alloc, node, _buffer_size());
            node = nullptr;
        }
    }

    void _destroy_elements_and_buffer() {
        std::destroy(begin(), end());
        // for (auto it = begin(); it != end(); ++it) {
        //     std::allocator_traits<allocator_type>::destroy(get_allocator(), std::to_address(it));
        // }

        for (buf *node = _start._node; node <= _finish._node; ++node) {
            _deallocate_buf(*node);
            *node = nullptr;
        }
    }

    void _initialize_map(size_type capacity) {
        _map = _create_map(capacity);
        _map_capacity = capacity;
        try {
            buf *start_node = _map + capacity / 2;
            _create_buf(*start_node);

            pointer start_pos = *start_node + _buffer_size() / 2;
            _start = iterator(start_node, start_pos);
            _finish = _start;
        } catch (...) {
            _deallocate_map(_map, _map_capacity);
            throw;
        }
    }

    void _reallocate_map(size_type nodes_to_add, bool add_at_front) {
        const size_type old_num_nodes = _finish._node - _start._node + 1;
        const size_type new_num_nodes = old_num_nodes + nodes_to_add;

        if (_map_capacity > 2 * new_num_nodes) {
            buf *new_start_node = _map + (_map_capacity - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            if (new_start_node < _start._node) {
                std::copy(_start._node, _finish._node + 1, new_start_node);
                std::fill(new_start_node + old_num_nodes, _finish._node + 1, nullptr);
            } else {
                std::copy_backward(_start._node, _finish._node + 1, new_start_node + old_num_nodes);
                std::fill(_start._node, new_start_node, nullptr);
            }
            difference_type start_node_diff = new_start_node - _start._node;
            _start._node += start_node_diff;
            _finish._node += start_node_diff;
        } else {
            const size_type new_map_capacity = _map_capacity + std::max(_map_capacity, nodes_to_add) + 2;
            map new_map = _create_map(new_map_capacity);
            buf *new_begin = new_map + (new_map_capacity - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
            try {
                std::copy(_start._node, _finish._node + 1, new_begin);
            } catch (...) {
                _deallocate_map(new_map, new_map_capacity);
            }
            _deallocate_map(_map, _map_capacity);
            _map = new_map;
            _map_capacity = new_map_capacity;
            _start._set_node(new_begin);
            _finish._set_node(new_begin + old_num_nodes - 1);
        }
    }

    // helper function
    void _move_state(deque &&x) noexcept {
        _map = x._map;
        _map_capacity = x._map_capacity;
        _start = x._start;
        _finish = x._finish;

        x._initialize_map(x._initial_map_size);
    }

    // buffer space must be capable
    template <class Generator>
    void _uninitialized_append_impl(size_type n, const size_type space_in_last_buffer, Generator &gen) {
        const iterator old_finish = _finish;
        try {
            size_type space_in_current_buffer = space_in_last_buffer;
            while (n > 0) {
                const size_type to_fill = std::min(n, space_in_current_buffer);
                gen(_finish._current, to_fill);
                n -= to_fill;
                _finish += to_fill;

                if (n > 0) {
                    _create_buf(*_finish._node);
                    _finish._set_node(_finish._node);
                    _finish._current = _finish._first;
                }
                space_in_current_buffer = _buffer_size();
            }
        } catch (...) {
            std::destroy(old_finish, _finish);

            if (old_finish._node != _finish._node) {
                for (buf *temp = old_finish._node + 1; temp != _finish._node; ++temp) {
                    _deallocate_buf(*temp);
                }
            }
            throw;
        }
    }

    template <class FwdIter>
        requires std::forward_iterator<FwdIter>
    void _range_construct(FwdIter first, FwdIter last) {
        auto dist = std::distance(first, last);
        if (dist == 0) {
            return;
        }
        const size_type num_nodes = (dist + _buffer_size() - 1) / _buffer_size();
        _map_capacity = std::max(_initial_map_size, num_nodes + 2);
        _map = _create_map(_map_capacity);
        buf *start_node = _map + (_map_capacity - num_nodes) / 2;
        try {
            const size_type offset = ((num_nodes * _buffer_size()) - dist) / 2;
            _create_buf(*start_node);
            _start = iterator(start_node, *start_node + offset);
            _finish = _start;
            auto copy_generator = [&](pointer dest, size_type count) {
                std::uninitialized_copy_n(first, count, dest);
                std::advance(first, count);
            };
            _uninitialized_append_impl(dist, _buffer_size() - offset, copy_generator);
        } catch (...) {
            _deallocate_map(_map, _map_capacity);
            _map = nullptr;
            _map_capacity = 0;
            throw;
        }
    }

  public:
    deque() : deque(Allocator()) {}
    explicit deque(const Allocator &alloc);
    explicit deque(size_type n, const Allocator &alloc = Allocator());
    deque(size_type n, const T &value, const Allocator &alloc = Allocator());
    template <class InputIter>
        requires std::input_iterator<InputIter>
    deque(InputIter first, InputIter last, const Allocator &alloc = Allocator());
    deque(const deque &x);
    deque(deque &&x);
    deque(const deque &x, const std::type_identity_t<Allocator> &alloc);
    deque(deque &&x, const std::type_identity_t<Allocator> &alloc);
    deque(std::initializer_list<T> il, const Allocator &alloc = Allocator());
    ~deque();

    deque &operator=(const deque &x);
    deque &
    operator=(deque &&x) noexcept(std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
                                  std::allocator_traits<Allocator>::is_always_equal::value);
    deque &operator=(std::initializer_list<T> il);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    void assign(InputIter first, InputIter last);
    void assign(size_type n, const T &value);
    void assign(std::initializer_list<T> il);
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
    void resize(size_type sz, const T &value);
    void shrink_to_fit();

    // element access
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    // modifiers
    template <class... Args> reference emplace_front(Args &&...args);
    template <class... Args> reference emplace_back(Args &&...args);
    template <class... Args> iterator emplace(const_iterator position, Args &&...args);

    void push_front(const T &value);
    void push_front(T &&value);
    void push_back(const T &value);
    void push_back(T &&value);

    iterator insert(const_iterator position, const T &value);
    iterator insert(const_iterator position, T &&value);
    iterator insert(const_iterator position, size_type count, const T &value);
    template <class InputIter>
        requires std::input_iterator<InputIter>
    iterator insert(const_iterator position, InputIter first, InputIter last);
    iterator insert(const_iterator position, std::initializer_list<T> il);

    void pop_front();
    void pop_back();

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    void swap(deque &other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value);
    void clear() noexcept;
};

template <class InputIter, class Allocator = std::allocator<typename std::iterator_traits<InputIter>::value_type>>
deque(InputIter, InputIter, Allocator = Allocator())
    -> deque<typename std::iterator_traits<InputIter>::value_type, Allocator>;

export template <class T, class Allocator>
bool operator==(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

export template <class T, class Allocator>
auto operator<=>(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) {
    return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                                                  std::compare_three_way{});
}

export template <class T, class Allocator>
void swap(deque<T, Allocator> &x, deque<T, Allocator> &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}

export template <class T, class Allocator, class U>
deque<T, Allocator>::size_type erase(deque<T, Allocator> &c, const U &value) {
    auto it = std::remove(c.begin(), c.end(), value);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}

export template <class T, class Allocator, class Pred>
deque<T, Allocator>::size_type erase_if(deque<T, Allocator> &c, Pred pred) {
    auto it = std::remove_if(c.begin(), c.end(), pred);
    auto r = c.end() - it;
    c.erase(it, c.end());
    return r;
}

template <class T, class Allocator> class deque<T, Allocator>::iterator {
    friend deque;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename deque::value_type;
    using difference_type = typename deque::difference_type;
    using pointer = typename deque::pointer;
    using reference = typename deque::reference;
    using buf = typename deque::buf;

  private:
    pointer _current; // pointer to current element
    pointer _first;   // pointer to first element of current chunk(buf)
    pointer _last;    // pointer to last element of current chunk(buf)
    buf* _node;        // pointer to current chunk(buf)
    void _set_node(buf* new_node) noexcept {
        _node = new_node;
        _first = *new_node;
        _last = *new_node + _buffer_size();
    }

  public:
    iterator() noexcept : _current(nullptr), _first(nullptr), _last(nullptr), _node(nullptr) {}
    iterator(buf* node, pointer current) noexcept : _current(current) {
        _set_node(node);
    }
    iterator(const iterator &other) noexcept = default;

    reference operator*() const noexcept {
        return *_current;
    }
    pointer operator->() const noexcept {
        return _current;
    }

    iterator &operator++() noexcept {
        if (++_current == _last) {
            _set_node(_node + 1);
            _current = _first;
        }
        return *this;
    }

    iterator operator++(int) noexcept {
        iterator temp = *this;
        ++(*this);
        return temp;
    }

    iterator &operator--() noexcept {
        if (_current == _first) {
            _set_node(_node - 1);
            _current = _last;
        }
        --_current;
        return *this;
    }

    iterator operator--(int) noexcept {
        iterator temp = *this;
        --(*this);
        return temp;
    }

    iterator &operator+=(difference_type n) noexcept {
        const difference_type offset = n + _current - _first;
        if (offset >= 0 && offset < _buffer_size()) {
            _current += n;
        } else {
            const difference_type node_offset =
                (offset > 0) ? offset / _buffer_size() : -((-offset - 1) / _buffer_size()) - 1;
            _set_node(_node + node_offset);
            _current = _first + (offset - node_offset * _buffer_size());
        }
        return *this;
    }

    iterator operator+(difference_type n) const noexcept {
        iterator temp = *this;
        return temp += n;
    }

    friend iterator operator+(difference_type n, const iterator &it) noexcept {
        return it + n;
    }

    iterator &operator-=(difference_type n) noexcept {
        return *this += -n;
    }

    iterator operator-(difference_type n) const noexcept {
        iterator temp = *this;
        return temp -= n;
    }

    difference_type operator-(const iterator &other) const noexcept {
        return (_node - other._node) * _buffer_size() + (_current - _first) - (other._current - other._first);
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const iterator &other) const noexcept {
        return _current == other._current;
    }
    auto operator<=>(const iterator &other) const noexcept {
        if (auto cmp = _node <=> other._node; cmp != 0) {
            return cmp;
        }
        return _current <=> other._current;
    }

    operator const_iterator() const noexcept {
        return const_iterator(_node, _current);
    }
};

template <class T, class Allocator> class deque<T, Allocator>::const_iterator {
    friend deque;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename deque::value_type;
    using difference_type = typename deque::difference_type;
    using pointer = typename deque::const_pointer;
    using reference = typename deque::const_reference;
    using const_pointer = typename deque::const_pointer;
    using const_reference = typename deque::const_reference;
    using buf = typename deque::buf;

  private:
    pointer _current; // pointer to current element
    pointer _first;   // pointer to first element of current chunk(buf)
    pointer _last;    // pointer to last element of current chunk(buf)
    buf* _node;        // pointer to current chunk(buf)

    void _set_node(buf* new_node) noexcept {
        _node = new_node;
        _first = *new_node;
        _last = *new_node + _buffer_size();
    }

  public:
    const_iterator() noexcept : _current(nullptr), _first(nullptr), _last(nullptr), _node(nullptr) {}
    const_iterator(buf* node, pointer current) noexcept : _current(current) {
        _set_node(node);
    }
    const_iterator(const const_iterator &other) noexcept = default;
    const_iterator &operator=(const const_iterator &other) noexcept = default;

    reference operator*() const noexcept {
        return *_current;
    }
    pointer operator->() const noexcept {
        return _current;
    }

    const_iterator &operator++() noexcept {
        if (++_current == _last) {
            _set_node(_node + 1);
            _current = _first;
        }
        return *this;
    }

    const_iterator operator++(int) noexcept {
        const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    const_iterator &operator--() noexcept {
        if (_current-- == _first) {
            _set_node(_node - 1);
            _current = _last - 1;
        }
        return *this;
    }

    const_iterator operator--(int) noexcept {
        const_iterator temp = *this;
        --(*this);
        return temp;
    }

    const_iterator &operator+=(difference_type n) noexcept {
        const difference_type offset = n + _current - _first;
        if (offset >= 0 && offset < _buffer_size()) {
            _current += n;
        } else {
            const difference_type node_offset =
                (offset > 0) ? offset / _buffer_size() : -((-offset - 1) / _buffer_size()) - 1;
            _set_node(_node + node_offset);
            _current = _first + (offset - node_offset * _buffer_size());
        }
        return *this;
    }

    const_iterator operator+(difference_type n) const noexcept {
        const_iterator temp = *this;
        return temp += n;
    }

    friend const_iterator operator+(difference_type n, const const_iterator &it) noexcept {
        return it + n;
    }

    const_iterator &operator-=(difference_type n) noexcept {
        return *this += -n;
    }

    const_iterator operator-(difference_type n) const noexcept {
        const_iterator temp = *this;
        return temp -= n;
    }

    difference_type operator-(const const_iterator &other) const noexcept {
        return (_node - other._node) * _buffer_size() + (_current - _first) - (other._current - other._first);
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const const_iterator &other) const noexcept {
        return _current == other._current;
    }
    auto operator<=>(const const_iterator &other) const noexcept {
        if (auto cmp = _node <=> other._node; cmp != 0) {
            return cmp;
        }
        return _current <=> other._current;
    }
};
} // namespace j

namespace j {
    template<class T, class Allocator>
    deque<T, Allocator>::deque(const Allocator &alloc) : _size(0), _map_alloc(alloc), _box_alloc(alloc) {
        _map = _create_map(default_map_size);
        _map_capacity = default_map_size;
        _begin_map_index = 0;
        _begin_box_index = 0;
        _end_map_index = 0;
        _end_box_index = 0;
    }

    template<class T, class Allocator>
    deque<T, Allocator>::deque(deque::size_type n, const Allocator &alloc) : deque(n, T(), alloc) {}

    template<class T, class Allocator>
    deque<T, Allocator>::deque(deque::size_type n, const T &value, const Allocator &alloc) : deque(alloc) {
        for (size_type i = 0; i < n; ++i) {
            emplace_back(value);
        }
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    deque<T, Allocator>::deque(InputIter first, InputIter last, const Allocator &alloc) : deque(alloc) {
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }

    template<class T, class Allocator>
    deque<T, Allocator>::deque(const deque &other)
        : deque(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())) {
        for (const T &t : other) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    deque<T, Allocator>::deque(deque &&other) : deque(std::move(other), other.get_allocator()) {}

    template<class T, class Allocator>
    deque<T, Allocator>::deque(const deque &other, const std::type_identity_t <Allocator> &alloc) : deque(alloc) {
        for (const T &t : other) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    deque<T, Allocator>::deque(deque &&other, const std::type_identity_t <Allocator> &alloc)
        : deque(alloc) {
        _destroy_map();
        _map = other._get_map();
        _map_capacity = other._get_map_capacity();
        _size = other.size();
        _begin_map_index = other._get_begin_map_index();
        _begin_box_index = other._get_begin_box_index();
        _end_map_index = other._get_end_map_index();
        _end_box_index = other._get_end_box_index();

        other._map = _create_map(default_map_size);
        other._set_map_capacity(default_map_size);
        other._set_size(0);
        other._set_begin_map_index(0);
        other._set_begin_box_index(0);
        other._set_end_map_index(0);
        other._set_end_box_index(0);
    }

    template<class T, class Allocator>
    deque<T, Allocator>::deque(std::initializer_list<T> il, const Allocator &alloc) : deque(alloc) {
        for (const T &t : il) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    deque<T, Allocator>::~deque() {
        clear();
        _destroy_map();
    }

    template<class T, class Allocator>
    deque<T, Allocator>& deque<T, Allocator>::operator=(const deque &other) {
        if (this != &other) {
            clear();
            _reallocate_map(other._get_map_capacity());
            for (const T& t : other) {
                emplace_back(t);
            }
        }
        return *this;
    }

    template<class T, class Allocator>
    deque<T, Allocator>& deque<T, Allocator>::operator=(deque &&other)
            noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
        if (this != &other) {
            clear();
            _destroy_map();
            _map = other._get_map();
            _map_capacity = other._get_map_capacity();
            _size = other.size();
            _begin_map_index = other._get_begin_map_index();
            _begin_box_index = other._get_begin_box_index();
            _end_map_index = other._get_end_map_index();
            _end_box_index = other._get_end_box_index();

            other._map = _create_map(default_map_size);
            other._set_map_capacity(default_map_size);
            other._set_size(0);
            other._set_begin_map_index(0);
            other._set_begin_box_index(0);
            other._set_end_map_index(0);
            other._set_end_box_index(0);
        }
        return *this;
    }

    template<class T, class Allocator>
    deque<T, Allocator>& deque<T, Allocator>::operator=(std::initializer_list<T> il) {
        clear();
        for (const T &t : il) {
            emplace_back(t);
        }
        return *this;
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    void deque<T, Allocator>::assign(InputIter first, InputIter last) {
        clear();
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::assign(deque::size_type n, const T &value) {
        clear();
        for (size_type i = 0; i < n; ++i) {
            emplace_back(value);
        }
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::assign(std::initializer_list<T> il) {
        clear();
        for (const T &t : il) {
            emplace_back(t);
        }
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::allocator_type deque<T, Allocator>::get_allocator() const noexcept {
        return _box_alloc;
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::begin() noexcept {
        return iterator(_map[_begin_map_index] + _begin_box_index, this, _begin_map_index, _begin_box_index);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_iterator deque<T, Allocator>::begin() const noexcept {
        return const_iterator(_map[_begin_map_index] + _begin_box_index);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::end() noexcept {
        return iterator(_map[_end_map_index] + _end_box_index, this, _end_map_index, _end_box_index);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_iterator deque<T, Allocator>::end() const noexcept {
        return const_iterator(_map[_end_map_index] + _end_box_index);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::reverse_iterator deque<T, Allocator>::rbegin() noexcept {
        return reverse_iterator(end());
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::reverse_iterator deque<T, Allocator>::rend() noexcept {
        return reverse_iterator(begin());
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_iterator deque<T, Allocator>::cbegin() const noexcept {
        return const_iterator(_map[_begin_map_index] + _begin_box_index);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_iterator deque<T, Allocator>::cend() const noexcept {
        return const_iterator(_map[_end_map_index] + _end_box_index);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    template<class T, class Allocator>
    bool deque<T, Allocator>::empty() const noexcept {
        return size() == 0;
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::size_type deque<T, Allocator>::size() const noexcept {
        return _size;
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::size_type deque<T, Allocator>::max_size() const noexcept {
        return std::allocator_traits<Allocator>::max_size(_box_alloc) * _box_size;
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::resize(deque::size_type sz) {
        resize(sz, T());
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::resize(deque::size_type sz, const T &value) {
        if (sz < size()) {
            const size_type target_size = size() - sz;
            for (size_type i = 0; i < target_size; ++i) {
                pop_back();
            }
        } else if (sz > size()) {
            const size_type target_size = sz - size();
            for (size_type i = 0; i < target_size; ++i) {
                emplace_back(value);
            }
        }
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::shrink_to_fit() {
        const size_type old_map_capacity = _get_map_capacity();
        if (_get_begin_map_index() <= _get_end_map_index()) {
            for (size_type i = 0; i < old_map_capacity; ++i) {
                if (i < _get_begin_map_index() || i > _get_end_map_index()) {
                    _destroy_box(i);
                }
            }
        }
        else {
            for (size_type i = 0; i < old_map_capacity; ++i) {
                if (i < _get_begin_map_index() && i > _get_end_map_index()) {
                    _destroy_box(i);
                }
            }
        }
        size_t fit_size = default_map_size;
        const size_t now_map_size = 2 * _map_size();
        while (fit_size < now_map_size) {
            fit_size *= 2;
        }
        _reallocate_map(fit_size);
    }   // free unused memory, and reallocate map (2 * size <= 4, 8, 16, ...)

    template<class T, class Allocator>
    typename deque<T, Allocator>::reference deque<T, Allocator>::operator[](deque::size_type n) {
        return _map[(_begin_map_index + (_begin_box_index + n) / _box_size)  % _map_capacity]
                   [(_begin_box_index + n) % _box_size];
    } // circular

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reference deque<T, Allocator>::operator[](deque::size_type n) const {
        return _get_map()[(_begin_map_index + (_begin_box_index + n) / _box_size)  % _map_capacity]
                         [(_begin_box_index + n) % _box_size];
    } // circular

    template<class T, class Allocator>
    typename deque<T, Allocator>::reference deque<T, Allocator>::at(deque::size_type n) {
        if (n >= size()) {
            throw std::out_of_range("deque::at() : index is out of range");
        }
        return this->operator[](n);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reference deque<T, Allocator>::at(deque::size_type n) const {
        if (n >= size()) {
            throw std::out_of_range("deque::at() : index is out of range");
        }
        return this->operator[](n);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::reference deque<T, Allocator>::front() {
        return _map[_begin_map_index][_begin_box_index];
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reference deque<T, Allocator>::front() const {
        return _get_map()[_begin_map_index][_begin_box_index];
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::reference deque<T, Allocator>::back() {
        return this->operator[](size() - 1);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::const_reference deque<T, Allocator>::back() const {
        return this->operator[](size() - 1);
    }

    template <class T, class Allocator>
    template <class... Args>
    typename deque<T, Allocator>::reference deque<T, Allocator>::emplace_front(Args &&... args) {
        if (_get_begin_box_index() == 0) {
            if (_map_size() == _get_map_capacity()) {
                _reallocate_map(2 * _get_map_capacity());
            }
            if (_get_begin_map_index() == 0) {
                _set_begin_map_index(_get_map_capacity() - 1);
            } else {
                _set_begin_map_index(_get_begin_map_index() - 1);
            }
            if (_get_map()[_get_begin_map_index()] == nullptr) {
                _create_box(_get_begin_map_index());
            }
            _set_begin_box_index(_box_size);
        }
        _set_begin_box_index(_get_begin_box_index() - 1);
        _set_size(size() + 1);
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_begin_map_index()] + _get_begin_box_index(), std::forward<Args>(args)...);
        return _map[_get_begin_map_index()][_get_begin_box_index()];
    }

    template<class T, class Allocator>
    template<class... Args>
    typename deque<T, Allocator>::reference deque<T, Allocator>::emplace_back(Args &&... args) {
        if (_get_map()[_get_end_map_index()] == nullptr) {
                _create_box(_get_end_map_index());
        }
        std::allocator_traits<box_allocator>::construct
            (_box_alloc, _map[_get_end_map_index()] + _get_end_box_index(), std::forward<Args>(args)...);
        if (_get_end_box_index() == _box_size - 1) {
            if (_map_size() == _get_map_capacity()) {
                _reallocate_map(2 * _get_map_capacity());
            }
            if (_get_end_map_index() == _get_map_capacity() - 1) {
                _set_end_map_index(0);
            } else {
                _set_end_map_index(_get_end_map_index() + 1);
            }
            if (_get_map()[_get_end_map_index()] == nullptr) {
                _create_box(_get_end_map_index());
            }
            _set_end_box_index(-1);
        }
        _set_end_box_index(_get_end_box_index() + 1);
        _set_size(size() + 1);
        return _map[_get_end_map_index()][_get_end_box_index()];
    }

    template<class T, class Allocator>
    template<class... Args>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::emplace(deque::const_iterator position, Args &&... args) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin == 0) {
            emplace_front(std::forward<Args>(args)...);
            return begin();
        } else if (distance_from_begin == size()) {
            emplace_back(std::forward<Args>(args)...);
            return end() - 1;
        } else if (distance_from_begin < size() / 2) {
            reference value = std::move(this->operator[](0));
            for (difference_type  i = 0; i < distance_from_begin; ++i) {
                this->operator[](i) = std::move(this->operator[](i + 1));
            }
            this->operator[](distance_from_begin) = value_type(std::forward<Args>(args)...);
            emplace_front(value);
        } else {
            reference value = std::move(this->operator[](size() - 1));
            for (difference_type  i = size() - 1; i > distance_from_begin; --i) {
                this->operator[](i) = std::move(this->operator[](i - 1));
            }
            this->operator[](distance_from_begin) = value_type(std::forward<Args>(args)...);
            emplace_back(value);
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::push_front(const T &value) {
        emplace_front(value);
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::push_front(T &&value) {
        emplace_front(std::move(value));
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::push_back(const T &value) {
        emplace_back(value);
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::push_back(T &&value) {
        emplace_back(std::move(value));
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::insert
            (deque::const_iterator position, const T &value) {
        return emplace(position, value);
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::insert
            (deque::const_iterator position, T &&value) {
        return emplace(position, std::move(value));
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::insert
            (deque::const_iterator position, deque::size_type count, const T &value) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin == 0) {
            for (size_type i = 0; i < count; ++i) {
                emplace_front(value);
            }
            return begin();
        } else if (distance_from_begin == size()) {
            for (size_type i = 0; i < count; ++i) {
                emplace_back(value);
            }
            return end() - count;
        } else if (distance_from_begin < size() / 2) {
            if (distance_from_begin + 1 <= count) {
                T arr[distance_from_begin + 1];
                difference_type i = 0;
                for (; i <= distance_from_begin; ++i) {
                    arr[i] = std::move(this->operator[](i));
                    this->operator[](i) = value;
                }
                for (; i < count; ++i) {
                    emplace_front(value);
                }
                for (i = distance_from_begin; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            } else {
                T arr[count];
                difference_type i = 0;
                for (; i < distance_from_begin - count; ++i) {
                    if (i < count) {
                        arr[i] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i + count));
                }
                for (; i <= distance_from_begin; ++i) {
                    this->operator[](i) = value;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            }
        } else {
            if (size() - distance_from_begin <= count) {
                const size_type current_size = size();
                T arr[current_size - distance_from_begin];
                difference_type i = distance_from_begin;
                for (; i < current_size; ++i) {
                    arr[i - distance_from_begin] = std::move(this->operator[](i));
                    this->operator[](i) = value;
                }
                for (; i < distance_from_begin + count; ++i) {
                    emplace_back(value);
                }
                for (i = current_size - distance_from_begin - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            } else {
                T arr[count];
                const size_type current_size = size();
                difference_type i = current_size - 1;
                for (; i >= distance_from_begin + count; --i) {
                    if (i >= current_size - count) {
                        arr[current_size - i - 1] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i - count));
                }
                for (; i >= distance_from_begin; --i) {
                    this->operator[](i) = value;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            }
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    template<class InputIter>
    requires (!std::is_integral_v<InputIter>)
    typename deque<T, Allocator>::iterator deque<T, Allocator>::insert
            (deque::const_iterator position, InputIter first, InputIter last) {
        difference_type count = std::distance(first, last);
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin == size()) {
            for (; first != last; ++first) {
                emplace_back(*first);
            }
            return end() - count;
        } else if (distance_from_begin < size() / 2) {
            if (distance_from_begin + 1 <= count) {
                T arr[distance_from_begin + 1];
                difference_type i = 0;
                for (; i <= distance_from_begin; ++i, ++first) {
                    arr[i] = std::move(this->operator[](i));
                    this->operator[](i) = *first;
                }
                for (; i < count; ++i, ++first) {
                    emplace_front(*first);
                }
                for (i = distance_from_begin; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            } else {
                T arr[count];
                difference_type i = 0;
                for (; i < distance_from_begin - count; ++i) {
                    if (i < count) {
                        arr[i] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i + count));
                }
                for (; i <= distance_from_begin; ++i, ++first) {
                    this->operator[](i) = *first;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_front(arr[i]);
                }
            }
        } else {
            if (size() - distance_from_begin <= count) {
                const size_type current_size = size();
                T arr[current_size - distance_from_begin];
                difference_type i = distance_from_begin;
                for (; i < current_size; ++i, ++first) {
                    arr[i - distance_from_begin] = std::move(this->operator[](i));
                    this->operator[](i) = *first;
                }
                for (; i < distance_from_begin + count; ++i, ++first) {
                    emplace_back(*first);
                }
                for (i = current_size - distance_from_begin - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            } else {
                T arr[count];
                const size_type current_size = size();
                difference_type i = current_size - 1;
                for (; i >= distance_from_begin + count; --i) {
                    if (i >= current_size - count) {
                        arr[current_size - i - 1] = std::move(this->operator[](i));
                    }
                    this->operator[](i) = std::move(this->operator[](i - count));
                }
                for (i = distance_from_begin; i < distance_from_begin + count; ++i, ++first) {
                    this->operator[](i) = *first;
                }
                for (i = count - 1; i >= 0; --i) {
                    emplace_back(std::move(arr[i]));
                }
            }
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::insert
            (deque::const_iterator position, std::initializer_list<T> il) {
        return insert(position, il.begin(), il.end());
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::pop_front() {
        if (_get_begin_box_index() == _box_size - 1) {
            _set_begin_map_index((_get_begin_map_index() + 1) % _map_capacity);
            _set_begin_box_index(0);
            _set_size(size() - 1);
            if (_get_map_capacity() != default_map_size && _map_size() <= _get_map_capacity() / 4) {
                shrink_to_fit();
            }
        } else {
            _set_begin_box_index(_get_begin_box_index() + 1);
            _set_size(size() - 1);
        }
        std::allocator_traits<box_allocator>::destroy_at(_map[_get_begin_map_index()] + _get_begin_box_index());
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::pop_back() {
        if (_get_end_box_index() == 0) {
            _set_end_map_index((_get_end_map_index() - 1) % _map_capacity);
            _set_end_box_index(_box_size - 1);
            _set_size(size() - 1);
            if (_get_map_capacity() != default_map_size && _map_size() <= _get_map_capacity() / 4) {
                shrink_to_fit();
            }
        } else {
            _set_size(size() - 1);
            _set_end_box_index(_get_end_box_index() - 1);
        }
        std::allocator_traits<box_allocator>::destroy_at(_map[_get_end_map_index()] + _get_end_box_index());
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::erase(deque::const_iterator position) {
        const difference_type distance_from_begin = std::distance(cbegin(), position);
        if (distance_from_begin < size() / 2) {
            for (difference_type i = distance_from_begin; i > 0; --i) {
                this->operator[](i) = std::move(this->operator[](i - 1));
            }
            pop_front();
        } else {
            for (difference_type i = distance_from_begin; i < size() - 1; ++i) {
                this->operator[](i) = std::move(this->operator[](i + 1));
            }
            pop_back();
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    typename deque<T, Allocator>::iterator deque<T, Allocator>::erase
            (deque::const_iterator first, deque::const_iterator last) {
        const difference_type count = std::distance(first, last);
        const difference_type distance_from_begin = std::distance(cbegin(), first);
        if (distance_from_begin < size() / 2) {
            difference_type i = distance_from_begin + count - 1;
            for (; i >= count; --i) {
                this->operator[](i) = std::move(this->operator[](i - count));
            }
            for (i = 0; i < count; ++i) {
                pop_front();
            }
        } else {
            difference_type i = distance_from_begin;
            for (; i < size() - count; ++i) {
                this->operator[](i) = std::move(this->operator[](i + count));
            }
            for (i = 0; i < count; ++i) {
                pop_back();
            }
        }
        return begin() + distance_from_begin;
    }

    template<class T, class Allocator>
    void deque<T, Allocator>::swap(deque &other)
    noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
        using std::swap;
        swap(_map, other._map);
        swap(_size, other._size);
        swap(_map_capacity, other._map_capacity);
        swap(_begin_map_index, other._begin_map_index);
        swap(_begin_box_index, other._begin_box_index);
        swap(_end_map_index, other._end_map_index);
        swap(_end_box_index, other._end_box_index);
        swap(_map_alloc, other._map_alloc);
        swap(_box_alloc, other._box_alloc);
    }
    template<class T, class Allocator>
    void deque<T, Allocator>::clear() noexcept {
        std::destroy(begin(), end());
        for (size_type i = 0; i < _map_capacity; ++i) {
            _destroy_box(i);
        }
    }
}