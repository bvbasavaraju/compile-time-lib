#pragma once

namespace ctl {

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

template <typename T1, typename T2>
using append = push_back<T1, T2>;

template <typename T1, typename T2>
using append_t = push_back<T1, T2>::type;

// first
template <typename types>
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
      static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = first_impl<types>::type;
};

template <typename types>
using first_t = first<types>::type;

// last
template <typename types>
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
      static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = last_impl<types>::type;
};

template <typename types>
using last_t = last<types>::type;

// head or front
template <typename types>
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
      static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = head_impl<types>::type;
};

template <typename types>
using head_t = head<types>::type;

template <typename types>
using front = head<types>;

template <typename types>
using front_t = front<types>::type;

template <typename types>
using pop_back = head<types>;

template <typename types>
using pop_back_t = pop_back<types>::type;

// tail or back
template <typename types>
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
      static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = tail_impl<types>::type;
};

template <typename types>
using tail_t = tail<types>::type;

template <typename types>
using back = tail<types>;

template <typename types>
using back_t = back<types>::type;

template <typename types>
using pop_front = tail<types>;

template <typename types>
using pop_front_t = pop_front<types>::type;

template <typename types>
struct size {
  private:
    template <typename L>
    struct size_impl;

    template <template <typename...> typename L>
    struct size_impl<L<>> {
      using type = std::integral_constant<uint32_t, 0>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct size_impl<L<T, Ts...>> {
      using type = std::integral_constant<uint32_t, 1 + size_impl<L<Ts...>>::type::value>;
    };

  public:
    using type = size_impl<types>::type;
};

template <typename types>
using size_t = size<types>::type;

template <typename types>
using count = size<types>;

template <typename types>
using count_t = count<types>::type;

template <typename types>
struct empty {
  private:
    template <typename L>
    struct empty_impl;

    template <template <typename...> typename L>
    struct empty_impl<L<>> {
      // using type = std::integral_constant<bool, true>;
      using type = std::true_type;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct empty_impl<L<Ts...>> {
      // using type = std::integral_constant<bool, false>;
      using type = std::false_type;
    };

  public:
    using type = empty_impl<types>::type;
};

template <typename types>
using empty_t = empty<types>::type;

template <typename types>
struct clear {
  private:
    template <typename L>
    struct clear_impl;

    template <template <typename...> typename L>
    struct clear_impl<L<>> {
      using type = L<>;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct clear_impl<L<Ts...>> {
      using type = L<>;
    };

  public:
    using type = clear_impl<types>::type;
};

template <typename types>
using clear_t = clear<types>::type;

} // namespace ctl