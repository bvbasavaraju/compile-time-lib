#pragma once

#include <array>
#include <utility>
#include <variant>

#include "utils/map_utils.hpp"

namespace ctl {

template <typename... T>
    requires (details::IsMappable<T> && ...)
struct mixed_map {
public:
    using key_t = std::uint64_t;
    using value_t = std::variant<T...>;
private:
    using data_t = std::array<std::pair<key_t, value_t>, sizeof...(T)>;

public:
    using iterator = typename data_t::iterator;    
    using const_iterator = typename data_t::const_iterator;

private:
    constexpr static data_t data = []() {
        if constexpr (!details::is_unique_key{}(T{}...)) {
            std::abort();
        }

        data_t arr = []() constexpr -> data_t {
            if constexpr ((details::HasKey<T> && ...)) {
                return {std::pair<key_t, value_t>(T{}.key(), value_t{T{}})...};
            } else {
                return {std::pair<key_t, value_t>(static_cast<std::uint64_t>(T{}), value_t{T{}})...};
            }
        }();

        for (std::size_t i = 0; i < arr.size(); ++i) {
            for (std::size_t j = i + 1; j < arr.size(); ++j) {
                if (arr[i].first > arr[j].first) {
                    auto entry = std::move(arr[i]);
                    arr[i] = std::move(arr[j]);
                    arr[j] = std::move(entry);
                }
            }
        }
        return arr;
    }();

    constexpr auto get_value(key_t key) const -> const_iterator {
        std::size_t low = 0;
        std::size_t high = data.size();
        while (low < high) {
            std::size_t mid = low + (high - low) / 2;
            if ((data[mid].first) == key) {
                return &(data[mid]);
            } else if ((data[mid].first) < key) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }

        return data.end();;
    };

public:
    constexpr auto count(key_t key) const noexcept -> std::size_t {
        return get_value(key) != data.end() ? 1 : 0;
    }

    constexpr auto find(key_t key) const noexcept -> const_iterator {
        return get_value(key);
    }

    constexpr auto contains(key_t key) const noexcept -> bool {
        return get_value(key) != data.end();
    }

    constexpr auto size() const noexcept -> std::size_t {
        return data.size();
    }

    constexpr auto begin() const noexcept -> const_iterator {
        return data.begin();
    }
    
    constexpr auto end() const noexcept -> const_iterator {
        return data.end();
    }
};  // struct mixed_map

template <typename... T>
mixed_map(T...) -> mixed_map<T...>;

}   // namespace ctl
