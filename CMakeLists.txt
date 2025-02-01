cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the project name
project(FireSimulation)

# Find SFML package
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Add the executable
add_executable(fire_simulation src/fire.cpp src/fire_engine.cpp)

# Link SFML libraries to the executable
target_link_libraries(fire_simulation sfml-graphics sfml-window sfml-system)
