cmake_minimum_required(VERSION 3.22)
project(Driver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(MATHTREE_PATH ../MathTree)
set(MATHTREE_BUILD_PATH lib)
option(TESTS "Build tests" ON)

add_subdirectory(${MATHTREE_PATH} ${MATHTREE_BUILD_PATH})

add_executable(driver expressionSolver.cpp)

target_include_directories(driver PUBLIC
                          ${PROJECT_SOURCE_DIR}
                          ${MATHTREE_PATH}/src/)

target_link_libraries(driver PUBLIC MathTree)