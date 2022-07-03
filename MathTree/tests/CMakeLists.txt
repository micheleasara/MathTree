cmake_minimum_required(VERSION 3.22)

include(FetchContent)
FetchContent_Declare(
  googletest
  # GoogleTest 1.11.0
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TestingLibs gtest_main gmock_main MathTree)

include(GoogleTest)
add_executable(ArithmeticLexerTest ArithmeticLexerTest.cpp)
target_link_libraries(ArithmeticLexerTest ${TestingLibs})
gtest_discover_tests(ArithmeticLexerTest)

add_executable(ArithmeticParserTest ArithmeticParserTest.cpp)
target_link_libraries(ArithmeticParserTest ${TestingLibs})
gtest_discover_tests(ArithmeticParserTest)

add_executable(BinaryExpressionsTest BinaryExpressionsTest.cpp)
target_link_libraries(BinaryExpressionsTest ${TestingLibs})
gtest_discover_tests(BinaryExpressionsTest)

add_executable(InfixParseletsTest InfixParseletsTest.cpp)
target_link_libraries(InfixParseletsTest ${TestingLibs})
gtest_discover_tests(InfixParseletsTest)

add_executable(MatchersTest MatchersTest.cpp)
target_link_libraries(MatchersTest ${TestingLibs})
gtest_discover_tests(MatchersTest)

add_executable(PrattParserTest PrattParserTest.cpp)
target_link_libraries(PrattParserTest ${TestingLibs})
gtest_discover_tests(PrattParserTest)

add_executable(PrefixParseletsTest PrefixParseletsTest.cpp)
target_link_libraries(PrefixParseletsTest ${TestingLibs})
gtest_discover_tests(PrefixParseletsTest)

add_executable(RealNumberTest RealNumberTest.cpp)
target_link_libraries(RealNumberTest ${TestingLibs})
gtest_discover_tests(RealNumberTest)

add_executable(UnaryExpressionsTest UnaryExpressionsTest.cpp)
target_link_libraries(UnaryExpressionsTest ${TestingLibs})
gtest_discover_tests(UnaryExpressionsTest)

add_executable(ValidationTest ValidationTest.cpp)
target_link_libraries(ValidationTest ${TestingLibs})
gtest_discover_tests(ValidationTest)