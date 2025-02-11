/*
 * @ Created by jaehyung409 on 25. 1. 24.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <initializer_list>
#include <memory>
#include <iterator>
#include <ranges>

export module j.queue;

import j.basics;
import j.deque;

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

        //template <class InputIter>
        //queue(InputIter first, InputIter last);
        //template <container-compatible-range<T> R>
        //queue(from_range_t, R&& range);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        explicit queue(const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        queue(const Container &cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        queue(Container &&cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        queue(const queue &other, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        queue(queue &&other, const Alloc &alloc);

        //template <class Alloc, class InputIter>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //queue(InputIter first, InputIter last, const Alloc& alloc);
        //template <class Alloc, container-compatible-range<T> R>
        //queue(from_range_t, R&& range, const Alloc& alloc);

        bool empty() const { return _c.empty(); }
        size_type size() const { return _c.size(); }
        reference front() { return _c.front(); }
        const_reference front() const { return _c.front(); }
        reference back() { return _c.back(); }
        const_reference back() const { return _c.back(); }

        void push(const value_type &x) { _c.push_back(x); }
        void push(value_type &&x) { _c.push_back(std::move(x)); }
        //template<container-compatible-range<T> R>
        //void push_range(R&& rg);
        template <class... Args>
        decltype(auto) emplace(Args &&... args) {
            return _c.emplace_back(std::forward<Args>(args)...);
        }

        void pop() { _c.pop_front(); }
        void swap(queue &q) noexcept(std::is_nothrow_swappable_v<Container>) {
            using std::swap;
            swap(_c, q._c);
        }
    };

    template <class Container>
    queue(Container cont) -> queue<typename Container::value_type, Container>;

    template <class InputIter>
    queue(InputIter, InputIter) -> queue<typename std::iterator_traits<InputIter>::value_type>;

    //template <ranges::input_range R>
    //queue(from_range_t, R &&) -> queue<std::ranges::range_value_t<R>>;

    template <class Container, class Allocator>
    queue(Container, Allocator) -> queue<typename Container::value_type, Container>;

    template <class InputIter, class Allocator>
    queue(InputIter, InputIter, Allocator)
        -> queue<typename std::iterator_traits<InputIter>::value_type,
                 deque<typename std::iterator_traits<InputIter>::value_type, Allocator>>;

    //template <ranges::input_range R, class Allocator>
    //queue(from_range_t, R &&, Allocator)
    //  -> queue<std::ranges::range_value_t<R>, dequedeque<std::ranges::range_value_t<R>>, Allocator>;

    template <class T, class Container, class Alloc>
    struct uses_allocator<queue<T, Container>, Alloc> : uses_allocator<Container, Alloc>::type {};

}


namespace j {
    template <class T, class Container>
    queue<T, Container>::queue(const Container &cont) : _c(cont) {}

    template <class T, class Container>
    queue<T, Container>::queue(Container &&cont) : _c(std::move(cont)) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    queue<T, Container>::queue(const Alloc &alloc) : _c(Container(), alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    queue<T, Container>::queue(const Container &cont, const Alloc &alloc) : _c(cont, alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    queue<T, Container>::queue(Container &&cont, const Alloc &alloc) : _c(std::move(cont), alloc){}

    template<class T, class Container>
    template<class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    queue<T, Container>::queue(const queue &other, const Alloc &alloc) : _c(other._c, alloc) {}

    template<class T, class Container>
    template<class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    queue<T, Container>::queue(queue &&other, const Alloc &alloc) : _c(std::move(other._c), alloc) {}

}