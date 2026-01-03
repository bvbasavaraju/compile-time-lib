enable_testing()

set(CTL_TEST ctl_test)

add_executable(${CTL_TEST} container/list_test.cpp utils_test.cpp)

target_include_directories(${CTL_TEST} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${CTL_TEST} PUBLIC GTest::gtest_main ctl)

include(GoogleTest)
gtest_discover_tests(${CTL_TEST})
