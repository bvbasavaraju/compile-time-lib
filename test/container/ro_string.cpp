#include <iostream>
#include <cstring>

#include "container/ro_string.hpp"

#include <gtest/gtest.h>

#include <type_traits>

class ro_string_test : public ::testing::Test {
  protected:
    void SetUp() override {
        // Add Test setup code here!
    }

    void TearDown() override {
        // Add Test teardown code here!
    }
};

using namespace ctl;

TEST_F(ro_string_test, ro_string_size) {
    using helloWorldType = ROST("Hello, World!");
    static_assert(helloWorldType::size() == 13, "Expected size of 13");

    using helloType = ROST("Hello");
    static_assert(helloType::size() == 5, "Expected size of 5");
    static_assert(helloType::hash() == HASH("Hello"), "Expected hash of match!!");

    using appendedT = append_t<helloType, helloWorldType>;    // Concatenate "Hello" and "Hello, World!" ==> "HelloHello, World!"
    static_assert(appendedT::size() == 18, "Expected size of 18");

    using removedPrefixT = remove_prefix_t<appendedT, helloType>;   // Remove "Hello" prefix from "HelloHello, World!" ==> "Hello, World!"
    static_assert(removedPrefixT::size() == 13, "Expected size of 13");

    using removedPrefixT2 = remove_prefix_t<helloWorldType, helloType>;  // Remove "Hello" prefix from "Hello, World!" ==> ", World!"
    static_assert(removedPrefixT2::size() == 8, "Expected size of 8");
}
