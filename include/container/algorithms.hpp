#pragma once

#include <cstdint>

#include <utils.hpp>

namespace ctl {

// rename
template <typename types, template <typename...> typename new_types>
struct rename {
  private:
    template <typename L>
    struct rename_impl;

    template <template <typename...> typename L>
    struct rename_impl<L<>> {
        using type = new_types<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct rename_impl<L<Ts...>> {
        using type = new_types<Ts...>;
    };

  public:
    using type = typename rename_impl<types>::type;
};

template <typename types, template <typename...> typename new_types>
using rename_t = typename rename<types, new_types>::type;

// apply
template <template <typename...> typename F, typename types>
using apply = rename<types, F>;

template <template <typename...> typename F, typename types>
using apply_t = typename apply<F, types>::type;

// push_front
template <typename T1, typename T2>
struct push_front {
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

    template <template <typename...> typename L, typename... T>
    struct push_front_impl<L<T...>, L<>> {
        using type = L<T...>;
    };

    template <template <typename...> typename L, typename... T>
    struct push_front_impl<L<>, L<T...>> {
        using type = L<T...>;
    };

    template <template <typename...> typename L, typename... T1s, typename T2_>
    struct push_front_impl<L<T1s...>, T2_> {
        using type = L<T2, T1s...>;
    };

    template <template <typename...> typename L, typename... T2s, typename T1_>
    struct push_front_impl<T1_, L<T2s...>> {
        using type = L<T1_, T2s...>;
    };

    template <template <typename...> typename L, typename... T1s, typename... T2s>
    struct push_front_impl<L<T1s...>, L<T2s...>> {
        using type = L<T2s..., T1s...>;
    };

  public:
    using type = typename push_front_impl<T1, T2>::type;
};

template <typename T1, typename T2>
using push_front_t = typename push_front<T1, T2>::type;

// push_back
template <typename T1, typename T2>
struct push_back {
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

    template <template <typename...> typename L, typename... T>
    struct push_back_impl<L<T...>, L<>> {
        using type = L<T...>;
    };

    template <template <typename...> typename L, typename... T>
    struct push_back_impl<L<>, L<T...>> {
        using type = L<T...>;
    };

    template <template <typename...> typename L, typename... T1s, typename T2_>
    struct push_back_impl<L<T1s...>, T2_> {
        using type = L<T1s..., T2_>;
    };

    template <template <typename...> typename L, typename... T2s, typename T1_>
    struct push_back_impl<T1_, L<T2s...>> {
        using type = L<T2s..., T1_>;
    };

    template <template <typename...> typename L, typename... T1s, typename... T2s>
    struct push_back_impl<L<T1s...>, L<T2s...>> {
        using type = L<T1s..., T2s...>;
    };

  public:
    using type = typename push_back_impl<T1, T2>::type;
};

template <typename T1, typename T2>
using push_back_t = typename push_back<T1, T2>::type;

template <typename T1, typename T2>
using append = push_back<T1, T2>;

template <typename T1, typename T2>
using append_t = typename push_back<T1, T2>::type;

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

    template <template <typename...> typename L, typename T, typename... Ts>
    struct first_impl<L<T, Ts...>> {
        using type = T;
    };

    template <template <typename...> typename L, typename... Ts>
    struct first_impl<L<Ts...>> {
        static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = typename first_impl<types>::type;
};

template <typename types>
using first_t = typename first<types>::type;

template <typename types>
using front = first<types>;

template <typename types>
using front_t = typename front<types>::type;

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

    template <template <typename...> typename L, typename T, typename... Ts>
    struct last_impl<L<T, Ts...>> {
        using type = typename last_impl<L<Ts...>>::type;
    };

    template <template <typename...> typename L, typename... Ts>
    struct last_impl<L<Ts...>> {
        static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = typename last_impl<types>::type;
};

template <typename types>
using last_t = typename last<types>::type;

template <typename types>
using back = last<types>;

template <typename types>
using back_t = typename back<types>::type;

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

    template <template <typename...> typename L, typename T, typename... Ts>
    struct head_impl<L<T, Ts...>> {
        using first = L<T>;
        using rest = typename head_impl<L<Ts...>>::type;
        using type = push_back_t<first, rest>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct head_impl<L<Ts...>> {
        static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = typename head_impl<types>::type;
};

template <typename types>
using head_t = typename head<types>::type;

template <typename types>
using pop_back = head<types>;

template <typename types>
using pop_back_t = typename pop_back<types>::type;

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

    template <template <typename...> typename L, typename T, typename... Ts>
    struct tail_impl<L<T, Ts...>> {
        using type = L<Ts...>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct tail_impl<L<Ts...>> {
        static_assert((sizeof...(Ts) > 0), "types can't be empty");
    };

  public:
    using type = typename tail_impl<types>::type;
};

template <typename types>
using tail_t = typename tail<types>::type;

template <typename types>
using pop_front = tail<types>;

template <typename types>
using pop_front_t = typename pop_front<types>::type;

// size
template <typename types>
struct size {
  private:
    template <typename L>
    struct size_impl;

    template <template <typename...> typename L>
    struct size_impl<L<>> {
        using type = std::integral_constant<uint32_t, 0>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct size_impl<L<T, Ts...>> {
        using type = std::integral_constant<uint32_t, 1 + size_impl<L<Ts...>>::type::value>;
    };

  public:
    using type = typename size_impl<types>::type;
};

template <typename types>
using size_t = typename size<types>::type;

// count
template <typename types>
using count = size<types>;

template <typename types>
using count_t = typename count<types>::type;

template <typename types, template <typename...> typename P>
struct count_if {
  private:
    template <bool C, typename L>
    struct count_if_true {
        using type = std::integral_constant<uint32_t, 0>;
    };

    template <typename L>
    struct count_if_true<true, L> {
        using type = std::integral_constant<uint32_t, 1>;
    };

    // count_if_impl
    template <typename L>
    struct count_if_impl;

    template <template <typename...> typename L>
    struct count_if_impl<L<>> {
        using type = std::integral_constant<uint32_t, 0>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct count_if_impl<L<T, Ts...>> {
        using first = typename count_if_true<P<T>::value, L<T, Ts...>>::type;
        using rest = typename count_if_impl<L<Ts...>>::type;

        using type = std::integral_constant<uint32_t, (first::value + rest::value)>;
    };

  public:
    using type = typename count_if_impl<types>::type;
};

template <typename types, template <typename...> typename P>
using count_if_t = typename count_if<types, P>::type;

template <typename types, typename QMF>
using count_if_qmf = count_if<types, QMF::template fn>;

template <typename types, typename QMF>
using count_if_qmf_t = typename count_if_qmf<types, QMF>::type;

// empty
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

    template <template <typename...> typename L, typename... Ts>
    struct empty_impl<L<Ts...>> {
        // using type = std::integral_constant<bool, false>;
        using type = std::false_type;
    };

  public:
    using type = typename empty_impl<types>::type;
};

template <typename types>
using empty_t = typename empty<types>::type;

// clear
template <typename types>
struct clear {
  private:
    template <typename L>
    struct clear_impl;

    template <template <typename...> typename L>
    struct clear_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct clear_impl<L<Ts...>> {
        using type = L<>;
    };

  public:
    using type = typename clear_impl<types>::type;
};

template <typename types>
using clear_t = typename clear<types>::type;

// remove type
template <typename types, typename type_to_remove>
struct remove_type {
  private:
    template <typename U, typename L>
    struct remove_type_impl;

    template <typename T, template <typename...> typename L>
    struct remove_type_impl<T, L<>> {
        using type = L<>;
    };

    template <typename T, template <typename...> typename L>
    struct remove_type_impl<T, L<T>> {
        using type = L<>;
    };

    template <typename U, template <typename...> typename L, typename T>
    struct remove_type_impl<U, L<T>> {
        using type = L<T>;
    };

    template <typename U, template <typename...> typename L, typename T, typename... Ts>
    struct remove_type_impl<U, L<T, Ts...>> {
        using first = typename remove_type_impl<U, L<T>>::type;
        using rest = typename remove_type<L<Ts...>, U>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename remove_type_impl<type_to_remove, types>::type;
};

template <typename types, typename type_to_remove>
using remove_type_t = typename remove_type<types, type_to_remove>::type;

// remove if
template <typename types, template <typename...> typename P>
struct remove_if {
  private:
    template <typename L>
    struct remove_if_impl;

    template <template <typename...> typename L>
    struct remove_if_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct remove_if_impl<L<T>> {
        using type = select_t<P<T>, L<>, L<T>>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct remove_if_impl<L<T, Ts...>> {
        using first = typename remove_if_impl<L<T>>::type;
        using rest = typename remove_if_impl<L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename remove_if_impl<types>::type;
};

template <typename types, template <typename...> typename P>
using remove_if_t = typename remove_if<types, P>::type;

template <typename types, typename QMF>
using remove_if_qmf = remove_if<types, QMF::template fn>;

template <typename types, typename QMF>
using remove_if_qmf_t = typename remove_if_qmf<types, QMF>::type;

// remove duplicates
template <typename types>
struct remove_duplicates {
  private:
    template <typename L>
    struct remove_duplicates_impl;

    template <template <typename...> typename L>
    struct remove_duplicates_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct remove_duplicates_impl<L<T>> {
        using type = L<T>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct remove_duplicates_impl<L<T, Ts...>> {
        using first = remove_type_t<L<Ts...>, T>;
        using rest = typename remove_duplicates<first>::type;

        using type = push_back_t<L<T>, rest>;
    };

  public:
    using type = typename remove_duplicates_impl<types>::type;
};

template <typename types>
using remove_duplicates_t = typename remove_duplicates<types>::type;

// reverse
template <typename types>
struct reverse {
  private:
    template <typename L>
    struct reverse_impl;

    template <template <typename...> typename L>
    struct reverse_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct reverse_impl<L<T>> {
        using type = L<T>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct reverse_impl<L<T, Ts...>> {
        using rest = typename reverse<L<Ts...>>::type;

        using type = push_back_t<rest, L<T>>;
    };

  public:
    using type = typename reverse_impl<types>::type;
};

template <typename types>
using reverse_t = typename reverse<types>::type;

// at
template <typename types, std::size_t N>
struct at_c {
  private:
    template <std::size_t U, typename L>
    struct at_c_impl;

    template <template <typename...> typename L, typename T, typename... Ts>
    struct at_c_impl<0, L<T, Ts...>> {
        using type = T;
    };

    template <std::size_t index, template <typename...> typename L, typename T, typename... Ts>
    struct at_c_impl<index, L<T, Ts...>> {
        using type = typename at_c_impl<index - 1, L<Ts...>>::type;
    };

    template <std::size_t U, template <typename...> typename L, typename... Ts>
    struct at_c_impl<U, L<Ts...>> {
        static_assert((U >= sizeof...(Ts)),
                      "Index should be within size of the container of types");
        static_assert((U < 0), "Index cannot be negative");
    };

  public:
    using type = typename at_c_impl<N, types>::type;
};

template <typename types, std::size_t N>
using at_c_t = typename at_c<types, N>::type;

template <typename types, typename N>
using at = at_c<types, N::value>;

template <typename types, typename N>
using at_t = typename at<types, N>::type;

// filter
template <typename types, template <typename...> typename P>
struct filter_if {
  private:
    template <typename L>
    struct filter_if_impl;

    template <template <typename...> typename L>
    struct filter_if_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct filter_if_impl<L<T>> {
        using type = select_t<P<T>, L<T>, L<>>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct filter_if_impl<L<T, Ts...>> {
        using first = typename filter_if_impl<L<T>>::type;
        using rest = typename filter_if_impl<L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename filter_if_impl<types>::type;
};

template <typename types, template <typename...> typename P>
using filter_if_t = typename filter_if<types, P>::type;

template <typename types, typename QMF>
using filter_if_qmf = filter_if<types, QMF::template fn>;

template <typename types, typename QMF>
using filter_if_qmf_t = typename filter_if_qmf<types, QMF>::type;

// contains
template <typename types, typename type_to_search>
struct contains {
  private:
    template <typename U, typename L>
    struct contains_impl;

    template <typename U, template <typename...> typename L>
    struct contains_impl<U, L<>> {
        using type = std::false_type;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct contains_impl<T, L<T, Ts...>> {
        using type = std::true_type;
    };

    template <typename U, template <typename...> typename L, typename T, typename... Ts>
    struct contains_impl<U, L<T, Ts...>> {
        using type = typename contains_impl<U, L<Ts...>>::type;
    };

  public:
    using type = typename contains_impl<type_to_search, types>::type;
};

template <typename types, typename type_to_search>
using contains_t = typename contains<types, type_to_search>::type;

// drop
template <typename types, std::size_t N>
struct drop_c {
  private:
    template <std::size_t count, typename L>
    struct drop_c_impl;

    template <std::size_t count, template <typename...> typename L>
    struct drop_c_impl<count, L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L>
    struct drop_c_impl<0, L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct drop_c_impl<0, L<Ts...>> {
        using type = L<Ts...>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct drop_c_impl<0, L<T, Ts...>> {
        using type = L<T, Ts...>;
    };

    template <std::size_t count, template <typename...> typename L, typename T, typename... Ts>
    struct drop_c_impl<count, L<T, Ts...>> {
        using type = typename drop_c_impl<count - 1, L<Ts...>>::type;
    };

  public:
    using type = typename drop_c_impl<N, types>::type;
};

template <typename types, std::size_t N>
using drop_c_t = typename drop_c<types, N>::type;

template <typename types, typename N>
using drop = drop_c<types, N::value>;

template <typename types, typename N>
using drop_t = typename drop<types, N>::type;

// take
template <typename types, std::size_t N>
struct take_c {
  private:
    template <std::size_t count, typename L>
    struct take_c_impl;

    template <template <typename...> typename L>
    struct take_c_impl<0, L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct take_c_impl<0, L<Ts...>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct take_c_impl<0, L<T, Ts...>> {
        using type = L<>;
    };

    template <std::size_t count, template <typename...> typename L>
    struct take_c_impl<count, L<>> {
        using type = L<>;
    };

    template <std::size_t count, template <typename...> typename L, typename T, typename... Ts>
    struct take_c_impl<count, L<T, Ts...>> {
        using first = L<T>;
        using rest = typename take_c_impl<count - 1, L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename take_c_impl<N, types>::type;
};

template <typename types, std::size_t N>
using take_c_t = typename take_c<types, N>::type;

template <typename types, typename N>
using take = take_c<types, N::value>;

template <typename types, typename N>
using take_t = typename take<types, N>::type;

// repeat
template <typename types, std::size_t N>
struct repeat_c {
  private:
    template <std::size_t count, typename Lo, typename Lr>
    struct repeat_c_impl;

    template <template <typename...> typename L>
    struct repeat_c_impl<0, L<>, L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct repeat_c_impl<0, L<Ts...>, L<Ts...>> {
        using type = L<>;
    };

    template <template <typename...> typename L>
    struct repeat_c_impl<1, L<>, L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Tos, typename... Trs>
    struct repeat_c_impl<1, L<Tos...>, L<Trs...>> {
        using type = L<Trs...>;
    };

    template <std::size_t count, template <typename...> typename L, typename... Tos,
              typename... Trs>
    struct repeat_c_impl<count, L<Tos...>, L<Trs...>> {
        using repeated = L<Tos..., Trs...>;
        using type = typename repeat_c_impl<count - 1, L<Tos...>, repeated>::type;
    };

  public:
    using type = typename repeat_c_impl<N, types, types>::type;
};

template <typename types, std::size_t N>
using repeat_c_t = typename repeat_c<types, N>::type;

template <typename types, typename N>
using repeat = repeat_c<types, N::value>;

template <typename types, typename N>
using repeat_t = typename repeat<types, N>::type;

// from integer sequence
template <typename sequence, template <typename...> typename result_type = std::tuple>
struct from_integer_sequence {
  private:
    /*
      Note: Sequence is of type
      template< class T, T... Ints >
      class integer_sequence;

      It is not a simple list of types. It is data type and values of those type.

      Hence while expanding the sequence, we should actually mention its signature appropriately.
      like:
        + template <typename T, T... vs> typename S => sequence type
        + typename DT => data type in sequence
        + DT ...vals => value(s) of DT
    */

    template <std::size_t N, typename S>
    struct from_integer_sequence_impl;

    template <template <typename T, T... vs> typename S, typename DT, DT... vals>
    struct from_integer_sequence_impl<0, S<DT, vals...>> {
        using type = result_type<>;
    };

    template <std::size_t N, template <typename T, T... vs> typename S, typename DT, DT val,
              DT... vals>
    struct from_integer_sequence_impl<N, S<DT, val, vals...>> {
        using first = result_type<std::integral_constant<DT, val>>;
        using rest = typename from_integer_sequence_impl<N - 1, S<DT, vals...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename from_integer_sequence_impl<sequence::size(), sequence>::type;
};

template <typename sequence, template <typename...> typename result_type = std::tuple>
using from_integer_sequence_t = typename from_integer_sequence<sequence, result_type>::type;

// iota
template <std::size_t N, typename DT = uint32_t,
          template <typename...> typename result_type = std::tuple>
struct iota_c {
  private:
    template <std::size_t count, typename T>
    struct iota_c_impl {
        using first = result_type<std::integral_constant<T, count - 1>>;
        using rest = typename iota_c_impl<count - 1, T>::type;

        using type = push_front_t<first, rest>;
    };

    template <typename T>
    struct iota_c_impl<0, T> {
        using type = result_type<>;
    };

    template <typename T>
    struct iota_c_impl<1, T> {
        using type = result_type<std::integral_constant<T, 0>>;
    };

  public:
    using type = typename iota_c_impl<N, DT>::type;
};

template <std::size_t N, typename DT = uint32_t,
          template <typename...> typename result_type = std::tuple>
using iota_c_t = typename iota_c<N, DT, result_type>::type;

template <typename N, template <typename...> typename result_type = std::tuple>
using iota = iota_c<N::value, typename N::value_type, result_type>;

template <typename N, template <typename...> typename result_type = std::tuple>
using iota_t = typename iota<N, result_type>::type;

// insert
template <typename types, std::size_t P, typename... types_to_insert>
struct insert_c {
  private:
    template <typename L, typename... IT>
    struct insert_c_impl;

    template <template <typename...> typename L, typename... Ts, typename... IT>
    struct insert_c_impl<L<Ts...>, IT...> {
        static_assert(P <= size_t<L<Ts...>>::value,
                      "Position has to be less than size of the container");

        using front = take_c_t<L<Ts...>, P>;
        using back = drop_c_t<L<Ts...>, P>;

        using type_ = push_back_t<front, L<IT...>>;
        using type = push_back_t<type_, back>;
    };

  public:
    using type = typename insert_c_impl<types, types_to_insert...>::type;
};

template <typename types, std::size_t P, typename... types_to_insert>
using insert_c_t = typename insert_c<types, P, types_to_insert...>::type;

template <typename types, typename P, typename... types_to_insert>
using insert = insert_c<types, P::value, types_to_insert...>;

template <typename types, typename P, typename... types_to_insert>
using insert_t = typename insert<types, P, types_to_insert...>::type;

// erase
template <typename types, std::size_t P1, std::size_t P2>
struct erase_c {
  private:
    template <typename L>
    struct erase_c_impl;

    template <template <typename...> typename L, typename... Ts>
    struct erase_c_impl<L<Ts...>> {
        static_assert(sizeof...(Ts) > 0, "container cannot be empty");
        static_assert(P1 < size_t<L<Ts...>>::value,
                      "Position 1 has to be less than size of the container");
        static_assert(P2 <= size_t<L<Ts...>>::value,
                      "Position 2 has to be less than size of the container");

        static_assert(P1 < P2, "Position 1 > Position 2");

        using front = take_c_t<L<Ts...>, P1>;
        using back = drop_c_t<L<Ts...>, P2>;

        using type = push_back_t<front, back>;
    };

  public:
    using type = typename erase_c_impl<types>::type;
};

template <typename types, std::size_t P1, std::size_t P2>
using erase_c_t = typename erase_c<types, P1, P2>::type;

template <typename types, typename P1, typename P2>
using erase = erase_c<types, P1::value, P2::value>;

template <typename types, typename P1, typename P2>
using erase_t = typename erase<types, P1, P2>::type;

// replace
template <typename types, typename type_to_replace, typename replace_with>
struct replace {
  private:
    template <typename TR, typename L>
    struct replace_impl;

    template <typename TR, template <typename...> typename L>
    struct replace_impl<TR, L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct replace_impl<T, L<T, Ts...>> {
        using first = L<replace_with>;
        using rest = typename replace_impl<T, L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

    template <typename TR, template <typename...> typename L, typename T, typename... Ts>
    struct replace_impl<TR, L<T, Ts...>> {
        using first = L<T>;
        using rest = typename replace_impl<TR, L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename replace_impl<type_to_replace, types>::type;
};

template <typename types, typename type_to_replace, typename replace_with>
using replace_t = typename replace<types, type_to_replace, replace_with>::type;

// replace at
template <typename types, std::size_t P, typename replace_with>
struct replace_at_c {
  private:
    template <typename L>
    struct replace_at_c_impl;

    template <template <typename...> typename L>
    struct replace_at_c_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct replace_at_c_impl<L<Ts...>> {
        static_assert(P < size_t<L<Ts...>>::value,
                      "Position has to be less than size of the container");

        using front = take_c_t<L<Ts...>, P>;
        using replaced = L<replace_with>;
        using back = drop_c_t<L<Ts...>, P + 1>;

        using type_ = push_back_t<front, replaced>;
        using type = push_back_t<type_, back>;
    };

  public:
    using type = typename replace_at_c_impl<types>::type;
};

template <typename types, std::size_t P, typename replace_with>
using replace_at_c_t = typename replace_at_c<types, P, replace_with>::type;

template <typename types, typename P, typename replace_with>
using replace_at = replace_at_c<types, P::value, replace_with>;

template <typename types, typename P, typename replace_with>
using replace_at_t = typename replace_at<types, P, replace_with>::type;

// replace if
template <typename types, template <typename...> typename P, typename replace_with>
struct replace_if {
  private:
    template <typename L>
    struct replace_if_impl;

    template <template <typename...> typename L>
    struct replace_if_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct replace_if_impl<L<T>> {
        using type = select_t<P<T>, L<replace_with>, L<T>>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct replace_if_impl<L<T, Ts...>> {
        using first = typename replace_if_impl<L<T>>::type;
        using rest = typename replace_if_impl<L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename replace_if_impl<types>::type;
};

template <typename types, template <typename...> typename P, typename replace_with>
using replace_if_t = typename replace_if<types, P, replace_with>::type;

template <typename types, typename QMF, typename replace_with>
using replace_if_qmf = replace_if<types, QMF::template fn, replace_with>;

template <typename types, typename QMF, typename replace_with>
using replace_if_qmf_t = typename replace_if_qmf<types, QMF, replace_with>::type;

// rotate left
template <typename types, std::size_t N>
struct rotate_left_c {
  private:
    template <typename L>
    struct rotate_left_c_impl;

    template <template <typename...> typename L>
    struct rotate_left_c_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct rotate_left_c_impl<L<Ts...>> {
        constexpr static auto len = size_t<L<Ts...>>::value;
        constexpr static auto count = N % len;

        using first = take_c_t<L<Ts...>, count>;
        using rest = drop_c_t<L<Ts...>, count>;

        using type = push_back_t<rest, first>;
    };

  public:
    using type = typename rotate_left_c_impl<types>::type;
};

template <typename types, std::size_t N>
using rotate_left_c_t = typename rotate_left_c<types, N>::type;

template <typename types, typename N>
using rotate_left = rotate_left_c<types, N::value>;

template <typename types, typename N>
using rotate_left_t = typename rotate_left<types, N>::type;

// rotate right
template <typename types, std::size_t N>
struct rotate_right_c {
  private:
    template <typename L>
    struct rotate_right_c_impl;

    template <template <typename...> typename L>
    struct rotate_right_c_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename... Ts>
    struct rotate_right_c_impl<L<Ts...>> {
        constexpr static auto len = size_t<L<Ts...>>::value;
        constexpr static auto count = N % len;

        using first = take_c_t<L<Ts...>, len - count>;
        using rest = drop_c_t<L<Ts...>, len - count>;

        using type = push_back_t<rest, first>;
    };

  public:
    using type = typename rotate_right_c_impl<types>::type;
};

template <typename types, std::size_t N>
using rotate_right_c_t = typename rotate_right_c<types, N>::type;

template <typename types, typename N>
using rotate_right = rotate_right_c<types, N::value>;

template <typename types, typename N>
using rotate_right_t = typename rotate_right<types, N>::type;

// copy if
template <typename types, template <typename...> typename P>
using copy_if = filter_if<types, P>;

template <typename types, template <typename...> typename P>
using copy_if_t = typename copy_if<types, P>::type;

template <typename types, typename QMF>
using copy_if_qmf = copy_if<types, QMF::template fn>;

template <typename types, typename QMF>
using copy_if_qmf_t = typename copy_if_qmf<types, QMF>::type;

// find
template <typename types, typename type_to_find>
struct find {
  private:
    template <typename U, typename L>
    struct find_impl;

    template <typename U, template <typename...> typename L>
    struct find_impl<U, L<>> {
        constexpr static uint32_t value = 0;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct find_impl<T, L<T, Ts...>> {
        constexpr static uint32_t value = 0;
    };

    template <typename U, template <typename...> typename L, typename T, typename... Ts>
    struct find_impl<U, L<T, Ts...>> {
        constexpr static uint32_t value = 1 + find_impl<U, L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<uint32_t, find_impl<type_to_find, types>::value>;
};

template <typename types, typename type_to_find>
using find_t = typename find<types, type_to_find>::type;

// find if
template <typename types, template <typename...> typename P>
struct find_if {
  private:
    // with condition
    template <bool C, typename L>
    struct find_if_cond_impl;

    template <template <typename...> typename L>
    struct find_if_cond_impl<true, L<>> {
        constexpr static uint32_t value = 0;
    };

    template <template <typename...> typename L>
    struct find_if_cond_impl<false, L<>> {
        constexpr static uint32_t value = 1;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct find_if_cond_impl<true, L<T, Ts...>> {
        constexpr static uint32_t value = 0;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct find_if_cond_impl<false, L<T, Ts...>> {
        constexpr static uint32_t value = 1 + find_if_cond_impl<P<T>::value, L<Ts...>>::value;
    };

    // without condition
    template <typename L>
    struct find_if_impl;

    template <template <typename...> typename L>
    struct find_if_impl<L<>> {
        constexpr static uint32_t value = 0;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct find_if_impl<L<T, Ts...>> {
        constexpr static uint32_t value = find_if_cond_impl<P<T>::value, L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<uint32_t, find_if_impl<types>::value>;
};

template <typename types, template <typename...> typename P>
using find_if_t = typename find_if<types, P>::type;

template <typename types, typename QMF>
using find_if_qmf = typename find_if<types, QMF::template fn>::type;

template <typename types, typename QMF>
using find_if_qmf_t = typename find_if_qmf<types, QMF>::type;

// unique
template <typename types>
using unique = remove_duplicates<types>;

template <typename types>
using unique_t = typename unique<types>::type;

template <typename types, template <typename...> typename P>
using unique_if = remove_if<types, P>;

template <typename types, template <typename...> typename P>
using unique_if_t = typename unique_if<types, P>::type;

template <typename types, typename QMF>
using unique_if_qmf = remove_if_qmf<types, QMF>;

template <typename types, typename QMF>
using unique_if_qmf_t = typename unique_if_qmf<types, QMF>::type;

// all of
template <typename types, template <typename...> typename P>
struct all_of {
  private:
    template <typename L>
    struct all_of_impl;

    template <template <typename...> typename L>
    struct all_of_impl<L<>> {
        constexpr static bool value = true;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct all_of_impl<L<T, Ts...>> {
        constexpr static bool value = P<T>::value && all_of_impl<L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<bool, all_of_impl<types>::value>;
};

template <typename types, template <typename...> typename P>
using all_of_t = typename all_of<types, P>::type;

template <typename types, typename QMF>
using all_of_qmf = all_of<types, QMF::template fn>;

template <typename types, typename QMF>
using all_of_qmf_t = typename all_of_qmf<types, QMF>::type;

// any of
template <typename types, template <typename...> typename P>
struct any_of {
  private:
    template <typename L>
    struct any_of_impl;

    template <template <typename...> typename L>
    struct any_of_impl<L<>> {
        constexpr static bool value = false;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct any_of_impl<L<T, Ts...>> {
        constexpr static bool value = P<T>::value || any_of_impl<L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<bool, any_of_impl<types>::value>;
};

template <typename types, template <typename...> typename P>
using any_of_t = typename any_of<types, P>::type;

template <typename types, typename QMF>
using any_of_qmf = any_of<types, QMF::template fn>;

template <typename types, typename QMF>
using any_of_qmf_t = typename any_of_qmf<types, QMF>::type;

// none of
template <typename types, template <typename...> typename P>
struct none_of {
  private:
    template <typename L>
    struct none_of_impl;

    template <template <typename...> typename L>
    struct none_of_impl<L<>> {
        constexpr static bool value = true;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct none_of_impl<L<T, Ts...>> {
        constexpr static bool value = (!P<T>::value) && none_of_impl<L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<bool, none_of_impl<types>::value>;
};

template <typename types, template <typename...> typename P>
using none_of_t = typename none_of<types, P>::type;

template <typename types, typename QMF>
using none_of_qmf = none_of<types, QMF::template fn>;

template <typename types, typename QMF>
using none_of_qmf_t = typename none_of_qmf<types, QMF>::type;

// transform
template <typename types, template <typename...> typename F>
struct transform {
  private:
    template <typename L>
    struct transform_impl;

    template <template <typename...> typename L>
    struct transform_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct transform_impl<L<T>> {
        using type = L<F<T>>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct transform_impl<L<T, Ts...>> {
        using first = L<F<T>>;
        using rest = typename transform_impl<L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename transform_impl<types>::type;
};

template <typename types, template <typename...> typename F>
using transform_t = typename transform<types, F>::type;

template <typename types, typename QMF>
using transform_qmf = transform<types, QMF::template fn>;

template <typename types, typename QMF>
using transform_qmf_t = typename transform_qmf<types, QMF>::type;

// transform if
template <typename types, template <typename...> typename F, template <typename...> typename P>
struct transform_if {
  private:
    template <typename L>
    struct transform_if_impl;

    template <template <typename...> typename L>
    struct transform_if_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct transform_if_impl<L<T>> {
        using type = select_t<P<T>, L<F<T>>, L<>>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct transform_if_impl<L<T, Ts...>> {
        using first = select_t<P<T>, L<F<T>>, L<>>;
        using rest = typename transform_if_impl<L<Ts...>>::type;

        using type = push_back_t<first, rest>;
    };

  public:
    using type = typename transform_if_impl<types>::type;
};

template <typename types, template <typename...> typename F, template <typename...> typename P>
using transform_if_t = typename transform_if<types, F, P>::type;

template <typename types, typename QMFf, typename QMFp>
using transform_if_qmf = transform_if<types, QMFf::template fn, QMFp::template fn>;

template <typename types, typename QMFf, typename QMFp>
using transform_if_qmf_t = typename transform_if_qmf<types, QMFf, QMFp>::type;

// sort
template <typename types>
struct sort {
  private:
    template <typename L>
    struct sort_impl;

    template <template <typename...> typename L>
    struct sort_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct sort_impl<L<T, Ts...>> {
        template <typename U>
        struct lesser_filter_pred {
            constexpr static bool value = U::value <= T::value ? true : false;
        };

        template <typename U>
        struct greater_filter_pred {
            constexpr static bool value = U::value > T::value ? true : false;
        };

        using lesser = filter_if_t<L<Ts...>, lesser_filter_pred>;
        using greater = filter_if_t<L<Ts...>, greater_filter_pred>;

        using sorted_left = typename sort<lesser>::type;
        using sorted_right = typename sort<greater>::type;

        using type_ = push_back_t<sorted_left, L<T>>;
        using type = push_back_t<type_, sorted_right>;
    };

  public:
    using type = typename sort_impl<types>::type;
};

template <typename types>
using sort_t = typename sort<types>::type;

// sort
template <typename types, template <typename...> typename comparator>
struct sort_p {
  private:
    template <typename L>
    struct sort_p_impl;

    template <template <typename...> typename L>
    struct sort_p_impl<L<>> {
        using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename... Ts>
    struct sort_p_impl<L<T, Ts...>> {
        template <typename U>
        struct lesser_filter_pred {
            constexpr static bool value = comparator<U, T>::value;
        };

        template <typename U>
        struct greater_filter_pred {
            constexpr static bool value = invert_t<comparator<U, T>>::value;
        };

        using lesser = filter_if_t<L<Ts...>, lesser_filter_pred>;
        using greater = filter_if_t<L<Ts...>, greater_filter_pred>;

        using sorted_left = typename sort<lesser>::type;
        using sorted_right = typename sort<greater>::type;

        using type_ = push_back_t<sorted_left, L<T>>;
        using type = push_back_t<type_, sorted_right>;
    };

  public:
    using type = typename sort_p_impl<types>::type;
};

template <typename types, template <typename...> typename predicate>
using sort_p_t = typename sort_p<types, predicate>::type;

template <typename types, typename QMF>
using sort_qmf_p = sort_p<types, QMF::template fn>;

template <typename types, typename QMF>
using sort_qmf_p_t = typename sort_qmf_p<types, QMF>::type;

}  // namespace ctl
