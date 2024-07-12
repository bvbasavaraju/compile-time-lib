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
    // constexpr static auto get_value = []<typename T>() {
    //     // if constexpr  (has_value_v<std::decltype(inst)>) {
    //     //     return std::decltype(inst)::value;
    //     // } /*else if constexpr (is_invocable_v<std::decltype<inst>>){
    //     //     return std::decltype<inst>();
    //     // } */else {
    //     //     return inst;
    //     // }

    //     if constexpr (ctl::is_invocable_v<T>) {
    //         return T();
    //     } else {
    //         return T::value;
    //     }
    // };

  public:
    constexpr auto operator()() {
        return (types{}(), ...);
    }

    constexpr auto operator()(auto &predicate) {
        return (predicate(types{}), ...);
    }

    // bitwise
    constexpr auto operator|(auto initialVal) {
        return (initialVal | ... | types{}());
    }

    constexpr auto operator&(auto initialVal) {
        return (initialVal & ... & types{}());
    }

    constexpr auto operator^(auto initialVal) {
        return (initialVal ^ ... ^ types{}());
    }

    // logical
    constexpr auto operator||(auto inital_val) -> bool {
        return (inital_val || ... || types{}());
    }

    constexpr auto operator&&(auto inital_val) -> bool {
        return (inital_val && ... && types{}());
    }

    // Calculative
    constexpr auto operator+(auto initialVal) {
        return (initialVal + ... + types{}());
    }

    constexpr auto operator-(auto initialVal) {
        return (initialVal - ... - types{}());
    }

    constexpr auto operator*(auto initialVal) {
        return (initialVal * ... * types{}());
    }

    // TODO: other operators!!
};

}  // namespace ctl
