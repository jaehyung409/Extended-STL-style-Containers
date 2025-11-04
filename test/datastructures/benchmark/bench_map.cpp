/*
 * @ Created by jaehyung409 on 25. 11. 4..
 * @ Copyright (c) 2025 jaehyung409
 * This software is licensed under the MIT License.
 * Note: This benchmark code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <numeric>
#include <algorithm>
#include <map>
#include <random>
import j;

constexpr size_t N = 1000;
std::random_device rd;
std::mt19937 gen(42); // Fixed seed for reproducibility
std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Map Benchmarks: Insert Operations") {
    SECTION("Insert single element") {
        BENCHMARK("j::map insert") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i), static_cast<int>(i * 10)});
            return m.size();
        };
        BENCHMARK("std::map insert") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i), static_cast<int>(i * 10)});
            return m.size();
        };
        BENCHMARK("j::multimap insert") {
            j::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i % 500), static_cast<int>(i * 10)});
            return m.size();
        };
        BENCHMARK("std::multimap insert") {
            std::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i % 500), static_cast<int>(i * 10)});
            return m.size();
        };
    }
    SECTION("Insert with hint") {
        BENCHMARK("j::map insert with hint") {
            j::map<int, int> m;
            auto hint = m.end();
            for (size_t i = 0; i < N; ++i) hint = m.insert(hint, {static_cast<int>(i), static_cast<int>(i * 10)});
            return m.size();
        };
        BENCHMARK("std::map insert with hint") {
            std::map<int, int> m;
            auto hint = m.end();
            for (size_t i = 0; i < N; ++i) hint = m.insert(hint, {static_cast<int>(i), static_cast<int>(i * 10)});
            return m.size();
        };
    }
    SECTION("Insert random order") {
        BENCHMARK("j::map insert random") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) {
                int key = dis(gen);
                m.insert({key, key * 10});
            }
            return m.size();
        };
        BENCHMARK("std::map insert random") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) {
                int key = dis(gen);
                m.insert({key, key * 10});
            }
            return m.size();
        };
    }
    SECTION("operator[] insertion") {
        BENCHMARK("j::map operator[]") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            return m.size();
        };
        BENCHMARK("std::map operator[]") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            return m.size();
        };
    }
}

TEST_CASE("Map Benchmarks: Emplace Operations") {
    SECTION("Emplace single element") {
        BENCHMARK("j::map emplace") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.emplace(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("std::map emplace") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.emplace(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("j::multimap emplace") {
            j::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.emplace(static_cast<int>(i % 500), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("std::multimap emplace") {
            std::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.emplace(static_cast<int>(i % 500), static_cast<int>(i * 10));
            return m.size();
        };
    }
    SECTION("Emplace with hint") {
        BENCHMARK("j::map emplace_hint") {
            j::map<int, int> m;
            auto hint = m.end();
            for (size_t i = 0; i < N; ++i) hint = m.emplace_hint(hint, static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("std::map emplace_hint") {
            std::map<int, int> m;
            auto hint = m.end();
            for (size_t i = 0; i < N; ++i) hint = m.emplace_hint(hint, static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
    }
    SECTION("try_emplace") {
        BENCHMARK("j::map try_emplace") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.try_emplace(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("std::map try_emplace") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.try_emplace(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
    }
    SECTION("insert_or_assign") {
        BENCHMARK("j::map insert_or_assign") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert_or_assign(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("std::map insert_or_assign") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert_or_assign(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
    }
}

TEST_CASE("Map Benchmarks: Erase Operations") {
    SECTION("Erase by key") {
        BENCHMARK("j::map erase by key") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            for (size_t i = 0; i < N; ++i) m.erase(static_cast<int>(i));
            return m.size();
        };
        BENCHMARK("std::map erase by key") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            for (size_t i = 0; i < N; ++i) m.erase(static_cast<int>(i));
            return m.size();
        };
        BENCHMARK("j::multimap erase by key") {
            j::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i % 500), static_cast<int>(i * 10)});
            for (size_t i = 0; i < 500; ++i) m.erase(static_cast<int>(i));
            return m.size();
        };
        BENCHMARK("std::multimap erase by key") {
            std::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i % 500), static_cast<int>(i * 10)});
            for (size_t i = 0; i < 500; ++i) m.erase(static_cast<int>(i));
            return m.size();
        };
    }
    SECTION("Erase by iterator") {
        BENCHMARK("j::map erase by iterator") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            while (!m.empty()) m.erase(m.begin());
            return m.size();
        };
        BENCHMARK("std::map erase by iterator") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            while (!m.empty()) m.erase(m.begin());
            return m.size();
        };
    }
    SECTION("Erase range") {
        BENCHMARK("j::map erase range") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            auto it = m.begin();
            std::advance(it, N / 2);
            m.erase(m.begin(), it);
            return m.size();
        };
        BENCHMARK("std::map erase range") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            auto it = m.begin();
            std::advance(it, N / 2);
            m.erase(m.begin(), it);
            return m.size();
        };
    }
}

TEST_CASE("Map Benchmarks: Find Operations") {
    SECTION("Find existing elements") {
        BENCHMARK("j::map find") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (m.find(static_cast<int>(i)) != m.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::map find") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (m.find(static_cast<int>(i)) != m.end()) ++found;
            }
            return found;
        };
    }
    SECTION("Find non-existing elements") {
        BENCHMARK("j::map find missing") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t found = 0;
            for (size_t i = N; i < 2 * N; ++i) {
                if (m.find(static_cast<int>(i)) != m.end()) ++found;
            }
            return found;
        };
        BENCHMARK("std::map find missing") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t found = 0;
            for (size_t i = N; i < 2 * N; ++i) {
                if (m.find(static_cast<int>(i)) != m.end()) ++found;
            }
            return found;
        };
    }
    SECTION("at() access") {
        BENCHMARK("j::map at()") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += m.at(static_cast<int>(i));
            }
            return sum;
        };
        BENCHMARK("std::map at()") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += m.at(static_cast<int>(i));
            }
            return sum;
        };
    }
    SECTION("operator[] access") {
        BENCHMARK("j::map operator[]") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += m[static_cast<int>(i)];
            }
            return sum;
        };
        BENCHMARK("std::map operator[]") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (size_t i = 0; i < N; ++i) {
                sum += m[static_cast<int>(i)];
            }
            return sum;
        };
    }
}

TEST_CASE("Map Benchmarks: Bound Operations") {
    SECTION("lower_bound") {
        BENCHMARK("j::map lower_bound") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i * 2)] = static_cast<int>(i * 10);
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = m.lower_bound(static_cast<int>(i));
                if (it != m.end()) ++count;
            }
            return count;
        };
        BENCHMARK("std::map lower_bound") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i * 2)] = static_cast<int>(i * 10);
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = m.lower_bound(static_cast<int>(i));
                if (it != m.end()) ++count;
            }
            return count;
        };
    }
    SECTION("upper_bound") {
        BENCHMARK("j::map upper_bound") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i * 2)] = static_cast<int>(i * 10);
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = m.upper_bound(static_cast<int>(i));
                if (it != m.end()) ++count;
            }
            return count;
        };
        BENCHMARK("std::map upper_bound") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i * 2)] = static_cast<int>(i * 10);
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto it = m.upper_bound(static_cast<int>(i));
                if (it != m.end()) ++count;
            }
            return count;
        };
    }
    SECTION("equal_range") {
        BENCHMARK("j::map equal_range") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto [first, last] = m.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
        BENCHMARK("std::map equal_range") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t count = 0;
            for (size_t i = 0; i < N; ++i) {
                auto [first, last] = m.equal_range(static_cast<int>(i));
                count += std::distance(first, last);
            }
            return count;
        };
    }
}

TEST_CASE("Map Benchmarks: Iteration") {
    SECTION("Forward iteration") {
        BENCHMARK("j::map forward iteration") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (const auto& [k, v] : m) {
                sum += v;
            }
            return sum;
        };
        BENCHMARK("std::map forward iteration") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (const auto& [k, v] : m) {
                sum += v;
            }
            return sum;
        };
    }
    SECTION("Reverse iteration") {
        BENCHMARK("j::map reverse iteration") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (auto it = m.rbegin(); it != m.rend(); ++it) {
                sum += it->second;
            }
            return sum;
        };
        BENCHMARK("std::map reverse iteration") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            int sum = 0;
            for (auto it = m.rbegin(); it != m.rend(); ++it) {
                sum += it->second;
            }
            return sum;
        };
    }
}

TEST_CASE("Map Benchmarks: Contains Operations") {
    SECTION("Contains existing elements") {
        BENCHMARK("j::map contains") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (m.contains(static_cast<int>(i))) ++found;
            }
            return found;
        };
        BENCHMARK("std::map contains") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t found = 0;
            for (size_t i = 0; i < N; ++i) {
                if (m.contains(static_cast<int>(i))) ++found;
            }
            return found;
        };
    }
}

TEST_CASE("Map Benchmarks: Count Operations") {
    SECTION("Count in map") {
        BENCHMARK("j::map count") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t total = 0;
            for (size_t i = 0; i < N; ++i) total += m.count(static_cast<int>(i));
            return total;
        };
        BENCHMARK("std::map count") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m[static_cast<int>(i)] = static_cast<int>(i * 10);
            size_t total = 0;
            for (size_t i = 0; i < N; ++i) total += m.count(static_cast<int>(i));
            return total;
        };
    }
    SECTION("Count in multimap") {
        BENCHMARK("j::multimap count") {
            j::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i % 100), static_cast<int>(i * 10)});
            size_t total = 0;
            for (size_t i = 0; i < 100; ++i) total += m.count(static_cast<int>(i));
            return total;
        };
        BENCHMARK("std::multimap count") {
            std::multimap<int, int> m;
            for (size_t i = 0; i < N; ++i) m.insert({static_cast<int>(i % 100), static_cast<int>(i * 10)});
            size_t total = 0;
            for (size_t i = 0; i < 100; ++i) total += m.count(static_cast<int>(i));
            return total;
        };
    }
}

TEST_CASE("Map Benchmarks: Special Map Operations") {
    SECTION("try_emplace") {
        BENCHMARK("j::map try_emplace") {
            j::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.try_emplace(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
        BENCHMARK("std::map try_emplace") {
            std::map<int, int> m;
            for (size_t i = 0; i < N; ++i) m.try_emplace(static_cast<int>(i), static_cast<int>(i * 10));
            return m.size();
        };
    }
}
