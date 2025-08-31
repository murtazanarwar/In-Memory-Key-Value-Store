# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name and language
project(InMemoryKVStore CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main executable
add_executable(
    kv_app
    main.cpp
    trie.cpp
    kv_store.cpp
)

# Link necessary libraries, std::thread might require pthread
target_link_libraries(kv_app PRIVATE pthread)