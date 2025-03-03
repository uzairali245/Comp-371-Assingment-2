cmake_minimum_required(VERSION 3.27)
project(assignment2)

set(CMAKE_CXX_STANDARD 14)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include directories for GLFW, GLEW
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# Add executable
add_executable(assignment2 main.cpp)

# Link required libraries
target_link_libraries(assignment2 glfw GLEW OpenGL)
