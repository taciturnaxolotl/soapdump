cmake_minimum_required(VERSION 3.10)
project(soapdump VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(transaction-parser src/transaction-parser.cpp)

# Set compiler flags
target_compile_options(transaction-parser PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O3
)

# Install target
install(TARGETS transaction-parser
    RUNTIME DESTINATION bin
)