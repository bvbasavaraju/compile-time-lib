#pragma once

#include <variant>

namespace ctl {

template <typename T, T ... Ts>
// TODO: Add a requirement where T should have following constexpr member functions:
//  - key() that returns a non floating number type
struct map {
public:
    using key_t = typename std::decay_t<decltype(T{}.key())>;
    using value_t = T;
private:
    constexpr static auto data = []() {
        std::array<std::pair<key_t, T>, sizeof...(Ts)> arr{std::pair<key_t, T>(T{}.key(), Ts)...};
        // Sort the array based on the key values in ascending order
        for(std::size_t i = 0; i < arr.size(); ++i) {
            for (std::size_t j = i + 1; j < arr.size(); ++j) {
                if (arr[i].first > arr[j].first) {
                    std::swap(arr[i], arr[j]);
                }
            }
        }
        return arr;
    }();

    constexpr auto get_value(key_t key) const -> value_t const* {
        key_t low = 0;
        key_t high = data.size() - 1;
        while (low <= high) {
            key_t mid = low + (high - low) / 2;
            if ((data[mid].first) == key) {
                return &data[mid].second;
            } else if ((data[mid].first) < key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        return nullptr;
    };

public:
    constexpr auto find(key_t key) const -> value_t const* {
        return get_value(key);
    }
    
};  // struct map

template <typename... T>
// TODO: Add a requirement where T should have following constexpr member functions:
//  - key() that returns a non floating number type
struct mixed_map {
public:
    using key_t = std::common_type_t<std::decay_t<decltype(std::declval<T>().key())>...>;
    using value_t = std::variant<T...>;
private:
    constexpr static auto data = []() {
        std::array<std::pair<key_t, value_t>, sizeof...(T)> arr{std::pair<key_t, value_t>(T{}.key(), value_t{T{}})...};
        for (std::size_t i = 0; i < arr.size(); ++i) {
            for (std::size_t j = i + 1; j < arr.size(); ++j) {
                if (arr[i].first > arr[j].first) {
                    std::swap(arr[i], arr[j]);
                }
            }
        }
        return arr;
    }();

    constexpr auto get_value(key_t key) const -> value_t const* {
        key_t low = 0;
        key_t high = data.size() - 1;
        while (low <= high) {
            key_t mid = low + (high - low) / 2;
            if ((data[mid].first) == key) {
                return &data[mid].second;
            } else if ((data[mid].first) < key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        return nullptr;
    };

public:
    constexpr auto find(key_t key) const {
        return get_value(key);
    }
    
};  // struct map

}   // namespace ctl
