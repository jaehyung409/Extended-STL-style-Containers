# C++ Data Structures and Algorithms

### ⚠️ **Caution**
This project is currently a personal project in development and not yet complete. <br>
Optimization and correctness of implementation are not guaranteed. <br>
Contributions and redistribution are politely declined until an initial draft is completed.
Thank you for your understanding.

## Introduction
This package provides various data structures and algorithms implemented in C++20 using concepts and modules.<br> 
The interfaces and behaviors of the data structures are referenced from [cppreference](https://en.cppreference.com/w/) and [GeeksforGeeks](https://www.geeksforgeeks.org/data-structures/).<br>
For the sake of implementation convenience, some internal functions reuse container functions, and minimal optimizations (such as O(1) optimizations) have not been applied.

## Features
- **Modern C++**: Utilizes C++20 features including concepts and modules.
- **Comprehensive**: Includes a wide range of data structures and associated algorithms.
- **Modular Design**: Each data structure and algorithm is encapsulated in its own module for easy maintenance and reusability.
- **Detailed Documentation**: Each module directory contains a README file with detailed information on the interface, complexity, and usage of the provided functions.

## Installation
(Installation steps will be added once the CMake build process is implemented.)

## Usage
To use the data structures and algorithms provided by this package, simply import the necessary modules in your C++ code. <br>
For example:

```cpp
import array;
import default_value;

int main(){
    j::Array<int, 5> arr = {1, 2, 3, 4, 5};
    return 0;
}
```