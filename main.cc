#include <iostream>
#include <Array.h>
#include <Sorted.h>
#include <Unsorted.h>


int main(){
	Array<int,6> arr = {5, 1, 3, 4, 9};
	std::cout << arr.find(2) << std::endl;
	arr.insert(6,1);
	arr.print();
	arr.remove(5);
	arr.print();
	arr.sorting(5);
	arr.print();
	std::cout << arr.sort_find(4,0,arr.size()) << std::endl;
	arr.sort_insert(2);
	arr.print();
	arr.sort_remove(2);
	arr.sort_remove(4);
	arr.print();
	return 0;
}
