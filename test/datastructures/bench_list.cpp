/*
 * @ Created by jaehyung409 on 25. 7. 31.
 * @ Copyright (c) 2025 jaehyung409.
 * This software is licensed under the MIT License.
 * Note: This test code was generated with the assistance of an AI agent.
 */

#define CATCH_CONFIG_MAIN

#include <list>
#include <algorithm>
#include <random>
#include <vector>
#include <catch2/catch_all.hpp>
import j.list;

constexpr size_t N = 1000;

TEST_CASE("List Benchmarks: Push Operations") {
    SECTION("Push back comparison") {
        BENCHMARK("j::list push_back") {
            j::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };

        BENCHMARK("std::list push_back") {
            std::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.push_back(static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };
    }

    SECTION("Push front comparison") {
        BENCHMARK("j::list push_front") {
            j::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.push_front(static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };

        BENCHMARK("std::list push_front") {
            std::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.push_front(static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };
    }

    SECTION("Emplace operations") {
        BENCHMARK("j::list emplace_back") {
            j::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.emplace_back(static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };

        BENCHMARK("std::list emplace_back") {
            std::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.emplace_back(static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };
    }
}

TEST_CASE("List Benchmarks: Insert Operations") {
    SECTION("Insert at beginning") {
        BENCHMARK("j::list insert begin") {
            j::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.insert(lst.begin(), static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };

        BENCHMARK("std::list insert begin") {
            std::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.insert(lst.begin(), static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };
    }

    SECTION("Insert at middle") {
        j::list<int> lst1;
        for (size_t i = 0; i < 100; ++i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list insert middle") {
            for (size_t i = 0; i < N; ++i) {
                auto it = lst1.begin();
                std::advance(it, lst1.size() / 2);
                lst1.insert(it, 999);
            }
            return lst1.size() + lst1.front() + lst1.back();
        };

        std::list<int> lst2;
        for (size_t i = 0; i < 100; ++i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list insert middle") {
            for (size_t i = 0; i < N; ++i) {
                auto it = lst2.begin();
                std::advance(it, lst2.size() / 2);
                lst2.insert(it, 999);
            }
            return lst2.size() + lst2.front() + lst2.back();
        };
    }

    SECTION("Insert multiple elements") {
        BENCHMARK("j::list insert multiple") {
            j::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.insert(lst.end(), 10, static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };

        BENCHMARK("std::list insert multiple") {
            std::list<int> lst;
            for (size_t i = 0; i < N; ++i) {
                lst.insert(lst.end(), 10, static_cast<int>(i));
            }
            return lst.size() + lst.front() + lst.back();
        };
    }
}

TEST_CASE("List Benchmarks: Erase Operations") {
    SECTION("Erase from beginning") {
        j::list<int> lst1;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list erase begin") {
            while (!lst1.empty()) {
                lst1.erase(lst1.begin());
            }
            return lst1.size();
        };
        std::list<int> lst2;
        for (size_t i = 0; i < N; ++i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list erase begin") {
            while (!lst2.empty()) {
                lst2.erase(lst2.begin());
            }
            return lst2.size();
        };
    }

    SECTION("Erase from middle") {
        j::list<int> lst1;
        for (size_t i = 0; i < 2 * N; ++i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list erase middle") {
            for (size_t i = 0; i < N && !lst1.empty(); ++i) {
                auto it = lst1.begin();
                std::advance(it, lst1.size() / 2);
                lst1.erase(it);
            }
            return lst1.size();
        };
        std::list<int> lst2;
        for (size_t i = 0; i < 2 * N; ++i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list erase middle") {
            for (size_t i = 0; i < N && !lst2.empty(); ++i) {
                auto it = lst2.begin();
                std::advance(it, lst2.size() / 2);
                lst2.erase(it);
            }
            return lst2.size();
        };
    }

    SECTION("Erase range") {
        j::list<int> lst1;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list erase range") {
            for (size_t i = 0; i < 10 && lst1.size() >= N / 10; ++i) {
                auto first = lst1.begin();
                auto last = lst1.begin();
                std::advance(last, N / 10);
                lst1.erase(first, last);
            }
            return lst1.size();
        };

        std::list<int> lst2;
        for (size_t i = 0; i < N; ++i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list erase range (100)") {
            for (size_t i = 0; i < 10 && lst2.size() >= N / 10; ++i) {
                auto first = lst2.begin();
                auto last = lst2.begin();
                std::advance(last, N / 10);
                lst2.erase(first, last);
            }
            return lst2.size();
        };
    }
}

TEST_CASE("List Benchmarks: Remove Operations") {
    SECTION("Remove specific value") {
        j::list<int> lst1;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i % 100));
        }
        BENCHMARK("j::list remove") {
            for (int val = 0; val < 50; ++val) {
                lst1.remove(val);
            }
            return lst1.size();
        };

        std::list<int> lst2;
        for (size_t i = 0; i < N; ++i) {
            lst2.push_back(static_cast<int>(i % 100));
        }
        BENCHMARK("std::list remove") {
            for (int val = 0; val < 50; ++val) {
                lst2.remove(val);
            }
            return lst2.size();
        };
    }

    SECTION("Remove if predicate") {
        j::list<int> lst1;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list remove_if") {
            lst1.remove_if([](int n) { return n % 2 == 0; });
            return lst1.size();
        };

        std::list<int> lst2;
        for (size_t i = 0; i < N; ++i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list remove_if") {
            lst2.remove_if([](int n) { return n % 2 == 0; });
            return lst2.size();
        };
    }
}

TEST_CASE("List Benchmarks: Sort Operations") {
    SECTION("Sort random data") {
        j::list<int> lst1;
        std::random_device rd;
        std::mt19937 gen(42); // Fixed seed for reproducibility
        std::uniform_int_distribution<> dis(1, 10000);
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(dis(gen));
        }

        BENCHMARK("j::list sort random") {
            lst1.sort();
            return lst1.size();
        };

        std::list<int> lst2;
        for (size_t i = 0; i < N; ++i) {
            lst2.push_back(dis(gen));
        }
        BENCHMARK("std::list sort random") {
            lst2.sort();
            return lst2.size();
        };
    }

    SECTION("Sort reverse ordered data") {
        j::list<int> lst1;
        for (size_t i = N; i > 0; --i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list sort reverse") {
            lst1.sort();
            return lst1.size();
        };

        std::list<int> lst2;
        for (size_t i = N; i > 0; --i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list sort reverse") {
            lst2.sort();
            return lst2.size();
        };
    }

    SECTION("Sort with custom comparator") {
        j::list<int> lst1;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list sort custom") {
            lst1.sort(std::greater<int>());
            return lst1.size();
        };

        std::list<int> lst2;
        for (size_t i = 0; i < N; ++i) {
            lst2.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list sort custom") {
            lst2.sort(std::greater<int>());
            return lst2.size();
        };
    }
}

TEST_CASE("List Benchmarks: Splice Operations") {
    SECTION("Splice entire list") {
        j::list<int> lst1, lst2;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
            lst2.push_back(static_cast<int>(i + N));
        }
        BENCHMARK("j::list splice entire") {
            lst1.splice(lst1.end(), lst2);
            return lst1.size();
        };

        std::list<int> lst3, lst4;
        for (size_t i = 0; i < N; ++i) {
            lst3.push_back(static_cast<int>(i));
            lst4.push_back(static_cast<int>(i + N));
        }
        BENCHMARK("std::list splice entire") {
            lst3.splice(lst3.end(), lst4);
            return lst3.size();
        };
    }

    SECTION("Splice single elements") {
        j::list<int> lst1, lst2;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
            lst2.push_back(static_cast<int>(i + N));
        }
        BENCHMARK("j::list splice single") {
            while (!lst2.empty()) {
                lst1.splice(lst1.end(), lst2, lst2.begin());
            }
            return lst1.size();
        };

        std::list<int> lst3, lst4;
        for (size_t i = 0; i < N; ++i) {
            lst3.push_back(static_cast<int>(i));
            lst4.push_back(static_cast<int>(i + N));
        }
        BENCHMARK("std::list splice single") {
            while (!lst4.empty()) {
                lst3.splice(lst3.end(), lst4, lst4.begin());
            }
            return lst3.size();
        };
    }

    SECTION("Splice ranges") {
        j::list<int> lst1, lst2;
        for (size_t i = 0; i < N; ++i) {
            lst1.push_back(static_cast<int>(i));
            lst2.push_back(static_cast<int>(i + N));
        }
        BENCHMARK("j::list splice range") {
            for (size_t i = 0; i < 10 && lst2.size() >= N / 10; ++i) {
                auto first = lst2.begin();
                auto last = lst2.begin();
                std::advance(last, N / 10);
                lst1.splice(lst1.end(), lst2, first, last);
            }
            return lst1.size();
        };

        std::list<int> lst3, lst4;
        for (size_t i = 0; i < N; ++i) {
            lst3.push_back(static_cast<int>(i));
            lst4.push_back(static_cast<int>(i + N));
        }
        BENCHMARK("std::list splice range") {
            for (size_t i = 0; i < 10 && lst4.size() >= N / 10; ++i) {
                auto first = lst4.begin();
                auto last = lst4.begin();
                std::advance(last, N / 10);
                lst3.splice(lst3.end(), lst4, first, last);
            }
            return lst3.size();
        };
    }
}

TEST_CASE("List Benchmarks: Merge Operations") {
    SECTION("Merge sorted lists") {
        j::list<int> j_lst1, j_lst2;
        for (size_t i = 0; i < N / 2; ++i) {
            j_lst1.push_back(static_cast<int>(i * 2));
            j_lst2.push_back(static_cast<int>(i * 2 + 1));
        }
        BENCHMARK("j::list merge") {
            j_lst1.merge(j_lst2);
            return j_lst1.size();
        };

        std::list<int> std_lst1, std_lst2;
        for (size_t i = 0; i < N / 2; ++i) {
            std_lst1.push_back(static_cast<int>(i * 2));
            std_lst2.push_back(static_cast<int>(i * 2 + 1));
        }
        BENCHMARK("std::list merge") {
            std_lst1.merge(std_lst2);
            return std_lst1.size();
        };
    }

    SECTION("Merge with custom comparator") {
        j::list<int> j_lst1, j_lst2;
        for (size_t i = N / 2; i > 0; --i) {
            j_lst1.push_back(static_cast<int>(i * 2));
            j_lst2.push_back(static_cast<int>(i * 2 + 1));
        }
        BENCHMARK("j::list merge custom") {
            j_lst1.merge(j_lst2, std::greater<int>());
            return j_lst1.size();
        };

        std::list<int> std_lst1, std_lst2;
        for (size_t i = N / 2; i > 0; --i) {
            std_lst1.push_back(static_cast<int>(i * 2));
            std_lst2.push_back(static_cast<int>(i * 2 + 1));
        }
        BENCHMARK("std::list merge custom") {
            std_lst1.merge(std_lst2, std::greater<int>());
            return std_lst1.size();
        };
    }
}

TEST_CASE("List Benchmarks: Unique Operations") {
    SECTION("Unique consecutive duplicates") {
        j::list<int> j_lst;
        for (size_t i = 0; i < N; ++i) {
            j_lst.push_back(static_cast<int>(i / 10));
        }
        BENCHMARK("j::list unique") {
            j_lst.unique();
            return j_lst.size();
        };

        std::list<int> std_lst;
        for (size_t i = 0; i < N; ++i) {
            std_lst.push_back(static_cast<int>(i / 10));
        }
        BENCHMARK("std::list unique") {
            std_lst.unique();
            return std_lst.size();
        };
    }

    SECTION("Unique with custom predicate") {
        j::list<int> j_lst;
        for (size_t i = 0; i < N; ++i) {
            j_lst.push_back(static_cast<int>(i));
        }
        BENCHMARK("j::list unique custom") {
            j_lst.unique([](int a, int b) { return (a % 10) == (b % 10); });
            return j_lst.size();
        };

        std::list<int> std_lst;
        for (size_t i = 0; i < N; ++i) {
            std_lst.push_back(static_cast<int>(i));
        }
        BENCHMARK("std::list unique custom") {
            std_lst.unique([](int a, int b) { return (a % 10) == (b % 10); });
            return std_lst.size();
        };
    }
}

TEST_CASE("List Benchmarks: Iteration") {
    SECTION("Forward iteration") {
        j::list<int> j_lst;
        for (size_t i = 0; i < N; ++i) j_lst.push_back(static_cast<int>(i));
        BENCHMARK("j::list forward iter") {
            size_t sum = 0;
            for (auto it = j_lst.begin(); it != j_lst.end(); ++it) sum += *it;
            return sum;
        };

        std::list<int> std_lst;
        for (size_t i = 0; i < N; ++i) std_lst.push_back(static_cast<int>(i));
        BENCHMARK("std::list forward iter") {
            size_t sum = 0;
            for (auto it = std_lst.begin(); it != std_lst.end(); ++it) sum += *it;
            return sum;
        };
    }

    SECTION("Reverse iteration") {
        j::list<int> j_lst;
        for (size_t i = 0; i < N; ++i) j_lst.push_back(static_cast<int>(i));
        BENCHMARK("j::list reverse iter") {
            size_t sum = 0;
            for (auto it = j_lst.rbegin(); it != j_lst.rend(); ++it) sum += *it;
            return sum;
        };

        std::list<int> std_lst;
        for (size_t i = 0; i < N; ++i) std_lst.push_back(static_cast<int>(i));
        BENCHMARK("std::list reverse iter") {
            size_t sum = 0;
            for (auto it = std_lst.rbegin(); it != std_lst.rend(); ++it) sum += *it;
            return sum;
        };
    }

    SECTION("Range-based for loop") {
        j::list<int> j_lst;
        for (size_t i = 0; i < N; ++i) j_lst.push_back(static_cast<int>(i));
        BENCHMARK("j::list range-for") {
            size_t sum = 0;
            for (const auto& val : j_lst) sum += val;
            return sum;
        };

        std::list<int> std_lst;
        for (size_t i = 0; i < N; ++i) std_lst.push_back(static_cast<int>(i));
        BENCHMARK("std::list range-for") {
            size_t sum = 0;
            for (const auto& val : std_lst) sum += val;
            return sum;
        };
    }
}

TEST_CASE("List Benchmarks: Memory Operations") {
    SECTION("Construction and destruction") {
        BENCHMARK("j::list construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                j::list<int> lst;
                for (size_t j = 0; j < N / 100; ++j) lst.push_back(static_cast<int>(j));
            }
            return 1;
        };

        BENCHMARK("std::list construct/destruct") {
            for (size_t i = 0; i < 100; ++i) {
                std::list<int> lst;
                for (size_t j = 0; j < N / 100; ++j) lst.push_back(static_cast<int>(j));
            }
            return 1;
        };
    }

    SECTION("Copy operations") {
        j::list<int> j_original;
        for (size_t i = 0; i < N; ++i) j_original.push_back(static_cast<int>(i));
        BENCHMARK("j::list copy") {
            j::list<int> copy = j_original;
            return copy.size();
        };

        std::list<int> std_original;
        for (size_t i = 0; i < N; ++i) std_original.push_back(static_cast<int>(i));
        BENCHMARK("std::list copy") {
            std::list<int> copy = std_original;
            return copy.size();
        };
    }

    SECTION("Move operations") {
        j::list<int> j_original;
        for (size_t i = 0; i < N; ++i) j_original.push_back(static_cast<int>(i));
        BENCHMARK("j::list move") {
            j::list<int> moved = std::move(j_original);
            return moved.size();
        };

        std::list<int> std_original;
        for (size_t i = 0; i < N; ++i) std_original.push_back(static_cast<int>(i));
        BENCHMARK("std::list move") {
            std::list<int> moved = std::move(std_original);
            return moved.size();
        };
    }
}
