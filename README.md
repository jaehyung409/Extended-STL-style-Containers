# Modern C++ Data Structures with STL-Compatible Interface

A comprehensive C++20 data structures library featuring modern module system integration, smart pointer-based RAII design, and STL-compatible interfaces. This project aims to provide high-performance, exception-safe data structures while maintaining code clarity and simplicity.

## 📑 Table of Contents
- [Goals](#-project-goals)
- [Features](#-key-features)
- [Getting Started](#-getting-started)
- [Usage Examples](#-usage-examples)
- [Development Guidelines](#-development-guidelines)
- [Implementation Status](#-implementation-status)
- [License](#-license)
- [Acknowledgments](#-acknowledgments)

## 🎯 Project Goals

- **STL Compatibility**: Provide interfaces that seamlessly integrate with standard C++ code
- **Modern C++20**: Leverage concepts and modules for type safety and modularity
- **Exception Safety**: Implement smart pointer-based RAII for STL-level exception safety
- **Performance**: Optimize through comprehensive benchmarking against standard library implementations
- **Simplicity**: Remove complex SCARY details while maintaining functionality
- **Flexibility**: Support multiple underlying data structure implementations through tree selectors
- **Production Ready**: Designed for deployment with comprehensive testing and quality assurance

## ✨ Key Features

### 🚀 Modern C++20 Integration
- **Module System**: Full C++20 module support with `import j;` syntax
- **Concepts**: Type-safe template constraints for compile-time error checking
- **Smart Pointer Design**: RAII-based memory management throughout the library

### 🌳 Flexible Tree Selection
Currently implementing a tree selector system that allows runtime switching of underlying data structures:
```cpp
// Current implementation (Skip List based)
j::set<int> s = {1, 2, 3, 4, 5};

// Future capabilities (Tree Selector)
j::set<int, j::use_skip_list> s_skip;     // Skip List implementation
// j::set<int, j::use_red_black_tree> s_rb;  // Red-Black Tree (coming soon)
// j::set<int, j::use_avl_tree> s_avl;       // AVL Tree (coming soon)
```

### 📊 Performance Benchmarking
Comprehensive benchmarking suite comparing against standard library implementations:
- Catch2-based benchmarking framework
- Performance metrics for insert, erase, find, and iteration operations
- Large-scale operation testing with various data patterns
- Memory usage and allocation efficiency analysis

### 🛡️ Exception Safety
- **STL-Standard Compliance**: Exception safety guarantees follow C++ STL development standards
- **RAII Principles**: Smart pointer-based resource management
- **No Memory Leaks**: Valgrind-tested memory safety

## 🚀 Getting Started

### Prerequisites
- **C++20 compliant compiler**: GCC 15+ or Clang 20+
- **CMake 3.28+**
- **Ninja build system** (recommended)
- **Docker** (for pre-configured build environment)

### Quick Start with Docker (Recommended)

The pre-built Docker image includes all necessary dependencies and build tools:
```bash
# Pull the pre-built Docker Hub image
docker pull extended-stl-style-containers/cpp-dev:latest

# Run with your workspace mounted
docker run -it --rm -v $(pwd):/workspace extended-stl-style-containers/cpp-dev:latest
```

The Docker container provides a complete build environment with all required dependencies pre-configured.

### Building from Source

#### Using GCC 15
```bash
# Create build directory
mkdir build-gcc && cd build-gcc

# Configure with Ninja generator
cmake -G Ninja -DCMAKE_CXX_COMPILER=g++-15 -DCMAKE_BUILD_TYPE=Release ..

# Build
ninja -j$(nproc)

# Run tests
ctest

# Run benchmarks
./bench_set
./bench_map
```

#### Using Clang 20
```bash
# Create build directory
mkdir build-clang && cd build-clang

# Configure with Ninja generator
cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++-20 -DCMAKE_BUILD_TYPE=Release ..

# Build
ninja -j$(nproc)

# Run tests
ctest

# Run benchmarks
./bench_set
./bench_map
```

## 📖 Usage Examples

### Basic Container Operations
```cpp
import j;

int main() {
    // Set operations
    j::set<int> my_set = {3, 1, 4, 1, 5};
    my_set.insert(2);
    my_set.erase(3);
    
    // Map operations  
    j::map<std::string, int> word_counts;
    word_counts["hello"] = 1;
    word_counts["world"] = 2;
    
    // Vector operations
    j::vector<int> vec = {1, 2, 3, 4, 5};
    vec.push_back(6);
    
    return 0;
}
```

## 🔧 Development Guidelines

### Code Style
All code must follow the project's coding standards:

```bash
# Apply clang-format
find . -name "*.cppm" -print0 | xargs -0 clang-format -i
```

### Contributing
1. **Issue First**: Create an issue and assign it to yourself before starting work.
2. **Quality Check**: Ensure code follows `clang-format` and passes all tests (`ctest`).
3. **PR Content**: Clearly explain your **design philosophy**.
4. **Consistency**: Run benchmarks within the provided **Docker environment** to ensure consistent metrics.

### CI/CD Pipeline Requirements
- All code must pass clang-format checks
- Unit tests must achieve 100% pass rate
- Benchmarks must not show performance regression
- Valgrind memory checks must pass
- Code must compile without warnings on both GCC and Clang

## 📈 Implementation Status

### ✅ Completed Components
- **Sequential Containers**: Array, Vector, Deque
- **Linked Lists**: List, Forward List
- **Container Adapters**: Stack, Queue
- **Associative Containers**: Set, MultiSet, Map, MultiMap (Skip List based)

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## 🤝 Acknowledgments

- **Catch2**: For the excellent testing framework
- **W. Pugh (1990)**: "Skip Lists: A Probabilistic Alternative to Balanced Trees"