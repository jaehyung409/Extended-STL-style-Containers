#include <iostream>
#include "Array.h"
#include "Sorted.h"
#include "Unsorted.h"
#include "Reverse.h"
#include "algorithms.h"


int main(){
	Array<int,6> arr = {16, 17, 4, 3, 5, 2};
    arr.print();
    arr.get_leader();
	return 0;
}
