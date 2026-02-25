# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(LinearEquationsSolver CXX)

# Require C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenMP package for parallel processing
find_package(OpenMP)

# Define the folder where your code lives to keep things clean
set(SRC_DIR "Linear Equations Solving")

# Add the executable and ALL .cpp source files
add_executable(LinearSolver 
    "${SRC_DIR}/Linear Equations Solving.cpp"
    "${SRC_DIR}/Command.cpp"
    "${SRC_DIR}/Equation.cpp"
    "${SRC_DIR}/EquationGenerator.cpp"
    "${SRC_DIR}/LinearSystem.cpp"
    "${SRC_DIR}/Matrix.cpp"
    "${SRC_DIR}/Vector.cpp"
)

# Tell CMake where to find the header files (they are in the same folder)
target_include_directories(LinearSolver PRIVATE "${SRC_DIR}")

# Link OpenMP if found
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found. Parallel elimination is enabled.")
    target_link_libraries(LinearSolver PUBLIC OpenMP::OpenMP_CXX)
else()
    message(WARNING "OpenMP not found. Solver will run in single-threaded mode.")
endif()