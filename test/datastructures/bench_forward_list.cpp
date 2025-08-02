/*
 * @ Created by jaehyung409 on 25. 8. 1.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <forward_list>
#include <algorithm>
#include <random>
#include <vector>
#include <catch2/catch_all.hpp>
import j.forward_list;

constexpr size_t N = 1000;
constexpr std::random_device rd;
constexpr std::mt19937 gen(42); // Fixed seed for reproducibility
constexpr std::uniform_int_distribution<> dis(1, 10000);

TEST_CASE("Forward List Benchmarks: Push Operations") {
    SECTION("Push front comparison") {
        BENCHMARK("j::forward_list push_front") {
            j::forward_list<int> flist;
            for (size_t i = 0; i < N; ++i) {
                flist.push_front(static_cast<int>(i));
            }
            return flist.front();
        };

        BENCHMARK("std::forward_list push_front") {
            std::forward_list<int> flist;
            for (size_t i = 0; i < N; ++i) {
                flist.push_front(static_cast<int>(i));
            }
            return flist.front();
        };
    }

    SECTION("Emplace operations") {
        BENCHMARK("j::forward_list emplace_front") {
            j::forward_list<int> flist;
            for (size_t i = 0; i < N; ++i) {
                flist.emplace_front(static_cast<int>(i));
            }
            return flist.front();
        };

        BENCHMARK("std::forward_list emplace_front") {
            std::forward_list<int> flist;
            for (size_t i = 0; i < N; ++i) {
                flist.emplace_front(static_cast<int>(i));
            }
            return flist.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Insert Operations") {
    SECTION("Insert after at beginning") {
        BENCHMARK("j::forward_list insert_after begin") {
            j::forward_list<int> flist;
            flist.push_front(0);
            auto it = flist.before_begin();
            for (size_t i = 1; i < N; ++i) {
                it = flist.insert_after(it, static_cast<int>(i));
            }
            return flist.front();
        };

        BENCHMARK("std::forward_list insert_after begin") {
            std::forward_list<int> flist;
            flist.push_front(0);
            auto it = flist.before_begin();
            for (size_t i = 1; i < N; ++i) {
                it = flist.insert_after(it, static_cast<int>(i));
            }
            return flist.front();
        };
    }

    SECTION("Insert after multiple elements") {
        BENCHMARK("j::forward_list insert_after multiple") {
            j::forward_list<int> flist;
            flist.push_front(0);
            auto it = flist.before_begin();
            for (size_t i = 0; i < N / 10; ++i) {
                it = flist.insert_after(it, 10, static_cast<int>(i));
            }
            return flist.front();
        };

        BENCHMARK("std::forward_list insert_after multiple") {
            std::forward_list<int> flist;
            flist.push_front(0);
            auto it = flist.before_begin();
            for (size_t i = 0; i < N / 10; ++i) {
                it = flist.insert_after(it, 10, static_cast<int>(i));
            }
            return flist.front();
        };
    }

    SECTION("Emplace after operations") {
        BENCHMARK("j::forward_list emplace_after") {
            j::forward_list<int> flist;
            flist.push_front(0);
            auto it = flist.before_begin();
            for (size_t i = 1; i < N; ++i) {
                it = flist.emplace_after(it, static_cast<int>(i));
            }
            return flist.front();
        };

        BENCHMARK("std::forward_list emplace_after") {
            std::forward_list<int> flist;
            flist.push_front(0);
            auto it = flist.before_begin();
            for (size_t i = 1; i < N; ++i) {
                it = flist.emplace_after(it, static_cast<int>(i));
            }
            return flist.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Erase Operations") {
    SECTION("Erase after from beginning") {
        BENCHMARK("j::forward_list erase_after begin") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            while (!j_flist.empty()) {
                j_flist.pop_front();
            }
            return 1;
        };

        BENCHMARK("std::forward_list erase_after begin") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            while (!std_flist.empty()) {
                std_flist.pop_front();
            }
            return 1;
        };
    }

    SECTION("Erase after from middle") {
        BENCHMARK("j::forward_list erase_after middle") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < 2 * N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            for (size_t i = 0; i < N && !j_flist.empty(); ++i) {
                auto it = j_flist.begin();
                if (std::next(it) != j_flist.end()) {
                    j_flist.erase_after(it);
                }
            }
            return 1;
        };

        BENCHMARK("std::forward_list erase_after middle") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < 2 * N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            for (size_t i = 0; i < N && !std_flist.empty(); ++i) {
                auto it = std_flist.begin();
                if (std::next(it) != std_flist.end()) {
                    std_flist.erase_after(it);
                }
            }
            return 1;
        };
    }

    SECTION("Erase after range") {
        BENCHMARK("j::forward_list erase_after range") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            for (size_t i = 0; i < 10; ++i) {
                auto first = j_flist.begin();
                auto last = j_flist.begin();
                for (size_t j = 0; j < N / 20 && last != j_flist.end(); ++j) {
                    ++last;
                }
                if (first != j_flist.end() && last != j_flist.end()) {
                    j_flist.erase_after(first, last);
                }
            }
            return 1;
        };

        BENCHMARK("std::forward_list erase_after range") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            for (size_t i = 0; i < 10; ++i) {
                auto first = std_flist.begin();
                auto last = std_flist.begin();
                for (size_t j = 0; j < N / 20 && last != std_flist.end(); ++j) {
                    ++last;
                }
                if (first != std_flist.end() && last != std_flist.end()) {
                    std_flist.erase_after(first, last);
                }
            }
            return 1;
        };
    }
}

TEST_CASE("Forward List Benchmarks: Remove Operations") {
    SECTION("Remove specific value") {
        BENCHMARK("j::forward_list remove") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i % 100));
            }
            for (int val = 0; val < 50; ++val) {
                j_flist.remove(val);
            }
            return 1;
        };

        BENCHMARK("std::forward_list remove") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i % 100));
            }
            for (int val = 0; val < 50; ++val) {
                std_flist.remove(val);
            }
            return 1;
        };
    }

    SECTION("Remove if predicate") {
        BENCHMARK("j::forward_list remove_if") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            j_flist.remove_if([](int n) { return n % 2 == 0; });
            return 1;
        };

        BENCHMARK("std::forward_list remove_if") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            std_flist.remove_if([](int n) { return n % 2 == 0; });
            return 1;
        };
    }
}

TEST_CASE("Forward List Benchmarks: Sort Operations") {
    SECTION("Sort random data") {
        BENCHMARK("j::forward_list sort random") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(dis(gen));
            }
            j_flist.sort();
            return j_flist.front();
        };

        BENCHMARK("std::forward_list sort random") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(dis(gen));
            }
            std_flist.sort();
            return std_flist.front();
        };
    }

    SECTION("Sort reverse ordered data") {
        BENCHMARK("j::forward_list sort reverse") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            j_flist.sort();
            return j_flist.front();
        };

        BENCHMARK("std::forward_list sort reverse") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            std_flist.sort();
            return std_flist.front();
        };
    }

    SECTION("Sort with custom comparator") {
        BENCHMARK("j::forward_list sort custom") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            j_flist.sort(std::greater<int>());
            return j_flist.front();
        };

        BENCHMARK("std::forward_list sort custom") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            std_flist.sort(std::greater<int>());
            return std_flist.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Splice Operations") {
    SECTION("Splice after entire list") {
        BENCHMARK("j::forward_list splice_after entire") {
            j::forward_list<int> j_flist1, j_flist2;
            for (size_t i = 0; i < N; ++i) {
                j_flist1.push_front(static_cast<int>(i));
                j_flist2.push_front(static_cast<int>(i + N));
            }
            j_flist1.splice_after(j_flist1.before_begin(), j_flist2);
            return j_flist1.front();
        };

        BENCHMARK("std::forward_list splice_after entire") {
            std::forward_list<int> std_flist1, std_flist2;
            for (size_t i = 0; i < N; ++i) {
                std_flist1.push_front(static_cast<int>(i));
                std_flist2.push_front(static_cast<int>(i + N));
            }
            std_flist1.splice_after(std_flist1.before_begin(), std_flist2);
            return std_flist1.front();
        };
    }

    SECTION("Splice after single elements") {
        BENCHMARK("j::forward_list splice_after single") {
            j::forward_list<int> j_flist1, j_flist2;
            for (size_t i = 0; i < N; ++i) {
                j_flist1.push_front(static_cast<int>(i));
                j_flist2.push_front(static_cast<int>(i + N));
            }
            while (!j_flist2.empty()) {
                j_flist1.splice_after(j_flist1.before_begin(), j_flist2, j_flist2.before_begin());
            }
            return j_flist1.front();
        };

        BENCHMARK("std::forward_list splice_after single") {
            std::forward_list<int> std_flist1, std_flist2;
            for (size_t i = 0; i < N; ++i) {
                std_flist1.push_front(static_cast<int>(i));
                std_flist2.push_front(static_cast<int>(i + N));
            }
            while (!std_flist2.empty()) {
                std_flist1.splice_after(std_flist1.before_begin(), std_flist2, std_flist2.before_begin());
            }
            return std_flist1.front();
        };
    }

    SECTION("Splice after ranges") {
        BENCHMARK("j::forward_list splice_after range") {
            j::forward_list<int> j_flist1, j_flist2;
            for (size_t i = 0; i < N; ++i) {
                j_flist1.push_front(static_cast<int>(i));
                j_flist2.push_front(static_cast<int>(i + N));
            }
            for (size_t i = 0; i < 10; ++i) {
                auto first = j_flist2.before_begin();
                auto last = j_flist2.begin();
                for (size_t j = 0; j < N / 10 && last != j_flist2.end(); ++j) {
                    ++last;
                }
                if (std::next(first) != j_flist2.end() && last != j_flist2.end()) {
                    j_flist1.splice_after(j_flist1.before_begin(), j_flist2, first, last);
                }
            }
            return j_flist1.front();
        };

        BENCHMARK("std::forward_list splice_after range") {
            std::forward_list<int> std_flist1, std_flist2;
            for (size_t i = 0; i < N; ++i) {
                std_flist1.push_front(static_cast<int>(i));
                std_flist2.push_front(static_cast<int>(i + N));
            }
            for (size_t i = 0; i < 10; ++i) {
                auto first = std_flist2.before_begin();
                auto last = std_flist2.begin();
                for (size_t j = 0; j < N / 10 && last != std_flist2.end(); ++j) {
                    ++last;
                }
                if (std::next(first) != std_flist2.end() && last != std_flist2.end()) {
                    std_flist1.splice_after(std_flist1.before_begin(), std_flist2, first, last);
                }
            }
            return std_flist1.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Merge Operations") {
    SECTION("Merge sorted lists") {
        BENCHMARK("j::forward_list merge") {
            j::forward_list<int> j_flist1, j_flist2;
            for (size_t i = 0; i < N / 2; ++i) {
                j_flist1.push_front(static_cast<int>(i * 2));
                j_flist2.push_front(static_cast<int>(i * 2 + 1));
            }
            j_flist1.sort();
            j_flist2.sort();
            j_flist1.merge(j_flist2);
            return j_flist1.front();
        };

        BENCHMARK("std::forward_list merge") {
            std::forward_list<int> std_flist1, std_flist2;
            for (size_t i = 0; i < N / 2; ++i) {
                std_flist1.push_front(static_cast<int>(i * 2));
                std_flist2.push_front(static_cast<int>(i * 2 + 1));
            }
            std_flist1.sort();
            std_flist2.sort();
            std_flist1.merge(std_flist2);
            return std_flist1.front();
        };
    }

    SECTION("Merge with custom comparator") {
        BENCHMARK("j::forward_list merge custom") {
            j::forward_list<int> j_flist1, j_flist2;
            for (size_t i = N / 2; i > 0; --i) {
                j_flist1.push_front(static_cast<int>(i * 2));
                j_flist2.push_front(static_cast<int>(i * 2 + 1));
            }
            j_flist1.merge(j_flist2, std::greater<int>());
            return j_flist1.front();
        };

        BENCHMARK("std::forward_list merge custom") {
            std::forward_list<int> std_flist1, std_flist2;
            for (size_t i = N / 2; i > 0; --i) {
                std_flist1.push_front(static_cast<int>(i * 2));
                std_flist2.push_front(static_cast<int>(i * 2 + 1));
            }
            std_flist1.merge(std_flist2, std::greater<int>());
            return std_flist1.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Unique Operations") {
    SECTION("Unique consecutive duplicates") {
        BENCHMARK("j::forward_list unique") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i / 10));
            }
            j_flist.unique();
            return j_flist.front();
        };

        BENCHMARK("std::forward_list unique") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i / 10));
            }
            std_flist.unique();
            return std_flist.front();
        };
    }

    SECTION("Unique with custom predicate") {
        BENCHMARK("j::forward_list unique custom") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) {
                j_flist.push_front(static_cast<int>(i));
            }
            j_flist.unique([](int a, int b) { return (a % 10) == (b % 10); });
            return j_flist.front();
        };

        BENCHMARK("std::forward_list unique custom") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) {
                std_flist.push_front(static_cast<int>(i));
            }
            std_flist.unique([](int a, int b) { return (a % 10) == (b % 10); });
            return std_flist.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Iteration") {
    SECTION("Forward iteration") {
        BENCHMARK("j::forward_list forward iter") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) j_flist.push_front(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = j_flist.begin(); it != j_flist.end(); ++it) sum += *it;
            return sum;
        };

        BENCHMARK("std::forward_list forward iter") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) std_flist.push_front(static_cast<int>(i));
            size_t sum = 0;
            for (auto it = std_flist.begin(); it != std_flist.end(); ++it) sum += *it;
            return sum;
        };
    }

    SECTION("Range-based for loop") {
        BENCHMARK("j::forward_list range-for") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) j_flist.push_front(static_cast<int>(i));
            size_t sum = 0;
            for (const auto& val : j_flist) sum += val;
            return sum;
        };

        BENCHMARK("std::forward_list range-for") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) std_flist.push_front(static_cast<int>(i));
            size_t sum = 0;
            for (const auto& val : std_flist) sum += val;
            return sum;
        };
    }
}

TEST_CASE("Forward List Benchmarks: Memory Operations") {
    SECTION("Construction and destruction") {
        BENCHMARK("j::forward_list construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                j::forward_list<int> flist;
                for (size_t j = 0; j < N / 100; ++j) flist.push_front(static_cast<int>(j));
            }
            return 1;
        };

        BENCHMARK("std::forward_list construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                std::forward_list<int> flist;
                for (size_t j = 0; j < N / 100; ++j) flist.push_front(static_cast<int>(j));
            }
            return 1;
        };
    }

    SECTION("Copy operations") {
        BENCHMARK("j::forward_list copy") {
            j::forward_list<int> j_original;
            for (size_t i = 0; i < N; ++i) j_original.push_front(static_cast<int>(i));
            j::forward_list<int> copy = j_original;
            return copy.front();
        };

        BENCHMARK("std::forward_list copy") {
            std::forward_list<int> std_original;
            for (size_t i = 0; i < N; ++i) std_original.push_front(static_cast<int>(i));
            std::forward_list<int> copy = std_original;
            return copy.front();
        };
    }

    SECTION("Move operations") {
        BENCHMARK("j::forward_list move") {
            j::forward_list<int> j_original;
            for (size_t i = 0; i < N; ++i) j_original.push_front(static_cast<int>(i));
            j::forward_list<int> moved = std::move(j_original);
            return moved.front();
        };

        BENCHMARK("std::forward_list move") {
            std::forward_list<int> std_original;
            for (size_t i = 0; i < N; ++i) std_original.push_front(static_cast<int>(i));
            std::forward_list<int> moved = std::move(std_original);
            return moved.front();
        };
    }

    SECTION("Resize operations") {
        BENCHMARK("j::forward_list resize") {
            j::forward_list<int> j_flist;
            j_flist.resize(N, 42);
            return j_flist.front();
        };

        BENCHMARK("std::forward_list resize") {
            std::forward_list<int> std_flist;
            std_flist.resize(N, 42);
            return std_flist.front();
        };
    }
}

TEST_CASE("Forward List Benchmarks: Reverse Operations") {
    SECTION("Reverse large list") {
        BENCHMARK("j::forward_list reverse") {
            j::forward_list<int> j_flist;
            for (size_t i = 0; i < N; ++i) j_flist.push_front(static_cast<int>(i));
            j_flist.reverse();
            return j_flist.front();
        };

        BENCHMARK("std::forward_list reverse") {
            std::forward_list<int> std_flist;
            for (size_t i = 0; i < N; ++i) std_flist.push_front(static_cast<int>(i));
            std_flist.reverse();
            return std_flist.front();
        };
    }
}
