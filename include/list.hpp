#pragma once

#include <fold_ops.hpp>

namespace ctl {

//list
template <typename ...>
struct list;

template <typename ...>
struct ilist{};

template <typename ...types>
struct clist : public ilist<types...> {
  constexpr auto operator()() {
    return (types{}(), ...);
  }

  constexpr auto operator()(auto &predicate) {
    return (predicate(types{}), ...);
  }

  // constexpr auto operator||(auto inital_val) -> bool {
  //   return (types{} || ... || inital_val);
  // }

  template <typename T>
  constexpr auto operator()(T &&);

  constexpr auto operator()(ctl::fold_ops::logical_or) {
    return (types{} || ... || false);
  }

  constexpr auto operator()(ctl::fold_ops::logical_and) {
    return (types{} && ... && true);
  }

  //TODO: other fold operators!!
};

} // namespace ctl