cmake_minimum_required(VERSION 3.22)

set(headers Expression.hpp Numbers.hpp Operations.hpp)
add_library(MathTree ${headers} Expression.cpp Numbers.cpp Operations.cpp)

install(TARGETS MathTree DESTINATION "lib/")
install(FILES ${headers} DESTINATION "include/MathTree")