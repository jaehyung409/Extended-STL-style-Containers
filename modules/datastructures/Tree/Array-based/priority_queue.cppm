/*
 * @ Created by jaehyung409 on 25. 1. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <functional>
#include <initializer_list>
#include <ranges>
#include <stack>

export module j.priority_queue;

import j.vector;
import j.basics;
import j.heap_algo;

namespace j {
    export template <class T,
              class Container = vector<T>,
              class Compare = std::less<typename Container::value_type>>
    class priority_queue {
    public:
        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using size_type = typename Container::size_type;
        using container_type = Container;
        using value_compare = Compare;

    protected:
        Container _c;
        Compare _comp;

    public:
        priority_queue() : priority_queue(Compare()) {}
        explicit priority_queue(const Compare &comp) : priority_queue(comp, Container()) {}
        priority_queue(const Compare& comp, const Container &cont);
        priority_queue(const Compare& comp, Container &&cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        priority_queue(InputIter first, InputIter last, const Compare &comp = Compare());
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        priority_queue(InputIter first, InputIter last, const Compare &comp, const Container &cont);
        template <class InputIter>
        requires (!std::is_integral_v<InputIter>)
        priority_queue(InputIter first, InputIter last, const Compare &comp, Container &&cont);
        //template <container-compatible-range<T> R>
        //priority_queue(from_range_t, R&& range, const Compare &comp = Compare());
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(const Compare &comp, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(const Compare &comp, Container &&cont, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(const priority_queue &other, const Alloc &alloc);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(priority_queue &&other, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(InputIter first, InputIter last, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(InputIter first, InputIter last, const Compare &comp, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(InputIter first, InputIter last, const Compare &comp, const Container &cont, const Alloc &alloc);
        template <class InputIter, class Alloc>
        requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
        priority_queue(InputIter first, InputIter last, const Compare &comp, Container &&cont, const Alloc &alloc);
        //template <container-compatible-range<T> R, class Alloc>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //priority_queue(from_range_t, R&& range, const Alloc &alloc);
        //template <container-compatible-range<T> R, class Alloc>

        bool empty() const { return _c.empty(); }
        size_type size() const { return _c.size(); }
        const_reference top() const { return _c.front(); }
        void push(const value_type &x);
        void push(value_type &&x);
        //template<container-compatible-range<T> R>
        //void push_range(R&& rg);
        template <class... Args>
        requires std::constructible_from<value_type, Args...>
        void emplace(Args&&... args);
        void pop();
        void swap(priority_queue &q) noexcept(
            std::is_nothrow_swappable_v<Container> &&
            std::is_nothrow_swappable_v<Compare>
        ) {
            using std::swap;
            swap(_c, q._c);
            swap(_comp, q._comp);
        }
    };

    template <class Compare, class Container>
    priority_queue(Compare, Container)
        -> priority_queue<typename Container::value_type, Container, Compare>;

    template <class InputIter,
              class Compare    = std::less<typename std::iterator_traits<InputIter>::value_type>,
              class Container  = vector<typename std::iterator_traits<InputIter>::value_type>>
    priority_queue(InputIter, InputIter, Compare = Compare(), Container = Container())
        -> priority_queue<typename std::iterator_traits<InputIter>::value_type, Container, Compare>;

    //template <container-compatible-range<T> R, class Compare = std::less<T>, class Container = vector<T>>
    //priority_queue(from_range_t, R&&, Compare = Compare(), Container = Container())
    //    -> priority_queue<T, Container, Compare>;

    template <class Compare, class Container, class Alloc>
    priority_queue(Compare, Container, Alloc)
        -> priority_queue<typename Container::value_type, Container, Compare>;

    template <class InputIter, class Alloc>
    priority_queue(InputIter, InputIter, Alloc)
        -> priority_queue<typename std::iterator_traits<InputIter>::value_type,
                          vector<typename std::iterator_traits<InputIter>::value_type>,
                          std::less<typename std::iterator_traits<InputIter>::value_type>>;

    template <class InputIter, class Compare, class Alloc>
    priority_queue(InputIter, InputIter, Compare, Alloc)
        -> priority_queue<typename std::iterator_traits<InputIter>::value_type,
                          vector<typename std::iterator_traits<InputIter>::value_type>,
                          Compare>;

    template <class InputIter, class Compare, class Container, class Alloc>
    priority_queue(InputIter, InputIter, Compare, Container, Alloc)
        -> priority_queue<typename std::iterator_traits<InputIter>::value_type, Container, Compare>;

    //template <container-compatible-range<T> R, class Compare, class Container, class Alloc>
    //priority_queue(from_range_t, R&&, Compare, Container, Alloc)
    //    -> priority_queue<T, Container, Compare>;

    //template <container-compatible-range<T> R, class Alloc>
    //priority_queue(from_range_t, R&&, Alloc)
    //    -> priority_queue<T, vector<T>, std::less<T>>;

    // no equality is provided
    //template <class T, class Container, class Compare, class Alloc>
    //struct uses_allocator<priority_queue<T, Container, Compare>, Alloc>
    //    : uses_allocator<Container, Alloc>::false_type {};
}

namespace j {
    template <class T, class Container, class Compare>
    priority_queue<T, Container, Compare>::priority_queue(const Compare& comp, const Container& cont)
        : _c(cont), _comp(comp) {
        std::copy(cont.begin(), cont.end(), std::back_inserter(_c));
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    priority_queue<T, Container, Compare>::priority_queue(const Compare& comp, Container&& cont)
        : _c(std::move(cont)), _comp(comp) {
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class InputIter>
    requires (!std::is_integral_v<InputIter>)
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare& comp)
        : priority_queue(first, last, comp, Container()) {
    }

    template <class T, class Container, class Compare>
    template <class InputIter>
    requires (!std::is_integral_v<InputIter>)
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare& comp,
        const Container& cont) : _c(cont), _comp(comp) {
        std::copy(first, last, std::back_inserter(_c));
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class InputIter>
    requires (!std::is_integral_v<InputIter>)
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare& comp,
        Container&& cont) : _c(std::move(cont)), _comp(comp) {
        std::copy(first, last, std::back_inserter(_c));
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(const Compare& comp, const Alloc& alloc)
        : priority_queue(comp, Container(), alloc) {}

    template <class T, class Container, class Compare>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(const Compare& comp, const Container& cont,
        const Alloc& alloc) : _c(cont, alloc), _comp(comp) {
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(const Compare& comp, Container&& cont, const Alloc& alloc)
        : _c(std::move(cont), alloc), _comp(comp) {
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(const priority_queue& other, const Alloc& alloc)
        : _c(other._c, alloc), _comp(other._comp) {}

    template <class T, class Container, class Compare>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(priority_queue&& other, const Alloc& alloc)
        : _c(std::move(other._c), alloc), _comp(other._comp) {}

    template <class T, class Container, class Compare>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Alloc& alloc)
        : priority_queue(first, last, Compare(), alloc) {}

    template <class T, class Container, class Compare>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare& comp,
        const Alloc& alloc) : priority_queue(first, last, comp, Container(), alloc) {}

    template <class T, class Container, class Compare>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare& comp,
        const Container& cont, const Alloc& alloc) : _c(cont, alloc), _comp(comp) {
        std::copy(first, last, std::back_inserter(_c));
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class InputIter, class Alloc>
    requires (!std::is_integral_v<InputIter>) && std::is_class_v<Alloc> &&
        requires { typename std::allocator_traits<Alloc>::value_type; }
    priority_queue<T, Container, Compare>::priority_queue(InputIter first, InputIter last, const Compare& comp,
        Container&& cont, const Alloc& alloc) : _c(std::move(cont), alloc), _comp(comp) {
        std::copy(first, last, std::back_inserter(_c));
        make_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::push(const value_type& x) {
        _c.push_back(x);
        push_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::push(value_type&& x) {
        _c.push_back(std::move(x));
        push_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    template <class ... Args>
    requires std::constructible_from<typename Container::value_type, Args...>
    void priority_queue<T, Container, Compare>::emplace(Args&&... args) {
        _c.emplace_back(std::forward<Args>(args)...);
        push_heap(_c.begin(), _c.end(), _comp);
    }

    template <class T, class Container, class Compare>
    void priority_queue<T, Container, Compare>::pop() {
        pop_heap(_c.begin(), _c.end(), _comp);
        _c.pop_back();
    }
}