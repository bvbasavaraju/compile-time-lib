#include <iostream>
#include <cstring>
#include <cstdint>
#include <array>
#include <limits>

#include "container/map.hpp"
#include "debug/show_type.hpp"

#include <gtest/gtest.h>

#include <type_traits>

class map_test : public ::testing::Test {
  protected:
    void SetUp() override {
        // Add Test setup code here!
    }

    void TearDown() override {
        // Add Test teardown code here!
    }
};

using namespace ctl;

struct uint64_entry {
    std::uint64_t stored_value;

    constexpr auto key() const noexcept -> std::uint64_t {
        return stored_value;
    }

    constexpr auto value() const noexcept -> std::uint64_t {
        return stored_value;
    }
};

constexpr auto constexpr_lookup(auto &map_, auto valueToFind) {
    return map_.find(valueToFind)->value();
}

constexpr auto constexpr_lookup_non_entry(auto &map_, auto valueToFind) {
    return map_.find(valueToFind);
}

TEST_F(map_test, basic_test) {
    constexpr auto map1 = map(map_entry{12}, map_entry{10}, map_entry{4}, map_entry{21}, map_entry{216}, map_entry{24}, map_entry{16});
    static_assert(map1.size() == 7, "Size of map should be 7!!");
    static_assert(map1.size() != 0, "Size of map should be 7!!");

    const auto val12 = map1.find(12);
    static_assert(constexpr_lookup(map1, 12) == 12, "Matching value not found!!");
    ASSERT_EQ(val12->value(), 12) << "Matching value not found!!";

    const auto val11 = map1.find(11);
    static_assert(constexpr_lookup_non_entry(map1, 11) == nullptr, "Non Entry value has been found!!??");
    ASSERT_EQ(val11, nullptr) << "Non Entry value has been found!!??";
}

TEST_F(map_test, empty_map_test) {
    constexpr ::ctl::map<ctl::map_entry<int>, 0> empty_map{};
    static_assert(empty_map.size() == 0, "Size of empty map is more than 0!!??");
    static_assert(!(empty_map.size() > 0), "Size of empty map is more than 0!!??");
    static_assert(constexpr_lookup_non_entry(empty_map, 0) == nullptr, "Non Entry value has been found!!??");
    static_assert(constexpr_lookup_non_entry(empty_map, std::numeric_limits<std::uint64_t>::max()) == nullptr, "Non Entry value has been found!!??");

    ASSERT_EQ(empty_map.find(0), nullptr) << "Value found in an empty map!!??";
    ASSERT_EQ(empty_map.find(std::numeric_limits<std::uint64_t>::max()), nullptr)
        << "Value found in an empty map!!??";
}

TEST_F(map_test, single_entry_boundary_test) {
    constexpr auto single_map = map(map_entry{12});

    static_assert(single_map.size() == 1);
    static_assert(constexpr_lookup(single_map, 12) == 12, "Matching value not found!!");
    static_assert(constexpr_lookup_non_entry(single_map, 11) == nullptr, "Non Entry value has been found!!??");
    static_assert(constexpr_lookup_non_entry(single_map, 13) == nullptr, "Non Entry value has been found!!??");

    ASSERT_EQ(single_map.size(), 1);
    ASSERT_NE(single_map.find(12), nullptr);
    ASSERT_EQ(single_map.find(12)->value(), 12);
    ASSERT_EQ(single_map.find(11), nullptr);
    ASSERT_EQ(single_map.find(13), nullptr);
}

TEST_F(map_test, unsorted_entries_are_searchable_test) {
    constexpr auto values = map(map_entry{30}, map_entry{10}, map_entry{40}, map_entry{20});

    static_assert(values.size() == 4);
    static_assert(constexpr_lookup(values, 10) == 10, "Matching value not found!!");
    static_assert(constexpr_lookup(values, 20) == 20, "Matching value not found!!");
    static_assert(constexpr_lookup(values, 30) == 30, "Matching value not found!!");
    static_assert(constexpr_lookup(values, 40) == 40, "Matching value not found!!");
    static_assert(constexpr_lookup_non_entry(values, 9) == nullptr, "Non Entry value has been found!!??");
    static_assert(constexpr_lookup_non_entry(values, 41) == nullptr, "Non Entry value has been found!!??");

    ASSERT_EQ(values.size(), 4);
    ASSERT_EQ(values.find(10)->value(), 10);
    ASSERT_EQ(values.find(20)->value(), 20);
    ASSERT_EQ(values.find(30)->value(), 30);
    ASSERT_EQ(values.find(40)->value(), 40);
    ASSERT_EQ(values.find(9), nullptr);
    ASSERT_EQ(values.find(41), nullptr);
}

TEST_F(map_test, duplicate_keys_test) {
    constexpr auto values = map(map_entry{12}, map_entry{12}, map_entry{24});

    static_assert(values.size() == 3);
    static_assert(constexpr_lookup(values, 12) == 12, "Matching value not found!!");
    static_assert(constexpr_lookup(values, 24) == 24, "Matching value not found!!");
    static_assert(constexpr_lookup_non_entry(values, 11) == nullptr, "Non Entry value has been found!!??");

    ASSERT_EQ(values.size(), 3);
    ASSERT_NE(values.find(12), nullptr);
    ASSERT_EQ(values.find(12)->value(), 12);
    ASSERT_EQ(values.find(24)->value(), 24);
    ASSERT_EQ(values.find(11), nullptr);
}

TEST_F(map_test, key_type_boundaries_test) {
    constexpr auto values = map(
        uint64_entry{std::numeric_limits<std::uint64_t>::max()},
        uint64_entry{0});

    static_assert(values.size() == 2);
    static_assert(constexpr_lookup(values, 0) == 0, "Matching value not found!!");
    static_assert(constexpr_lookup(values, std::numeric_limits<std::uint64_t>::max()) ==
                      std::numeric_limits<std::uint64_t>::max(),
                  "Matching value not found!!");
    static_assert(constexpr_lookup_non_entry(values, 1) == nullptr, "Non Entry value has been found!!??");

    ASSERT_EQ(values.size(), 2);
    ASSERT_EQ(values.find(0)->value(), 0);
    ASSERT_EQ(values.find(std::numeric_limits<std::uint64_t>::max())->value(),
              std::numeric_limits<std::uint64_t>::max());
    ASSERT_EQ(values.find(1), nullptr);
}
