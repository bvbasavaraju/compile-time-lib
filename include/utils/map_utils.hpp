#pragma once

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

}   // namespace ctl
