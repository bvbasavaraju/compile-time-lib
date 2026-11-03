#include <cstdint>
#include <iterator>
#include <limits>
#include <type_traits>
#include <variant>

#include "container/mixed_map.hpp"

#include <gtest/gtest.h>

class mixed_map_test : public ::testing::Test {};

using namespace ctl;

struct first_entry {
    std::uint64_t stored_value = 30;

    constexpr auto key() const noexcept -> std::uint64_t {
        return stored_value;
    }
};

struct second_entry {
    std::uint64_t stored_value = 10;

    constexpr auto key() const noexcept -> std::uint64_t {
        return stored_value;
    }
};

template <std::uint64_t Key, int Payload>
struct keyed_entry {
    int payload = Payload;

    constexpr auto key() const noexcept -> std::uint64_t {
        return Key;
    }
};

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

TEST_F(mixed_map_test, heterogeneous_keyed_entries_test) {
    constexpr mixed_map<first_entry, second_entry> values{};

    static_assert(values.size() == 2);
    static_assert(keys_are_sorted(values));
    static_assert(values.begin()->first == 10);
    static_assert((values.end() - 1)->first == 30);
    static_assert(values.find(10) != values.end());
    static_assert(values.find(30) != values.end());
    static_assert(values.find(20) == values.end());
    static_assert(values.count(10) == 1);
    static_assert(values.count(20) == 0);
    static_assert(values.contains(30));
    static_assert(!values.contains(20));
    static_assert(std::get_if<second_entry>(&values.find(10)->second) != nullptr);
    static_assert(std::get_if<first_entry>(&values.find(30)->second) != nullptr);

    EXPECT_EQ(values.find(10)->first, 10);
    EXPECT_EQ(std::get<second_entry>(values.find(10)->second).key(), 10);
    EXPECT_EQ(std::get<first_entry>(values.find(30)->second).key(), 30);
    EXPECT_EQ(values.find(20), values.end());
}

TEST_F(mixed_map_test, visit_value_by_active_type_test) {
    constexpr mixed_map<first_entry, second_entry> values{};
    constexpr auto describe = [](auto const& value) {
        using value_type = std::remove_cvref_t<decltype(value)>;
        if constexpr (std::is_same_v<value_type, first_entry>) {
            return value.key() + 100;
        } else {
            return value.key() + 200;
        }
    };

    constexpr auto first = values.find(30);
    constexpr auto second = values.find(10);
    static_assert(first != values.end());
    static_assert(second != values.end());
    static_assert(std::visit(describe, first->second) == 130);
    static_assert(std::visit(describe, second->second) == 210);

    ASSERT_NE(first, values.end());
    ASSERT_NE(second, values.end());
    EXPECT_EQ(std::visit(describe, first->second), 130);
    EXPECT_EQ(std::visit(describe, second->second), 210);
}

TEST_F(mixed_map_test, numeric_entry_test) {
    constexpr mixed_map<int> values{};

    static_assert(values.size() == 1);
    static_assert(keys_are_sorted(values));
    static_assert(values.begin()->first == 0);
    static_assert(values.find(0) != values.end());
    static_assert(values.find(1) == values.end());
    static_assert(std::get_if<int>(&values.find(0)->second) != nullptr);

    EXPECT_EQ(std::get<int>(values.find(0)->second), 0);
    EXPECT_EQ(values.count(0), 1);
    EXPECT_EQ(values.count(1), 0);
    EXPECT_TRUE(values.contains(0));
    EXPECT_FALSE(values.contains(1));
    EXPECT_EQ(std::distance(values.begin(), values.end()), 1);
}

TEST_F(mixed_map_test, single_entry_test) {
    constexpr mixed_map<first_entry> values{};

    static_assert(values.size() == 1);
    static_assert(values.begin()->first == 30);
    static_assert(values.begin() + 1 == values.end());
    static_assert(values.find(30) != values.end());
    static_assert(values.find(29) == values.end());
    static_assert(values.find(31) == values.end());
    static_assert(values.count(30) == 1);
    static_assert(values.count(29) == 0);
    static_assert(values.contains(30));
    static_assert(!values.contains(31));

    EXPECT_EQ(std::get<first_entry>(values.find(30)->second).key(), 30);
    EXPECT_EQ(values.find(29), values.end());
    EXPECT_EQ(std::distance(values.begin(), values.end()), 1);
}

TEST_F(mixed_map_test, key_boundaries_test) {
    using minimum_entry = keyed_entry<0, 0>;
    using next_entry = keyed_entry<1, 1>;
    using maximum_entry = keyed_entry<std::numeric_limits<std::uint64_t>::max(), 2>;
    using previous_to_maximum_entry =
        keyed_entry<std::numeric_limits<std::uint64_t>::max() - 1, 3>;
    constexpr mixed_map<maximum_entry, next_entry, minimum_entry, previous_to_maximum_entry> values{};
    constexpr auto maximum_key = std::numeric_limits<std::uint64_t>::max();

    static_assert(values.size() == 4);
    static_assert(keys_are_sorted(values));
    static_assert(values.begin()->first == 0);
    static_assert((values.end() - 1)->first == maximum_key);
    static_assert(values.find(0) != values.end());
    static_assert(values.find(1) != values.end());
    static_assert(values.find(maximum_key - 1) != values.end());
    static_assert(values.find(maximum_key) != values.end());
    static_assert(values.find(2) == values.end());
    static_assert(values.find(maximum_key - 2) == values.end());
    static_assert(std::get_if<minimum_entry>(&values.find(0)->second) != nullptr);
    static_assert(std::get_if<maximum_entry>(&values.find(maximum_key)->second) != nullptr);

    EXPECT_EQ(std::get<minimum_entry>(values.find(0)->second).payload, 0);
    EXPECT_EQ(std::get<maximum_entry>(values.find(maximum_key)->second).payload, 2);
    EXPECT_EQ(values.find(2), values.end());
    EXPECT_EQ(values.find(maximum_key - 2), values.end());
}

TEST_F(mixed_map_test, larger_unsorted_map_lookup_test) {
    using entry_0 = keyed_entry<0, 0>;
    using entry_10 = keyed_entry<10, 10>;
    using entry_20 = keyed_entry<20, 20>;
    using entry_30 = keyed_entry<30, 30>;
    using entry_40 = keyed_entry<40, 40>;
    using entry_50 = keyed_entry<50, 50>;
    using entry_60 = keyed_entry<60, 60>;
    using entry_70 = keyed_entry<70, 70>;
    using entry_80 = keyed_entry<80, 80>;
    constexpr mixed_map<entry_50, entry_10, entry_80, entry_20, entry_60,
                        entry_0, entry_70, entry_30, entry_40> values{};

    static_assert(values.size() == 9);
    static_assert(keys_are_sorted(values));
    static_assert(values.begin()->first == 0);
    static_assert((values.end() - 1)->first == 80);

    static_assert(values.find(0) != values.end());
    static_assert(values.find(10) != values.end());
    static_assert(values.find(20) != values.end());
    static_assert(values.find(30) != values.end());
    static_assert(values.find(40) != values.end());
    static_assert(values.find(50) != values.end());
    static_assert(values.find(60) != values.end());
    static_assert(values.find(70) != values.end());
    static_assert(values.find(80) != values.end());
    static_assert(values.find(5) == values.end());
    static_assert(values.find(45) == values.end());
    static_assert(values.find(75) == values.end());
    static_assert(values.find(81) == values.end());

    EXPECT_EQ(std::get<entry_0>(values.find(0)->second).payload, 0);
    EXPECT_EQ(std::get<entry_40>(values.find(40)->second).payload, 40);
    EXPECT_EQ(std::get<entry_80>(values.find(80)->second).payload, 80);
    EXPECT_EQ(values.count(45), 0);
    EXPECT_EQ(values.count(50), 1);
    EXPECT_FALSE(values.contains(81));
    EXPECT_TRUE(values.contains(70));
    EXPECT_EQ(std::distance(values.begin(), values.end()), 9);
}