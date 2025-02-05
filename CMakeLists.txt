cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the project name
project(FireSimulation)

# Find SFML package
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
find_package(OpenCV REQUIRED COMPONENTS core imgproc)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Add the executable
add_executable(test_fire_simulation src/fire.cpp src/fire_engine.cpp)

# Link SFML libraries to the executable
target_link_libraries(test_fire_simulation sfml-graphics sfml-window sfml-system opencv_core opencv_imgproc)
