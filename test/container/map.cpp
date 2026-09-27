#include <cstdint>
#include <iterator>
#include <limits>

#include "container/map.hpp"

#include <gtest/gtest.h>

class map_test : public ::testing::Test {};

using namespace ctl;

static_assert(details::is_unique_key{}(12, 10));
static_assert(!details::is_unique_key{}(12, 12));
static_assert(!details::is_unique_key{}(1.25, 1.75));

struct uint64_entry {
    std::uint64_t stored_value;

    constexpr auto key() const noexcept -> std::uint64_t {
        return stored_value;
    }

    constexpr auto value() const noexcept -> std::uint64_t {
        return stored_value;
    }
};

static_assert(details::is_unique_key{}(uint64_entry{0}, uint64_entry{1}));
static_assert(!details::is_unique_key{}(uint64_entry{1}, uint64_entry{1}));

constexpr auto constexpr_lookup(auto const& map_, auto value_to_find) {
    return map_.find(value_to_find)->second;
}

constexpr auto constexpr_lookup_non_entry(auto const& map_, auto value_to_find) {
    return map_.find(value_to_find);
}

constexpr auto keys_are_sorted(auto const& map_) -> bool {
    if (map_.begin() == map_.end()) {
        return true;
    }

    auto previous = map_.begin()->first;
    for (auto entry = map_.begin() + 1; entry != map_.end(); ++entry) {
        if (previous >= entry->first) {
            return false;
        }
        previous = entry->first;
    }
    return true;
}

TEST_F(map_test, basic_test) {
    constexpr auto values = map(12, 10, 4, 21, 216, 24, 16);

    static_assert(values.size() == 7);
    static_assert(values.size() != 0);
    static_assert(constexpr_lookup(values, 12) == 12);
    static_assert(constexpr_lookup_non_entry(values, 11) == values.end());
    static_assert(keys_are_sorted(values));

    ASSERT_EQ(values.find(12)->second, 12);
    ASSERT_EQ(values.find(11), values.end());
    ASSERT_TRUE(keys_are_sorted(values));
}

TEST_F(map_test, empty_map_test) {
    constexpr map<int, 0> empty_map{};

    static_assert(empty_map.size() == 0);
    static_assert(empty_map.begin() == empty_map.end());
    static_assert(empty_map.count(0) == 0);
    static_assert(!empty_map.contains(0));
    static_assert(constexpr_lookup_non_entry(empty_map, 0) == empty_map.end());
    static_assert(constexpr_lookup_non_entry(empty_map, std::numeric_limits<std::uint64_t>::max()) == empty_map.end());

    ASSERT_EQ(empty_map.find(0), empty_map.end());
    ASSERT_EQ(empty_map.find(std::numeric_limits<std::uint64_t>::max()), empty_map.end());
    ASSERT_EQ(std::distance(empty_map.begin(), empty_map.end()), 0);
    ASSERT_EQ(empty_map.count(0), 0);
    ASSERT_FALSE(empty_map.contains(0));
}

TEST_F(map_test, single_entry_boundary_test) {
    constexpr auto single_map = map(12);

    static_assert(single_map.size() == 1);
    static_assert(single_map.begin()->first == 12);
    static_assert(single_map.begin() + 1 == single_map.end());
    static_assert(constexpr_lookup(single_map, 12) == 12);
    static_assert(constexpr_lookup_non_entry(single_map, 11) == single_map.end());
    static_assert(constexpr_lookup_non_entry(single_map, 13) == single_map.end());
    static_assert(single_map.count(12) == 1);
    static_assert(single_map.contains(12));

    ASSERT_EQ(single_map.find(12)->second, 12);
    ASSERT_EQ(single_map.find(11), single_map.end());
    ASSERT_EQ(single_map.find(13), single_map.end());
    ASSERT_EQ(std::distance(single_map.begin(), single_map.end()), 1);
    ASSERT_EQ(single_map.count(13), 0);
    ASSERT_FALSE(single_map.contains(13));
}

TEST_F(map_test, unsorted_entries_are_searchable_test) {
    constexpr auto values = map(30, 10, 40, 20);

    static_assert(values.size() == 4);
    static_assert(constexpr_lookup(values, 10) == 10);
    static_assert(constexpr_lookup(values, 20) == 20);
    static_assert(constexpr_lookup(values, 30) == 30);
    static_assert(constexpr_lookup(values, 40) == 40);
    static_assert(constexpr_lookup_non_entry(values, 9) == values.end());
    static_assert(constexpr_lookup_non_entry(values, 41) == values.end());
    static_assert(keys_are_sorted(values));

    ASSERT_EQ(values.find(10)->second, 10);
    ASSERT_EQ(values.find(20)->second, 20);
    ASSERT_EQ(values.find(30)->second, 30);
    ASSERT_EQ(values.find(40)->second, 40);
    ASSERT_EQ(values.find(9), values.end());
    ASSERT_EQ(values.find(41), values.end());
    ASSERT_EQ(values.count(10), 1);
    ASSERT_EQ(values.count(9), 0);
    ASSERT_TRUE(values.contains(40));
    ASSERT_FALSE(values.contains(41));
    ASSERT_EQ(std::distance(values.begin(), values.end()), 4);
    ASSERT_EQ(values.begin()->first, 10);
    ASSERT_EQ((values.end() - 1)->first, 40);
}

TEST_F(map_test, signed_integer_key_boundaries_test) {
    constexpr auto values = map(std::numeric_limits<int>::min(), -1, 0);
    constexpr auto minimum_key = static_cast<std::uint64_t>(std::numeric_limits<int>::min());

    static_assert(values.size() == 3);
    static_assert(values.contains(minimum_key));
    static_assert(values.contains(std::numeric_limits<std::uint64_t>::max()));
    static_assert(values.contains(0));
    static_assert(values.find(minimum_key)->second == std::numeric_limits<int>::min());
    static_assert(values.find(std::numeric_limits<std::uint64_t>::max())->second == -1);
    static_assert(values.find(1) == values.end());

    ASSERT_EQ(values.count(minimum_key), 1);
    ASSERT_EQ(values.find(std::numeric_limits<std::uint64_t>::max())->second, -1);
    ASSERT_EQ(values.find(1), values.end());
}

TEST_F(map_test, key_type_boundaries_test) {
    constexpr auto values = map(
        uint64_entry{std::numeric_limits<std::uint64_t>::max()},
        uint64_entry{0});

    static_assert(values.size() == 2);
    static_assert(constexpr_lookup(values, 0).value() == 0);
    static_assert(constexpr_lookup(values, std::numeric_limits<std::uint64_t>::max()).value() ==
                  std::numeric_limits<std::uint64_t>::max());
    static_assert(constexpr_lookup_non_entry(values, 1) == values.end());
    static_assert(keys_are_sorted(values));

    ASSERT_EQ(values.find(0)->second.value(), 0);
    ASSERT_EQ(values.find(std::numeric_limits<std::uint64_t>::max())->second.value(),
              std::numeric_limits<std::uint64_t>::max());
    ASSERT_EQ(values.find(1), values.end());
    ASSERT_EQ(values.count(std::numeric_limits<std::uint64_t>::max()), 1);
}

TEST_F(map_test, floating_point_key_conversion_test) {
    constexpr auto values = map(12.75, 3.5, 11.25);

    static_assert(values.size() == 3);
    static_assert(values.find(12)->second == 12.75);
    static_assert(values.find(3)->second == 3.5);
    static_assert(values.find(11)->second == 11.25);
    static_assert(values.find(13) == values.end());
    static_assert(keys_are_sorted(values));

    ASSERT_EQ(values.count(12), 1);
    ASSERT_TRUE(values.contains(3));
    ASSERT_FALSE(values.contains(13));
    ASSERT_EQ(values.find(11)->second, 11.25);
    ASSERT_EQ(std::distance(values.begin(), values.end()), 3);
}

TEST_F(map_test, double_test) {
    constexpr auto values = map(12.1, 11.2);

    static_assert(values.size() == 2);
    static_assert(values.find(12)->second == 12.1);
    static_assert(values.find(11)->second == 11.2);
    static_assert(values.find(23) == values.end());

    ASSERT_EQ(values.find(12)->second, 12.1);
    ASSERT_EQ(values.find(11)->second, 11.2);
    ASSERT_EQ(values.find(23), values.end());
}
