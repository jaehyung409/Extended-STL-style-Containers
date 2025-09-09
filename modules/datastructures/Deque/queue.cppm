/*
 * @ Created by jaehyung409 on 25. 1. 24.
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License. 
 */

module;
#include <type_traits>
#include <utility>
#include <iterator>

export module j:queue;

import :deque;
import :vector;
import :algorithm;

namespace j {
    export template<class T, class Container = deque<T>>
    class queue {
    public:
        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using size_type = typename Container::size_type;
        using container_type = Container;

    protected:
        Container _c;

    public:
        queue() : queue(Container()) {}
        explicit queue(const Container &cont);
        explicit queue(Container &&cont);
        queue(const queue&) = default;
        queue(queue&&) = default;
        template <class InputIter>
        requires std::input_iterator<InputIter>
        queue(InputIter first, InputIter last);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        explicit queue(const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        queue(const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        queue(Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        queue(const queue &other, const Alloc &alloc);
        template <class Alloc>
        requires std::uses_allocator_v<Container, Alloc>
        queue(queue &&other, const Alloc &alloc);

        queue &operator=(const queue&) = default;
        queue &operator=(queue&&) = default;

        template <class InputIter, class Alloc>
        requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
        queue(InputIter first, InputIter last, const Alloc& alloc);
        ~queue() = default;

        [[nodiscard]] bool empty() const { return _c.empty(); }
        [[nodiscard]] size_type size() const { return _c.size(); }
        reference front() { return _c.front(); }
        const_reference front() const { return _c.front(); }
        reference back() { return _c.back(); }
        const_reference back() const { return _c.back(); }
        void push(const value_type &x) { _c.push_back(x); }
        void push(value_type &&x) { _c.push_back(std::move(x)); }
        template <class... Args>
        decltype(auto) emplace(Args &&... args) {
            return _c.emplace_back(std::forward<Args>(args)...);
        }

        void pop() { _c.pop_front(); }
        void swap(queue &q) noexcept(std::is_nothrow_swappable_v<Container>) {
            using std::swap;
            swap(_c, q._c);
        }
        bool operator==(const queue& q) const {
            return _c == q._c;
        }
        auto operator<=>(const queue& q) const -> std::strong_ordering {
            return _c <=> q._c;
        }
    };

    template <class Container>
    queue(Container cont) -> queue<typename Container::value_type, Container>;

    template <class InputIter>
    queue(InputIter, InputIter) -> queue<typename std::iterator_traits<InputIter>::value_type>;

    template <class Container, class Allocator>
    queue(Container, Allocator) -> queue<typename Container::value_type, Container>;

    template <class InputIter, class Allocator>
    queue(InputIter, InputIter, Allocator)
        -> queue<typename std::iterator_traits<InputIter>::value_type,
                 deque<typename std::iterator_traits<InputIter>::value_type, Allocator>>;

    export template <class T, class Container>
    bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
        return lhs.operator==(rhs);
    }

    export template <class T, class Container>
    auto operator<=>(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
        return lhs.operator<=>(rhs);
    }

    export template <class T, class Container>
    void swap(queue<T, Container>& x, queue<T, Container>& y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }
}

template <class T, class Container, class Alloc>
struct std::uses_allocator<j::queue<T, Container>, Alloc> : std::uses_allocator<Container, Alloc>::type {};

namespace j {
    template <class T, class Container>
    queue<T, Container>::queue(const Container &cont) : _c(cont) {}

    template <class T, class Container>
    queue<T, Container>::queue(Container &&cont) : _c(std::move(cont)) {}

    template <class T, class Container>
    template <class InputIter>
requires std::input_iterator<InputIter>
    queue<T, Container>::queue(InputIter first, InputIter last) : _c(first, last) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    queue<T, Container>::queue(const Alloc &alloc) : _c(alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    queue<T, Container>::queue(const Container &cont, const Alloc &alloc) : _c(cont, alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    queue<T, Container>::queue(Container &&cont, const Alloc &alloc) : _c(std::move(cont), alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    queue<T, Container>::queue(const queue &other, const Alloc &alloc) : _c(other._c, alloc) {
    }

    template <class T, class Container>
    template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
    queue<T, Container>::queue(queue &&other, const Alloc &alloc) : _c(std::move(other._c), alloc) {
    }

    template <class T, class Container>
    template <class InputIter, class Alloc>
requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
    queue<T, Container>::queue(InputIter first, InputIter last, const Alloc &alloc) : _c(first, last, alloc) {
    }

}

namespace j {
export template <class T, class Container = vector<T>, class Compare = std::less<typename Container::value_type>>
class priority_queue {
public:
    using value_type      = typename Container::value_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type       = typename Container::size_type;
    using container_type  = Container;
    using value_compare   = Compare;

protected:
    Container _c;
    Compare _comp;

public:
    priority_queue()
      : priority_queue(Compare())
    {
    }
    explicit priority_queue(const Compare& x)
      : priority_queue(x, Container())
    {
    }
    priority_queue(const Compare& comp, const Container& cont);
    priority_queue(const Compare& comp, Container&& cont);
    priority_queue(const priority_queue& pq);
    priority_queue(priority_queue&& pq);
    template<class InputIter>
    requires std::input_iterator<InputIter>
    priority_queue(InputIter first, InputIter last, const Compare& comp = Compare());
    template<class InputIter>
    requires std::input_iterator<InputIter>
    priority_queue(InputIter first, InputIter last, const Compare& comp, const Container& cont);
    template<class InputIter>
    requires std::input_iterator<InputIter>
    priority_queue(InputIter first, InputIter last, const Compare& comp, Container&& cont);
    template<class Alloc>
    requires std::uses_allocator_v<Container, Alloc>
    explicit priority_queue(const Alloc& alloc);
    template<class Alloc>
    requires std::uses_allocator_v<Container, Alloc>
    priority_queue(const Compare& comp, const Alloc& alloc);
    template<class Alloc>
    requires std::uses_allocator_v<Container, Alloc>
    priority_queue(const Compare& comp, const Container& cont, const Alloc& alloc);
    template<class Alloc>
    requires std::uses_allocator_v<Container, Alloc>
    priority_queue(const Compare& comp, Container&& cont, const Alloc& alloc);
    template<class Alloc>
    requires std::uses_allocator_v<Container, Alloc>
    priority_queue(const priority_queue& pq, const Alloc& alloc);
    template<class Alloc>
    requires std::uses_allocator_v<Container, Alloc>
    priority_queue(priority_queue&& pq, const Alloc& alloc);
    template<class InputIter, class Alloc>
    requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
    priority_queue(InputIter first, InputIter last, const Alloc& alloc);
    template<class InputIter, class Alloc>
    requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
    priority_queue(InputIter first, InputIter last, const Compare& comp, const Alloc& alloc);
    template<class InputIter, class Alloc>
    requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
    priority_queue(InputIter first, InputIter last, const Compare& comp, const Container& cont, const Alloc& alloc);
    template<class InputIter, class Alloc>
    requires std::input_iterator<InputIter> && std::uses_allocator_v<Container, Alloc>
    priority_queue(InputIter first, InputIter last, const Compare& comp, Container&& cont, const Alloc& alloc);

    [[nodiscard]] bool empty() const { return _c.empty(); }
    [[nodiscard]] size_type size() const { return _c.size(); }
    const_reference top() const { return _c.front(); }
    void push(const value_type& x);
    void push(value_type&& x);
    template<class... Args>
    void emplace(Args&&... args);
    void pop();
    void swap(priority_queue& pq) noexcept(
      std::is_nothrow_swappable_v<Container>&& std::is_nothrow_swappable_v<Compare>)
    {
        using std::swap;
        swap(_c, pq._c);
        swap(_comp, pq._comp);
    }
};

template< class Compare, class Container >
priority_queue( Compare, Container )
    -> priority_queue<typename Container::value_type, Container, Compare>;

template< class InputIter,
      class Comp = std::less<typename std::iterator_traits<InputIter>::value_type>,
      class Container = vector<typename std::iterator_traits<InputIter>::value_type>
priority_queue(InputIter, InputIter, Comp = Comp(), Container = Container() )
-> priority_queue<typename std::iterator_traits<InputIter>::value_type, Container, Comp>;

template< class Comp, class Container, class Alloc >
priority_queue( Comp, Container, Alloc )
-> priority_queue<typename Container::value_type, Container, Comp>;

template< class InputIter, class Alloc >
priority_queue( InputIter, InputIter, Alloc )
-> priority_queue<typename std::iterator_traits<InputIter>::value_type,
                  vector<typename std::iterator_traits<InputIter>::value_type, Alloc>,
                  std::less<typename std::iterator_traits<InputIter>::value_type>>;

template< class InputIter, class Compare, class Alloc >
priority_queue( InputIter, InputIter, Compare, Alloc )
-> priority_queue<typename std::iterator_traits<InputIter>::value_type,
                  vector<typename std::iterator_traits<InputIter>::value_type, Alloc>, Compare>;

template< class InputIt, class Comp, class Container, class Alloc >
priority_queue( InputIt, InputIt, Comp, Container, Alloc )
-> priority_queue<typename Container::value_type, Container, Comp>;


export template <class T, class Container>
void swap(priority_queue<T, Container>& x, priority_queue<T, Container>& y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
}
}

template<class T, class Container, class Compare, class Alloc>
struct std::uses_allocator<j::priority_queue<T, Container, Compare>, Alloc> : std::uses_allocator<Compare, Alloc>::type {};

namespace j {
template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(const Compare &comp, const Container &cont) : _c(cont), _comp(comp) {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(const Compare &comp, Container &&cont) : _c(std::move(cont)), _comp(comp) {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(const priority_queue &pq)
    : _c(pq._c), _comp(pq._comp) {
}

template <class T, class Container, class Compare>
priority_queue<T, Container, Compare>::priority_queue(priority_queue &&pq)
    : _c(std::move(pq._c)), _comp(std::move(pq._comp)) {
}

template <class T, class Container, class Compare>
template <class InputIter> requires std::input_iterator<InputIter>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare &comp) : _c(first, last), _comp(comp) {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class InputIter> requires std::input_iterator<InputIter>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare &comp,
    const Container &cont) : _c(cont), _comp(comp) {
    _c.insert(_c.end(), first, last);
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class InputIter> requires std::input_iterator<InputIter>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare &comp, Container && cont)
    : _c(std::move(cont)), _comp(comp) {
    _c.insert(_c.end(), first, last);
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class Alloc>
requires std::uses_allocator_v<Container, Alloc>
priority_queue<T, Container, Compare>::priority_queue(const Alloc &alloc) : _c(alloc), _comp() {
}

template <class T, class Container, class Compare>
template <class Alloc> requires std::uses_allocator_v<Container, Alloc>
priority_queue<T, Container, Compare>::priority_queue(const Compare &comp, const Alloc &alloc) : _c(alloc), _comp(comp) {
}

template <class T, class Container, class Compare>
template <class Alloc> requires std::uses_allocator_v<Container, Alloc>
priority_queue<T, Container, Compare>::priority_queue(const Compare &comp, const Container &cont, const Alloc &alloc)
    : _c(cont, alloc), _comp(comp) {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class Alloc> requires std::uses_allocator_v<Container, Alloc>
priority_queue<T, Container, Compare>::priority_queue(const Compare & comp, Container &&cont, const Alloc &alloc)
    : _c(std::move(cont), alloc), _comp(comp) {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class Alloc> requires std::uses_allocator_v<Container, Alloc>
priority_queue<T, Container, Compare>::priority_queue(const priority_queue &pq, const Alloc &alloc)
    : _c(pq._c, alloc), _comp(pq._comp) {
}

template <class T, class Container, class Compare>
template <class Alloc> requires std::uses_allocator_v<Container, Alloc>
priority_queue<T, Container, Compare>::priority_queue(priority_queue &&pq, const Alloc &alloc)
    : _c(std::move(pq._c), alloc), _comp(std::move(pq._comp)) {
}

template <class T, class Container, class Compare>
template <class InputIter, class Alloc> requires std::input_iterator<InputIter> && std::uses_allocator_v<Container,
    Alloc>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Alloc &alloc)
    : _c(first, last, alloc), _comp() {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class InputIter, class Alloc> requires std::input_iterator<InputIter> && std::uses_allocator_v<Container,
    Alloc>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare &comp, const Alloc &alloc)
    : _c(first, last, alloc), _comp(comp) {
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class InputIter, class Alloc> requires std::input_iterator<InputIter> && std::uses_allocator_v<Container,
    Alloc>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare &comp, const Container &cont,
    const Alloc &alloc) : _c(cont, alloc), _comp(comp) {
    _c.insert(_c.end(), first, last);
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class InputIter, class Alloc> requires std::input_iterator<InputIter> && std::uses_allocator_v<Container,
    Alloc>
priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare &comp, Container &&cont,
    const Alloc &alloc) : _c(std::move(cont), alloc), _comp(comp) {
    _c.insert(_c.end(), first, last);
    make_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
void priority_queue<T, Container, Compare>::push(const value_type &x) {
    _c.push_back(x);
    push_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
void priority_queue<T, Container, Compare>::push(value_type &&x) {
    _c.push_back(std::move(x));
    push_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
template <class ... Args>
void priority_queue<T, Container, Compare>::emplace(Args &&...args) {
    _c.emplace_back(std::forward<Args>(args)...);
    push_heap(_c.begin(), _c.end(), _comp);
}

template <class T, class Container, class Compare>
void priority_queue<T, Container, Compare>::pop() {
    pop_heap(_c.begin(), _c.end(), _comp);
    _c.pop_back();
}
}