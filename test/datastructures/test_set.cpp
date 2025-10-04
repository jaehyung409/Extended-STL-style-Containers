/*
 * @ Created by jaehyung409 on 25. 10. 2.
 * @ Copyright (c) 2025 jaehyung409 All rights reserved.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <functional>
import j;

const int N = 10000;
const int DUPLICATES = 10;

TEST_CASE("Set Basic") {
    j::set<int> s;
    j::set<int> s_init = {5, 2, 8, 1, 9, 3};
    j::set<std::string> s_str = {"apple", "banana", "cherry"};

    SECTION("Construction and Initialization") {
        REQUIRE(s.empty());
        REQUIRE(s.size() == 0);

        REQUIRE(!s_init.empty());
        REQUIRE(s_init.size() == 6);

        // Test deduction guide
        j::set s_deduction = {10, 20, 30};
        REQUIRE(s_deduction.size() == 3);
        REQUIRE(s_deduction.contains(20));
    }

    SECTION("Iterator construction") {
        std::vector<int> vec = {4, 1, 7, 2, 9};
        j::set<int> s_iter(vec.begin(), vec.end());
        REQUIRE(s_iter.size() == 5);
        REQUIRE(s_iter.contains(4));
        REQUIRE(s_iter.contains(9));
    }

    SECTION("Copy and Move construction") {
        j::set<int> s_copy(s_init);
        REQUIRE(s_copy.size() == s_init.size());
        REQUIRE(s_copy == s_init);

        j::set<int> s_move(std::move(s_copy));
        REQUIRE(s_move.size() == 6);
        REQUIRE((s_copy.empty() || s_copy.size() == 6)); // Implementation dependent
    }

    SECTION("Insert operations") {
        auto [it1, inserted1] = s.insert(10);
        REQUIRE(inserted1);
        REQUIRE(*it1 == 10);
        REQUIRE(s.size() == 1);

        auto [it2, inserted2] = s.insert(10); // Duplicate
        REQUIRE(!inserted2);
        REQUIRE(*it2 == 10);
        REQUIRE(s.size() == 1);

        // Insert with hint
        auto it3 = s.insert(it1, 15);
        REQUIRE(*it3 == 15);
        REQUIRE(s.size() == 2);
    }

    SECTION("Emplace operations") {
        auto [it1, inserted1] = s.emplace(42);
        REQUIRE(inserted1);
        REQUIRE(*it1 == 42);

        auto [it2, inserted2] = s.emplace(42); // Duplicate
        REQUIRE(!inserted2);

        // Emplace with hint
        auto it3 = s.emplace_hint(it1, 50);
        REQUIRE(*it3 == 50);
    }

    SECTION("Find and Contains") {
        REQUIRE(s_init.contains(5));
        REQUIRE(!s_init.contains(100));

        auto it = s_init.find(8);
        REQUIRE(it != s_init.end());
        REQUIRE(*it == 8);

        auto it_not_found = s_init.find(100);
        REQUIRE(it_not_found == s_init.end());
    }

    SECTION("Count operations") {
        REQUIRE(s_init.count(5) == 1);
        REQUIRE(s_init.count(100) == 0);
    }

    SECTION("Bound operations") {
        // lower_bound
        auto lb = s_init.lower_bound(5);
        REQUIRE(lb != s_init.end());
        REQUIRE(*lb == 5);

        // upper_bound
        auto ub = s_init.upper_bound(5);
        REQUIRE(ub != s_init.end());
        REQUIRE(*ub > 5);

        // equal_range
        auto [first, last] = s_init.equal_range(5);
        REQUIRE(first != s_init.end());
        REQUIRE(*first == 5);
        REQUIRE(std::distance(first, last) == 1);
    }

    SECTION("Erase operations") {
        j::set<int> s_erase = {1, 2, 3, 4, 5};

        // Erase by key
        auto erased_count = s_erase.erase(3);
        REQUIRE(erased_count == 1);
        REQUIRE(!s_erase.contains(3));
        REQUIRE(s_erase.size() == 4);

        // Erase by iterator
        auto it = s_erase.find(2);
        REQUIRE(it != s_erase.end());
        auto next_it = s_erase.erase(it);
        REQUIRE(!s_erase.contains(2));
        REQUIRE(*next_it > 2);

        // Erase range
        auto first = s_erase.begin();
        auto last = std::next(first, 2);
        s_erase.erase(first, last);
        REQUIRE(s_erase.size() == 1);
    }

    SECTION("Iterators") {
        std::vector<int> result;
        for (auto it = s_init.begin(); it != s_init.end(); ++it) {
            result.push_back(*it);
        }
        REQUIRE(std::is_sorted(result.begin(), result.end()));

        // Reverse iterators
        std::vector<int> reverse_result;
        for (auto it = s_init.rbegin(); it != s_init.rend(); ++it) {
            reverse_result.push_back(*it);
        }
        REQUIRE(std::is_sorted(reverse_result.rbegin(), reverse_result.rend()));
    }

    SECTION("Clear and Swap") {
        j::set<int> s1 = {1, 2, 3};
        j::set<int> s2 = {4, 5, 6};

        s1.clear();
        REQUIRE(s1.empty());

        s1 = {7, 8, 9};
        s1.swap(s2);
        REQUIRE(s1.contains(4));
        REQUIRE(s2.contains(7));
    }

    SECTION("Comparison operators") {
        j::set<int> s1 = {1, 2, 3};
        j::set<int> s2 = {1, 2, 3};
        j::set<int> s3 = {1, 2, 4};

        REQUIRE(s1 == s2);
        REQUIRE(s1 != s3);
        REQUIRE((s1 <=> s3) == std::strong_ordering::less);
    }

    SECTION("Key and Value comparison") {
        auto key_comp = s_init.key_comp();
        auto value_comp = s_init.value_comp();

        REQUIRE(key_comp(1, 2));
        REQUIRE(!key_comp(2, 1));
        REQUIRE(value_comp(1, 2));
        REQUIRE(!value_comp(2, 1));
    }
}

TEST_CASE("MultiSet Basic") {
    j::multiset<int> ms;
    j::multiset<int> ms_init = {5, 2, 8, 1, 9, 3, 5, 2}; // With duplicates
    j::multiset<std::string> ms_str = {"apple", "banana", "cherry", "apple"};

    SECTION("Construction and Initialization") {
        REQUIRE(ms.empty());
        REQUIRE(ms.size() == 0);

        REQUIRE(!ms_init.empty());
        REQUIRE(ms_init.size() == 8); // Includes duplicates

        // Test deduction guide
        j::multiset ms_deduction = {10, 20, 30, 20};
        REQUIRE(ms_deduction.size() == 4);
        REQUIRE(ms_deduction.count(20) == 2);
    }

    SECTION("Insert operations with duplicates") {
        auto it1 = ms.insert(10);
        REQUIRE(*it1 == 10);
        REQUIRE(ms.size() == 1);

        auto it2 = ms.insert(10); // Duplicate allowed
        REQUIRE(*it2 == 10);
        REQUIRE(ms.size() == 2);
        REQUIRE(ms.count(10) == 2);

        // Insert with hint
        auto it3 = ms.insert(it1, 15);
        REQUIRE(*it3 == 15);
        REQUIRE(ms.size() == 3);
    }

    SECTION("Emplace operations with duplicates") {
        auto it1 = ms.emplace(42);
        REQUIRE(*it1 == 42);

        auto it2 = ms.emplace(42); // Duplicate allowed
        REQUIRE(*it2 == 42);
        REQUIRE(ms.count(42) == 2);
    }

    SECTION("Count operations with duplicates") {
        REQUIRE(ms_init.count(5) == 2);
        REQUIRE(ms_init.count(2) == 2);
        REQUIRE(ms_init.count(8) == 1);
        REQUIRE(ms_init.count(100) == 0);
    }

    SECTION("Find operations with duplicates") {
        auto it = ms_init.find(5);
        REQUIRE(it != ms_init.end());
        REQUIRE(*it == 5);

        // Should find any occurrence of the key
        auto count = 0;
        for (auto iter = ms_init.begin(); iter != ms_init.end(); ++iter) {
            if (*iter == 5) count++;
        }
        REQUIRE(count == 2);
    }

    SECTION("Equal range with duplicates") {
        auto [first, last] = ms_init.equal_range(5);
        REQUIRE(std::distance(first, last) == 2);

        for (auto it = first; it != last; ++it) {
            REQUIRE(*it == 5);
        }
    }

    SECTION("Erase operations with duplicates") {
        j::multiset<int> ms_erase = {1, 2, 2, 3, 3, 3, 4};

        // Erase all occurrences by key
        auto erased_count = ms_erase.erase(3);
        REQUIRE(erased_count == 3);
        REQUIRE(!ms_erase.contains(3));

        // Erase single occurrence by iterator
        auto it = ms_erase.find(2);
        REQUIRE(it != ms_erase.end());
        ms_erase.erase(it);
        REQUIRE(ms_erase.count(2) == 1); // One still remains
    }

    SECTION("Iterators with duplicates") {
        std::vector<int> result;
        for (const auto& val : ms_init) {
            result.push_back(val);
        }
        REQUIRE(ms_init.size() == 8);
        REQUIRE(std::is_sorted(result.begin(), result.end()));
        REQUIRE(result.size() == 8);
    }
}

TEST_CASE("Set Edge Cases") {
    SECTION("Empty set operations") {
        j::set<int> empty_set;

        REQUIRE(empty_set.empty());
        REQUIRE(empty_set.size() == 0);
        REQUIRE(empty_set.begin() == empty_set.end());
        REQUIRE(empty_set.find(1) == empty_set.end());
        REQUIRE(empty_set.count(1) == 0);
        REQUIRE(!empty_set.contains(1));

        // Operations on empty set should be safe
        empty_set.clear(); // Should not crash
        REQUIRE(empty_set.erase(1) == 0);
    }

    SECTION("Single element set") {
        j::set<int> single = {42};

        REQUIRE(single.size() == 1);
        REQUIRE(!single.empty());
        REQUIRE(single.contains(42));
        REQUIRE(*single.begin() == 42);
        REQUIRE(*single.rbegin() == 42);

        single.erase(42);
        REQUIRE(single.empty());
    }

    SECTION("Custom comparator") {
        j::set<int, std::greater<int>> desc_set = {3, 1, 4, 1, 5};

        std::vector<int> result;
        for (const auto& val : desc_set) {
            result.push_back(val);
        }

        // Should be in descending order
        REQUIRE(std::is_sorted(result.begin(), result.end(), std::greater<int>()));
        REQUIRE(result.front() == 5);
        REQUIRE(result.back() == 1);
    }

    SECTION("Node extraction and insertion") {
        j::set<int> s1 = {1, 2, 3};
        j::set<int> s2 = {4, 5, 6};

        // Extract node
        auto node = s1.extract(2);
        REQUIRE(!node.empty());
        REQUIRE(node.value() == 2);
        REQUIRE(!s1.contains(2));
        REQUIRE(s1.size() == 2);

        // Insert node
        auto result = s2.insert(std::move(node));
        REQUIRE(result.inserted);
        REQUIRE(*result.position == 2);
        REQUIRE(s2.contains(2));
        REQUIRE(s2.size() == 4);
    }

    SECTION("Merge operations") {
        j::set<int> s1 = {1, 3, 5};
        j::set<int> s2 = {2, 4, 6};
        j::set<int> s3 = {1, 2, 7}; // Has overlapping elements

        s1.merge(s2);
        REQUIRE(s1.size() == 6);
        REQUIRE(s2.empty());

        auto original_size = s1.size();
        s1.merge(s3);
        REQUIRE(s1.size() == original_size + 1); // Only 7 is new
        REQUIRE(s3.size() == 2); // 1 and 2 remain in s3
    }

    SECTION("Large range operations") {
        std::vector<int> large_range(1000);
        std::iota(large_range.begin(), large_range.end(), 1);

        j::set<int> large_set(large_range.begin(), large_range.end());
        REQUIRE(large_set.size() == 1000);

        // Erase half
        auto mid = std::next(large_set.begin(), 500);
        large_set.erase(large_set.begin(), mid);
        REQUIRE(large_set.size() == 500);
        REQUIRE(*large_set.begin() >= 501);
    }
}

TEST_CASE("MultiSet Edge Cases") {
    SECTION("Empty multiset operations") {
        j::multiset<int> empty_ms;

        REQUIRE(empty_ms.empty());
        REQUIRE(empty_ms.size() == 0);
        REQUIRE(empty_ms.begin() == empty_ms.end());
        REQUIRE(empty_ms.find(1) == empty_ms.end());
        REQUIRE(empty_ms.count(1) == 0);
        REQUIRE(!empty_ms.contains(1));
    }

    SECTION("All same elements") {
        j::multiset<int> all_same = {5, 5, 5, 5, 5};

        REQUIRE(all_same.size() == 5);
        REQUIRE(all_same.count(5) == 5);
        REQUIRE(all_same.count(1) == 0);

        auto [first, last] = all_same.equal_range(5);
        REQUIRE(std::distance(first, last) == 5);

        // Erase one occurrence
        all_same.erase(all_same.begin());
        REQUIRE(all_same.count(5) == 4);

        // Erase all occurrences
        all_same.erase(5);
        REQUIRE(all_same.empty());
    }

    SECTION("Custom comparator with duplicates") {
        j::multiset<int, std::greater<int>> desc_ms = {3, 1, 4, 1, 5, 3};

        std::vector<int> result;
        for (const auto& val : desc_ms) {
            result.push_back(val);
        }

        // Should be in descending order with duplicates
        REQUIRE(std::is_sorted(result.begin(), result.end(), std::greater<int>()));
        REQUIRE(desc_ms.count(1) == 2);
        REQUIRE(desc_ms.count(3) == 2);
    }

    SECTION("Node operations with duplicates") {
        j::multiset<int> ms1 = {1, 2, 2, 3};
        j::multiset<int> ms2 = {4, 5};

        // Extract one occurrence
        auto node = ms1.extract(ms1.find(2));
        REQUIRE(!node.empty());
        REQUIRE(node.value() == 2);
        REQUIRE(ms1.count(2) == 1); // One still remains
        REQUIRE(ms1.size() == 3);

        // Insert node
        auto result = ms2.insert(std::move(node));
        REQUIRE(*result == 2);
        REQUIRE(ms2.count(2) == 1);
        REQUIRE(ms2.size() == 3);
    }

    SECTION("Merge with duplicates") {
        j::multiset<int> ms1 = {1, 2, 3, 3};
        j::multiset<int> ms2 = {2, 3, 4, 4};

        auto original_size1 = ms1.size();
        auto original_size2 = ms2.size();

        ms1.merge(ms2);
        REQUIRE(ms1.size() == original_size1 + original_size2);
        REQUIRE(ms2.empty());
        REQUIRE(ms1.count(2) == 2);
        REQUIRE(ms1.count(3) == 3);
        REQUIRE(ms1.count(4) == 2);
    }
}

TEST_CASE("Set Big") {
    SECTION("Large scale insertion and lookup") {
        j::set<int> big_set;

        // Insert many elements
        for (int i = 0; i < N; ++i) {
            big_set.insert(i * 2); // Even numbers
        }

        REQUIRE(big_set.size() == N);

        // Verify all elements exist
        for (int i = 0; i < N; ++i) {
            REQUIRE(big_set.contains(i * 2));
            REQUIRE(!big_set.contains(i * 2 + 1)); // Odd numbers don't exist
        }

        // Performance test - should be fast
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            auto temp = big_set.find(i * 20);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        // Should complete quickly (adjust threshold as needed)
        REQUIRE(duration.count() < 10000); // 10ms
    }

    SECTION("Large scale range operations") {
        j::set<int> big_set;

        // Insert elements
        for (int i = 0; i < N; ++i) {
            big_set.insert(i);
        }

        // Erase middle half
        auto first = big_set.lower_bound(N / 4);
        auto last = big_set.upper_bound(3 * N / 4);
        big_set.erase(first, last);

        REQUIRE(big_set.size() == N / 2 - 1);
        REQUIRE(!big_set.contains(N / 2));
        REQUIRE(big_set.contains(N / 8));
        REQUIRE(big_set.contains(7 * N / 8));
    }

    SECTION("Memory efficiency test") {
        std::vector<j::set<int>> many_sets(100);

        for (auto& s : many_sets) {
            for (int i = 0; i < 100; ++i) {
                s.insert(i);
            }
        }

        // Verify all sets are properly constructed
        for (const auto& s : many_sets) {
            REQUIRE(s.size() == 100);
            REQUIRE(s.contains(50));
        }
    }
}

TEST_CASE("MultiSet Big") {
    SECTION("Large scale with many duplicates") {
        j::multiset<int> big_ms;

        // Insert many duplicates
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < DUPLICATES; ++j) {
                big_ms.insert(i);
            }
        }

        REQUIRE(big_ms.size() == N * DUPLICATES);

        // Verify counts
        for (int i = 0; i < N; ++i) {
            REQUIRE(big_ms.count(i) == DUPLICATES);
        }

        // Test equal_range on large multiset
        auto [first, last] = big_ms.equal_range(N / 2);
        REQUIRE(std::distance(first, last) == DUPLICATES);
    }

    SECTION("Bulk operations performance") {
        j::multiset<int> ms1, ms2;

        // Populate both multisets
        for (int i = 0; i < N; ++i) {
            ms1.insert(i);
            ms1.insert(i); // Duplicate
            ms2.insert(i + N / 2);
            ms2.insert(i + N / 2); // Duplicate
        }

        // Merge operation on large multisets
        auto start_time = std::chrono::high_resolution_clock::now();
        ms1.merge(ms2);
        auto end_time = std::chrono::high_resolution_clock::now();

        REQUIRE(ms2.empty());
        REQUIRE(ms1.size() == 4 * N); // Accounting for overlaps

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        REQUIRE(duration.count() < 100); // Should complete in reasonable time
    }

    SECTION("Iterator stability test") {
        j::multiset<int> ms = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

        std::vector<j::multiset<int>::iterator> iterators;

        // Collect iterators to all elements
        for (auto it = ms.begin(); it != ms.end(); ++it) {
            iterators.push_back(it);
        }

        // Insert more elements
        ms.insert(0);
        ms.insert(5);

        // Original iterators should still be valid (implementation dependent)
        for (auto it : iterators) {
            REQUIRE(it != ms.end());
            REQUIRE((*it >= 1 && *it <= 4));
        }
    }
}
