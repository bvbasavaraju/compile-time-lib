#pragma once

// #include <type_traits>
// #include "fold_ops.hpp"

namespace ctl {

//list
template <typename ...>
struct list;

// TODO: enable this as a new feature
// // instantiable
// template <typename ...>
// struct instantiable_types{};

// template <typename ...elements>
// struct callable_types : public instantiable_types<elements...> {

//   constexpr auto operator()() {
//     return ((elements{}()), ...);
//   }

//   //TODO: enable this for usage!
//   // constexpr auto operator()(ctl::fold_ops &fold_op) {
//   //   if constexpr (std::is_same_v<ctl::logical_or, decltype(fold_op)>) {
//   //     return ((elements{}() || ...));
//   //   } else {
//   //     return ((elements{}()), ...);
//   //   }
//   // }

//   constexpr auto operator()(auto &predicate) {
//     return ((elements{}(predicate)), ...);
//   }
// };


} // namespace ctl