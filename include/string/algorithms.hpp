#pragma once

#include <string/ro_string.hpp>

namespace ctl {

// String operations
// Append two ro_string
template <typename Ros1, typename Ros2>
struct append {
    private:
        template <typename T1, typename T2>
        struct append_impl;

        // Specialization for ro_string
        template <typename T, T... Ts1, T... Ts2>
        struct append_impl<ro_string<T, Ts1...>, ro_string<T, Ts2...>> {
            using type = ro_string<T, Ts1..., Ts2...>;
        };

    public:
        using type = typename append_impl<Ros1, Ros2>::type;
};

template <typename Ros1, typename Ros2>
using append_t = typename append<Ros1, Ros2>::type;

template <typename Ros1, typename Ros2>
using concat_t = typename append<Ros1, Ros2>::type;

// Remove prefix from ro_string
template <typename Str, typename Prefix>
struct remove_prefix {
private:
    template <typename T1, typename T2>
    struct remove_prefix_impl;

    // when prefix is empty, return the original string
    template <typename T, T... Ts>
    struct remove_prefix_impl<ro_string<T, Ts...>, ro_string<T>> {
        using type = ro_string<T, Ts...>;
    };

    template <typename T, T t, T... Ts1, T... Ts2>
    struct remove_prefix_impl<ro_string<T, t, Ts1...>, ro_string<T, t, Ts2...>> {
        using type = typename remove_prefix_impl<ro_string<T, Ts1...>, ro_string<T, Ts2...>>::type;
    };

public:
    using type = typename remove_prefix_impl<Str, Prefix>::type; 
};

template <typename T1, typename T2>
using remove_prefix_t = typename remove_prefix<T1, T2>::type;

// TODO
// Substring

// Remove Postfix

// Contains, HasSubstring

// Equal

// Split based on pattern!

// Replace (replace portion of string with other string!)


// operator overloading required!!??
template <typename Ch, Ch... chs1, Ch... chs2>
constexpr auto operator+(ro_string<Ch, chs1...>, ro_string<Ch, chs2...>) -> ro_string<Ch, chs1..., chs2...> {
    return {};
}

}   // namespace ctl
