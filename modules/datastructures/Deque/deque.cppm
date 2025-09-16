/*
 * @ Created by jaehyung409 on 25. 1. 27.
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 */

module;
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>

export module j:deque;

import :unique_ptr;
import :memory;
import :vector;

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

    // helper functions (create and delete)
    [[nodiscard]] map _allocate_map(size_type n) {
        map new_map = std::allocator_traits<map_allocator>::allocate(_map_alloc, n); // T* is trivial type
        std::fill_n(new_map, n, nullptr); // initialize all pointers to nullptr (empty)
        return new_map;
    }

    void _deallocate_map(map &deallocate_map, size_type capacity) noexcept {
        std::allocator_traits<map_allocator>::deallocate(_map_alloc, deallocate_map, capacity);
        deallocate_map = nullptr;
    }

    [[nodiscard]] buf _allocate_buf() {
        return std::allocator_traits<buf_allocator>::allocate(_buf_alloc, _buffer_size());
    }

    void _deallocate_buf(buf &node) noexcept {
        if (node != nullptr) {
            std::allocator_traits<buf_allocator>::deallocate(_buf_alloc, node, _buffer_size());
            node = nullptr;
        }
    }

    void _destroy_elements_and_buffer() {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (auto it = begin(); it != end(); ++it) {
                std::allocator_traits<buf_allocator>::destroy(_buf_alloc, std::to_address(it));
            }
        }
        for (buf *node = _start._node; node <= _finish._node; ++node) {
            _deallocate_buf(*node);
            *node = nullptr;
        }
    }

    // helper class (guard)
    class buffer_guard {
      private:
        buf _buffer;
        buf_allocator _buf_alloc;
        size_type _constructed_count;

      public:
        buffer_guard() : _buffer(nullptr), _constructed_count(0) {}
        buffer_guard(buf buffer, const buf_allocator &allocator)
            : _buffer(buffer), _buf_alloc(allocator), _constructed_count(0) {}
        ~buffer_guard() {
            if (_buffer) {
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    for (size_type i = 0; i < _constructed_count; ++i) {
                        std::allocator_traits<buf_allocator>::destroy(_buf_alloc, _buffer + i);
                    }
                }
                std::allocator_traits<buf_allocator>::deallocate(_buf_alloc, _buffer, _buffer_size());
                _buffer = nullptr;
            }
        }
        buffer_guard(const buffer_guard &) = delete;
        buffer_guard &operator=(const buffer_guard &) = delete;
        buffer_guard(buffer_guard &&other) noexcept
            : _buffer(other._buffer), _buf_alloc(other._buf_alloc), _constructed_count(other._constructed_count) {
            other._buffer = nullptr;
            other._constructed_count = 0;
        }
        buffer_guard &operator=(buffer_guard &&other) noexcept {
            if (this != &other) {
                if (_buffer) {
                    if constexpr (!std::is_trivially_destructible_v<T>) {
                        for (size_type i = 0; i < _constructed_count; ++i) {
                            std::allocator_traits<buf_allocator>::destroy(_buf_alloc, _buffer + i);
                        }
                    }
                    std::allocator_traits<buf_allocator>::deallocate(_buf_alloc, _buffer, _buffer_size());
                }
                _buffer = other._buffer;
                _constructed_count = other._constructed_count;
                _buf_alloc = std::move(other._buf_alloc);
                other._buffer = nullptr;
                other._constructed_count = 0;
            }
            return *this;
        }

        void set_constructed_count(size_type count) {
            _constructed_count = count;
        }

        buf release() {
            buf temp = _buffer;
            _buffer = nullptr;
            return temp;
        }

        buf get() const {
            return _buffer;
        }
    };

    // helper functions
    void _initialize_map(size_type capacity) {
        unique_ptr map_guard(_allocate_map(capacity), _map_alloc, capacity);
        unique_ptr buf_guard(_allocate_buf(), _buf_alloc, _buffer_size());

        buf *start_node = map_guard.get() + capacity / 2;

        _map = map_guard.release();
        _map_capacity = capacity;
        *start_node = buf_guard.release();
        pointer start_pos = *start_node + _buffer_size() / 2;
        _start = iterator(start_node, start_pos);
        _finish = _start;
    }

    void _reallocate_map(size_type nodes_to_add, bool add_at_front) {
        const size_type old_num_nodes = _finish._node - _start._node + 1;
        const size_type new_num_nodes = old_num_nodes + nodes_to_add;
        const size_type new_map_capacity = (_map_capacity > 2 * new_num_nodes)
                                               ? _map_capacity
                                               : _map_capacity + std::max(_map_capacity, nodes_to_add) + 2;

        unique_ptr new_map_guard(_allocate_map(new_map_capacity), _map_alloc, new_map_capacity);

        buf *new_start_node =
            new_map_guard.get() + (new_map_capacity - old_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
        std::copy(_start._node, _finish._node + 1, new_start_node);

        _deallocate_map(_map, _map_capacity);
        _map = new_map_guard.release();
        _map_capacity = new_map_capacity;
        _start._set_node(new_start_node);
        _finish._set_node(new_start_node + (old_num_nodes - 1));
    }

    void _move_state(deque &&x) noexcept {
        _map = x._map;
        _map_capacity = x._map_capacity;
        _start = x._start;
        _finish = x._finish;

        x._initialize_map(x._initial_map_size);
    }

    // pre-require : _start._current == _start._first
    void _ensure_front_map_space() {
        if (_start._node == _map) {
            _reallocate_map(1, true);
        }
    }

    // pre-require : _finish._current == _finish._last - 1
    void _ensure_back_map_space() {
        if (_finish._node == _map + _map_capacity - 1) {
            _reallocate_map(1, false);
        }
    }

    template <class... Args>
    void _shift_left_and_emplace(const difference_type distance_from_begin, iterator emplace_pos, Args &&...args);

    template <class... Args>
    void _shift_right_and_emplace(const difference_type distance_from_end, iterator emplace_pos, Args &&...args);

    void _shift_left_and_insert(const T &value, const difference_type distance_from_begin, iterator emplace_pos);

    void _shift_right_and_insert(const T &value, const difference_type distance_from_end, iterator emplace_pos);

    iterator _insert_impl(const_iterator position, const T &value);

    template <class InputIter>
        requires std::forward_iterator<InputIter>
    size_type calc_move_now(InputIter first, size_type count, iterator dest);
    template <class InputIter>
        requires std::forward_iterator<InputIter>
    size_type calc_move_backward_now(InputIter last, size_type count, iterator dest);

    void _uninitialized_fill_n(Allocator alloc, iterator first, size_type count, const T &value);

    void _fill_n(iterator first, size_type count, const T &value);

    template <class InputIter>
        requires std::forward_iterator<InputIter>
    iterator _uninitialized_move_n(Allocator alloc, InputIter first, size_type count, iterator dest);

    template <class InputIter>
        requires std::forward_iterator<InputIter>
    iterator _move_n(InputIter first, size_type count, iterator dest);

    template <class InputIter>
        requires std::forward_iterator<InputIter>
    iterator _move_backward_n(InputIter first, size_type count, iterator dest);

    template <class InputIter>
        requires std::forward_iterator<InputIter>
    iterator _uninitialized_copy_n(Allocator alloc, InputIter first, size_type count, iterator dest);

    template <class InputIter>
        requires std::forward_iterator<InputIter>
    iterator _copy_n(InputIter first, size_type count, iterator dest);

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
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;
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
auto operator<=>(const deque<T, Allocator> &lhs, const deque<T, Allocator> &rhs) -> std::strong_ordering {
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
    buf *_node;       // pointer to current chunk(buf)
    void _set_node(buf *new_node) noexcept {
        _node = new_node;
        _first = *new_node;
        _last = *new_node + _buffer_size();
    }

  public:
    iterator() noexcept : _current(nullptr), _first(nullptr), _last(nullptr), _node(nullptr) {}
    iterator(buf *node, pointer current) noexcept : _current(current) {
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
    auto operator<=>(const iterator &other) const noexcept -> std::strong_ordering {
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
    buf *_node;       // pointer to current chunk(buf)

    void _set_node(buf *new_node) noexcept {
        _node = new_node;
        _first = *new_node;
        _last = *new_node + _buffer_size();
    }

  public:
    const_iterator() noexcept : _current(nullptr), _first(nullptr), _last(nullptr), _node(nullptr) {}
    const_iterator(buf *node, pointer current) noexcept : _current(current) {
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
    auto operator<=>(const const_iterator &other) const noexcept -> std::strong_ordering {
        if (auto cmp = _node <=> other._node; cmp != 0) {
            return cmp;
        }
        return _current <=> other._current;
    }
};
} // namespace j

namespace j {
template <class T, class Allocator>
template <class... Args>
void deque<T, Allocator>::_shift_left_and_emplace(const difference_type distance_from_begin, iterator emplace_pos,
                                                  Args &&...args) {
    std::allocator_traits<buf_allocator>::construct(_buf_alloc, (_start - 1)._current, std::move(*_start._current));
    _move_n(_start + 1, distance_from_begin - 1, _start);
    std::allocator_traits<buf_allocator>::destroy(_buf_alloc, emplace_pos._current);
    std::allocator_traits<buf_allocator>::construct(_buf_alloc, emplace_pos._current, std::forward<Args>(args)...);
}

template <class T, class Allocator>
template <class... Args>
void deque<T, Allocator>::_shift_right_and_emplace(const difference_type distance_from_end, iterator emplace_pos,
                                                   Args &&...args) {
    std::allocator_traits<buf_allocator>::construct(_buf_alloc, _finish._current, std::move(*(_finish - 1)._current));
    _move_backward_n(emplace_pos, distance_from_end - 1, _finish);
    std::allocator_traits<buf_allocator>::destroy(_buf_alloc, emplace_pos._current);
    std::allocator_traits<buf_allocator>::construct(_buf_alloc, emplace_pos._current, std::forward<Args>(args)...);
}

template <class T, class Allocator>
void deque<T, Allocator>::_shift_left_and_insert(const T &value, const difference_type distance_from_begin,
                                                 iterator emplace_pos) {
    std::allocator_traits<buf_allocator>::construct(_buf_alloc, (_start - 1)._current, std::move(*_start._current));
    _move_n(_start + 1, distance_from_begin - 1, _start);
    *emplace_pos = value;
}

template <class T, class Allocator>
void deque<T, Allocator>::_shift_right_and_insert(const T &value, const difference_type distance_from_end,
                                                  iterator emplace_pos) {
    std::allocator_traits<buf_allocator>::construct(_buf_alloc, _finish._current, std::move(*(_finish - 1)._current));
    _move_backward_n(emplace_pos, distance_from_end - 1, _finish);
    *emplace_pos = value;
}

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::_insert_impl(const_iterator position, const T &value) {
    if (cbegin() == position) {
        push_front(value);
        return begin();
    }
    if (cend() == position) {
        push_back(value);
        return end() - 1;
    }
    const difference_type distance_from_begin = std::distance(cbegin(), position);
    const difference_type distance_from_end = std::distance(position, cend());
    iterator insert_pos = _start + distance_from_begin;
    if (distance_from_begin < distance_from_end) {
        if (_start._current == _start._first) {
            _ensure_front_map_space();
            buffer_guard buf_guard(_allocate_buf(), _buf_alloc);
            *(_start._node - 1) = buf_guard.get();
            _shift_left_and_insert(value, distance_from_begin, insert_pos);
            buf_guard.release();
        } else {
            _shift_left_and_insert(value, distance_from_begin, insert_pos);
        }
        --_start;
    } else {
        if (_finish._current == _finish._last - 1) {
            _ensure_back_map_space();
            buffer_guard buf_guard(_allocate_buf(), _buf_alloc);
            *(_finish._node + 1) = buf_guard.get();
            _shift_right_and_insert(value, distance_from_end, insert_pos);
            buf_guard.release();
        } else {
            _shift_right_and_insert(value, distance_from_end, insert_pos);
        }
        ++_finish;
    }
    return insert_pos;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::size_type deque<T, Allocator>::calc_move_now(InputIter first, size_type count, iterator dest) {
    const size_type dest_buffer_remaining = _buffer_size() - (dest._current - dest._first);

    if constexpr (std::is_same_v<InputIter, iterator> || std::is_same_v<InputIter, const_iterator>) {
        const size_type source_buffer_remaining = _buffer_size() - (first._current - first._first);
        return std::min({count, source_buffer_remaining, dest_buffer_remaining});
    } else {
        return std::min(count, dest_buffer_remaining);
    }
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::size_type deque<T, Allocator>::calc_move_backward_now(InputIter last, size_type count,
                                                                           iterator dest) {
    const size_type dest_buffer_remaining =
        (dest._current - dest._first) ? dest._current - dest._first : _buffer_size();

    if constexpr (std::is_same_v<InputIter, iterator> || std::is_same_v<InputIter, const_iterator>) {
        const size_type source_buffer_remaining =
            (last._current - last._first) ? last._current - last._first : _buffer_size();
        return std::min({count, source_buffer_remaining, dest_buffer_remaining});
    } else {
        return std::min(count, dest_buffer_remaining);
    }
}

template <class T, class Allocator>
void deque<T, Allocator>::_uninitialized_fill_n(Allocator alloc, iterator first, size_type count, const T &value) {
    if (count == 0)
        return;

    iterator original_first = first;
    try {
        while (count > 0) {
            const size_type buffer_remaining = _buffer_size() - (first._current - first._first);

            const size_type fill_now = std::min(count, buffer_remaining);

            uninitialized_fill_n_contiguous(alloc, first._current, fill_now, value);

            first += fill_now;
            count -= fill_now;
        }
    } catch (...) {
        for (iterator it = original_first; it != first; ++it) {
            std::allocator_traits<Allocator>::destroy(alloc, std::addressof(*it));
        }
        throw;
    }
}

template <class T, class Allocator> void deque<T, Allocator>::_fill_n(iterator first, size_type count, const T &value) {
    if (count == 0)
        return;

    while (count > 0) {
        const size_type buffer_remaining = _buffer_size() - (first._current - first._first);

        const size_type fill_now = std::min(count, buffer_remaining);

        std::fill_n(first._current, fill_now, value);

        first += fill_now;
        count -= fill_now;
    }
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::iterator deque<T, Allocator>::_uninitialized_move_n(Allocator alloc, InputIter first,
                                                                         size_type count, iterator dest) {
    if (count == 0)
        return dest;

    iterator original_dest = dest;

    try {
        while (count > 0) {
            const size_type move_now = calc_move_now(first, count, dest);
            if constexpr (std::is_same_v<InputIter, iterator>) {
                uninitialized_move_n_contiguous(alloc, first._current, move_now, dest._current);
            } else {
                uninitialized_move_n_contiguous(alloc, first, move_now, dest._current);
            }

            first += move_now;
            dest += move_now;
            count -= move_now;
        }
        return dest;
    } catch (...) {
        for (; original_dest != dest; ++original_dest) {
            std::allocator_traits<Allocator>::destroy(alloc, std::addressof(*original_dest));
        }
        throw;
    }
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::iterator deque<T, Allocator>::_move_n(InputIter first, size_type count, iterator dest) {
    if (count == 0)
        return dest;

    iterator original_dest = dest;

    while (count > 0) {
        const size_type move_now = calc_move_now(first, count, dest);
        if constexpr (std::is_same_v<InputIter, iterator> || std::is_same_v<InputIter, const_iterator>) {
            move_n_contiguous(first._current, move_now, dest._current);
        } else {
            move_n_contiguous(first, move_now, dest._current);
        }

        first += move_now;
        dest += move_now;
        count -= move_now;
    }
    return dest;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::iterator deque<T, Allocator>::_move_backward_n(InputIter first, size_type count, iterator dest) {
    if (count == 0)
        return dest;

    InputIter last = first + count;

    while (count > 0) {
        const size_type move_now = calc_move_backward_now(last, count, dest);

        last -= move_now;
        pointer destination = (dest._current == dest._first) ? (dest - 1)._last : dest._current;
        if constexpr (std::is_same_v<InputIter, iterator> || std::is_same_v<InputIter, const_iterator>) {
            move_backward_n_contiguous(last._current, move_now, destination);
        } else {
            move_backward_n_contiguous(last, move_now, destination);
        }

        dest -= move_now;
        count -= move_now;
    }
    return dest;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::iterator deque<T, Allocator>::_uninitialized_copy_n(Allocator alloc, InputIter first,
                                                                         size_type count, iterator dest) {
    if (count == 0)
        return dest;

    iterator original_dest = dest;

    try {
        while (count > 0) {
            const size_type move_now = calc_move_now(first, count, dest);
            if constexpr (std::is_same_v<InputIter, iterator> || std::is_same_v<InputIter, const_iterator>) {
                uninitialized_copy_n_contiguous(alloc, first._current, move_now, dest._current);
            } else {
                uninitialized_copy_n_contiguous(alloc, first, move_now, dest._current);
            }

            first += move_now;
            dest += move_now;
            count -= move_now;
        }
        return dest;
    } catch (...) {
        for (; original_dest != dest; ++original_dest) {
            std::allocator_traits<Allocator>::destroy(alloc, std::addressof(*original_dest));
        }
        throw;
    }
}

template <class T, class Allocator>
template <class InputIter>
    requires std::forward_iterator<InputIter>
deque<T, Allocator>::iterator deque<T, Allocator>::_copy_n(InputIter first, size_type count, iterator dest) {
    if (count == 0)
        return dest;

    iterator original_dest = dest;

    while (count > 0) {
        const size_type move_now = calc_move_now(first, count, dest);
        if constexpr (std::is_same_v<InputIter, iterator> || std::is_same_v<InputIter, const_iterator>) {
            copy_n_contiguous(first._current, move_now, dest._current);
        } else {
            copy_n_contiguous(first, move_now, dest._current);
        }

        first += move_now;
        dest += move_now;
        count -= move_now;
    }
    return dest;
}

template <class T, class Allocator>
deque<T, Allocator>::deque(const Allocator &alloc)
    : _map(nullptr), _map_capacity(0), _start(), _finish(), _map_alloc(alloc), _buf_alloc(alloc) {
    _initialize_map(_initial_map_size);
}

template <class T, class Allocator>
deque<T, Allocator>::deque(size_type n, const Allocator &alloc) : deque(n, T(), alloc) {}

template <class T, class Allocator>
deque<T, Allocator>::deque(size_type n, const T &value, const Allocator &alloc)
    : _map(nullptr), _map_capacity(0), _start(), _finish(), _map_alloc(alloc), _buf_alloc(alloc) {
    if (n == 0)
        return;
    const size_type num_nodes = (n + _buffer_size() - 1) / _buffer_size();
    const size_type new_map_capacity = std::max(_initial_map_size, num_nodes + 2);

    unique_ptr map_guard(_allocate_map(new_map_capacity), _map_alloc, new_map_capacity);
    buf *start_node = map_guard.get() + (new_map_capacity - num_nodes) / 2;

#if defined(__GNUC__)
    buffer_guard bufs_guard[num_nodes];

    for (size_type i = 0; i < num_nodes; ++i) {
        bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
    }
#else
    vector<buffer_guard> bufs_guard;

    bufs_guard.reserve(num_nodes);
    for (size_type i = 0; i < num_nodes; ++i) {
        bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
    }
#endif

    for (size_type i = 0; i < num_nodes; ++i) {
        *(start_node + i) = bufs_guard[i].get();
    }

    iterator new_start(start_node, *start_node + ((num_nodes * _buffer_size()) - n) / 2);
    auto new_finish = std::uninitialized_fill_n(new_start, n, value);

    _map = map_guard.release();
    _map_capacity = new_map_capacity;
    for (auto &buf_ptr : bufs_guard) {
        buf_ptr.release();
    }
    _start = new_start;
    _finish = new_finish;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
deque<T, Allocator>::deque(InputIter first, InputIter last, const Allocator &alloc)
    : _map(nullptr), _map_capacity(0), _start(), _finish(), _map_alloc(alloc), _buf_alloc(alloc) {
    if constexpr (std::random_access_iterator<InputIter>) { // BENCHMARK !! list, linked-list
        auto dist = std::distance(first, last);
        if (dist == 0) {
            _initialize_map(_initial_map_size);
            return;
        }
        const size_type num_nodes = (dist + _buffer_size() - 1) / _buffer_size();
        const size_type new_map_capacity = std::max(_initial_map_size, num_nodes + 2);

        unique_ptr map_guard(_allocate_map(new_map_capacity), _map_alloc, new_map_capacity);
        buf *start_node = map_guard.get() + (new_map_capacity - num_nodes) / 2;

#if defined(__GNUC__)
        buffer_guard bufs_guard[num_nodes];

        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
        }
#else
        vector<buffer_guard> bufs_guard;

        bufs_guard.reserve(num_nodes);
        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
        }
#endif

        for (size_type i = 0; i < num_nodes; ++i) {
            *(start_node + i) = bufs_guard[i].get();
        }

        const size_type offset = ((num_nodes * _buffer_size()) - dist) / 2;
        iterator new_start(start_node, *start_node + offset);

        auto new_finish = _uninitialized_copy_n(_buf_alloc, first, dist, new_start);

        _map = map_guard.release();
        _map_capacity = new_map_capacity;
        for (auto &buf_ptr : bufs_guard) {
            buf_ptr.release();
        }
        _start = new_start;
        _finish = new_finish;
    } else {
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }
}

template <class T, class Allocator>
deque<T, Allocator>::deque(const deque &x)
    : deque(x.begin(), x.end(),
            std::allocator_traits<Allocator>::select_on_container_copy_construction(x.get_allocator())) {}

template <class T, class Allocator>
deque<T, Allocator>::deque(deque &&x)
    : _map(nullptr), _map_capacity(0), _start(), _finish(), _map_alloc(std::move(x.get_allocator())),
      _buf_alloc(std::move(x.get_allocator())) {
    _move_state(std::move(x));
}

template <class T, class Allocator>
deque<T, Allocator>::deque(const deque &x, const std::type_identity_t<Allocator> &alloc)
    : deque(x.begin(), x.end(), alloc) {}

template <class T, class Allocator>
deque<T, Allocator>::deque(deque &&x, const std::type_identity_t<Allocator> &alloc)
    : _map(nullptr), _map_capacity(0), _start(), _finish(), _map_alloc(alloc), _buf_alloc(alloc) {
    if (get_allocator() == x.get_allocator()) {
        _move_state(std::move(x));
    } else {
        auto dist = x.size();
        if (dist == 0) {
            _initialize_map(_initial_map_size);
            return;
        }
        const size_type num_nodes = (dist + _buffer_size() - 1) / _buffer_size();
        const size_type new_map_capacity = std::max(_initial_map_size, num_nodes + 2);

        unique_ptr map_guard(_allocate_map(new_map_capacity), _map_alloc, new_map_capacity);
        buf *start_node = map_guard.get() + (new_map_capacity - num_nodes) / 2;

#if defined(__GNUC__)
        buffer_guard bufs_guard[num_nodes];

        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
        }
#else
        vector<buffer_guard> bufs_guard;

        bufs_guard.reserve(num_nodes);
        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
        }
#endif

        for (size_type i = 0; i < num_nodes; ++i) {
            *(start_node + i) = bufs_guard[i].get();
        }

        const size_type offset = ((num_nodes * _buffer_size()) - dist) / 2;
        iterator new_start(start_node, *start_node + offset);

        auto new_finish = _uninitialized_move_n(_buf_alloc, x.begin(), dist, new_start);

        _map = map_guard.release();
        _map_capacity = new_map_capacity;
        for (auto &buf_ptr : bufs_guard) {
            buf_ptr.release();
        }
        _start = new_start;
        _finish = new_finish;

        x.clear();
    }
}

template <class T, class Allocator>
deque<T, Allocator>::deque(std::initializer_list<T> il, const Allocator &alloc) : deque(il.begin(), il.end(), alloc) {}

template <class T, class Allocator> deque<T, Allocator>::~deque() {
    _destroy_elements_and_buffer();
    _deallocate_map(_map, _map_capacity);
}

template <class T, class Allocator> deque<T, Allocator> &deque<T, Allocator>::operator=(const deque &x) {
    if (this != std::addressof(x)) {
        if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value &&
            get_allocator() != x.get_allocator()) {
            deque tmp(x);
            swap(tmp);
        } else {
            if (size() >= x.size()) {
                std::copy(x.begin(), x.end(), begin());
                erase(begin() + x.size(), end());
            } else {
                deque temp(x, get_allocator());
                swap(temp);
            }
        }
    }
    return *this;
}

template <class T, class Allocator>
deque<T, Allocator> &deque<T, Allocator>::operator=(deque &&x) noexcept(
    std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
    std::allocator_traits<Allocator>::is_always_equal::value) {
    if (this != std::addressof(x)) {
        if constexpr (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
            _destroy_elements_and_buffer();
            _deallocate_map(_map, _map_capacity);

            _buf_alloc = std::move(x._buf_alloc);
            _map_alloc = std::move(x._map_alloc);
            _move_state(std::move(x));
        } else {
            if (get_allocator() == x.get_allocator()) {
                _destroy_elements_and_buffer();
                _deallocate_map(_map, _map_capacity);
                _move_state(std::move(x));
            } else {
                deque temp(std::make_move_iterator(x.begin()), std::make_move_iterator(x.end()), get_allocator());
                swap(temp);
                x.clear();
            }
        }
    }
    return *this;
}

template <class T, class Allocator> deque<T, Allocator> &deque<T, Allocator>::operator=(std::initializer_list<T> il) {
    clear();
    for (const T &t : il) {
        emplace_back(t);
    }
    return *this;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
void deque<T, Allocator>::assign(InputIter first, InputIter last) {
    clear();
    for (auto it = first; it != last; ++it) {
        emplace_back(*it);
    }
}

template <class T, class Allocator> void deque<T, Allocator>::assign(size_type n, const T &value) {
    clear();
    for (size_type i = 0; i < n; ++i) {
        emplace_back(value);
    }
}

template <class T, class Allocator> void deque<T, Allocator>::assign(std::initializer_list<T> il) {
    clear();
    for (const T &t : il) {
        emplace_back(t);
    }
}

template <class T, class Allocator>
deque<T, Allocator>::allocator_type deque<T, Allocator>::get_allocator() const noexcept {
    return _buf_alloc;
}

template <class T, class Allocator> deque<T, Allocator>::iterator deque<T, Allocator>::begin() noexcept {
    return _start;
}

template <class T, class Allocator> deque<T, Allocator>::const_iterator deque<T, Allocator>::begin() const noexcept {
    return const_iterator(_start);
}

template <class T, class Allocator> deque<T, Allocator>::iterator deque<T, Allocator>::end() noexcept {
    return _finish;
}

template <class T, class Allocator> deque<T, Allocator>::const_iterator deque<T, Allocator>::end() const noexcept {
    return const_iterator(_finish);
}

template <class T, class Allocator> deque<T, Allocator>::reverse_iterator deque<T, Allocator>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class T, class Allocator>
deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class T, class Allocator> deque<T, Allocator>::reverse_iterator deque<T, Allocator>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class T, class Allocator>
deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class T, class Allocator> deque<T, Allocator>::const_iterator deque<T, Allocator>::cbegin() const noexcept {
    return const_iterator(_start);
}

template <class T, class Allocator> deque<T, Allocator>::const_iterator deque<T, Allocator>::cend() const noexcept {
    return const_iterator(_finish);
}

template <class T, class Allocator>
deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <class T, class Allocator>
deque<T, Allocator>::const_reverse_iterator deque<T, Allocator>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <class T, class Allocator> bool deque<T, Allocator>::empty() const noexcept {
    return _start == _finish;
}

template <class T, class Allocator> deque<T, Allocator>::size_type deque<T, Allocator>::size() const noexcept {
    return std::distance(_start, _finish);
}

template <class T, class Allocator> deque<T, Allocator>::size_type deque<T, Allocator>::max_size() const noexcept {
    return std::allocator_traits<Allocator>::max_size(get_allocator());
}

template <class T, class Allocator> void deque<T, Allocator>::resize(size_type sz) {
    const size_type current_size = size();
    if (sz < current_size) {
        erase(begin() + sz, end());
    } else if (sz > current_size) {
        const size_type diff = sz - current_size;
        const size_type space_in_last_buffer = _finish._first + _buffer_size() - _finish._current;
        const size_type fill_in_last_buffer = std::min(space_in_last_buffer, diff);
        const size_type num_nodes = (diff - fill_in_last_buffer + _buffer_size() - 1) / _buffer_size();

        if (_map + _map_capacity - (_finish._node + 1) < num_nodes) {
            _reallocate_map(num_nodes, false);
        }

#if defined(__GNUC__)
        buffer_guard bufs_guard[num_nodes];

        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
        }
#else
        vector<buffer_guard> bufs_guard;

        bufs_guard.reserve(num_nodes);
        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
        }
#endif

        size_type remain = diff - fill_in_last_buffer;
        for (size_type i = 0; i < num_nodes; ++i) {
            const size_type fill_now = std::min(remain, _buffer_size());
            uninitialized_default_construct_n(get_allocator(), bufs_guard[i].get(), fill_now);

            bufs_guard[i].set_constructed_count(fill_now);
            remain -= fill_now;
        }

        if (space_in_last_buffer) {
            uninitialized_default_construct_n(get_allocator(), _finish._current, fill_in_last_buffer);
        }

        for (size_type i = 0; i < num_nodes; ++i) {
            *(_finish._node + 1) = bufs_guard[i].release();
        }

        _finish += diff;
    }
}

template <class T, class Allocator> void deque<T, Allocator>::resize(size_type sz, const T &value) {
    const size_type current_size = size();
    if (sz < current_size) {
        erase(begin() + sz, end());
    } else if (sz > current_size) {
        const size_type diff = sz - current_size;
        const size_type space_in_last_buffer = _finish._first + _buffer_size() - _finish._current;
        const size_type fill_in_last_buffer = std::min(space_in_last_buffer, diff);
        const size_type num_nodes = (diff - fill_in_last_buffer + _buffer_size() - 1) / _buffer_size();

        if (_map + _map_capacity - (_finish._node + 1) < num_nodes) {
            _reallocate_map(num_nodes, false);
        }

#if defined(__GNUC__)
        buffer_guard bufs_guard[num_nodes];

        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
        }
#else
        vector<buffer_guard> bufs_guard;

        bufs_guard.reserve(num_nodes);
        for (size_type i = 0; i < num_nodes; ++i) {
            bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
        }
#endif

        for (size_type i = 0; i < num_nodes; ++i) {
            *(_finish._node + 1 + i) = bufs_guard[i].get();
        }

        _uninitialized_fill_n(_buf_alloc, _finish + fill_in_last_buffer, diff - fill_in_last_buffer, value);
        _uninitialized_fill_n(_buf_alloc, _finish, fill_in_last_buffer, value);

        for (auto &guard : bufs_guard) {
            guard.release();
        }

        _finish += diff;
    }
}

template <class T, class Allocator> void deque<T, Allocator>::shrink_to_fit() {
    const size_type old_num_nodes = _finish._node - _start._node + 1;
    const size_type new_capacity = std::max(old_num_nodes + 2, _initial_map_size);
    if (_map_capacity != new_capacity) {
        unique_ptr map_guard(_allocate_map(new_capacity), _map_alloc, new_capacity);
        std::copy(_start._node, _finish._node + 1, map_guard.get() + (new_capacity - old_num_nodes) / 2);

        _deallocate_map(_map, _map_capacity);
        _map = map_guard.release();
        _map_capacity = new_capacity;
        _start._set_node(_map + (new_capacity - old_num_nodes) / 2);
        _finish._set_node(_start._node + old_num_nodes - 1);
    }
}

template <class T, class Allocator> deque<T, Allocator>::reference deque<T, Allocator>::operator[](size_type n) {
    return *(_start + n);
}

template <class T, class Allocator>
deque<T, Allocator>::const_reference deque<T, Allocator>::operator[](size_type n) const {
    return *(_start + n);
}

template <class T, class Allocator> deque<T, Allocator>::reference deque<T, Allocator>::at(size_type n) {
    if (n >= size()) {
        throw std::out_of_range("deque::at() : index is out of range");
    }
    return this->operator[](n);
}

template <class T, class Allocator> deque<T, Allocator>::const_reference deque<T, Allocator>::at(size_type n) const {
    if (n >= size()) {
        throw std::out_of_range("deque::at() : index is out of range");
    }
    return this->operator[](n);
}

template <class T, class Allocator> deque<T, Allocator>::reference deque<T, Allocator>::front() {
    return *_start;
}

template <class T, class Allocator> deque<T, Allocator>::const_reference deque<T, Allocator>::front() const {
    return *_start;
}

template <class T, class Allocator> deque<T, Allocator>::reference deque<T, Allocator>::back() {
    iterator tmp = _finish;
    --tmp;
    return *tmp;
}

template <class T, class Allocator> deque<T, Allocator>::const_reference deque<T, Allocator>::back() const {
    iterator tmp = _finish;
    --tmp;
    return *tmp;
}

template <class T, class Allocator>
template <class... Args>
deque<T, Allocator>::reference deque<T, Allocator>::emplace_front(Args &&...args) {
    if (_start._current == _start._first) {
        _ensure_front_map_space();
        buffer_guard buf_guard(_allocate_buf(), _buf_alloc);
        *(_start._node - 1) = buf_guard.get();
        std::allocator_traits<buf_allocator>::construct(_buf_alloc, (_start - 1)._current, std::forward<Args>(args)...);
        buf_guard.release();
    } else {
        std::allocator_traits<buf_allocator>::construct(_buf_alloc, _start._current - 1, std::forward<Args>(args)...);
    }
    --_start;
    return *_start;
}

template <class T, class Allocator>
template <class... Args>
deque<T, Allocator>::reference deque<T, Allocator>::emplace_back(Args &&...args) {
    pointer old_finish = _finish._current;
    if (_finish._current == _finish._last - 1) {
        _ensure_back_map_space();
        old_finish = _finish._current;
        buffer_guard buf_guard(_allocate_buf(), _buf_alloc);
        *(_finish._node + 1) = buf_guard.get();
        std::allocator_traits<buf_allocator>::construct(_buf_alloc, old_finish, std::forward<Args>(args)...);
        buf_guard.release();
    } else {
        std::allocator_traits<buf_allocator>::construct(_buf_alloc, old_finish, std::forward<Args>(args)...);
    }
    ++_finish;
    return *old_finish;
}

template <class T, class Allocator>
template <class... Args>
deque<T, Allocator>::iterator deque<T, Allocator>::emplace(const_iterator position, Args &&...args) {
    if (_start._current == position._current) {
        emplace_front(std::forward<Args>(args)...);
        return begin();
    }
    if (_finish._current == position._current) {
        emplace_back(std::forward<Args>(args)...);
        return end() - 1;
    }
    const difference_type distance_from_begin = std::distance(cbegin(), position);
    const difference_type distance_from_end = std::distance(position, cend());
    iterator emplace_pos = _start + distance_from_begin;
    if (distance_from_begin < distance_from_end) {
        if (_start._current == _start._first) {
            _ensure_front_map_space();
            buffer_guard buf_guard(_allocate_buf(), _buf_alloc);
            *(_start._node - 1) = buf_guard.get();
            _shift_left_and_emplace(distance_from_begin, emplace_pos, std::forward<Args>(args)...);
            buf_guard.release();
        } else {
            _shift_left_and_emplace(distance_from_begin, emplace_pos, std::forward<Args>(args)...);
        }
        --_start;
    } else {
        if (_finish._current == _finish._last - 1) {
            _ensure_back_map_space();
            buffer_guard buf_guard(_allocate_buf(), _buf_alloc);
            *(_finish._node + 1) = buf_guard.get();
            _shift_right_and_emplace(distance_from_end, emplace_pos, std::forward<Args>(args)...);
            buf_guard.release();
        } else {
            _shift_right_and_emplace(distance_from_end, emplace_pos, std::forward<Args>(args)...);
        }
        ++_finish;
    }
    return emplace_pos;
}

template <class T, class Allocator> void deque<T, Allocator>::push_front(const T &value) {
    emplace_front(value);
}

template <class T, class Allocator> void deque<T, Allocator>::push_front(T &&value) {
    emplace_front(std::move(value));
}

template <class T, class Allocator> void deque<T, Allocator>::push_back(const T &value) {
    emplace_back(value);
}

template <class T, class Allocator> void deque<T, Allocator>::push_back(T &&value) {
    emplace_back(std::move(value));
}

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator position, const T &value) {
    return _insert_impl(position, value);
}

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator position, T &&value) {
    return _insert_impl(position, std::move(value));
}

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator position, size_type count, const T &value) {
    if (count == 0)
        return iterator(position._node, const_cast<pointer>(position._current));

    const difference_type distance_from_begin = std::distance(cbegin(), position);
    const difference_type distance_from_end = std::distance(position, cend());
    iterator insert_pos;

    if (distance_from_begin < distance_from_end) {
        if (distance_from_begin >= count) {
            insert_pos = _start + distance_from_begin;
            _uninitialized_move_n(_buf_alloc, _start, count, _start - count);
            _move_n(_start + count, distance_from_begin - count, _start);
            std::fill(insert_pos - count, insert_pos, value);
            _start -= count;
        } else {
            const size_type space_in_first_buffer = _start._current - _start._first;
            const size_type num_nodes = (count - space_in_first_buffer + _buffer_size() - 1) / _buffer_size();

            if (_map + _map_capacity - (_finish._node + 1) < num_nodes) {
                _reallocate_map(num_nodes, false);
            }
            insert_pos = _start + distance_from_begin;

#if defined(__GNUC__)
            buffer_guard bufs_guard[num_nodes];

            for (size_type i = 0; i < num_nodes; ++i) {
                bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
            }
#else
            vector<buffer_guard> bufs_guard;

            bufs_guard.reserve(num_nodes);
            for (size_type i = 0; i < num_nodes; ++i) {
                bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
            }
#endif

            for (size_type i = 0; i < num_nodes; ++i) {
                *(_start._node - 1 - i) = bufs_guard[i].get();
            }

            iterator final_new_start = _start - count;
            _uninitialized_move_n(_buf_alloc, _start, distance_from_begin, final_new_start);
            _fill_n(_start, distance_from_begin, value);
            _uninitialized_fill_n(_buf_alloc, final_new_start + distance_from_begin, count - distance_from_begin,
                                  value);

            for (auto &guard : bufs_guard) {
                guard.release();
            }
            _start = final_new_start;
        }
    } else {
        if (distance_from_end >= count) {
            insert_pos = _start + distance_from_begin;
            _uninitialized_move_n(_buf_alloc, _finish - count, count, _finish);
            _move_backward_n(insert_pos, distance_from_end - count, _finish);
            std::fill(insert_pos, insert_pos + count, value);
            _finish += count;
        } else {
            const size_type space_in_last_buffer = _finish._last - _finish._current;
            const size_type num_nodes = (count - space_in_last_buffer + _buffer_size() - 1) / _buffer_size();

            if (_map + _map_capacity - (_finish._node + 1) < num_nodes) {
                _reallocate_map(num_nodes, true);
            }
            insert_pos = _start + distance_from_begin;

#if defined(__GNUC__)
            buffer_guard bufs_guard[num_nodes];

            for (size_type i = 0; i < num_nodes; ++i) {
                bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
            }
#else
            vector<buffer_guard> bufs_guard;

            bufs_guard.reserve(num_nodes);
            for (size_type i = 0; i < num_nodes; ++i) {
                bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
            }
#endif

            for (size_type i = 0; i < num_nodes; ++i) {
                *(_finish._node + 1 + i) = bufs_guard[i].get();
            }

            _uninitialized_move_n(_buf_alloc, insert_pos, distance_from_end, _finish + count - distance_from_end);
            _fill_n(insert_pos, distance_from_end, value);
            _uninitialized_fill_n(_buf_alloc, _finish, count - distance_from_end, value);

            for (auto &guard : bufs_guard) {
                guard.release();
            }
            _finish += count;
        }
    }
    return insert_pos;
}

template <class T, class Allocator>
template <class InputIter>
    requires std::input_iterator<InputIter>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator position, InputIter first, InputIter last) {
    if constexpr (std::forward_iterator<InputIter>) {
        const difference_type count = std::distance(first, last);
        if (count == 0)
            return iterator(position._node, const_cast<pointer>(position._current));

        const difference_type distance_from_begin = std::distance(cbegin(), position);
        const difference_type distance_from_end = std::distance(position, cend());
        iterator insert_pos;
        if (distance_from_begin < distance_from_end) {
            if (distance_from_begin >= count) {
                insert_pos = _start + distance_from_begin;
                _uninitialized_move_n(_buf_alloc, _start, count, _start - count);
                _move_n(_start + count, distance_from_begin - count, _start);
                std::copy(first, last, insert_pos - count);
                _start -= count;
            } else {
                const size_type space_in_first_buffer = _start._current - _start._first;
                const size_type num_nodes = (count - space_in_first_buffer + _buffer_size() - 1) / _buffer_size();

                if (_map + _map_capacity - (_finish._node + 1) < num_nodes) {
                    _reallocate_map(num_nodes, false);
                }
                insert_pos = _start + distance_from_begin;

#if defined(__GNUC__)
                buffer_guard bufs_guard[num_nodes];

                for (size_type i = 0; i < num_nodes; ++i) {
                    bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
                }
#else
                vector<buffer_guard> bufs_guard;

                bufs_guard.reserve(num_nodes);
                for (size_type i = 0; i < num_nodes; ++i) {
                    bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
                }
#endif

                for (size_type i = 0; i < num_nodes; ++i) {
                    *(_start._node - 1 - i) = bufs_guard[i].get();
                }

                iterator final_new_start = _start - count;
                _uninitialized_move_n(_buf_alloc, _start, distance_from_begin, final_new_start);
                _copy_n(first + count - distance_from_begin, distance_from_begin, _start);
                _uninitialized_copy_n(_buf_alloc, first, count - distance_from_begin,
                                      final_new_start + distance_from_begin);

                for (auto &guard : bufs_guard) {
                    guard.release();
                }
                _start = final_new_start;
            }
        } else {
            if (distance_from_end >= count) {
                insert_pos = _start + distance_from_begin;
                _uninitialized_move_n(_buf_alloc, _finish - count, count, _finish);
                _move_backward_n(insert_pos, distance_from_end - count, _finish);
                std::copy(first, last, insert_pos);
                _finish += count;
            } else {
                const size_type space_in_last_buffer = _finish._last - _finish._current;
                const size_type num_nodes = (count - space_in_last_buffer + _buffer_size() - 1) / _buffer_size();

                if (_map + _map_capacity - (_finish._node + 1) < num_nodes) {
                    _reallocate_map(num_nodes, true);
                }
                insert_pos = _start + distance_from_begin;

#if defined(__GNUC__)
                buffer_guard bufs_guard[num_nodes];

                for (size_type i = 0; i < num_nodes; ++i) {
                    bufs_guard[i] = buffer_guard(_allocate_buf(), _buf_alloc);
                }
#else
                vector<buffer_guard> bufs_guard;

                bufs_guard.reserve(num_nodes);
                for (size_type i = 0; i < num_nodes; ++i) {
                    bufs_guard.emplace_back(_allocate_buf(), _buf_alloc);
                }
#endif

                for (size_type i = 0; i < num_nodes; ++i) {
                    *(_finish._node + 1 + i) = bufs_guard[i].get();
                }

                _uninitialized_move_n(_buf_alloc, insert_pos, distance_from_end, _finish + count - distance_from_end);
                _copy_n(first, distance_from_end, insert_pos);
                _uninitialized_copy_n(_buf_alloc, first + distance_from_end, count - distance_from_end, _finish);

                for (auto &guard : bufs_guard) {
                    guard.release();
                }
                _finish += count;
            }
        }
        return insert_pos;
    } else {
        iterator current_pos(position._node, position._current);
        if (first == last)
            return current_pos;
        current_pos = emplace(current_pos, *first);
        iterator first_inserted = current_pos;
        ++current_pos;
        ++first;
        for (; first != last; ++first) {
            current_pos = emplace(current_pos, *first);
            ++current_pos;
        }
        return first_inserted;
    }
}

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::insert(const_iterator position, std::initializer_list<T> il) {
    return insert(position, il.begin(), il.end());
}

template <class T, class Allocator> void deque<T, Allocator>::pop_front() {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::allocator_traits<buf_allocator>::destroy(_buf_alloc, _start._current);
    }
    iterator old_start = _start;
    ++_start;
    if (old_start._node != _start._node && old_start._node > _map) {
        _deallocate_buf(*(old_start._node - 1));
    }
}

template <class T, class Allocator> void deque<T, Allocator>::pop_back() {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::allocator_traits<buf_allocator>::destroy(_buf_alloc, (_finish - 1)._current);
    }
    iterator old_finish = _finish;
    --_finish;
    if (old_finish._node != _finish._node && old_finish._node < _map + _map_capacity - 1) {
        _deallocate_buf(*(old_finish._node + 1));
    }
}

template <class T, class Allocator> deque<T, Allocator>::iterator deque<T, Allocator>::erase(const_iterator position) {
    iterator erase_pos = _start + std::distance(cbegin(), position);

    if (erase_pos == begin()) {
        pop_front();
        return begin();
    }
    if (erase_pos == end() - 1) {
        pop_back();
        return end();
    }
    const difference_type distance_from_begin = std::distance(begin(), erase_pos);
    const difference_type distance_from_end = std::distance(erase_pos, end()) - 1;
    if (distance_from_begin < distance_from_end) {
        _move_backward_n(begin(), distance_from_begin, erase_pos + 1);
        pop_front();
    } else {
        _move_n(erase_pos + 1, distance_from_end, erase_pos);
        pop_back();
    }
    return erase_pos;
}

template <class T, class Allocator>
deque<T, Allocator>::iterator deque<T, Allocator>::erase(const_iterator first, const_iterator last) {
    if (first == last) {
        return iterator(first._node, const_cast<pointer>(first._current));
    }
    if (first == cbegin() && last == cend()) {
        clear();
        return begin();
    }

    const difference_type distance_from_begin = std::distance(cbegin(), first);
    const difference_type distance_from_end = std::distance(last, cend());

    iterator first_iter(first._node, const_cast<pointer>(first._current));
    iterator last_iter(last._node, const_cast<pointer>(last._current));

    if (distance_from_begin < distance_from_end) {
        iterator new_start = std::move_backward(begin(), first_iter, last_iter);
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (auto it = begin(); it != new_start; ++it) {
                std::allocator_traits<buf_allocator>::destroy(_buf_alloc, std::to_address(it));
            }
        }
        for (auto temp = _start._node; temp < new_start._node; ++temp) {
            _deallocate_buf(*temp);
        }
        _start = new_start;
        return last_iter;
    }
    iterator new_finish = std::move(last_iter, end(), first_iter);
    if constexpr (!std::is_trivially_destructible_v<T>) {
        for (auto it = new_finish; it != end(); ++it) {
            std::allocator_traits<buf_allocator>::destroy(_buf_alloc, std::to_address(it));
        }
    }
    for (auto temp = new_finish._node + 1; temp <= _finish._node; ++temp) {
        _deallocate_buf(*temp);
    }
    _finish = new_finish;
    return first_iter;
}

template <class T, class Allocator>
void deque<T, Allocator>::swap(deque &other) noexcept(std::allocator_traits<Allocator>::is_always_equal::value) {
    using std::swap;
    swap(_map, other._map);
    swap(_map_capacity, other._map_capacity);
    swap(_start, other._start);
    swap(_finish, other._finish);
    swap(_map_alloc, other._map_alloc);
    swap(_buf_alloc, other._buf_alloc);
}

template <class T, class Allocator> void deque<T, Allocator>::clear() noexcept {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        for (auto it = begin(); it != end(); ++it) {
            std::allocator_traits<buf_allocator>::destroy(_buf_alloc, std::to_address(it));
        }
    }
    buf *start_node = _map + _map_capacity / 2;
    pointer start_pos = *start_node + _buffer_size() / 2;

    for (buf *node = _start._node; node < start_node; ++node) {
        _deallocate_buf(*node);
        *node = nullptr;
    }

    for (buf *node = _finish._node; node > start_node; --node) {
        _deallocate_buf(*node);
        *node = nullptr;
    }

    _start = iterator(start_node, start_pos);
    _finish = _start;
}
} // namespace j