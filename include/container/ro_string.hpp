#pragma once

namespace ctls {

namespace details {

// Hash
constexpr static std::uint64_t PRIME_NUM = 0x01000193ULL;
constexpr static std::uint64_t OFFSET_BASIS = 0x811C9DC5ULL;

struct hash {
    constexpr auto operator()(const char* str, std::size_t len) const -> std::uint64_t {
        std::uint64_t hash = OFFSET_BASIS;
        for (std::size_t i = 0; i < len; ++i) {
            hash ^= static_cast<std::uint64_t>(str[i]);
            hash *= PRIME_NUM;
        }
        return hash;
    }

    constexpr auto operator()(const char* str) const -> std:: uint64_t {
        return operator()(str, std::strlen(str));
    }
};

inline constexpr auto getHash(const char* str) -> std::uint64_t {
    return details::hash{}(str);
}

// Read Only String
template <typename ch, ch... chs>
struct ro_string {
    constexpr static std::size_t size() {
        constexpr static std::size_t len = sizeof...(chs);
        return len;
    }

    static auto data() -> const char* {
        constexpr static char storage[] = {chs..., 0};
        return storage;
    }

    constexpr static auto hash() -> std::uint64_t {
        constexpr char str[] = {chs..., 0};
        return hash{}(str, sizeof...(chs));
    }
};
}   // namespace details

template <typename Char, Char... chs>
constexpr auto operator""_ros() -> details::ro_string<Char, chs...> {
    return {};
}

#define ROST(data) (decltype(data##_ros))
#define HASH(data) (decltype(data##_ros)::hash())

// String operations
// Append two ro_string
template <typename T1, typename T2>
struct append;

template <typename T, T... Ts1, T... Ts2>
struct append<details::ro_string<T, Ts1...>, details::ro_string<T, Ts2...>> {
    using type = details::ro_string<T, Ts1..., Ts2...>;
}

template <typename T1, typename T2>
using append_t = typename append<T1, T2>::type;

template <typename T1, typename T2>
using concat_t = typename append<T1, T2>::type;

// Remove prefix from ro_string
template <typename T1, typename T2>
struct remove_prefix;

template <typename T, T... Ts1, T... Ts2>
struct remove_prefix<details::ro_string<T, Ts2..., Ts1...>, details::ro_string<T, Ts2...>> {
    using type = details::ro_string<T, Ts1...>;
}

// operator overloading required!!??
template <typename Ch, Ch... chs1, Ch... chs2>
constexpr auto operator+(details::ro_string<Ch, chs1...>, details::ro_string<Ch, chs2...>) -> details::ro_string<Ch, chs1..., chs2...> {
    return {};
}

}   // namespace ctl
