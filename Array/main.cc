#include <iostream>
#include "Array.h"
#include "Sorted.h"
#include "Unsorted.h"
#include "Reverse.h"
#include "algorithms.h"


int main(){
	Array<int,6> arr = {1, 4, 45, 6, 10, -8};
    arr.print();
    std::cout << arr.pair_sum_unsorted(16) << '\n';
    std::cout << arr.pair_sum_unsorted_using_remainders(16) << '\n';
    arr.sorting(arr.size());
    std::cout << arr.pair_sum_sorted_binary_search(16) << '\n';
    std::cout << arr.pair_sum_sorted_hashing(16) << '\n';
    std::cout << arr.pair_sum_sorted_two_point(16) << '\n';
    arr.get_leader();
	return 0;
}
