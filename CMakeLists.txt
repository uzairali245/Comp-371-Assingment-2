cmake_minimum_required(VERSION 3.27)
project(assignment2)

set(CMAKE_CXX_STANDARD 14)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW (installed via Homebrew)
find_package(glfw3 REQUIRED)

# Find GLEW (installed via Homebrew)
find_package(GLEW REQUIRED)

# Find GLM (installed via Homebrew)
find_package(glm REQUIRED)

# Include directories for Homebrew-installed libraries
include_directories(/opt/homebrew/include)

# Link directories for Homebrew-installed libraries
link_directories(/opt/homebrew/lib)

# Add executable
add_executable(assignment2 main.cpp)

# Link required libraries
target_link_libraries(assignment2
        glfw
        GLEW::GLEW
        OpenGL::GL
        glm::glm
)