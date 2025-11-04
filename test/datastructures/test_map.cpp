/*
 * @ Created by jaehyung409 on 25. 11. 4.
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

#define TREE_SELECTOR j::use_skip_list
const int N = 10000;
const int DUPLICATES = 10;

TEST_CASE("Map Basic") {
    j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m;
    j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m_init = {
        {5, 50}, {2, 20}, {8, 80}, {1, 10}, {9, 90}, {3, 30}
    };
    j::map<std::string, int, std::less<std::string>, std::allocator<std::pair<const std::string, int>>, TREE_SELECTOR> m_str = {
        {"apple", 1}, {"banana", 2}, {"cherry", 3}
    };
    j::map<int, std::string, std::less<int>, std::allocator<std::pair<const int, std::string>>, TREE_SELECTOR> m_str_key = {
        {10, "ten"}, {20, "twenty"}, {30, "thirty"}
    };

    SECTION("Construction and Initialization") {
        REQUIRE(m.empty());
        REQUIRE(m.size() == 0);

        REQUIRE(!m_init.empty());
        REQUIRE(m_init.size() == 6);

        // Test deduction guide
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m_deduction = {
            {10, 100}, {20, 200}, {30, 300}
        };
        REQUIRE(m_deduction.size() == 3);
        REQUIRE(m_deduction.contains(20));
    }

    SECTION("Iterator construction") {
        std::vector<std::pair<int, int>> vec = {{4, 40}, {1, 10}, {7, 70}, {2, 20}, {9, 90}};
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m_iter(vec.begin(), vec.end());
        REQUIRE(m_iter.size() == 5);
        REQUIRE(m_iter.contains(4));
        REQUIRE(m_iter.contains(9));
        REQUIRE(m_iter[4] == 40);
    }

    SECTION("Copy and Move construction") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m_copy(m_init);
        REQUIRE(m_copy.size() == m_init.size());
        REQUIRE(m_copy == m_init);

        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m_move(std::move(m_copy));
        REQUIRE(m_move.size() == 6);
        REQUIRE((m_copy.empty() || m_copy.size() == 6)); // Implementation dependent
    }

    SECTION("Element access - at()") {
        REQUIRE(m_init.at(5) == 50);
        REQUIRE(m_init.at(1) == 10);
        REQUIRE_THROWS_AS(m_init.at(100), std::out_of_range);

        m_init.at(5) = 555;
        REQUIRE(m_init.at(5) == 555);
    }

    SECTION("Element access - operator[]") {
        m[10] = 100;
        REQUIRE(m[10] == 100);
        REQUIRE(m.size() == 1);

        m[10] = 200;  // Update existing
        REQUIRE(m[10] == 200);
        REQUIRE(m.size() == 1);

        int val = m[99];  // Default construct
        REQUIRE(val == 0);
        REQUIRE(m.size() == 2);
    }

    SECTION("Insert operations") {
        auto [it1, inserted1] = m.insert({10, 100});
        REQUIRE(inserted1);
        REQUIRE(it1->first == 10);
        REQUIRE(it1->second == 100);
        REQUIRE(m.size() == 1);

        auto [it2, inserted2] = m.insert({10, 999}); // Duplicate key
        REQUIRE(!inserted2);
        REQUIRE(it2->first == 10);
        REQUIRE(it2->second == 100);  // Original value unchanged
        REQUIRE(m.size() == 1);

        // Insert with hint
        auto it3 = m.insert(it1, {15, 150});
        REQUIRE(it3->first == 15);
        REQUIRE(m.size() == 2);
    }

    SECTION("Emplace operations") {
        auto [it1, inserted1] = m_str_key.emplace(42, "answer");
        REQUIRE(inserted1);
        REQUIRE(it1->first == 42);
        REQUIRE(it1->second == "answer");

        auto [it2, inserted2] = m_str_key.emplace(42, "duplicate"); // Duplicate
        REQUIRE(!inserted2);
        REQUIRE(it2->second == "answer");  // Original unchanged

        // Emplace with hint
        auto it3 = m_str_key.emplace_hint(it1, 50, "fifty");
        REQUIRE(it3->first == 50);
    }

    SECTION("try_emplace operations") {
        auto [it1, inserted1] = m_str_key.try_emplace(42, "answer");
        REQUIRE(inserted1);
        REQUIRE(it1->first == 42);
        REQUIRE(it1->second == "answer");

        auto [it2, inserted2] = m_str_key.try_emplace(42, "duplicate");
        REQUIRE(!inserted2);
        REQUIRE(it2->second == "answer");  // Original unchanged

        // try_emplace with hint
        auto [it3, inserted3] = m_str_key.try_emplace(it1, 50, "fifty");
        REQUIRE(inserted3);
        REQUIRE(it3->first == 50);
        REQUIRE(it3->second == "fifty");
    }

    SECTION("insert_or_assign operations") {
        auto [it1, inserted1] = m_str_key.insert_or_assign(42, "answer");
        REQUIRE(inserted1);
        REQUIRE(it1->first == 42);
        REQUIRE(it1->second == "answer");

        auto [it2, inserted2] = m_str_key.insert_or_assign(42, "updated");
        REQUIRE(!inserted2);
        REQUIRE(it2->first == 42);
        REQUIRE(it2->second == "updated");  // Value was updated

        // insert_or_assign with hint
        auto it3 = m_str_key.insert_or_assign(it1, 50, "fifty");
        REQUIRE(it3->first == 50);
    }

    SECTION("Find and Contains") {
        REQUIRE(m_init.contains(5));
        REQUIRE(!m_init.contains(100));

        auto it = m_init.find(8);
        REQUIRE(it != m_init.end());
        REQUIRE(it->first == 8);
        REQUIRE(it->second == 80);

        auto it_not_found = m_init.find(100);
        REQUIRE(it_not_found == m_init.end());
    }

    SECTION("Count operations") {
        REQUIRE(m_init.count(5) == 1);
        REQUIRE(m_init.count(100) == 0);
    }

    SECTION("Bound operations") {
        // lower_bound
        auto lb = m_init.lower_bound(5);
        REQUIRE(lb != m_init.end());
        REQUIRE(lb->first == 5);

        // upper_bound
        auto ub = m_init.upper_bound(5);
        REQUIRE(ub != m_init.end());
        REQUIRE(ub->first > 5);

        // equal_range
        auto [first, last] = m_init.equal_range(5);
        REQUIRE(first != m_init.end());
        REQUIRE(first->first == 5);
        REQUIRE(std::distance(first, last) == 1);
    }

    SECTION("Erase operations") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m_erase = {
            {1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}
        };

        // Erase by key
        auto erased_count = m_erase.erase(3);
        REQUIRE(erased_count == 1);
        REQUIRE(!m_erase.contains(3));
        REQUIRE(m_erase.size() == 4);

        // Erase by iterator
        auto it = m_erase.find(2);
        REQUIRE(it != m_erase.end());
        auto next_it = m_erase.erase(it);
        REQUIRE(!m_erase.contains(2));
        REQUIRE(next_it->first > 2);

        // Erase range
        auto first = m_erase.begin();
        auto last = std::next(first, 2);
        m_erase.erase(first, last);
        REQUIRE(m_erase.size() == 1);
    }

    SECTION("Iterators") {
        std::vector<int> keys;
        for (auto it = m_init.begin(); it != m_init.end(); ++it) {
            keys.push_back(it->first);
        }
        REQUIRE(std::is_sorted(keys.begin(), keys.end()));

        // Reverse iterators
        std::vector<int> reverse_keys;
        for (auto it = m_init.rbegin(); it != m_init.rend(); ++it) {
            reverse_keys.push_back(it->first);
        }
        REQUIRE(std::is_sorted(reverse_keys.rbegin(), reverse_keys.rend()));
    }

    SECTION("Clear and Swap") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m1 = {
            {1, 10}, {2, 20}, {3, 30}
        };
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m2 = {
            {4, 40}, {5, 50}, {6, 60}
        };

        m1.clear();
        REQUIRE(m1.empty());

        m1 = {{7, 70}, {8, 80}, {9, 90}};
        m1.swap(m2);
        REQUIRE(m1.contains(4));
        REQUIRE(m2.contains(7));
    }

    SECTION("Comparison operators") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m1 = {
            {1, 10}, {2, 20}, {3, 30}
        };
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m2 = {
            {1, 10}, {2, 20}, {3, 30}
        };
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m3 = {
            {1, 10}, {2, 20}, {4, 40}
        };

        REQUIRE(m1 == m2);
        REQUIRE(m1 != m3);
        REQUIRE((m1 <=> m3) == std::strong_ordering::less);
    }

    SECTION("Key and Value comparison") {
        auto key_comp = m_init.key_comp();
        auto value_comp = m_init.value_comp();

        REQUIRE(key_comp(1, 2));
        REQUIRE(!key_comp(2, 1));
        REQUIRE(value_comp(std::make_pair(1, 10), std::make_pair(2, 20)));
        REQUIRE(!value_comp(std::make_pair(2, 20), std::make_pair(1, 10)));
    }
}

TEST_CASE("MultiMap Basic") {
    j::multimap<int, std::string, std::less<int>, std::allocator<std::pair<const int, std::string>>, TREE_SELECTOR> mm;
    j::multimap<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> mm_init = {
        {5, 50}, {2, 20}, {8, 80}, {1, 10}, {9, 90}, {3, 30}, {5, 51}, {2, 21}  // With duplicates
    };

    SECTION("Construction and Initialization") {
        REQUIRE(mm.empty());
        REQUIRE(mm.size() == 0);

        REQUIRE(!mm_init.empty());
        REQUIRE(mm_init.size() == 8);  // Including duplicates
    }

    SECTION("Insert operations with duplicates") {
        auto it1 = mm.insert({10, "first"});
        REQUIRE(it1->first == 10);
        REQUIRE(it1->second == "first");
        REQUIRE(mm.size() == 1);

        auto it2 = mm.insert({10, "second"}); // Duplicate key allowed
        REQUIRE(it2->first == 10);
        REQUIRE(it2->second == "second");
        REQUIRE(mm.size() == 2);

        auto it3 = mm.insert({10, "third"}); // Another duplicate
        REQUIRE(it3->first == 10);
        REQUIRE(mm.size() == 3);
    }

    SECTION("Count with duplicates") {
        REQUIRE(mm_init.count(5) == 2);  // Two entries with key 5
        REQUIRE(mm_init.count(2) == 2);  // Two entries with key 2
        REQUIRE(mm_init.count(1) == 1);  // One entry with key 1
        REQUIRE(mm_init.count(100) == 0);
    }

    SECTION("Equal range with duplicates") {
        auto [first, last] = mm_init.equal_range(5);
        REQUIRE(std::distance(first, last) == 2);

        std::vector<int> values;
        for (auto it = first; it != last; ++it) {
            values.push_back(it->second);
        }
        REQUIRE(values.size() == 2);
    }

    SECTION("Erase duplicates") {
        auto count_before = mm_init.count(5);
        REQUIRE(count_before == 2);

        auto erased = mm_init.erase(5);  // Erases all entries with key 5
        REQUIRE(erased == 2);
        REQUIRE(mm_init.count(5) == 0);
    }

    SECTION("Find returns first occurrence") {
        auto it = mm_init.find(5);
        REQUIRE(it != mm_init.end());
        REQUIRE(it->first == 5);
        // Should be one of the two values
        REQUIRE((it->second == 50 || it->second == 51));
    }
}

TEST_CASE("Map Large Scale Operations") {
    SECTION("Insert many elements") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m;
        for (int i = 0; i < N; ++i) {
            m[i] = i * 10;
        }
        REQUIRE(m.size() == N);
        REQUIRE(m[N/2] == (N/2) * 10);
    }

    SECTION("Find many elements") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m;
        for (int i = 0; i < N; ++i) {
            m[i] = i * 10;
        }

        for (int i = 0; i < N; ++i) {
            auto it = m.find(i);
            REQUIRE(it != m.end());
            REQUIRE(it->second == i * 10);
        }
    }

    SECTION("Erase many elements") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m;
        for (int i = 0; i < N; ++i) {
            m[i] = i * 10;
        }

        for (int i = 0; i < N/2; ++i) {
            m.erase(i);
        }
        REQUIRE(m.size() == N/2);
    }
}

TEST_CASE("Map Edge Cases") {
    SECTION("Empty map operations") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> empty_map;

        REQUIRE(empty_map.empty());
        REQUIRE(empty_map.size() == 0);
        REQUIRE(empty_map.begin() == empty_map.end());
        REQUIRE(empty_map.find(1) == empty_map.end());
        REQUIRE(empty_map.count(1) == 0);
        REQUIRE(!empty_map.contains(1));
        REQUIRE_THROWS_AS(empty_map.at(1), std::out_of_range);

        // Operations on empty map should be safe
        empty_map.clear(); // Should not crash
        REQUIRE(empty_map.erase(1) == 0);
    }

    SECTION("Single element map") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> single = {{42, 420}};

        REQUIRE(single.size() == 1);
        REQUIRE(!single.empty());
        REQUIRE(single.contains(42));
        REQUIRE(single[42] == 420);
        REQUIRE(single.begin()->first == 42);

        single.erase(42);
        REQUIRE(single.empty());
    }

    SECTION("Custom comparator") {
        j::map<int, int, std::greater<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> desc_map = {
            {3, 30}, {1, 10}, {4, 40}, {1, 11}, {5, 50}  // Note: duplicate 1 will be ignored
        };

        std::vector<int> keys;
        for (const auto& [k, v] : desc_map) {
            keys.push_back(k);
        }

        // Should be in descending order
        REQUIRE(std::is_sorted(keys.begin(), keys.end(), std::greater<int>()));
        REQUIRE(keys.front() == 5);
        REQUIRE(keys.back() == 1);
    }

    SECTION("Node extraction and insertion") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m1 = {
            {1, 10}, {2, 20}, {3, 30}
        };
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m2 = {
            {4, 40}, {5, 50}, {6, 60}
        };

        // Extract node
        auto node = m1.extract(2);
        REQUIRE(!node.empty());
        REQUIRE(node.key() == 2);
        REQUIRE(node.mapped() == 20);
        REQUIRE(!m1.contains(2));
        REQUIRE(m1.size() == 2);

        // Insert node
        auto result = m2.insert(std::move(node));
        REQUIRE(result.inserted);
        REQUIRE(result.position->first == 2);
        REQUIRE(m2.contains(2));
        REQUIRE(m2.size() == 4);
    }

    SECTION("Merge operations") {
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m1 = {
            {1, 10}, {3, 30}, {5, 50}
        };
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m2 = {
            {2, 20}, {4, 40}, {6, 60}
        };
        j::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> m3 = {
            {1, 11}, {2, 22}, {7, 70}  // Has overlapping keys
        };

        m1.merge(m2);
        REQUIRE(m1.size() == 6);
        REQUIRE(m2.empty());

        auto original_size = m1.size();
        m1.merge(m3);
        REQUIRE(m1.size() == original_size + 1); // Only 7 is new
        REQUIRE(m3.size() == 2); // 1 and 2 remain in m3
        REQUIRE(m1[1] == 10);  // Original value preserved
    }
}

TEST_CASE("MultiMap Edge Cases") {
    SECTION("Empty multimap operations") {
        j::multimap<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> empty_mm;

        REQUIRE(empty_mm.empty());
        REQUIRE(empty_mm.size() == 0);
        REQUIRE(empty_mm.begin() == empty_mm.end());
        REQUIRE(empty_mm.find(1) == empty_mm.end());
        REQUIRE(empty_mm.count(1) == 0);
        REQUIRE(!empty_mm.contains(1));
    }

    SECTION("All same keys") {
        j::multimap<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> all_same = {
            {5, 50}, {5, 51}, {5, 52}, {5, 53}, {5, 54}
        };

        REQUIRE(all_same.size() == 5);
        REQUIRE(all_same.count(5) == 5);

        auto [first, last] = all_same.equal_range(5);
        REQUIRE(std::distance(first, last) == 5);

        std::vector<int> values;
        for (auto it = first; it != last; ++it) {
            values.push_back(it->second);
        }
        REQUIRE(values.size() == 5);
    }

    SECTION("Multimap merge") {
        j::multimap<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> mm1 = {
            {1, 10}, {2, 20}, {3, 30}
        };
        j::multimap<int, int, std::less<int>, std::allocator<std::pair<const int, int>>, TREE_SELECTOR> mm2 = {
            {1, 11}, {2, 21}, {4, 40}
        };

        mm1.merge(mm2);
        REQUIRE(mm1.size() == 6);  // All elements merged (duplicates allowed)
        REQUIRE(mm2.empty());
        REQUIRE(mm1.count(1) == 2);
        REQUIRE(mm1.count(2) == 2);
    }
}
