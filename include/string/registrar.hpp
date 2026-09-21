#pragma once

#include <container/algorithms.hpp>

namespace ctl::string {

namespace details {

// Collision detection for hash values of 2 different types!!
// If the hash values are same for 2 different types then it is a collision and it should be detected at compile time!!
// If the hash values are same for same type then it is expected and should not be detected as collision!! - to avoid the false collision detection!!
template <typename ListT, typename NewT>
struct collision_detector {
    private:
        template <typename L, typename T>
        struct collision_detector_impl;

        // for empty list, no collision
        template <template <typename...> typename L, typename T>
        struct collision_detector_impl<L<>, T> {
            using type = std::false_type;
        };

        // For same type it is expected that hashes matches!!
        template <template <typename...> typename L, typename T>
        struct collision_detector_impl<L<T>, T> {
            using type = std::conditional_t<T::hash() == T::hash(), std::false_type, std::true_type>;
            static_assert(T::hash() == T::hash(), "Hash did not match for same type!!");
        };

        // For different types it is expected that hashes do not match!!
        template <template <typename...> typename L, typename T1, typename T2>
        struct collision_detector_impl<L<T1>, T2> {
            using type = std::conditional_t<T1::hash() == T2::hash(), std::true_type, std::false_type>;
            static_assert(T1::hash() != T2::hash(), "Hash matched for different types!!");
        };

        template <template <typename...> typename L, typename T1, typename T2, typename ...T1s>
        struct collision_detector_impl<L<T1, T1s...>, T2> {
            using type = std::conditional_t<collision_detector_impl<L<T1>, T2>::type::value, 
                            std::true_type, 
                            typename collision_detector_impl<L<T1s...>, T2>::type>;
        };

    public:
        using type = typename collision_detector_impl<ListT, NewT>::type;
};

template <typename ListT, typename NewT>
using collision_detector_t = typename collision_detector<ListT, NewT>::type;

template <typename ListT, typename NewT>
constexpr static auto collision_detector_v = collision_detector_t<ListT, NewT>::value;

}   // namespace details

template <typename ... T>
using registrar_t = ::ctl::list<T...>;

template <typename R, typename NewT>
struct register_entry {
    public:
    static_assert(!details::collision_detector_v<R, NewT>, "Hash collision detected!!");
    using type = remove_duplicates_t<push_back_t<R, NewT>>;
};

template <typename R, typename NewT>
using register_entry_t = typename register_entry<R, NewT>::type;

// Convenience alias for register_entry_t
#define REGISTRAR_INIT          registrar_t<>
#define REGISTRAR_ADD(R, NewT)  register_entry_t<R, NewT>
}   // namespace ctl::string
