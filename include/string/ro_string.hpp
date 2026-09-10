#pragma once

#include <string/hash.hpp>

namespace ctl {

namespace details {

// Read Only String
template <typename ch, ch... chs>
struct ro_string {
    private:
        constexpr static char storage[] = {chs..., 0};
    public:
        constexpr static auto size() noexcept -> std::size_t {
            return sizeof...(chs);
        }

        constexpr static auto c_str() noexcept -> const char* {
            return storage;
        }

        constexpr static auto hash() noexcept -> std::uint64_t {
            constexpr char str[] = {chs..., 0};
            return ::ctl::details::hash{}(str, sizeof...(chs));
        }
};
}   // namespace details

template <typename Char, Char... chs>
constexpr auto operator""_ros() -> details::ro_string<Char, chs...> {
    return {};
}

// Note: As #define will replace during preprocessing stage operator""_ros might not be available if the namespace is not used.
// So, make sure to forward declare the operator""_ros in your namespace or use the ctl namespace to access it.
// ex: using ctl::operator""_ros;
// OR
// using namespace ctl;
#define ROST(data) decltype(data##_ros)

// String operations
// Append two ro_string
template <typename T1, typename T2>
struct append;

template <typename T, T... Ts1, T... Ts2>
struct append<details::ro_string<T, Ts1...>, details::ro_string<T, Ts2...>> {
    using type = details::ro_string<T, Ts1..., Ts2...>;
};

template <typename T1, typename T2>
using append_t = typename append<T1, T2>::type;

template <typename T1, typename T2>
using concat_t = typename append<T1, T2>::type;

// Remove prefix from ro_string
template <typename Str, typename Prefix>
struct remove_prefix {
private:
    template <typename T1, typename T2>
    struct remove_prefix_impl;

    // when prefix is empty, return the original string
    template <typename T, T... Ts>
    struct remove_prefix_impl<details::ro_string<T, Ts...>, details::ro_string<T>> {
        using type = details::ro_string<T, Ts...>;
    };

    template <typename T, T t, T... Ts1, T... Ts2>
    struct remove_prefix_impl<details::ro_string<T, t, Ts1...>, details::ro_string<T, t, Ts2...>> {
        using type = typename remove_prefix_impl<details::ro_string<T, Ts1...>, details::ro_string<T, Ts2...>>::type;
    };

public:
    using type = typename remove_prefix_impl<Str, Prefix>::type; 
};

template <typename T1, typename T2>
using remove_prefix_t = typename remove_prefix<T1, T2>::type;

// Substring

// Remove Postfix

// Contains, HasSubstring

// Equal

// Split based on pattern!

// Replace (replace portion of string with other string!)


// operator overloading required!!??
template <typename Ch, Ch... chs1, Ch... chs2>
constexpr auto operator+(details::ro_string<Ch, chs1...>, details::ro_string<Ch, chs2...>) -> details::ro_string<Ch, chs1..., chs2...> {
    return {};
}

}   // namespace ctl

// export operator""_ros to global namespace for convenience
using ctl::operator""_ros;
