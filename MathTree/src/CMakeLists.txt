cmake_minimum_required(VERSION 3.22)

set(headers Expression.hpp InfixParselets.hpp Lexer.hpp Parser.hpp PrefixParselets.hpp Token.hpp Utils.hpp)
add_library(MathTree ${headers} Expression.cpp InfixParselets.cpp Lexer.cpp Parser.cpp PrefixParselets.cpp Token.cpp Utils.cpp)

install(TARGETS MathTree DESTINATION "lib/")
install(FILES ${headers} DESTINATION "include/MathTree")