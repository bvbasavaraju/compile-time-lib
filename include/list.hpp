#pragma once

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

  constexpr auto operator||(auto initialVal) {
    return (types{}() || ... || initialVal);
  }

  constexpr auto operator&&(auto initialVal) {
    return (types{}() && ... && initialVal);
  }

  constexpr auto operator+(auto initialVal) {
    // return (types{}() + ... + initialVal);
    // return (types{} + ... + initialVal);
    return (types::value + ... + initialVal);
  }

  //TODO: other fold operators!!
};

} // namespace ctl
