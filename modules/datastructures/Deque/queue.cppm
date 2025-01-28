/*
 * @ Created by jaehyung409 on 25. 1. 24.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

module;
#include <initializer_list>
#include <iterator>
#include <ranges>

export module queue;

import basics;
import deque;

namespace j {
    export template<class T, class Container = Deque<T>>
    class Queue {
    public:
        using value_type = typename Container::value_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;
        using size_type = typename Container::size_type;
        using container_type = Container;

    protected:
        Container _c;

    public:
        Queue() : Queue(Container()) {}
        explicit Queue(const Container &cont);
        explicit Queue(Container &&cont);

        //template <class InputIter>
        //Queue(InputIter first, InputIter last);
        //template <container-compatible-range<T> R>
        //Queue(from_range_t, R&& range);
        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        explicit Queue(const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Queue(const Container &cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Queue(Container &&cont, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Queue(const Queue &other, const Alloc &alloc);

        template <class Alloc>
        requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        Queue(Queue &&other, const Alloc &alloc);

        //template <class Alloc, class InputIter>
        //requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
        //Queue(InputIter first, InputIter last, const Alloc& alloc);
        //template <class Alloc, container-compatible-range<T> R>
        //Queue(from_range_t, R&& range, const Alloc& alloc);

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
        void swap(Queue &q) noexcept(std::is_nothrow_swappable_v<Container>) {
            using std::swap;
            swap(_c, q._c);
        }
    };

    template <class Container>
    Queue(Container cont) -> Queue<typename Container::value_type, Container>;

    template <class InputIter>
    Queue(InputIter, InputIter) -> Queue<typename std::iterator_traits<InputIter>::value_type>;

    //template <ranges::input_range R>
    //Queue(from_range_t, R &&) -> Queue<std::ranges::range_value_t<R>>;

    template <class Container, class Allocator>
    Queue(Container, Allocator) -> Queue<typename Container::value_type, Container>;

    template <class InputIter, class Allocator>
    Queue(InputIter, InputIter, Allocator)
        -> Queue<typename std::iterator_traits<InputIter>::value_type,
                 Deque<typename std::iterator_traits<InputIter>::value_type, Allocator>>;

    //template <ranges::input_range R, class Allocator>
    //Queue(from_range_t, R &&, Allocator)
    //  -> Queue<std::ranges::range_value_t<R>, Deque<std::ranges::range_value_t<R>>, Allocator>;

    template <class T, class Container, class Alloc>
    struct Uses_allocator<Queue<T, Container>, Alloc> : Uses_allocator<Container, Alloc>::type {};

}


namespace j {
    template <class T, class Container>
    Queue<T, Container>::Queue(const Container &cont) : _c(cont) {}

    template <class T, class Container>
    Queue<T, Container>::Queue(Container &&cont) : _c(std::move(cont)) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Queue<T, Container>::Queue(const Alloc &alloc) : _c(Container(), alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Queue<T, Container>::Queue(const Container &cont, const Alloc &alloc) : _c(cont, alloc) {}

    template <class T, class Container>
    template <class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Queue<T, Container>::Queue(Container &&cont, const Alloc &alloc) : _c(std::move(cont), alloc){}

    template<class T, class Container>
    template<class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Queue<T, Container>::Queue(const Queue &other, const Alloc &alloc) : _c(other._c, alloc) {}

    template<class T, class Container>
    template<class Alloc>
    requires std::is_class_v<Alloc> && requires { typename std::allocator_traits<Alloc>::value_type; }
    Queue<T, Container>::Queue(Queue &&other, const Alloc &alloc) : _c(std::move(other._c), alloc) {}

}