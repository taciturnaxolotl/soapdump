cmake_minimum_required(VERSION 3.10)
project(soapdump VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable
add_executable(soapdump src/soapdump.cpp)

# Set compiler flags
target_compile_options(soapdump PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O3
)

# Install target
install(TARGETS soapdump
    RUNTIME DESTINATION bin
)