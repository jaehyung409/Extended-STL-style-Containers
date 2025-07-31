/*
 * @ Created by jaehyung409 on 25. 7. 31.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <list>
#include <algorithm>
#include <random>
#include <vector>
import j.list;

constexpr size_t SMALL_N = 1000;
constexpr size_t MEDIUM_N = 10000;
constexpr size_t LARGE_N = 100000;

TEST_CASE("List Benchmarks: Push Operations") {
    SECTION("Push back comparison") {
        BENCHMARK("j::list push_back (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            return lst.size();
        };

        BENCHMARK("std::list push_back (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            return lst.size();
        };
    }

    SECTION("Push front comparison") {
        BENCHMARK("j::list push_front (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_front(static_cast<int>(i));
            }
            return lst.size();
        };

        BENCHMARK("std::list push_front (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_front(static_cast<int>(i));
            }
            return lst.size();
        };
    }

    SECTION("Emplace operations") {
        BENCHMARK("j::list emplace_back (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.emplace_back(static_cast<int>(i));
            }
            return lst.size();
        };

        BENCHMARK("std::list emplace_back (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.emplace_back(static_cast<int>(i));
            }
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Insert Operations") {
    SECTION("Insert at beginning") {
        BENCHMARK("j::list insert begin (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.insert(lst.begin(), static_cast<int>(i));
            }
            return lst.size();
        };

        BENCHMARK("std::list insert begin (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.insert(lst.begin(), static_cast<int>(i));
            }
            return lst.size();
        };
    }

    SECTION("Insert at middle") {
        BENCHMARK("j::list insert middle (100)") {
            j::list<int> lst;
            for (size_t i = 0; i < 100; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 100; ++i) {
                auto it = lst.begin();
                std::advance(it, lst.size() / 2);
                lst.insert(it, 999);
            }
            return lst.size();
        };

        BENCHMARK("std::list insert middle (100)") {
            std::list<int> lst;
            for (size_t i = 0; i < 100; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 100; ++i) {
                auto it = lst.begin();
                std::advance(it, lst.size() / 2);
                lst.insert(it, 999);
            }
            return lst.size();
        };
    }

    SECTION("Insert multiple elements") {
        BENCHMARK("j::list insert multiple (100)") {
            j::list<int> lst;
            for (size_t i = 0; i < 100; ++i) {
                lst.insert(lst.end(), 10, static_cast<int>(i));
            }
            return lst.size();
        };

        BENCHMARK("std::list insert multiple (100)") {
            std::list<int> lst;
            for (size_t i = 0; i < 100; ++i) {
                lst.insert(lst.end(), 10, static_cast<int>(i));
            }
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Erase Operations") {
    SECTION("Erase from beginning") {
        BENCHMARK("j::list erase begin (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            while (!lst.empty()) {
                lst.erase(lst.begin());
            }
            return lst.size();
        };

        BENCHMARK("std::list erase begin (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            while (!lst.empty()) {
                lst.erase(lst.begin());
            }
            return lst.size();
        };
    }

    SECTION("Erase from middle") {
        BENCHMARK("j::list erase middle (500)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 500 && !lst.empty(); ++i) {
                auto it = lst.begin();
                std::advance(it, lst.size() / 2);
                lst.erase(it);
            }
            return lst.size();
        };

        BENCHMARK("std::list erase middle (500)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 500 && !lst.empty(); ++i) {
                auto it = lst.begin();
                std::advance(it, lst.size() / 2);
                lst.erase(it);
            }
            return lst.size();
        };
    }

    SECTION("Erase range") {
        BENCHMARK("j::list erase range (100)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 10 && lst.size() > 100; ++i) {
                auto first = lst.begin();
                auto last = lst.begin();
                std::advance(last, 100);
                lst.erase(first, last);
            }
            return lst.size();
        };

        BENCHMARK("std::list erase range (100)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            for (size_t i = 0; i < 10 && lst.size() > 100; ++i) {
                auto first = lst.begin();
                auto last = lst.begin();
                std::advance(last, 100);
                lst.erase(first, last);
            }
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Remove Operations") {
    SECTION("Remove specific value") {
        BENCHMARK("j::list remove (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i % 100));
            }
            for (int val = 0; val < 50; ++val) {
                lst.remove(val);
            }
            return lst.size();
        };

        BENCHMARK("std::list remove (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i % 100));
            }
            for (int val = 0; val < 50; ++val) {
                lst.remove(val);
            }
            return lst.size();
        };
    }

    SECTION("Remove if predicate") {
        BENCHMARK("j::list remove_if (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.remove_if([](int n) { return n % 2 == 0; });
            return lst.size();
        };

        BENCHMARK("std::list remove_if (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.remove_if([](int n) { return n % 2 == 0; });
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Sort Operations") {
    SECTION("Sort random data") {
        BENCHMARK("j::list sort random (1K)") {
            j::list<int> lst;
            std::random_device rd;
            std::mt19937 gen(42); // Fixed seed for reproducibility
            std::uniform_int_distribution<> dis(1, 10000);

            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(dis(gen));
            }
            lst.sort();
            return lst.size();
        };

        BENCHMARK("std::list sort random (1K)") {
            std::list<int> lst;
            std::random_device rd;
            std::mt19937 gen(42); // Fixed seed for reproducibility
            std::uniform_int_distribution<> dis(1, 10000);

            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(dis(gen));
            }
            lst.sort();
            return lst.size();
        };
    }

    SECTION("Sort reverse ordered data") {
        BENCHMARK("j::list sort reverse (1K)") {
            j::list<int> lst;
            for (size_t i = SMALL_N; i > 0; --i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.sort();
            return lst.size();
        };

        BENCHMARK("std::list sort reverse (1K)") {
            std::list<int> lst;
            for (size_t i = SMALL_N; i > 0; --i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.sort();
            return lst.size();
        };
    }

    SECTION("Sort with custom comparator") {
        BENCHMARK("j::list sort custom (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.sort(std::greater<int>());
            return lst.size();
        };

        BENCHMARK("std::list sort custom (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.sort(std::greater<int>());
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Splice Operations") {
    SECTION("Splice entire list") {
        BENCHMARK("j::list splice entire (500)") {
            j::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i));
                lst2.push_back(static_cast<int>(i + 500));
            }
            lst1.splice(lst1.end(), lst2);
            return lst1.size();
        };

        BENCHMARK("std::list splice entire (500)") {
            std::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i));
                lst2.push_back(static_cast<int>(i + 500));
            }
            lst1.splice(lst1.end(), lst2);
            return lst1.size();
        };
    }

    SECTION("Splice single elements") {
        BENCHMARK("j::list splice single (500)") {
            j::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i));
                lst2.push_back(static_cast<int>(i + 500));
            }
            while (!lst2.empty()) {
                lst1.splice(lst1.end(), lst2, lst2.begin());
            }
            return lst1.size();
        };

        BENCHMARK("std::list splice single (500)") {
            std::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i));
                lst2.push_back(static_cast<int>(i + 500));
            }
            while (!lst2.empty()) {
                lst1.splice(lst1.end(), lst2, lst2.begin());
            }
            return lst1.size();
        };
    }

    SECTION("Splice ranges") {
        BENCHMARK("j::list splice range (100)") {
            j::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i));
                lst2.push_back(static_cast<int>(i + 500));
            }
            for (size_t i = 0; i < 5 && lst2.size() >= 100; ++i) {
                auto first = lst2.begin();
                auto last = lst2.begin();
                std::advance(last, 100);
                lst1.splice(lst1.end(), lst2, first, last);
            }
            return lst1.size();
        };

        BENCHMARK("std::list splice range (100)") {
            std::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i));
                lst2.push_back(static_cast<int>(i + 500));
            }
            for (size_t i = 0; i < 5 && lst2.size() >= 100; ++i) {
                auto first = lst2.begin();
                auto last = lst2.begin();
                std::advance(last, 100);
                lst1.splice(lst1.end(), lst2, first, last);
            }
            return lst1.size();
        };
    }
}

TEST_CASE("List Benchmarks: Merge Operations") {
    SECTION("Merge sorted lists") {
        BENCHMARK("j::list merge (500+500)") {
            j::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i * 2));     // Even numbers
                lst2.push_back(static_cast<int>(i * 2 + 1)); // Odd numbers
            }
            lst1.merge(lst2);
            return lst1.size();
        };

        BENCHMARK("std::list merge (500+500)") {
            std::list<int> lst1, lst2;
            for (size_t i = 0; i < 500; ++i) {
                lst1.push_back(static_cast<int>(i * 2));     // Even numbers
                lst2.push_back(static_cast<int>(i * 2 + 1)); // Odd numbers
            }
            lst1.merge(lst2);
            return lst1.size();
        };
    }

    SECTION("Merge with custom comparator") {
        BENCHMARK("j::list merge custom (500+500)") {
            j::list<int> lst1, lst2;
            for (size_t i = 500; i > 0; --i) {
                lst1.push_back(static_cast<int>(i * 2));     // Even numbers, descending
                lst2.push_back(static_cast<int>(i * 2 + 1)); // Odd numbers, descending
            }
            lst1.merge(lst2, std::greater<int>());
            return lst1.size();
        };

        BENCHMARK("std::list merge custom (500+500)") {
            std::list<int> lst1, lst2;
            for (size_t i = 500; i > 0; --i) {
                lst1.push_back(static_cast<int>(i * 2));     // Even numbers, descending
                lst2.push_back(static_cast<int>(i * 2 + 1)); // Odd numbers, descending
            }
            lst1.merge(lst2, std::greater<int>());
            return lst1.size();
        };
    }
}

TEST_CASE("List Benchmarks: Unique Operations") {
    SECTION("Unique consecutive duplicates") {
        BENCHMARK("j::list unique (1K with duplicates)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i / 10)); // Each value repeated 10 times
            }
            lst.unique();
            return lst.size();
        };

        BENCHMARK("std::list unique (1K with duplicates)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i / 10)); // Each value repeated 10 times
            }
            lst.unique();
            return lst.size();
        };
    }

    SECTION("Unique with custom predicate") {
        BENCHMARK("j::list unique custom (1K)") {
            j::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.unique([](int a, int b) { return (a % 10) == (b % 10); });
            return lst.size();
        };

        BENCHMARK("std::list unique custom (1K)") {
            std::list<int> lst;
            for (size_t i = 0; i < SMALL_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            lst.unique([](int a, int b) { return (a % 10) == (b % 10); });
            return lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Iteration") {
    SECTION("Forward iteration") {
        BENCHMARK("j::list forward iter (10K)") {
            j::list<int> lst;
            for (size_t i = 0; i < MEDIUM_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }

            size_t sum = 0;
            for (auto it = lst.begin(); it != lst.end(); ++it) {
                sum += *it;
            }
            return sum;
        };

        BENCHMARK("std::list forward iter (10K)") {
            std::list<int> lst;
            for (size_t i = 0; i < MEDIUM_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }

            size_t sum = 0;
            for (auto it = lst.begin(); it != lst.end(); ++it) {
                sum += *it;
            }
            return sum;
        };
    }

    SECTION("Reverse iteration") {
        BENCHMARK("j::list reverse iter (10K)") {
            j::list<int> lst;
            for (size_t i = 0; i < MEDIUM_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }

            size_t sum = 0;
            for (auto it = lst.rbegin(); it != lst.rend(); ++it) {
                sum += *it;
            }
            return sum;
        };

        BENCHMARK("std::list reverse iter (10K)") {
            std::list<int> lst;
            for (size_t i = 0; i < MEDIUM_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }

            size_t sum = 0;
            for (auto it = lst.rbegin(); it != lst.rend(); ++it) {
                sum += *it;
            }
            return sum;
        };
    }

    SECTION("Range-based for loop") {
        BENCHMARK("j::list range-for (10K)") {
            j::list<int> lst;
            for (size_t i = 0; i < MEDIUM_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }

            size_t sum = 0;
            for (const auto& val : lst) {
                sum += val;
            }
            return sum;
        };

        BENCHMARK("std::list range-for (10K)") {
            std::list<int> lst;
            for (size_t i = 0; i < MEDIUM_N; ++i) {
                lst.push_back(static_cast<int>(i));
            }

            size_t sum = 0;
            for (const auto& val : lst) {
                sum += val;
            }
            return sum;
        };
    }
}

TEST_CASE("List Benchmarks: Memory Operations") {
    SECTION("Construction and destruction") {
        BENCHMARK("j::list construct/destruct (1K)") {
            for (size_t i = 0; i < 100; ++i) {
                j::list<int> lst;
                for (size_t j = 0; j < SMALL_N / 100; ++j) {
                    lst.push_back(static_cast<int>(j));
                }
            }
            return 1;
        };

        BENCHMARK("std::list construct/destruct (1K)") {
            for (size_t i = 0; i < 100; ++i) {
                std::list<int> lst;
                for (size_t j = 0; j < SMALL_N / 100; ++j) {
                    lst.push_back(static_cast<int>(j));
                }
            }
            return 1;
        };
    }

    SECTION("Copy operations") {
        BENCHMARK("j::list copy (1K)") {
            j::list<int> original;
            for (size_t i = 0; i < SMALL_N; ++i) {
                original.push_back(static_cast<int>(i));
            }

            j::list<int> copy = original;
            return copy.size();
        };

        BENCHMARK("std::list copy (1K)") {
            std::list<int> original;
            for (size_t i = 0; i < SMALL_N; ++i) {
                original.push_back(static_cast<int>(i));
            }

            std::list<int> copy = original;
            return copy.size();
        };
    }

    SECTION("Move operations") {
        BENCHMARK("j::list move (1K)") {
            j::list<int> original;
            for (size_t i = 0; i < SMALL_N; ++i) {
                original.push_back(static_cast<int>(i));
            }

            j::list<int> moved = std::move(original);
            return moved.size();
        };

        BENCHMARK("std::list move (1K)") {
            std::list<int> original;
            for (size_t i = 0; i < SMALL_N; ++i) {
                original.push_back(static_cast<int>(i));
            }

            std::list<int> moved = std::move(original);
            return moved.size();
        };
    }
}
