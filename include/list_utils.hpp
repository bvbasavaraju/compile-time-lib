#pragma once

namespace ctl {

namespace list {

// push_front
template <typename T1, typename T2>
struct push_front{
  private:
    template <typename L1, typename L2>
    struct push_front_impl;

    template <template <typename...> typename L>
    struct push_front_impl<L<>, L<>> {
      using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct push_front_impl<L<>, T> {
      using type = L<T>;
    };

    template <template <typename...> typename L, typename T>
    struct push_front_impl<T, L<>> {
      using type = L<T>;
    };

    template <template <typename...> typename L, typename ...T>
    struct push_front_impl<L<T...>, L<>> {
      using type = L<T...>;
    };

    template <template <typename...> typename L, typename ...T>
    struct push_front_impl<L<>, L<T...>> {
      using type = L<T...>;
    };

    template <template <typename...> typename L, typename ...T1s, typename T2_>
    struct push_front_impl<L<T1s...>, T2_> {
      using type = L<T2, T1s...>;
    };

    template <template <typename...> typename L, typename ...T2s, typename T1_>
    struct push_front_impl<T1_, L<T2s...>> {
      using type = L<T1_, T2s...>;
    };

    template <template <typename...> typename L, typename ...T1s, typename ...T2s>
    struct push_front_impl<L<T1s...>, L<T2s...>> {
      using type = L<T2s..., T1s...>;
    };

  public:
    using type = push_front_impl<T1, T2>::type;
};

template <typename T1, typename T2>
using push_front_t = push_front<T1, T2>::type;

// push_back
template <typename T1, typename T2>
struct push_back{
  private:
    template <typename L1, typename L2>
    struct push_back_impl;

    template <template <typename...> typename L>
    struct push_back_impl<L<>, L<>> {
      using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct push_back_impl<L<>, T> {
      using type = L<T>;
    };

    template <template <typename...> typename L, typename T>
    struct push_back_impl<T, L<>> {
      using type = L<T>;
    };

    template <template <typename...> typename L, typename ...T>
    struct push_back_impl<L<T...>, L<>> {
      using type = L<T...>;
    };

    template <template <typename...> typename L, typename ...T>
    struct push_back_impl<L<>, L<T...>> {
      using type = L<T...>;
    };

    template <template <typename...> typename L, typename ...T1s, typename T2_>
    struct push_back_impl<L<T1s...>, T2_> {
      using type = L<T1s..., T2_>;
    };

    template <template <typename...> typename L, typename ...T2s, typename T1_>
    struct push_back_impl<T1_, L<T2s...>> {
      using type = L<T2s..., T1_>;
    };

    template <template <typename...> typename L, typename ...T1s, typename ...T2s>
    struct push_back_impl<L<T1s...>, L<T2s...>> {
      using type = L<T1s..., T2s...>;
    };

  public:
    using type = push_back_impl<T1, T2>::type;
};

template <typename T1, typename T2>
using push_back_t = push_back<T1, T2>::type;

// head or first
template <typename List>
struct head {
  private:
    template <typename L>
    struct head_impl;

    template <template <typename...> typename L, typename T>
    struct head_impl<L<T>> {
      using type = T;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct head_impl<L<T, Ts...>> {
      using type = T;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct head_impl<L<Ts...>> {
      static_assert((sizeof...(Ts) > 0), "List can't be empty");
    };

  public:
    using type = head_impl<List>::type;
};

template <typename List>
using head_t = head<List>::type;

template <typename List>
using front = head<List>;

template <typename List>
using front_t = front<List>::type;

// // back or last
// template <typename List>
// struct back {
//   private:
//     template <typename L>
//     struct back_impl;

//     // template <template <typename...> typename L, typename T>
//     // struct back_impl<L<T>> {
//     //   using end_but_one = T;
//     //   using end = L<>;
//     // };

//     // template <template <typename...> typename L, typename T1, typename T2>
//     // struct back_impl<L<T1, T2>> {
//     //   using end_but_one = T1;
//     //   using end = T2;
//     // };

//     // template <template <typename...> typename L, typename T1, typename ...Ts>
//     // struct back_impl<L<T1, Ts...>> {
//     //   using end_but_one = back_impl<Ts...>::end_but_one;
//     //   using end = back_impl<Ts...>::end;
//     // };

//     template <template <typename...> typename L, typename T>
//     struct back_impl<L<T>> {
//       using type = L<>;
//     };

//     template <template <typename...> typename L, typename ...Ts, typename T>
//     struct back_impl<L<Ts..., T>> {
//       using type = T;
//     };

//     template <template <typename...> typename L, typename ...Ts>
//     struct back_impl<L<Ts...>> {
//       static_assert((sizeof...(Ts) > 0), "List can't be empty");
//     };

//   public:
//     using type = back_impl<List>::type;
//     // using type = back_impl<List>::end_but_one;
// };

// template <typename List>
// using back_t = back<List>::type;

// template <typename List>
// using last = back<List>;

// template <typename List>
// using last_t = last<List>::type;

// // init 
// template <typename List>
// struct init {
//   private:
//     template <typename L>
//     struct init_impl;

//     template <template <typename...> typename L, typename T>
//     struct init_impl<L<T>> {
//       using type = L<T>;
//     };

//     template <template <typename...> typename L, typename T, typename ...Ts>
//     struct init_impl<L<T, Ts...>> {
//       using first = L<T>;
//       using rest = init_impl<L<Ts...>> :: type;

//       using type = push_front_t<rest, first>::type;

//       // template <typename ...types>
//       // constexpr static auto rest = []() {
//       //   if constexpr (sizeof...(types) > 1) {
//       //     //using remaining_types = Ts...;
//       //     return init<types...>::type;
//       //   } else {
//       //     return L<>{};
//       //   }
//       // };

//       // using type = push_front_t<first, decltype(rest<Ts...>())>;
//     };

//     // template <template <typename...> typename L, typename ...Ts>
//     // struct init_impl<L<Ts...>> {
//     //   static_assert((sizeof...(Ts) > 0), "List can't be empty");
//     // };

//   public:
//     using type = init_impl<List>::type;
// };

// template <typename List>
// using init_t = init<List>::type;

// tail
template <typename List>
struct tail {
  private:
    template <typename L>
    struct tail_impl;

    template <template <typename...> typename L, typename T>
    struct tail_impl<L<T>> {
      using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct tail_impl<L<T, Ts...>> {
      using type = L<Ts...>;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct tail_impl<L<Ts...>> {
      static_assert((sizeof...(Ts) > 0), "List can't be empty");
    };

  public:
    using type = tail_impl<List>::type;
};

template <typename List>
using tail_t = tail<List>::type;

} // namespace list
} // namespace ctl