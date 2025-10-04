/*
* @ Created by jaehyung409 on 25. 8. 22.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <algorithm>
#include <iterator>
#include <random>
import j;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42);
std::uniform_int_distribution<> dis(0, static_cast<int>(N - 1));

TEST_CASE("Deque Basic") {
    SECTION("Iterator Category") {
        j::deque<int> deq;
        using It = decltype(deq.begin());
        REQUIRE(std::is_same_v<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>);
    }
    SECTION("Constructors") {
        j::deque<int> default_d;
        REQUIRE(default_d.empty());
        REQUIRE(default_d.size() == 0);

        j::deque<int> size_d(5);
        REQUIRE(size_d.size() == 5);
        for (auto& x : size_d) REQUIRE(x == 0);

        j::deque<int> val_d(3, 42);
        REQUIRE(val_d.size() == 3);
        for (auto& x : val_d) REQUIRE(x == 42);

        j::deque<int> src = {1, 2, 3};
        j::deque<int> iter_d(src.begin(), src.end());
        REQUIRE(iter_d.size() == src.size());
        auto it = iter_d.begin();
        for (auto v : src) REQUIRE(*it++ == v);

        j::deque<int> copy_d(iter_d);
        REQUIRE(copy_d == iter_d);

        j::deque<int> move_src = {7, 8, 9};
        size_t old = move_src.size();
        j::deque<int> move_d(std::move(move_src));
        REQUIRE(move_d.size() == old);
        REQUIRE(move_src.size() == 0);
        it = move_d.begin(); REQUIRE(*it++ == 7);

        j::deque<int> init_d = {4, 5, 6};
        REQUIRE(init_d.size() == 3);
        it = init_d.begin(); REQUIRE(*it++ == 4); REQUIRE(*it++ == 5); REQUIRE(*it++ == 6);

        j::deque<int> alloc_d(std::allocator<int>{});
        REQUIRE(alloc_d.empty());
    }
    SECTION("Assignment operators") {
        j::deque<int> src = {1, 2, 3};
        j::deque<int> copy_assign;
        copy_assign = src;
        REQUIRE(copy_assign == src);

        j::deque<int> move_src = {10, 20, 30};
        j::deque<int> move_assign;
        move_assign = std::move(move_src);
        REQUIRE(move_assign.size() == 3);
        REQUIRE(move_src.size() == 0);

        j::deque<int> il_assign;
        il_assign = {11, 22, 33};
        REQUIRE(il_assign.size() == 3);
        auto it = il_assign.begin(); REQUIRE(*it++ == 11); REQUIRE(*it++ == 22); REQUIRE(*it++ == 33);
    }
    SECTION("Assign methods") {
        j::deque<int> d;
        d.assign(4, 99);
        REQUIRE(d.size() == 4);
        for (auto x : d) REQUIRE(x == 99);

        j::deque<int> tmp = {5, 6, 7};
        d.assign(tmp.begin(), tmp.end());
        REQUIRE(d.size() == tmp.size());
        auto it = d.begin(); for (auto x : tmp) REQUIRE(*it++ == x);

        d.assign({15, 25, 35});
        REQUIRE(d.size() == 3);
        it = d.begin(); REQUIRE(*it++ == 15); REQUIRE(*it++ == 25); REQUIRE(*it++ == 35);
    }
    SECTION("Iterators") {
        j::deque<int> d = {1, 2, 3, 4, 5};
        auto it = d.begin(); REQUIRE(*it == 1);
        REQUIRE(*(it + 2) == 3);
        ++it; REQUIRE(*it == 2);
        --it; REQUIRE(*it == 1);
        it += 3; REQUIRE(*it == 4);
        it -= 2; REQUIRE(*it == 2);
        REQUIRE(it[1] == 3);
        auto rit = d.rbegin(); REQUIRE(*rit == 5);
        ++rit; REQUIRE(*rit == 4);
        --rit; REQUIRE(*rit == 5);
        const auto& cd = d;
        auto cit = cd.begin(); REQUIRE(*cit == 1);
        auto cit2 = d.cbegin(); REQUIRE(*cit2 == 1);
        REQUIRE(std::distance(d.begin(), d.end()) == 5);
        REQUIRE(std::distance(d.cbegin(), d.cend()) == 5);
        REQUIRE(std::distance(d.rbegin(), d.rend()) == 5);
        REQUIRE(std::distance(d.crbegin(), d.crend()) == 5);
    }
    SECTION("Capacity") {
        j::deque<int> d;
        REQUIRE(d.empty());
        REQUIRE(d.max_size() > 0);
        d.resize(3);
        REQUIRE(d.size() == 3);
        d.resize(5, 7);
        REQUIRE(d.size() == 5);
        REQUIRE(d.back() == 7);
        d.clear();
        REQUIRE(d.empty());
    }
    SECTION("Element Access") {
        j::deque<int> d = {1, 2, 3};
        REQUIRE(d[0] == 1);
        REQUIRE(d.at(1) == 2);
        REQUIRE(d.front() == 1);
        REQUIRE(d.back() == 3);
        // Test bounds checking
        REQUIRE_THROWS_AS(d.at(10), std::out_of_range);
        const auto& cd = d;
        REQUIRE(cd[0] == 1);
        REQUIRE(cd.at(1) == 2);
        REQUIRE(cd.front() == 1);
        REQUIRE(cd.back() == 3);
    }
    SECTION("Modifiers") {
        j::deque<int> d;
        d.push_back(10);
        d.emplace_back(20);
        REQUIRE(d.size() == 2);
        REQUIRE(d.front() == 10);
        REQUIRE(d.back() == 20);
        d.pop_back();
        REQUIRE(d.size() == 1);
        d.clear();
        d = {1, 4};
        auto pos = d.emplace(d.begin() + 1, 2);
        REQUIRE(*pos == 2);
        REQUIRE(d[1] == 2);
        d.insert(d.begin() + 2, 3);
        REQUIRE(d[2] == 3);
        d.insert(d.begin(), 2, 5);
        REQUIRE(d.size() >= 5);
        REQUIRE(d[0] == 5);
        REQUIRE(d[1] == 5);
        j::deque<int> r = {6, 7};
        d.insert(d.end(), r.begin(), r.end());
        REQUIRE(d.size() >= 7);
        d.insert(d.begin(), {-1, -2});
        REQUIRE(d.size() >= 9);
        auto old_size = d.size();
        auto erase_it = d.erase(d.begin());
        REQUIRE(d.size() == old_size - 1);
        old_size = d.size();
        d.erase(d.begin() + 1, d.begin() + 3);
        REQUIRE(d.size() == old_size - 2);
        j::deque<int> a = {1, 2}, b = {3, 4};
        a.swap(b);
        REQUIRE(a.front() == 3);
        REQUIRE(b.front() == 1);
        j::swap(a, b);
        REQUIRE(a.front() == 1);
    }
    SECTION("Comparison operators") {
        j::deque<int> d1 = {1, 2, 3};
        j::deque<int> d2 = {1, 2, 3};
        j::deque<int> d3 = {3, 2, 1};
        j::deque<int> d4 = {1, 2, 3, 4};
        REQUIRE(d1 == d2);
        REQUIRE(!(d1 != d2));
        REQUIRE(d1 != d3);
        REQUIRE(d1 != d4);
        REQUIRE((d1 <=> d2) == std::strong_ordering::equal);
        REQUIRE((d1 <=> d3) == std::strong_ordering::less);
        REQUIRE((d3 <=> d1) == std::strong_ordering::greater);
        REQUIRE((d1 <=> d4) == std::strong_ordering::less);
    }
    SECTION("Allocator") {
        j::deque<int> d;
        auto alloc = d.get_allocator();
        REQUIRE(std::is_same_v<decltype(alloc), std::allocator<int>>);
    }
}

TEST_CASE("Deque Edge Cases") {
    SECTION("Empty deque operations") {
        j::deque<int> empty_d;
        REQUIRE(empty_d.begin() == empty_d.end());
        REQUIRE(empty_d.cbegin() == empty_d.cend());
        REQUIRE(empty_d.rbegin() == empty_d.rend());
        empty_d.clear();
        REQUIRE(empty_d.empty());
        empty_d.resize(3, 42);
        REQUIRE(empty_d.size() == 3);
        for (auto& val : empty_d) REQUIRE(val == 42);
    }
    SECTION("Single element operations") {
        j::deque<int> single_d = {42};
        REQUIRE(single_d.front() == 42);
        REQUIRE(single_d.back() == 42);
        REQUIRE(single_d.size() == 1);
        REQUIRE(single_d[0] == 42);
        REQUIRE(single_d.at(0) == 42);
        single_d.pop_back();
        REQUIRE(single_d.empty());
    }
    SECTION("Large capacity operations") {
        j::deque<int> d;
        d.resize(1000);
        REQUIRE(d.size() == 1000);
        d.clear();
        REQUIRE(d.empty());
    }
    SECTION("Iterator invalidation scenarios") {
        j::deque<int> d = {1, 2, 3, 4, 5};
        auto it = d.begin() + 2;
        REQUIRE(*it == 3);
        d.insert(d.begin(), 0);
        REQUIRE(d[3] == 3);
    }
}

TEST_CASE("Deque Large Dataset") {
    SECTION("push_back & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);
        REQUIRE(d.size() == N);
        int v = 1;
        for (auto it = d.begin(); it != d.end(); ++it, ++v) REQUIRE(*it == v);
    }
    SECTION("emplace_back & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.emplace_back(i);
        REQUIRE(d.size() == N);
        int v = 1;
        for (auto it = d.begin(); it != d.end(); ++it, ++v) REQUIRE(*it == v);
    }
    SECTION("push_front & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_front(i);
        REQUIRE(d.size() == N);
        int v = N;
        for (auto it = d.begin(); it != d.end(); ++it, --v) REQUIRE(*it == v);
    }
    SECTION("emplace_front & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.emplace_front(i);
        REQUIRE(d.size() == N);
        int v = N;
        for (auto it = d.begin(); it != d.end(); ++it, --v) REQUIRE(*it == v);
    }
    SECTION("pop_back & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);
        for (int i = N; i >= 1; --i) {
            REQUIRE(d.back() == i);
            d.pop_back();
        }
        REQUIRE(d.empty());
    }
    SECTION("pop_front & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);
        for (int i = 1; i <= N; ++i) {
            REQUIRE(d.front() == i);
            d.pop_front();
        }
        REQUIRE(d.empty());
    }
    SECTION("erase middle range & value check") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);
        auto it = d.erase(d.begin() + N/4, d.begin() + 3*N/4);
        REQUIRE(d.size() == N/2);
        int v = 1;
        for (auto iter = d.begin(); iter != d.end(); ++iter, ++v) {
            if (v == N/4 + 1) v = 3*N/4 + 1;
            REQUIRE(*iter == v);
        }
    }
    SECTION("N insert + emplace middle + verify") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);

        // emplace at middle position
        auto mid_pos = d.begin() + N/2;
        d.emplace(mid_pos, -999);

        REQUIRE(d.size() == N + 1);
        REQUIRE(d[N/2] == -999);

        // verify elements before middle
        for (int i = 0; i < N/2; ++i) {
            REQUIRE(d[i] == i + 1);
        }
        // verify elements after middle
        for (int i = N/2 + 1; i < N + 1; ++i) {
            REQUIRE(d[i] == i);  // shifted by one due to emplace
        }
    }
    SECTION("N insert + insert range + verify") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);

        // prepare range to insert
        std::vector<int> range_to_insert;
        for (int i = 2000; i < 2000 + N; ++i) {
            range_to_insert.push_back(i);
        }

        // insert range at position N/3
        auto insert_pos = d.begin() + N/3;
        d.insert(insert_pos, range_to_insert.begin(), range_to_insert.end());

        REQUIRE(d.size() == N + N);

        // verify elements before insertion point (1 to N/3)
        for (int i = 0; i < N/3; ++i) {
            REQUIRE(d[i] == i + 1);
        }
        // verify inserted range
        for (int i = N/3; i < N/3 + N; ++i) {
            REQUIRE(d[i] == 2000 + (i - N/3));
        }
        // verify elements after insertion point
        for (int i = N/3 + N; i < 2*N; ++i) {
            REQUIRE(d[i] == (i - N) + 1);
        }
    }
    SECTION("N insert + erase middle N/2 + verify") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);

        // erase N/2 elements from middle
        size_t start_erase = N/4;
        size_t end_erase = start_erase + N/2;
        d.erase(d.begin() + start_erase, d.begin() + end_erase);

        REQUIRE(d.size() == N - N/2);

        // verify elements before erased range
        for (size_t i = 0; i < start_erase; ++i) {
            REQUIRE(d[i] == static_cast<int>(i + 1));
        }
        // verify elements after erased range (shifted)
        for (size_t i = start_erase; i < d.size(); ++i) {
            REQUIRE(d[i] == static_cast<int>(i + N/2 + 1));
        }
    }
    SECTION("N insert + erase single element + verify") {
        j::deque<int> d;
        for (int i = 1; i <= N; ++i) d.push_back(i);

        // erase single element at position N/2
        size_t erase_pos = N/2;
        int erased_value = d[erase_pos];
        d.erase(d.begin() + erase_pos);

        REQUIRE(d.size() == N - 1);
        REQUIRE(erased_value == static_cast<int>(erase_pos + 1));

        // verify elements before erased position
        for (size_t i = 0; i < erase_pos; ++i) {
            REQUIRE(d[i] == static_cast<int>(i + 1));
        }
        // verify elements after erased position (shifted)
        for (size_t i = erase_pos; i < d.size(); ++i) {
            REQUIRE(d[i] == static_cast<int>(i + 2));
        }
    }
}

TEST_CASE("Deque Interface Coverage") {
    SECTION("Constructors and Destructor") {
        j::deque<int> d1;
        j::deque<int> d2(5);
        j::deque<int> d3(3, 42);
        std::vector<int> v = {1,2,3};
        j::deque<int> d4(v.begin(), v.end());
        j::deque<int> d5 = d4;
        j::deque<int> d6 = std::move(d5);
        j::deque<int> d7(d4, std::allocator<int>{});
        j::deque<int> d8(std::move(d6), std::allocator<int>{});
        j::deque<int> d9 = {7,8,9};
        j::deque<int> d10({1,2,3}, std::allocator<int>{});
        // Destructor is implicit
    }
    SECTION("Assignment and Assign Methods") {
        j::deque<int> d1 = {1,2,3};
        j::deque<int> d2;
        d2 = d1;
        j::deque<int> d3;
        d3 = std::move(d2);
        j::deque<int> d4;
        d4 = {4,5,6};
        j::deque<int> d5;
        d5.assign(3, 7);
        std::vector<int> v = {8,9};
        d5.assign(v.begin(), v.end());
        d5.assign({10,11});
        auto alloc = d5.get_allocator();
        REQUIRE(std::is_same_v<decltype(alloc), std::allocator<int>>);
    }
    SECTION("Iterators") {
        j::deque<int> d = {1,2,3};
        auto it = d.begin();
        auto cit = d.cbegin();
        auto eit = d.end();
        auto ceit = d.cend();
        auto rit = d.rbegin();
        auto crit = d.crbegin();
        auto reit = d.rend();
        auto creit = d.crend();
        REQUIRE(*it == 1);
        REQUIRE(*cit == 1);
        REQUIRE(*(rit) == 3);
        REQUIRE(*(crit) == 3);
        REQUIRE(std::distance(it, eit) == 3);
        REQUIRE(std::distance(rit, reit) == 3);
        REQUIRE(std::distance(cit, ceit) == 3);
        REQUIRE(std::distance(crit, creit) == 3);
    }
    SECTION("Capacity") {
        j::deque<int> d;
        REQUIRE(d.empty());
        d.resize(2);
        REQUIRE(d.size() == 2);
        d.resize(4, 5);
        REQUIRE(d.size() == 4);
        d.shrink_to_fit();
        REQUIRE(d.size() == 4);
        REQUIRE(d.max_size() > 0);
    }
    SECTION("Element Access") {
        j::deque<int> d = {1,2,3};
        REQUIRE(d[0] == 1);
        REQUIRE(d.at(1) == 2);
        REQUIRE(d.front() == 1);
        REQUIRE(d.back() == 3);
        const auto& cd = d;
        REQUIRE(cd[0] == 1);
        REQUIRE(cd.at(1) == 2);
        REQUIRE(cd.front() == 1);
        REQUIRE(cd.back() == 3);
        REQUIRE_THROWS_AS(d.at(10), std::out_of_range);
    }
    SECTION("Modifiers: emplace/push/insert/erase/pop/clear/swap") {
        j::deque<int> d;
        // emplace_front/back
        d.emplace_front(1);
        d.emplace_back(2);
        REQUIRE(d.front() == 1);
        REQUIRE(d.back() == 2);
        // push_front/back lvalue/rvalue
        int x = 3, y = 4;
        d.push_front(x);
        d.push_front(std::move(y));
        d.push_back(x);
        d.push_back(std::move(y));
        // emplace
        d.emplace(d.begin() + 1, 99);
        // insert lvalue/rvalue/count/iterator/il
        d.insert(d.begin(), 5);
        d.insert(d.begin(), 6);
        d.insert(d.begin(), 2, 7);
        std::vector<int> v = {8,9};
        d.insert(d.begin(), v.begin(), v.end());
        d.insert(d.begin(), {10,11});
        // erase single/range
        d.erase(d.begin());
        d.erase(d.begin(), d.begin() + 2);
        // pop_front/back
        d.pop_front();
        d.pop_back();
        // clear
        d.clear();
        REQUIRE(d.empty());
        // swap
        j::deque<int> a = {1,2}, b = {3,4};
        a.swap(b);
        REQUIRE(a.front() == 3);
        j::swap(a, b);
        REQUIRE(a.front() == 1);
    }
}

TEST_CASE("Deque Iterator Behavior") {
    SECTION("for-range forward iteration") {
        j::deque<int> d;
        std::vector<int> v;
        for (int i = 0; i < 100; ++i) { d.push_back(i); v.push_back(i); }
        size_t idx = 0;
        for (int x : d) {
            REQUIRE(x == v[idx]);
            ++idx;
        }
        REQUIRE(idx == v.size());
    }
    SECTION("for-range reverse iteration") {
        j::deque<int> d;
        std::vector<int> v;
        for (int i = 0; i < 100; ++i) { d.push_back(i); v.push_back(i); }
        size_t idx = v.size();
        for (auto it = d.rbegin(); it != d.rend(); ++it) {
            --idx;
            REQUIRE(*it == v[idx]);
        }
        REQUIRE(idx == 0);
    }
    SECTION("const_iterator and crbegin/crend") {
        j::deque<int> d;
        for (int i = 0; i < 10; ++i) d.push_back(i);
        const j::deque<int>& cd = d;
        int sum = 0;
        for (auto it = cd.cbegin(); it != cd.cend(); ++it) sum += *it;
        int sum2 = 0;
        for (auto it = cd.crbegin(); it != cd.crend(); ++it) sum2 += *it;
        REQUIRE(sum == 45); // 0+1+...+9
        REQUIRE(sum2 == 45);
    }
    SECTION("empty deque iteration") {
        j::deque<int> d;
        int cnt = 0;
        for (auto x : d) ++cnt;
        REQUIRE(cnt == 0);
        cnt = 0;
        for (auto it = d.rbegin(); it != d.rend(); ++it) ++cnt;
        REQUIRE(cnt == 0);
    }
    SECTION("single element iteration") {
        j::deque<int> d = {42};
        int cnt = 0;
        for (auto x : d) { REQUIRE(x == 42); ++cnt; }
        REQUIRE(cnt == 1);
        cnt = 0;
        for (auto it = d.rbegin(); it != d.rend(); ++it) { REQUIRE(*it == 42); ++cnt; }
        REQUIRE(cnt == 1);
    }
    SECTION("iterator after erase/insert") {
        j::deque<int> d;
        for (int i = 0; i < 10; ++i) d.push_back(i);
        d.erase(d.begin() + 5); // remove 5
        std::vector<int> v = {0,1,2,3,4,6,7,8,9};
        size_t idx = 0;
        for (int x : d) { REQUIRE(x == v[idx]); ++idx; }
        d.insert(d.begin() + 5, 5); // re-insert 5
        idx = 0;
        for (int x : d) { REQUIRE(x == idx); ++idx; }
    }
}
