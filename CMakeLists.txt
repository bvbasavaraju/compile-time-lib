cmake_minimum_required(VERSION 3.25.1)

PROJECT(ctl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

# --------------------------------------- #
# Adding the GTest support
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# --------------------------------------- #

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
add_library(${PROJECT_NAME} INTERFACE)
