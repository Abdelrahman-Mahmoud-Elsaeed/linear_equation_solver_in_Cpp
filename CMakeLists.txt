cmake_minimum_required(VERSION 3.10)

project(LinearEquationsSolver CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)

set(SRC_DIR "Linear Equations Solving")

add_executable(LinearSolver 
    "${SRC_DIR}/Linear Equations Solving.cpp"
)

target_include_directories(LinearSolver PRIVATE "${SRC_DIR}")

if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found. Parallel elimination is enabled.")
    target_link_libraries(LinearSolver PUBLIC OpenMP::OpenMP_CXX)
else()
    message(WARNING "OpenMP not found. Solver will run in single-threaded mode.")
endif()