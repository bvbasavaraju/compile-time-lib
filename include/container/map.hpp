#pragma once

#include <array>
#include <concepts>
#include <cstdlib>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace ctl {

namespace details {

// For wrapping a type with MapEntry type!!
template <typename T>
concept HasKey = requires(T t) {
    { t.key() } -> std::convertible_to<std::uint64_t>;
};

template <typename T>
concept IsMappable = 
    (std::is_integral_v<T>) 
    || (std::is_floating_point_v<T>) 
    || (HasKey<T> && std::is_constructible_v<T> && std::is_copy_constructible_v<T>);

// To check the uniqueness of the key in a map
struct is_unique_key {
    template <typename... Entries>
        requires (HasKey<Entries> && ...)
    constexpr auto operator()(Entries const&... entries) -> bool {
            const std::array keys{static_cast<std::uint64_t>(entries.key())...};
            for (std::size_t i = 0; i < keys.size(); ++i) {
                for (std::size_t j = i + 1; j < keys.size(); ++j) {
                    if (keys[i] == keys[j]) {
                        return false;
                    }
                }
            }
            return true;
        }

    template <typename... Entries>
        requires (((std::is_integral_v<Entries>) || (std::is_floating_point_v<Entries>)) && ...)
    constexpr auto operator()(Entries const&... entries) -> bool {
            const std::array keys{static_cast<std::uint64_t>(entries)...};
            for (std::size_t i = 0; i < keys.size(); ++i) {
                for (std::size_t j = i + 1; j < keys.size(); ++j) {
                    if (keys[i] == keys[j]) {
                        return false;
                    }
                }
            }
            return true;
        }
};

}   // namespace details

template <typename T, std::size_t N>
    requires (details::IsMappable<T>)
struct map {
public:
    using value_t = T;
    using key_t = std::uint64_t;

private:
    using data_t = std::array<std::pair<key_t, value_t>, N>;

public:
    using iterator = data_t::iterator;
    using const_iterator = data_t::const_iterator;

private:

    template <typename U>
    struct data_initializer {
            template <typename ...Us>
                requires((std::is_same_v<U, Us> && ...))
            consteval auto operator()(U first, Us... rest) -> data_t {
                if (!details::is_unique_key{}(first, rest...)) {
                    std::abort();
                }

                // Sort the array based on the key values in ascending order
                constexpr auto sort_entries = [](auto &arr) -> void {
                    for(std::size_t i = 0; i < arr.size(); ++i) {
                        for (std::size_t j = i + 1; j < arr.size(); ++j) {
                            if (arr[i].first > arr[j].first) {
                                std::swap(arr[i], arr[j]);
                            }
                        }
                    }
                };

                if constexpr (details::HasKey<U>) {
                    data_t arr{ std::pair<key_t, value_t>(first.key(), first),
                            std::pair<key_t, value_t>(rest.key(), rest)...};
                    sort_entries(arr);
                    return arr;
                } else {
                    data_t arr{ std::pair<key_t, value_t>(static_cast<std::uint64_t>(first), first),
                            std::pair<key_t, value_t>(static_cast<std::uint64_t>(rest), rest)...};
                    sort_entries(arr);
                    return arr;
                }
            };
    };

    // members
    const data_t data;

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

        return nullptr;
    };
public:
    template <typename... Ts>
        requires(std::is_same_v<T, Ts> && ...)
    consteval map(T first, Ts... rest) 
    : data(data_initializer<T>{}(first, rest...)) 
    {}

    constexpr map() requires (N == 0) : data{} {}

    constexpr auto count(key_t key) const noexcept -> std::size_t {
        return (get_value(key) == nullptr) ? 0 : 1;
    }

    constexpr auto find(key_t key) const noexcept -> const_iterator {
        return get_value(key);
    }

    constexpr auto contains(key_t key) const noexcept -> bool {
        return (get_value(key) != nullptr); 
    }

    constexpr auto size() const noexcept -> std::size_t {
        return data.size();
    }

    constexpr auto begin() const -> const_iterator {
        return data.begin();
    }
    
    constexpr auto end() const -> const_iterator {
        return data.end();
    }
    
};  // struct map

template <typename T, typename... Ts>
    requires(std::is_same_v<T, Ts> && ...)
map(T, Ts...) -> map<T, sizeof...(Ts) + 1>;

}   // namespace ctl
