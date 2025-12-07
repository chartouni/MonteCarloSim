cmake_minimum_required(VERSION 3.14)
project(MonteCarloSim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -O3 -march=native)
endif()

# Find Threads package for multithreading
find_package(Threads REQUIRED)

# Library source files
set(SOURCES
    src/Engine.cpp
    src/RandomGenerator.cpp
    src/Statistics.cpp
)

# Create library
add_library(montecarlo STATIC ${SOURCES})
target_include_directories(montecarlo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(montecarlo PUBLIC Threads::Threads)

# Examples
add_executable(estimate_pi examples/estimate_pi.cpp)
target_link_libraries(estimate_pi PRIVATE montecarlo)

add_executable(monte_carlo_integration examples/monte_carlo_integration.cpp)
target_link_libraries(monte_carlo_integration PRIVATE montecarlo)

# Install rules
install(TARGETS montecarlo
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
