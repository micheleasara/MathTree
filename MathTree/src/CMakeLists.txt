cmake_minimum_required(VERSION 3.22)

set(headers Expression.hpp InfixParselets.hpp Lexer.hpp Parser.hpp PrefixParselets.hpp Token.hpp Utils.hpp)
add_library(MathTree ${headers} Expression.cpp InfixParselets.cpp Lexer.cpp Parser.cpp PrefixParselets.cpp Token.cpp Utils.cpp)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  if(MSVC)
    target_compile_options(MathTree PRIVATE /W4)
  endif()

  if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(MathTree PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif(CMAKE_BUILD_TYPE MATCHES Debug)

install(TARGETS MathTree DESTINATION "lib/")
install(FILES ${headers} DESTINATION "include/MathTree")