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

// first
template <typename List>
struct first {
  private:
    template <typename L>
    struct first_impl;

    template <template <typename...> typename L, typename T>
    struct first_impl<L<T>> {
      using type = T;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct first_impl<L<T, Ts...>> {
      using type = T;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct first_impl<L<Ts...>> {
      static_assert((sizeof...(Ts) > 0), "List can't be empty");
    };

  public:
    using type = first_impl<List>::type;
};

template <typename List>
using first_t = first<List>::type;

// last
template <typename List>
struct last {
  private:
    template <typename L>
    struct last_impl;

    template <template <typename...> typename L, typename T>
    struct last_impl<L<T>> {
      using type = T;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct last_impl<L<T, Ts...>> {
      using type = last_impl<L<Ts...>>::type;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct last_impl<L<Ts...>> {
      static_assert((sizeof...(Ts) > 0), "List can't be empty");
    };

  public:
    using type = last_impl<List>::type;
};

template <typename List>
using last_t = last<List>::type;

// head or front
template <typename List>
struct head {
  private:
    template <typename L>
    struct head_impl;

    template <template <typename...> typename L, typename T>
    struct head_impl<L<T>> {
      using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct head_impl<L<T, Ts...>> {
      using first = L<T>;
      using rest = head_impl<L<Ts...>>::type;
      using type = push_back_t<first, rest>;
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

// tail or back
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

template <typename List>
using back = tail<List>;

template <typename List>
using back_t = back<List>::type;

} // namespace list
} // namespace ctl