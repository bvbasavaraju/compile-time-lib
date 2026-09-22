#include <iostream>
#include <cstring>
#include <cstdint>

#include "string/algorithms.hpp"
#include "string/registrar.hpp"
#include "debug/show_type.hpp"

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
    ASSERT_EQ(std::strcmp(helloType::c_str(), "Hello"), 0) << "Expected string to be 'Hello'";

    using worldType = ROST("World");
    static_assert(worldType::size() == 5, "Expected size of 5");
    ASSERT_EQ(std::strcmp(worldType::c_str(), "World"), 0) << "Expected string to be 'World'";

    using helloWorldType = ROST("Hello, World!");
    static_assert(helloWorldType::size() == 13, "Expected size of 13");
    ASSERT_EQ(std::strcmp(helloWorldType::c_str(), "Hello, World!"), 0) << "Expected string to be 'Hello, World!'";
}

TEST_F(ro_string_test, ro_string_size) {
    using helloWorldType = ROST("Hello, World!");
    static_assert(helloWorldType::size() == 13, "Expected size of 13");
    static_assert(helloWorldType::hash() == HASH("Hello, World!"), "Expected hash of match!!");

    using helloType = ROST("Hello");
    static_assert(helloType::size() == 5, "Expected size of 5");
    static_assert(helloType::hash() == HASH("Hello"), "Expected hash of match!!");

    using appendedT = append_t<helloType, helloWorldType>;    // Concatenate "Hello" and "Hello, World!" ==> "HelloHello, World!"
    static_assert(appendedT::size() == 18, "Expected size of 18");\
    static_assert(appendedT::hash() == HASH("HelloHello, World!"), "Expected hash of match!!");
    static_assert(std::is_same_v<appendedT, ROST("HelloHello, World!")>, "Expected type to be 'HelloHello, World!'");

    using removedPrefixT = remove_prefix_t<appendedT, helloType>;   // Remove "Hello" prefix from "HelloHello, World!" ==> "Hello, World!"
    static_assert(removedPrefixT::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT::hash() == HASH("Hello, World!"), "Expected hash of match!!");
    static_assert(std::is_same_v<removedPrefixT, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");
}

TEST_F(ro_string_test, remove_prefix) {
    using helloWorldType = ROST("Hello, World!");
    static_assert(helloWorldType::size() == 13, "Expected size of 13");
    static_assert(helloWorldType::hash() == HASH("Hello, World!"), "Expected hash of match!!");

    using helloType = ROST("Hello");
    static_assert(helloType::size() == 5, "Expected size of 5");
    static_assert(helloType::hash() == HASH("Hello"), "Expected hash of match!!");

    using appendedT = append_t<helloType, helloWorldType>;    // Concatenate "Hello" and "Hello, World!" ==> "HelloHello, World!"
    static_assert(appendedT::size() == 18, "Expected size of 18");\
    static_assert(appendedT::hash() == HASH("HelloHello, World!"), "Expected hash of match!!");
    static_assert(std::is_same_v<appendedT, ROST("HelloHello, World!")>, "Expected type to be 'HelloHello, World!'");

    using removedPrefixT = remove_prefix_t<appendedT, helloType>;   // Remove "Hello" prefix from "HelloHello, World!" ==> "Hello, World!"
    static_assert(removedPrefixT::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT::hash() == HASH("Hello, World!"), "Expected hash of match!!");
    static_assert(std::is_same_v<removedPrefixT, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    using removedPrefixT2 = remove_prefix_t<helloWorldType, helloType>;  // Remove "Hello" prefix from "Hello, World!" ==> ", World!"
    static_assert(removedPrefixT2::size() == 8, "Expected size of 8");
    static_assert(removedPrefixT2::hash() == HASH(", World!"), "Expected hash of match!!");
    static_assert(std::is_same_v<removedPrefixT2, ROST(", World!")>, "Expected type to be ', World!'");

    // try Remove "hello" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT2a = remove_prefix_t<helloWorldType, ROST("hello")>; 
    static_assert(removedPrefixT2a::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT2a::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT2a, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT2a, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    // try Remove "HEllo" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT2b = remove_prefix_t<helloWorldType, ROST("HEllo")>; 
    static_assert(removedPrefixT2b::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT2b::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT2b, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT2b, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    // try Remove "HeLlo" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT2c = remove_prefix_t<helloWorldType, ROST("HeLlo")>; 
    static_assert(removedPrefixT2c::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT2c::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT2c, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT2c, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    // try Remove "HelLo" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT2d = remove_prefix_t<helloWorldType, ROST("HelLo")>; 
    static_assert(removedPrefixT2d::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT2d::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT2d, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT2d, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    // try Remove "HellO" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT2e = remove_prefix_t<helloWorldType, ROST("HellO")>; 
    static_assert(removedPrefixT2e::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT2e::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT2e, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT2e, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    // try Remove "HELLO" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT2f = remove_prefix_t<helloWorldType, ROST("HELLO")>; 
    static_assert(removedPrefixT2f::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT2f::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT2f, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT2f, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");

    // Try to remove ",World!" prefix from "Hello, World!". As, it is not a prefix, it will return the original string "Hello, World!"
    using removedPrefixT3 = remove_prefix_t<helloWorldType, ROST(", World!")>;  
    static_assert(removedPrefixT3::size() == 13, "Expected size of 13");
    static_assert(removedPrefixT3::hash() == helloWorldType::hash(), "Expected hash of match of Hello, World!!");
    static_assert(std::is_same_v<removedPrefixT3, helloWorldType>, "Expected type to be 'Hello, World!'");
    static_assert(std::is_same_v<removedPrefixT3, ROST("Hello, World!")>, "Expected type to be 'Hello, World!'");
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
    static_assert(HASH("Hello") == HASH("Hello"), "Hash mismatch for 'Hello'");
    static_assert(HASH("Hello") == "Hello"_ros.hash(), "Hash mismatch for 'Hello'");
    ASSERT_EQ(HASH("Hello"), hash{}("Hello")) << "Hash mismatch for 'Hello'";

    static_assert(HASH("World") == ROST("World")::hash(), "Hash mismatch for 'World'");
    static_assert(HASH("World") == HASH("World"), "Hash mismatch for 'World'");
    static_assert(HASH("World") == "World"_ros.hash(), "Hash mismatch for 'World'");
    ASSERT_EQ(HASH("World"), hash{}("World")) << "Hash mismatch for 'World'";

    static_assert(HASH("Hello World") == ROST("Hello World")::hash(), "Hash mismatch for 'Hello World'");
    static_assert(HASH("Hello World") == HASH("Hello World"), "Hash mismatch for 'Hello World'");
    static_assert(HASH("Hello World") == "Hello World"_ros.hash(), "Hash mismatch for 'Hello World'");
    ASSERT_EQ(HASH("Hello World"), hash{}("Hello World")) << "Hash mismatch for 'Hello World'";
}

TEST_F(ro_string_test, RegistrarTest) {
    using emptyList = REGISTRAR_INIT;
    static_assert(std::is_same_v<emptyList, ::ctl::string::registrar_t<>>, "Expected empty list");

    using list1 = REGISTRAR_ADD(emptyList, ROST("Hello"));
    static_assert(std::is_same_v<list1, ::ctl::string::registrar_t<ROST("Hello")>>, "Expected list with Hello");
    static_assert(std::is_same_v<list1, ::ctl::list<ROST("Hello")>>, "Expected list with Hello");

    using list1b = REGISTRAR_ADD(list1, ROST("Hello"));
    static_assert(std::is_same_v<list1b, ::ctl::string::registrar_t<ROST("Hello")>>, "Expected list with Hello");
    static_assert(std::is_same_v<list1b, ::ctl::list<ROST("Hello")>>, "Expected list with Hello");

    using list2 = REGISTRAR_ADD(list1, ROST("World"));
    static_assert(std::is_same_v<list2, ::ctl::string::registrar_t<ROST("Hello"), ROST("World")>>, "Expected list with Hello and World");
    static_assert(std::is_same_v<list2, ::ctl::list<ROST("Hello"), ROST("World")>>, "Expected list with Hello and World");

    using list2b = REGISTRAR_ADD(list2, ROST("Hello"));
    static_assert(std::is_same_v<list2b, ::ctl::string::registrar_t<ROST("Hello"), ROST("World")>>, "Expected list with Hello, World");
    static_assert(std::is_same_v<list2b, ::ctl::list<ROST("Hello"), ROST("World")>>, "Expected list with Hello, World");
    static_assert(std::is_same_v<list2b, list2>, "Expected that list2 and list3 are same as Hello is already present in list2");

    using list2c = REGISTRAR_ADD(list2, ROST("World"));
    static_assert(std::is_same_v<list2c, ::ctl::string::registrar_t<ROST("Hello"), ROST("World")>>, "Expected list with Hello, World");
    static_assert(std::is_same_v<list2c, ::ctl::list<ROST("Hello"), ROST("World")>>, "Expected list with Hello, World");
    static_assert(std::is_same_v<list2c, list2>, "Expected that list2 and list4 are same as World is already present in list2");

    using list3 = REGISTRAR_ADD(list2, ROST("HelloWorld"));
    static_assert(std::is_same_v<list3, ::ctl::string::registrar_t<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3, ::ctl::list<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");

    using list3b = REGISTRAR_ADD(list3, ROST("HelloWorld"));
    static_assert(std::is_same_v<list3b, ::ctl::string::registrar_t<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3b, ::ctl::list<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3b, list3>, "Expected that list3 and list3b are same as HelloWorld is already present in both");

    using list3c = REGISTRAR_ADD(list3, ROST("Hello"));
    static_assert(std::is_same_v<list3c, ::ctl::string::registrar_t<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3c, ::ctl::list<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3c, list3>, "Expected that list3 and list3c are same as HelloWorld is already present in both");

    using list3d = REGISTRAR_ADD(list3, ROST("World"));
    static_assert(std::is_same_v<list3d, ::ctl::string::registrar_t<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3d, ::ctl::list<ROST("Hello"), ROST("World"), ROST("HelloWorld")>>, "Expected list with Hello, World, and HelloWorld");
    static_assert(std::is_same_v<list3d, list3>, "Expected that list3 and list3d are same as HelloWorld is already present in both");
}
