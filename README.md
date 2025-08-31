# High-Performance In-Memory Key-Value Store

This project is a high-performance, thread-safe in-memory key-value store implemented in modern C++. It uses a **Radix Trie** as its core data structure to efficiently store key-value pairs while also supporting lexicographical ordering operations.

The primary goal of this project is to demonstrate advanced C++ concepts, including multi-threading, modern memory management, and complex data structure implementation.

-----

## \#\# Features ✨

  * **Thread-Safe Operations**: All public APIs—`put`, `get`, and `del`—are fully thread-safe, allowing for concurrent access from multiple threads.
  * **High-Performance Concurrency**: Utilizes a `std::shared_mutex` (reader-writer lock) to allow multiple threads to read data simultaneously, dramatically increasing performance in read-heavy workloads.
  * **Efficient Radix Trie Structure**: Employs a space-optimized Radix Trie, which provides fast lookups and enables unique features like Nth-element searching.
  * **Modern C++ Implementation**: Written in C++17, the project leverages modern features for safety and performance:
      * **RAII & Smart Pointers**: Automatic memory management with `std::unique_ptr` to prevent memory leaks.
      * **RAII-Style Locking**: Exception-safe mutex handling with `std::unique_lock` and `std::shared_lock`.
      * **Performance-Oriented Types**: Use of `std::string_view` to avoid unnecessary string copies in API calls.
  * **Cross-Platform Build System**: Uses **CMake** for easy, cross-platform project configuration and compilation.

-----

## \#\# Core Concepts Demonstrated 🧠

This project is an excellent showcase of the following skills:

  * **Concurrency and Parallelism**:
      * Deep understanding of race conditions and data protection.
      * Implementation of reader-writer locks for performance optimization.
      * Use of the C++17 threading library (`std::thread`, `std::shared_mutex`).
  * **Advanced Data Structures**:
      * Custom implementation of a Radix Trie, including complex operations like node splitting.
      * Understanding of tree traversal and manipulation algorithms.
  * **Modern C++ Best Practices**:
      * Resource Acquisition Is Initialization (RAII) for managing resources like memory and locks.
      * Automatic memory management with smart pointers.
      * Object-Oriented Design with clear separation of concerns (data structure vs. thread-safe wrapper).

-----

## \#\# Project Structure

The codebase is organized to be modular and easy to understand.

```
├── CMakeLists.txt      # The build script for CMake
├── main.cpp            # Example usage and multi-threading demonstration
├── kv_store.hpp        # Header for the public-facing thread-safe KVStore class
├── kv_store.cpp        # Implementation of public-facing thread-safe KVStore class
├── trie.hpp            # Header for the core Trie data structure
└── trie.cpp            # Implementation of the Trie logic
```

-----

## \#\# How to Build and Run 🚀

This project uses CMake, which makes it easy to build on any platform.

### \#\#\# Prerequisites

  * A C++17 compliant compiler (e.g., GCC 7+, Clang 5+, MSVC 2017+).
  * **CMake** (version 3.10 or newer).

### \#\#\# Build Steps

1.  **Clone the repository:**

    ```sh
    git clone <your-repository-url>
    cd <your-repository-name>
    ```

2.  **Create a build directory:**

    ```sh
    mkdir build
    cd build
    ```

3.  **Configure the project with CMake:**

    ```sh
    cmake ..
    ```

4.  **Compile the project:**

    ```sh
    cmake --build .
    ```

    (On Linux/macOS, you can also just run `make` inside the `build` directory).

5.  **Run the application:**
    The compiled executable will be in the `build` directory.

    ```sh
    ./kv_app
    ```
