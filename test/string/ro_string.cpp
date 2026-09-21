#include <iostream>
#include <cstring>
#include <cstdint>

#include "string/algorithms.hpp"
#include "string/registrar.hpp"

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

using namespace ctl::string;

TEST_F(ro_string_test, ro_string_basic) {
    using helloType = ROST("Hello");
    static_assert(helloType::size() == 5, "Expected size of 5");
    static_assert(std::strcmp(helloType::c_str(), "Hello") == 0, "Expected string to be 'Hello'");

    using worldType = ROST("World");
    static_assert(worldType::size() == 5, "Expected size of 5");
    static_assert(std::strcmp(worldType::c_str(), "World") == 0, "Expected string to be 'World'");

    using helloWorldType = ROST("Hello, World!");
    static_assert(helloWorldType::size() == 13, "Expected size of 13");
    static_assert(std::strcmp(helloWorldType::c_str(), "Hello, World!") == 0, "Expected string to be 'Hello, World!'");
}

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

TEST_F(ro_string_test, hash_collision_detection) {
    // HASH Collision for 2 different types are considered as collision!! for Same type it is expected to match!! - So, it should not be considered as collision!!
    using helloType = ROST("Hello");
    using worldType = ROST("World");
    using helloWorldType = ROST("Hello, World!");

    using emptyList = ::ctl::list<>;
    static_assert(!details::collision_detector_v<emptyList, helloType>, "Hash collision detected!!");

    using list1 = ::ctl::list<helloType>;
    static_assert(!details::collision_detector_v<list1, helloType>, "Hash Should match for for known type!!");
    static_assert(!details::collision_detector_v<list1, worldType>, "Hash Should NOT match for for unknown type!!");

    using list2 = ::ctl::list<helloType, worldType>;
    static_assert(!details::collision_detector_v<list2, helloType>, "Hash Should match for for known type!!");
    static_assert(!details::collision_detector_v<list2, worldType>, "Hash Should match for for known type!!");
    static_assert(!details::collision_detector_v<list2, helloWorldType>, "Hash Should NOT match for for unknown type!!");

    using list3 = ::ctl::list<helloType, worldType, helloWorldType>;
    static_assert(!details::collision_detector_v<list3, helloType>, "Hash Should match for for known type!!");
    static_assert(!details::collision_detector_v<list3, worldType>, "Hash Should match for for known type!!");
    static_assert(!details::collision_detector_v<list3, helloWorldType>, "Hash Should match for for known type!!");
}

TEST_F(ro_string_test, hash_check) {
    static_assert(HASH("Hello") == ROST("Hello")::hash(), "Hash mismatch for 'Hello'");
    static_assert(HASH("Hello") == hash{}("Hello"), "Hash mismatch for 'Hello'");
    static_assert(HASH("Hello") == HASH("Hello"), "Hash mismatch for 'Hello'");
    static_assert(HASH("Hello") == "Hello"_ros.hash(), "Hash mismatch for 'Hello'");

    static_assert(HASH("World") == ROST("World")::hash(), "Hash mismatch for 'World'");
    static_assert(HASH("World") == hash{}("World"), "Hash mismatch for 'World'");
    static_assert(HASH("World") == HASH("World"), "Hash mismatch for 'World'");
    static_assert(HASH("World") == "World"_ros.hash(), "Hash mismatch for 'World'");

    static_assert(HASH("Hello World") == ROST("Hello World")::hash(), "Hash mismatch for 'Hello World'");
    static_assert(HASH("Hello World") == hash{}("Hello World"), "Hash mismatch for 'Hello World'");
    static_assert(HASH("Hello World") == HASH("Hello World"), "Hash mismatch for 'Hello World'");
    static_assert(HASH("Hello World") == "Hello World"_ros.hash(), "Hash mismatch for 'Hello World'");
}

TEST_F(ro_string_test, RegistrarTest) {
    using emptyList = REGISTRAR_INIT;
    static_assert(std::is_same_v<emptyList, ::ctl::string::registrar_t<>>, "Expected empty list");

    using list1 = REGISTRAR_ADD(emptyList, ROST("Hello"));
    static_assert(std::is_same_v<list1, ::ctl::string::registrar_t<ROST("Hello")>>, "Expected list with Hello");
    static_assert(std::is_same_v<list1, ::ctl::list<ROST("Hello")>>, "Expected list with Hello");


    using list2 = REGISTRAR_ADD(list1, ROST("World"));
    static_assert(std::is_same_v<list2, ::ctl::string::registrar_t<ROST("Hello"), ROST("World")>>, "Expected list with Hello and World");
    static_assert(std::is_same_v<list2, ::ctl::list<ROST("Hello"), ROST("World")>>, "Expected list with Hello and World");

    using list3 = REGISTRAR_ADD(list2, ROST("Hello"));
    static_assert(std::is_same_v<list3, ::ctl::string::registrar_t<ROST("Hello"), ROST("World")>>, "Expected list with Hello, World");
    static_assert(std::is_same_v<list3, ::ctl::list<ROST("Hello"), ROST("World")>>, "Expected list with Hello, World");
    static_assert(std::is_same_v<list3, list2>, "Expected that list2 and list3 are same as Hello is already present in list2");

    using list4 = REGISTRAR_ADD(list2, ROST("HelloWorld"));
    static_assert(std::is_same_v<list4, ::ctl::string::registrar_t<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list4, ::ctl::list<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");

    using list5 = REGISTRAR_ADD(list3, ROST("HelloWorld"));
    static_assert(std::is_same_v<list5, ::ctl::string::registrar_t<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list5, ::ctl::list<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list4, list5>, "Expected that list4 and list5 are same as HelloWorld is already present in both");
}
