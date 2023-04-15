cmake_minimum_required(VERSION 3.5)
project(pnp_comparison)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Create the executable and link OpenCV libraries
add_executable(pnp_comparison src/test.cpp)
target_link_libraries(pnp_comparison ${OpenCV_LIBS})