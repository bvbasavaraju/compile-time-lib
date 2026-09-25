#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace ctl {

template <typename T>
    requires(std::is_integral_v<T>)
struct map_entry {
private:
    T val;

public:
    constexpr map_entry(T val_) : val(val_) {}

    constexpr auto key() const noexcept -> std::uint64_t {
        return static_cast<std::uint64_t>(val);
    }

    constexpr auto value() const noexcept -> T const {
        return val;
    }
};

template <typename T, std::size_t N>
// TODO: Add a requirement where T should have following constexpr member functions:
//  - key() that returns a non floating number type
struct map {
public:
    using key_t = std::decay_t<decltype(std::declval<T>().key())>;
    using value_t = T;

private:
    using data_t = std::array<std::pair<key_t, value_t>, N>;

    template <typename U>
    struct data_initializer {
        template <typename ...Us>
            requires(std::is_same_v<U, Us> && ...)
        constexpr auto operator()(U first, Us... rest) -> data_t {
            data_t arr{ std::pair<key_t, value_t>(first.key(), first),
                        std::pair<key_t, value_t>(rest.key(), rest)...};
            // Sort the array based on the key values in ascending order
            for(std::size_t i = 0; i < arr.size(); ++i) {
                for (std::size_t j = i + 1; j < arr.size(); ++j) {
                    if (arr[i].first > arr[j].first) {
                        std::swap(arr[i], arr[j]);
                    }
                }
            }
            return arr;
        };
    };

    // members
    const data_t data;

    constexpr auto get_value(key_t key) const -> value_t const* {
        std::size_t low = 0;
        std::size_t high = data.size();
        while (low < high) {
            std::size_t mid = low + (high - low) / 2;
            if ((data[mid].first) == key) {
                return &data[mid].second;
            } else if ((data[mid].first) < key) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }

        return nullptr;
    };
public:
    template <typename... Ts>
        requires(std::is_same_v<T, Ts> && ...)
    constexpr map(T first, Ts... rest) : data(data_initializer<T>{}(first, rest...)) {}

    constexpr map() requires (N == 0) : data{} {}

    constexpr auto find(key_t key) const noexcept -> value_t const* {
        return get_value(key);
    }

    constexpr auto size() const noexcept -> std::size_t {
        return data.size();
    }

#ifdef DEBUG_MAP
    auto print() const -> void {
        for(auto entry : data) {
            std::cout << entry.second.value() << "\n";
        }
    }
#endif  // DEBUG_MAP
    
};  // struct map

template <typename T, typename... Ts>
    requires(std::is_same_v<T, Ts> && ...)
map(T, Ts...) -> map<T, sizeof...(Ts) + 1>;

}   // namespace ctl
