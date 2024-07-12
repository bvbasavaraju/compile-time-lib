#pragma once

#include <utils.hpp>
// #include <type_traits>

namespace ctl {

// list
template <typename...>
struct list;

template <typename...>
struct ilist {};

template <typename... types>
struct clist : public ilist<types...> {
  private:
    constexpr static auto get_value = []<typename T>() {
        if constexpr (ctl::is_invocable_v<T>) {
            return T{}();
        } else {
            return T::value;
        }
    };

  public:
    constexpr auto operator()() {
        return (types{}(), ...);
    }

    constexpr auto operator()(auto &predicate) {
        return (predicate(types{}), ...);
    }

    // bitwise
    constexpr auto operator|(auto initialVal) {
        return (initialVal | ... | (get_value.template operator()<types>()));
    }

    constexpr auto operator&(auto initialVal) {
        return (initialVal & ... & (get_value.template operator()<types>()));
    }

    constexpr auto operator^(auto initialVal) {
        return (initialVal ^ ... ^ (get_value.template operator()<types>()));
    }

    // logical
    constexpr auto operator||(auto inital_val) -> bool {
        return (inital_val || ... || (get_value.template operator()<types>()));
    }

    constexpr auto operator&&(auto inital_val) -> bool {
        return (inital_val && ... && (get_value.template operator()<types>()));
    }

    // Calculative
    constexpr auto operator+(auto initialVal) {
        return (initialVal + ... + (get_value.template operator()<types>()));
    }

    constexpr auto operator-(auto initialVal) {
        return (initialVal - ... - (get_value.template operator()<types>()));
    }

    constexpr auto operator*(auto initialVal) {
        return (initialVal * ... * (get_value.template operator()<types>()));
    }

    // TODO: other operators!!
};

}  // namespace ctl
