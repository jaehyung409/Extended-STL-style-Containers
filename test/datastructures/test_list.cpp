/*
 * @ Created by jaehyung409 on 25. 7. 29.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License. 
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <list>
#include <algorithm>
import j.list;

TEST_CASE("List Basic") {
    j::list<int> lst = {1, 2, 3, 4, 5};
    j::list char_lst = {'a', 'b', 'c'};

    SECTION("Constructors") {
        // Default constructor
        j::list<int> default_lst;
        REQUIRE(default_lst.empty());
        REQUIRE(default_lst.size() == 0);

        // Size constructor
        j::list<int> size_lst(5);
        REQUIRE(size_lst.size() == 5);
        for (auto& val : size_lst) {
            REQUIRE(val == 0);
        }

        // Size + value constructor
        j::list<int> size_val_lst(3, 42);
        REQUIRE(size_val_lst.size() == 3);
        for (auto& val : size_val_lst) {
            REQUIRE(val == 42);
        }

        // Iterator constructor
        std::vector<int> vec = {10, 20, 30};
        j::list<int> iter_lst(vec.begin(), vec.end());
        REQUIRE(iter_lst.size() == 3);
        auto it = iter_lst.begin();
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 20);
        REQUIRE(*it++ == 30);

        // Copy constructor
        j::list<int> copy_lst(lst);
        REQUIRE(copy_lst.size() == lst.size());
        auto it1 = lst.begin();
        auto it2 = copy_lst.begin();
        while (it1 != lst.end() && it2 != copy_lst.end()) {
            REQUIRE(*it1++ == *it2++);
        }

        // Move constructor
        j::list<int> move_source = {100, 200, 300};
        j::list<int> move_lst(std::move(move_source));
        REQUIRE(move_lst.size() == 3);
        REQUIRE(move_source.size() == 0);

        // Initializer list constructor
        j::list<int> init_lst = {7, 8, 9};
        REQUIRE(init_lst.size() == 3);
        auto init_it = init_lst.begin();
        REQUIRE(*init_it++ == 7);
        REQUIRE(*init_it++ == 8);
        REQUIRE(*init_it++ == 9);
    }

    SECTION("Assignment operators") {
        // Copy assignment
        j::list<int> assign_lst;
        assign_lst = lst;
        REQUIRE(assign_lst.size() == lst.size());
        auto it1 = lst.begin();
        auto it2 = assign_lst.begin();
        while (it1 != lst.end() && it2 != assign_lst.end()) {
            REQUIRE(*it1++ == *it2++);
        }

        // Move assignment
        j::list<int> move_source = {10, 20, 30};
        j::list<int> move_assign_lst;
        move_assign_lst = std::move(move_source);
        REQUIRE(move_assign_lst.size() == 3);
        REQUIRE(move_source.size() == 0);

        // Initializer list assignment
        j::list<int> init_assign_lst;
        init_assign_lst = {11, 22, 33};
        REQUIRE(init_assign_lst.size() == 3);
        auto it = init_assign_lst.begin();
        REQUIRE(*it++ == 11);
        REQUIRE(*it++ == 22);
        REQUIRE(*it++ == 33);
    }

    SECTION("Assign methods") {
        j::list<int> assign_lst;

        // assign with count and value
        assign_lst.assign(4, 99);
        REQUIRE(assign_lst.size() == 4);
        for (auto& val : assign_lst) {
            REQUIRE(val == 99);
        }

        // assign with iterators
        std::vector<int> vec = {5, 6, 7};
        assign_lst.assign(vec.begin(), vec.end());
        REQUIRE(assign_lst.size() == 3);
        auto it = assign_lst.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);
        REQUIRE(*it++ == 7);

        // assign with initializer list
        assign_lst.assign({15, 25, 35});
        REQUIRE(assign_lst.size() == 3);
        it = assign_lst.begin();
        REQUIRE(*it++ == 15);
        REQUIRE(*it++ == 25);
        REQUIRE(*it++ == 35);
    }

    SECTION("Iterators") {
        auto it = lst.begin();
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
        --it;
        REQUIRE(*it == 1);

        auto rit = lst.rbegin();
        REQUIRE(*rit == 5);
        ++rit;
        REQUIRE(*rit == 4);
        --rit;
        REQUIRE(*rit == 5);

        auto const_it = lst.cbegin();
        REQUIRE(*const_it == 1);
        ++const_it;
        REQUIRE(*const_it == 2);

        // Test iterator distance
        REQUIRE(std::distance(lst.begin(), lst.end()) == 5);
    }

    SECTION("Capacity") {
        j::list<int> empty_lst;
        REQUIRE(lst.size() == 5);
        REQUIRE(!lst.empty());
        REQUIRE(empty_lst.size() == 0);
        REQUIRE(empty_lst.empty());

        // max_size should be reasonable
        REQUIRE(lst.max_size() > 0);
    }

    SECTION("Element access") {
        REQUIRE(lst.front() == 1);
        REQUIRE(lst.back() == 5);
        REQUIRE(char_lst.front() == 'a');
        REQUIRE(char_lst.back() == 'c');

        // Test const versions
        const auto& const_lst = lst;
        REQUIRE(const_lst.front() == 1);
        REQUIRE(const_lst.back() == 5);
    }

    SECTION("Resize") {
        j::list<int> resize_lst = {1, 2, 3};

        // Resize smaller
        resize_lst.resize(2);
        REQUIRE(resize_lst.size() == 2);
        auto it = resize_lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);

        // Resize larger
        resize_lst.resize(4);
        REQUIRE(resize_lst.size() == 4);
        it = resize_lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 0);
        REQUIRE(*it++ == 0);

        // Resize with value
        resize_lst.resize(6, 42);
        REQUIRE(resize_lst.size() == 6);
        it = resize_lst.begin();
        std::advance(it, 4);
        REQUIRE(*it++ == 42);
        REQUIRE(*it++ == 42);
    }
}

TEST_CASE("List Modifiers") {
    j::list<int> lst;

    SECTION("Push/Pop front") {
        lst.push_front(10);
        lst.push_front(20);
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 20);
        REQUIRE(lst.back() == 10);

        lst.pop_front();
        REQUIRE(lst.size() == 1);
        REQUIRE(lst.front() == 10);
        REQUIRE(lst.back() == 10);
    }

    SECTION("Push/Pop back") {
        lst.push_back(30);
        lst.push_back(40);
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 30);
        REQUIRE(lst.back() == 40);

        lst.pop_back();
        REQUIRE(lst.size() == 1);
        REQUIRE(lst.front() == 30);
        REQUIRE(lst.back() == 30);
    }

    SECTION("Emplace front/back") {
        lst.emplace_front(100);
        lst.emplace_back(200);
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 100);
        REQUIRE(lst.back() == 200);
    }

    SECTION("Insert") {
        lst = {1, 2, 4, 5};
        auto it = lst.begin();
        std::advance(it, 2);

        // Insert single element
        auto insert_it = lst.insert(it, 3);
        REQUIRE(*insert_it == 3);
        REQUIRE(lst.size() == 5);

        // Check order
        auto check_it = lst.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);
        REQUIRE(*check_it++ == 4);
        REQUIRE(*check_it++ == 5);

        // Insert multiple elements
        it = lst.begin();
        std::advance(it, 1);
        lst.insert(it, 3, 10);
        REQUIRE(lst.size() == 8);

        // Insert with iterators
        std::vector<int> vec = {20, 21};
        it = lst.end();
        lst.insert(it, vec.begin(), vec.end());
        REQUIRE(lst.size() == 10);

        // Insert with initializer list
        it = lst.begin();
        lst.insert(it, {-1, -2});
        REQUIRE(lst.size() == 12);
    }

    SECTION("Emplace") {
        lst = {1, 3};
        auto it = lst.begin();
        ++it;
        auto emplace_it = lst.emplace(it, 2);
        REQUIRE(*emplace_it == 2);
        REQUIRE(lst.size() == 3);

        auto check_it = lst.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);
    }

    SECTION("Erase") {
        lst = {1, 2, 3, 4, 5};

        // Erase single element
        auto it = lst.begin();
        std::advance(it, 2);
        auto erase_it = lst.erase(it);
        REQUIRE(*erase_it == 4);
        REQUIRE(lst.size() == 4);

        // Erase range
        it = lst.begin();
        auto end_it = lst.begin();
        std::advance(end_it, 2);
        auto range_erase_it = lst.erase(it, end_it);
        REQUIRE(lst.size() == 2);
        REQUIRE(*range_erase_it == 4);
    }

    SECTION("Clear") {
        lst = {1, 2, 3};
        lst.clear();
        REQUIRE(lst.empty());
        REQUIRE(lst.size() == 0);
    }

    SECTION("Swap") {
        j::list<int> lst1 = {1, 2, 3};
        j::list<int> lst2 = {4, 5, 6};

        lst1.swap(lst2);
        REQUIRE(lst1.size() == 3);
        REQUIRE(lst2.size() == 3);
        REQUIRE(lst1.front() == 4);
        REQUIRE(lst2.front() == 1);

        // Test non-member swap
        j::swap(lst1, lst2);
        REQUIRE(lst1.front() == 1);
        REQUIRE(lst2.front() == 4);
    }
}

TEST_CASE("List Operations") {
    SECTION("Splice") {
        j::list<int> lst1 = {1, 2, 3};
        j::list<int> lst2 = {4, 5, 6};

        // Splice entire list
        auto it = lst1.begin();
        ++it;
        lst1.splice(it, lst2);
        REQUIRE(lst1.size() == 6);
        REQUIRE(lst2.empty());

        auto check_it = lst1.begin();
        REQUIRE(*check_it++ == 1);
        REQUIRE(*check_it++ == 4);
        REQUIRE(*check_it++ == 5);
        REQUIRE(*check_it++ == 6);
        REQUIRE(*check_it++ == 2);
        REQUIRE(*check_it++ == 3);

        // Splice single element
        lst2 = {10, 20, 30};
        it = lst1.begin();
        auto splice_it = lst2.begin();
        ++splice_it;
        lst1.splice(it, lst2, splice_it);
        REQUIRE(lst1.size() == 7);
        REQUIRE(lst2.size() == 2);
        REQUIRE(lst1.front() == 20);

        // Splice range
        lst2 = {100, 200, 300};
        it = lst1.end();
        lst1.splice(it, lst2, lst2.begin(), lst2.end());
        REQUIRE(lst1.size() == 10);
        REQUIRE(lst2.empty());
    }

    SECTION("Remove") {
        j::list<int> lst = {1, 2, 2, 3, 2, 4};
        lst.remove(2);
        REQUIRE(lst.size() == 3);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);
    }

    SECTION("Remove if") {
        j::list<int> lst = {1, 2, 3, 4, 5, 6};
        lst.remove_if([](int n) { return n % 2 == 0; });
        REQUIRE(lst.size() == 3);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 5);
    }

    SECTION("Unique") {
        j::list<int> lst = {1, 1, 2, 2, 2, 3, 3};
        lst.unique();
        REQUIRE(lst.size() == 3);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);

        // Unique with predicate
        lst = {1, 3, 5, 2, 4, 6};
        lst.unique([](int a, int b) { return (a % 2) == (b % 2); });
        REQUIRE(lst.size() == 2);
        REQUIRE(lst.front() == 1);
        REQUIRE(lst.back() == 2);
    }

    SECTION("Merge") {
        j::list<int> lst1 = {1, 3, 5};
        j::list<int> lst2 = {2, 4, 6};

        lst1.merge(lst2);
        REQUIRE(lst1.size() == 6);
        REQUIRE(lst2.empty());

        auto it = lst1.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 6);

        // Merge with custom comparator
        lst1 = {5, 3, 1};
        lst2 = {6, 4, 2};
        lst1.merge(lst2, std::greater<int>());
        REQUIRE(lst1.size() == 6);
        REQUIRE(lst2.empty());

        it = lst1.begin();
        REQUIRE(*it++ == 6);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);
    }

    SECTION("Sort") {
        j::list<int> lst = {5, 2, 8, 1, 9};
        lst.sort();
        REQUIRE(lst.size() == 5);

        auto it = lst.begin();
        REQUIRE(*it++ == 1);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 8);
        REQUIRE(*it++ == 9);

        // Sort with custom comparator
        lst = {1, 3, 2, 5, 4};
        lst.sort(std::greater<int>());

        it = lst.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);
    }

    SECTION("Reverse") {
        j::list<int> lst = {1, 2, 3, 4, 5};
        lst.reverse();
        REQUIRE(lst.size() == 5);

        auto it = lst.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 4);
        REQUIRE(*it++ == 3);
        REQUIRE(*it++ == 2);
        REQUIRE(*it++ == 1);
    }
}

TEST_CASE("List Edge Cases") {
    SECTION("Empty list operations") {
        j::list<int> empty_lst;
        REQUIRE(empty_lst.empty());
        REQUIRE(empty_lst.size() == 0);

        // Operations on empty list should not crash
        empty_lst.clear();
        empty_lst.reverse();
        empty_lst.unique();
        empty_lst.sort();

        j::list<int> other;
        empty_lst.merge(other);
        empty_lst.splice(empty_lst.begin(), other);
    }

    SECTION("Single element list") {
        j::list<int> single_lst = {42};
        REQUIRE(single_lst.size() == 1);
        REQUIRE(single_lst.front() == 42);
        REQUIRE(single_lst.back() == 42);

        single_lst.reverse();
        REQUIRE(single_lst.front() == 42);

        single_lst.sort();
        REQUIRE(single_lst.front() == 42);

        single_lst.unique();
        REQUIRE(single_lst.size() == 1);
    }
}

constexpr size_t N = 10000;

TEST_CASE("List Benchmarks: Basic Operations") {
    SECTION("j::list push_back") {
        j::list<int> lst;
        BENCHMARK("Push back j::list") {
            lst.clear();
            for (size_t i = 0; i < N; ++i) {
                lst.push_back(i);
            }
            return lst.size();
        };
    }

    SECTION("std::list push_back") {
        std::list<int> lst;
        BENCHMARK("Push back std::list") {
            lst.clear();
            for (size_t i = 0; i < N; ++i) {
                lst.push_back(i);
            }
            return lst.size();
        };
    }

    SECTION("j::list push_front") {
        j::list<int> lst;
        BENCHMARK("Push front j::list") {
            lst.clear();
            for (size_t i = 0; i < N; ++i) {
                lst.push_front(i);
            }
            return lst.size();
        };
    }

    SECTION("std::list push_front") {
        std::list<int> lst;
        BENCHMARK("Push front std::list") {
            lst.clear();
            for (size_t i = 0; i < N; ++i) {
                lst.push_front(i);
            }
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Insert Operations") {
    SECTION("j::list insert middle") {
        j::list<int> lst;
        for (size_t i = 0; i < N; ++i) {
            lst.push_back(i);
        }
        BENCHMARK("Insert middle j::list") {
            auto it = lst.begin();
            std::advance(it, lst.size() / 2);
            return *lst.insert(it, 999);
        };
    }

    SECTION("std::list insert middle") {
        std::list<int> lst;
        for (size_t i = 0; i < N; ++i) {
            lst.push_back(i);
        }
        BENCHMARK("Insert middle std::list") {
            auto it = lst.begin();
            std::advance(it, lst.size() / 2);
            return *lst.insert(it, 999);
        };
    }
}

TEST_CASE("List Benchmarks: Sort Operations") {
    SECTION("j::list sort") {
        j::list<int> lst;
        for (size_t i = N; i > 0; --i) {
            lst.push_back(i);
        }
        BENCHMARK("Sort j::list") {
            lst.sort();
            return lst.front() + lst.back();
        };
    }

    SECTION("std::list sort") {
        std::list<int> lst;
        for (size_t i = N; i > 0; --i) {
            lst.push_back(i);
        }
        BENCHMARK("Sort std::list") {
            lst.sort();
            return lst.front() + lst.back();
        };
    }
}

TEST_CASE("List Benchmarks: Splice Operations") {
    SECTION("j::list splice") {
        j::list<int> lst1, lst2;
        for (size_t i = 0; i < N / 2; ++i) {
            lst1.push_back(i);
            lst2.push_back(i + N / 2);
        }
        BENCHMARK("Splice j::list") {
            auto it = lst1.begin();
            std::advance(it, lst1.size() / 2);
            lst1.splice(it, lst2);
            return lst1.size();
        };
    }

    SECTION("std::list splice") {
        std::list<int> lst1, lst2;
        for (size_t i = 0; i < N / 2; ++i) {
            lst1.push_back(i);
            lst2.push_back(i + N / 2);
        }
        BENCHMARK("Splice std::list") {
            auto it = lst1.begin();
            std::advance(it, lst1.size() / 2);
            lst1.splice(it, lst2);
            return lst1.size();
        };
    }
}

TEST_CASE("List Benchmarks: Iteration") {
    SECTION("j::list iteration") {
        j::list<int> lst;
        for (size_t i = 0; i < N; ++i) {
            lst.push_back(i);
        }
        BENCHMARK("Iterate j::list") {
            int sum = 0;
            for (const auto& val : lst) {
                sum += val;
            }
            return sum;
        };
    }

    SECTION("std::list iteration") {
        std::list<int> lst;
        for (size_t i = 0; i < N; ++i) {
            lst.push_back(i);
        }
        BENCHMARK("Iterate std::list") {
            int sum = 0;
            for (const auto& val : lst) {
                sum += val;
            }
            return sum;
        };
    }
}