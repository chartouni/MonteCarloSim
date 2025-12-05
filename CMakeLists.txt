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
    src/StochasticProcess.cpp
    src/Option.cpp
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
add_executable(european_option examples/european_option.cpp)
target_link_libraries(european_option PRIVATE montecarlo)

add_executable(asian_option examples/asian_option.cpp)
target_link_libraries(asian_option PRIVATE montecarlo)

# Install rules
install(TARGETS montecarlo
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
