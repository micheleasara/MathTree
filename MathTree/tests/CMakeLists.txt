cmake_minimum_required(VERSION 3.22)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TestingLibs gtest_main gmock_main MathTree)

include(GoogleTest)
add_executable(NumbersTest NumbersTest.cpp)
target_link_libraries(NumbersTest ${TestingLibs})
gtest_discover_tests(NumbersTest)

add_executable(OperationsTest OperationsTest.cpp)
target_link_libraries(OperationsTest ${TestingLibs})
gtest_discover_tests(OperationsTest)

add_executable(ExpressionTest ExpressionTest.cpp)
target_link_libraries(ExpressionTest ${TestingLibs})
gtest_discover_tests(ExpressionTest)
