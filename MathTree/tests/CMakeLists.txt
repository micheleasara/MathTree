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
add_executable(RealNumberTest RealNumberTest.cpp)
target_link_libraries(RealNumberTest ${TestingLibs})
gtest_discover_tests(RealNumberTest)

add_executable(BinaryExpressionTest BinaryExpressionTest.cpp)
target_link_libraries(BinaryExpressionTest ${TestingLibs})
gtest_discover_tests(BinaryExpressionTest)