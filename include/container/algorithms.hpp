#pragma once

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

    template <template <typename...> typename L, typename ...Ts>
    struct rename_impl<L<Ts...>> {
      using type = new_types<Ts...>;
    };
  public:
    using type = rename_impl<types>::type;
};

template <typename types, template <typename...> typename new_types>
using rename_t = rename<types, new_types>::type;

// apply
template <typename types, template <typename...> typename F>
using apply = rename<types, F>;

template <typename types, template <typename...> typename F>
using apply_t = apply<types, F>::type;

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

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct size_impl<L<T, Ts...>> {
      using type = std::integral_constant<uint32_t, 1 + size_impl<L<Ts...>>::type::value>;
    };

  public:
    using type = size_impl<types>::type;
};

template <typename types>
using size_t = size<types>::type;

// count
template <typename types>
using count = size<types>;

template <typename types>
using count_t = count<types>::type;

template <typename types, template <typename...> typename P>
struct count_if{
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
    struct count_if_impl<L<>>{
      using type = std::integral_constant<uint32_t, 0>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct count_if_impl<L<T, Ts...>>{
      using first = count_if_true<P<T>::value, L<T, Ts...>>::type;
      using rest = count_if_impl<L<Ts...>>::type;

      using type = std::integral_constant<uint32_t, (first::value + rest::value)>;
    };

  public:
    using type = count_if_impl<types>::type;
};

template <typename types, template <typename...> typename P>
using count_if_t = count_if<types, P>::type;

template <typename types, typename QMF>
using count_if_qmf_t = count_if_t<types, QMF::template fn>;

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

    template <template <typename...> typename L, typename ...Ts>
    struct clear_impl<L<Ts...>> {
      using type = L<>;
    };

  public:
    using type = clear_impl<types>::type;
};

template <typename types>
using clear_t = clear<types>::type;

// remove type
template <typename types, typename type_to_remove>
struct remove_type {
  private:
    template <typename U, typename L>
    struct remove_type_impl;

    template <typename T, template <typename ...> typename L>
    struct remove_type_impl<T, L<>> {
      using type = L<>;
    };

    template <typename T, template <typename ...> typename L>
    struct remove_type_impl<T, L<T>> {
      using type = L<>;
    };

    template <typename U, template <typename ...> typename L, typename T>
    struct remove_type_impl<U, L<T>> {
      using type = L<T>;
    };

    template <typename U, template <typename ...> typename L, typename T, typename ...Ts>
    struct remove_type_impl<U, L<T, Ts...>> {
      using first = remove_type_impl<U, L<T>>::type;
      using rest = remove_type<L<Ts...>, U>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = remove_type_impl<type_to_remove, types>::type;
};

template <typename types, typename type_to_remove>
using remove_type_t = remove_type<types, type_to_remove>::type;

// remove if
template <typename types, template <typename...> typename P>
struct remove_if {
  private:
    template <typename L>
    struct remove_if_impl;

    template <template <typename...> typename L>
    struct remove_if_impl<L<>>{
      using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct remove_if_impl<L<T>>{
      using type = select_t<P<T>, L<>, L<T>>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct remove_if_impl<L<T, Ts...>>{
      using first = remove_if_impl<L<T>>::type;
      using rest = remove_if_impl<L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = remove_if_impl<types>::type;
};

template <typename types, template <typename...> typename P>
using remove_if_t = remove_if<types, P>::type;

template <typename types, typename QMF>
using remove_if_qmf_t = remove_if_t<types, QMF::template fn>;

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

    template <template <typename ...> typename L, typename T, typename ...Ts>
    struct remove_duplicates_impl<L<T, Ts...>>{
      using first = remove_type_t<L<Ts...>, T>;
      using rest = remove_duplicates<first>::type;

      using type = push_back_t<L<T>, rest>;
    };
  
  public:
    using type = remove_duplicates_impl<types>::type;
};

template <typename types>
using remove_duplicates_t = remove_duplicates<types>::type;

// reverse
template <typename types>
struct reverse {
  private:
    template <typename L>
    struct reverse_impl;

    template <template <typename ...> typename L>
    struct reverse_impl<L<>> {
      using type = L<>;
    };

    template <template <typename ...> typename L, typename T>
    struct reverse_impl<L<T>> {
      using type = L<T>;
    };

    template <template <typename ...> typename L, typename T, typename ...Ts>
    struct reverse_impl<L<T, Ts...>> {
      using rest = reverse<L<Ts...>>::type;

      using type = push_back_t<rest, L<T>>;
    };

  public:
    using type = reverse_impl<types>::type;
};

template <typename types>
using reverse_t = reverse<types>::type;

// at
template <typename types, std::size_t N>
struct at_c{
  private:
    template <std::size_t U, typename L>
    struct at_c_impl;

    template <template <typename ...>typename L, typename T, typename ...Ts>
    struct at_c_impl<0, L<T, Ts...>> {
      using type = T;
    };

    template <std::size_t index, template <typename ...>typename L, typename T, typename ...Ts>
    struct at_c_impl<index, L<T, Ts...>> {
      using type = at_c_impl<index-1, L<Ts...>>::type;
    };

    template <std::size_t U, template <typename...> typename L, typename ...Ts>
    struct at_c_impl<U, L<Ts...>> {
      static_assert((U >= sizeof...(Ts)), "Index should be within size of the container of types");
      static_assert((U < 0), "Index cannot be negative");
    };

  public:
    using type = at_c_impl<N, types>::type;
};

template <typename types, std::size_t N>
using at_c_t = at_c<types, N>::type;

template <typename types, typename N>
using at_t = at_c_t<types, N::value>;

// filter
template <typename types, template <typename ...> typename P>
struct filter {
  private:
    template <typename L>
    struct filter_impl;

    template <template <typename...> typename L>
    struct filter_impl<L<>>{
      using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct filter_impl<L<T>>{
      using type = select_t<P<T>, L<T>, L<>>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct filter_impl<L<T, Ts...>>{
      using first = filter_impl<L<T>>::type;
      using rest = filter_impl<L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = filter_impl<types>::type;
};

template <typename types, template <typename ...> typename P>
using filter_t = filter<types, P>::type;

template <typename types, typename QMF>
using filter_qmf_t = filter_t<types, QMF::template fn>;

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

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct contains_impl<T, L<T, Ts...>> {
      using type = std::true_type;
    };

    template <typename U, template <typename...> typename L, typename T, typename ...Ts>
    struct contains_impl<U, L<T, Ts...>> {
      using type = contains_impl<U, L<Ts...>>::type;
    };

  public:
    using type = contains_impl<type_to_search, types>::type;
};

template <typename types, typename type_to_search>
using contains_t = contains<types, type_to_search>::type;

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

    template <template <typename...> typename L, typename ...Ts>
    struct drop_c_impl<0, L<Ts...>> {
      using type = L<Ts...>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct drop_c_impl<0, L<T, Ts...>> {
      using type = L<T, Ts...>;
    };

    template <std::size_t count, template <typename...> typename L, typename T, typename ...Ts>
    struct drop_c_impl<count, L<T, Ts...>> {
      using type = drop_c_impl<count-1, L<Ts...>>::type;
    };

  public:
    using type = drop_c_impl<N, types>::type;
};

template <typename types, std::size_t N>
using drop_c_t = drop_c<types, N>::type;

template <typename types, typename N>
using drop_t = drop_c_t<types, N::value>;

// take
template <typename types, std::size_t N>
struct take_c {
  private:
    template <std::size_t count, typename L>
    struct take_c_impl;

    template <template <typename...> typename L>
    struct take_c_impl<0, L<>>{
      using type = L<>;
    };

    template <template <typename...> typename L, typename ...Ts>
    struct take_c_impl<0, L<Ts...>>{
      using type = L<>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct take_c_impl<0, L<T, Ts...>>{
      using type = L<>;
    };

    template <std::size_t count, template <typename...> typename L>
    struct take_c_impl<count, L<>>{
      using type = L<>;
    };

    template <std::size_t count, template <typename...> typename L, typename T, typename ...Ts>
    struct take_c_impl<count, L<T, Ts...>>{
      using first = L<T>;
      using rest = take_c_impl<count-1, L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = take_c_impl<N, types>::type;
};

template <typename types, std::size_t N>
using take_c_t = take_c<types, N>::type;

template <typename types, typename N>
using take_t = take_c_t<types, N::value>;

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

    template <template <typename...> typename L, typename ...Ts>
    struct repeat_c_impl<0, L<Ts...>, L<Ts...>> {
      using type = L<>;
    };

    template <template <typename...> typename L>
    struct repeat_c_impl<1, L<>, L<>> {
      using type = L<>;
    };

    template <template <typename...> typename L, typename ...Tos, typename ...Trs>
    struct repeat_c_impl<1, L<Tos...>, L<Trs...>> {
      using type = L<Trs...>;
    };

    template <std::size_t count, template <typename...> typename L, typename ...Tos, typename ...Trs>
    struct repeat_c_impl<count, L<Tos...>, L<Trs...>> {
      using repeated = L<Tos..., Trs...>;
      using type = repeat_c_impl<count-1, L<Tos...>, repeated>::type;
    };

  public:
    using type = repeat_c_impl<N, types, types>::type;
};

template <typename types, std::size_t N>
using repeat_c_t = repeat_c<types, N>::type;

template <typename types, typename N>
using repeat_t = repeat_c_t<types, N::value>;

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
    
    template<std::size_t N, typename S>
    struct from_integer_sequence_impl;

    template <template <typename T, T... vs> typename S, typename DT, DT ...vals>
    struct from_integer_sequence_impl<0, S<DT, vals...>> {
      using type = result_type<>;
    };

    template <std::size_t N, template <typename T, T... vs> typename S, typename DT, DT val, DT ...vals>
    struct from_integer_sequence_impl<N, S<DT, val, vals...>> {
      using first = result_type<std::integral_constant<DT, val>>;
      using rest = from_integer_sequence_impl<N-1, S<DT, vals...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = from_integer_sequence_impl<sequence::size(), sequence>::type;
};

template <typename sequence, template <typename...> typename result_type = std::tuple>
using from_integer_sequence_t = from_integer_sequence<sequence, result_type>::type;

// iota
template <std::size_t N, typename DT = uint32_t, template <typename...> typename result_type = std::tuple>
struct iota_c {
  private:
    template <std::size_t count, typename T>
    struct iota_c_impl {
      using first = result_type<std::integral_constant<T, count-1>>;
      using rest = iota_c_impl<count-1, T>::type;

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
    using type = iota_c_impl<N, DT>::type;
};

template <std::size_t N, typename DT = uint32_t, template <typename...> typename result_type = std::tuple>
using iota_c_t = iota_c<N, DT, result_type>::type;

template <typename N, template <typename...> typename result_type = std::tuple>
using iota_t = iota_c_t<N::value, typename N::value_type, result_type>;

// insert
template <typename types, std::size_t P, typename ...types_to_insert>
struct insert_c {
  private:
    template <typename L, typename ...IT>
    struct insert_c_impl;

    template <template <typename ...> typename L, typename ...Ts, typename ...IT>
    struct insert_c_impl<L<Ts...>, IT...> {
      static_assert(P <= size_t<L<Ts...>>::value, "Position has to be less than size of the container");

      using front = take_c_t<L<Ts...>, P>;
      using back = drop_c_t<L<Ts...>, P>;

      using type_ = push_back_t<front, L<IT...>>;
      using type = push_back_t<type_, back>;
    };

  public:
    using type = insert_c_impl<types, types_to_insert...>::type;
};

template <typename types, std::size_t P, typename ...types_to_insert>
using insert_c_t = insert_c<types, P, types_to_insert...>::type;

template <typename types, typename P, typename ...types_to_insert>
using insert_t = insert_c_t<types, P::value, types_to_insert...>;

// erase
template <typename types, std::size_t P1, std::size_t P2>
struct erase_c {
  private:
    template <typename L>
    struct erase_c_impl;

    template <template <typename...> typename L, typename ...Ts>
    struct erase_c_impl<L<Ts...>> {
      static_assert(sizeof...(Ts) > 0, "container cannot be empty");
      static_assert(P1 < size_t<L<Ts...>>::value, "Position 1 has to be less than size of the container");
      static_assert(P2 <= size_t<L<Ts...>>::value, "Position 2 has to be less than size of the container");

      static_assert(P1 < P2, "Position 1 > Position 2");

      using front = take_c_t<L<Ts...>, P1>;
      using back = drop_c_t<L<Ts...>, P2>;

      using type = push_back_t<front, back>;
    };

  public:
    using type = erase_c_impl<types>::type;
};

template <typename types, std::size_t P1, std::size_t P2>
using erase_c_t = erase_c<types, P1, P2>::type;

template <typename types, typename P1, typename P2>
using erase_t = erase_c_t<types, P1::value, P2::value>;

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

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct replace_impl<T, L<T, Ts...>> {
      using first = L<replace_with>;
      using rest = replace_impl<T, L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

    template <typename TR, template <typename...> typename L, typename T, typename ...Ts>
    struct replace_impl<TR, L<T, Ts...>> {
      using first = L<T>;
      using rest = replace_impl<TR, L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = replace_impl<type_to_replace, types>::type;
};

template <typename types, typename type_to_replace, typename replace_with>
using replace_t = replace<types, type_to_replace, replace_with>::type;

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

    template <template <typename...> typename L, typename ...Ts>
    struct replace_at_c_impl<L<Ts...>> {
      static_assert(P < size_t<L<Ts...>>::value, "Position has to be less than size of the container");

      using front = take_c_t<L<Ts...>, P>;
      using replaced = L<replace_with>;
      using back = drop_c_t<L<Ts...>, P+1>;

      using type_ = push_back_t<front, replaced>;
      using type = push_back_t<type_, back>;
    };

  public:
    using type = replace_at_c_impl<types>::type;
};

template <typename types, std::size_t P, typename replace_with>
using replace_at_c_t = replace_at_c<types, P, replace_with>::type;

template <typename types, typename P, typename replace_with>
using replace_at_t = replace_at_c_t<types, P::value, replace_with>;

// replace if
template <typename types, template <typename...> typename P, typename replace_with>
struct replace_if {
  private:
    template <typename L>
    struct replace_if_impl;

    template <template <typename...> typename L>
    struct replace_if_impl<L<>>{
      using type = L<>;
    };

    template <template <typename...> typename L, typename T>
    struct replace_if_impl<L<T>>{
      using type = select_t<P<T>, L<replace_with>, L<T>>;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct replace_if_impl<L<T, Ts...>>{
      using first = replace_if_impl<L<T>>::type;
      using rest = replace_if_impl<L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = replace_if_impl<types>::type;
};

template <typename types, template <typename...> typename P, typename replace_with>
using replace_if_t = replace_if<types, P, replace_with>::type;

template <typename types, typename QMF, typename replace_with>
using replace_if_qmf_t = replace_if_t<types, QMF::template fn, replace_with>;

// rotate left
template <typename types, std::size_t N>
struct rotate_left_c {
  private:
    template <typename L>
    struct rotate_left_c_impl;

    template <template <typename ...> typename L>
    struct rotate_left_c_impl<L<>>{
      using type = L<>;
    };

    template <template <typename ...> typename L, typename ...Ts>
    struct rotate_left_c_impl<L<Ts...>>{
      constexpr static auto len = size_t<L<Ts...>>::value;
      constexpr static auto count = N % len;

      using first = take_c_t<L<Ts...>, count>;
      using rest = drop_c_t<L<Ts...>, count>;

      using type = push_back_t<rest, first>;
    };

  public:
    using type = rotate_left_c_impl<types>::type;
};

template <typename types, std::size_t N>
using rotate_left_c_t = rotate_left_c<types, N>::type;

template <typename types, typename N>
using rotate_left_t = rotate_left_c_t<types, N::value>;

// rotate right
template <typename types, std::size_t N>
struct replace_right_c {
  private:
    template <typename L>
    struct replace_right_c_impl;

    template <template <typename ...> typename L>
    struct replace_right_c_impl<L<>>{
      using type = L<>;
    };

    template <template <typename ...> typename L, typename ...Ts>
    struct replace_right_c_impl<L<Ts...>>{
      constexpr static auto len = size_t<L<Ts...>>::value;
      constexpr static auto count = N % len;

      using first = take_c_t<L<Ts...>, len-count>;
      using rest = drop_c_t<L<Ts...>, len-count>;

      using type = push_back_t<rest, first>;
    };

  public:
    using type = replace_right_c_impl<types>::type;
};

template <typename types, std::size_t N>
using replace_right_c_t = replace_right_c<types, N>::type;

template <typename types, typename N>
using replace_right_t = replace_right_c_t<types, N::value>;

// copy if
template <typename types, template <typename ...> typename P>
using copy_if_t = filter_t<types, P>;

template <typename types, typename QMF>
using copy_if_qmf_t = copy_if_t<types, QMF::template fn>;

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

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct find_impl<T, L<T, Ts...>> {
      constexpr static uint32_t value = 0;
    };

    template <typename U, template <typename...> typename L, typename T, typename ...Ts>
    struct find_impl<U, L<T, Ts...>> {
      constexpr static uint32_t value = 1 + find_impl<U, L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<uint32_t, find_impl<type_to_find, types>::value>;
};

template <typename types, typename type_to_find>
using find_t = find<types, type_to_find>::type;

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

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct find_if_cond_impl<true, L<T, Ts...>> {
      constexpr static uint32_t value = 0;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct find_if_cond_impl<false, L<T, Ts...>> {
      constexpr static uint32_t value = 1 + find_if_cond_impl<P<T>::value, L<Ts...>>::value;
    };

    // without condition
    template <typename L>
    struct find_if_impl;

    template <template <typename ...> typename L>
    struct find_if_impl<L<>> {
      constexpr static uint32_t value = 0;
    };

    template <template <typename ...>typename L, typename T, typename ...Ts>
    struct find_if_impl<L<T, Ts...>> {
      constexpr static uint32_t value = find_if_cond_impl<P<T>::value, L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<uint32_t, find_if_impl<types>::value>;
};

template <typename types, template <typename...> typename P>
using find_if_t = find_if<types, P>::type;

template <typename types, typename QMF>
using find_if_qmf_t = find_if<types, QMF::template fn>::type;

// unique
template <typename types>
using unique_t = remove_duplicates_t<types>;

template <typename types, template <typename...> typename P>
using unique_if_t = remove_if_t<types, P>;

template <typename types, typename QMF>
using unique_if_qmf_t = remove_if_qmf_t<types, QMF>;

// all of
template <typename types, template <typename ...> typename P>
struct all_of{
  private:
    template <typename L>
    struct all_of_impl;

    template <template <typename...> typename L>
    struct all_of_impl<L<>> {
      constexpr static bool value = true;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct all_of_impl<L<T, Ts...>> {
      constexpr static bool value = P<T>::value && all_of_impl<L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<bool, all_of_impl<types>::value>;
};

template <typename types, template <typename ...> typename P>
using all_of_t = all_of<types, P>::type;

template <typename types, typename QMF>
using all_of_qmf_t = all_of_t<types, QMF::template fn>;

// any of
template <typename types, template <typename ...> typename P>
struct any_of{
  private:
    template <typename L>
    struct any_of_impl;

    template <template <typename...> typename L>
    struct any_of_impl<L<>> {
      constexpr static bool value = false;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct any_of_impl<L<T, Ts...>> {
      constexpr static bool value = P<T>::value || any_of_impl<L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<bool, any_of_impl<types>::value>;
};

template <typename types, template <typename ...> typename P>
using any_of_t = any_of<types, P>::type;

template <typename types, typename QMF>
using any_of_qmf_t = any_of_t<types, QMF::template fn>;

// none of
template <typename types, template <typename ...> typename P>
struct none_of{
  private:
    template <typename L>
    struct none_of_impl;

    template <template <typename...> typename L>
    struct none_of_impl<L<>> {
      constexpr static bool value = true;
    };

    template <template <typename...> typename L, typename T, typename ...Ts>
    struct none_of_impl<L<T, Ts...>> {
      constexpr static bool value = (!P<T>::value) && none_of_impl<L<Ts...>>::value;
    };

  public:
    using type = std::integral_constant<bool, none_of_impl<types>::value>;
};

template <typename types, template <typename ...> typename P>
using none_of_t = none_of<types, P>::type;

template <typename types, typename QMF>
using none_of_qmf_t = none_of_t<types, QMF::template fn>;

// transform
template <typename types, template <typename ...> typename F>
struct transform {
  private:
    template <typename L>
    struct transform_impl;

    template <template <typename ...> typename L>
    struct transform_impl<L<>>{
      using type = L<>;
    };

    template <template <typename ...> typename L, typename T>
    struct transform_impl<L<T>>{
      using type = L<F<T>>;
    };

    template <template <typename ...> typename L, typename T, typename ...Ts>
    struct transform_impl<L<T, Ts...>>{
      using first = L<F<T>>;
      using rest = transform_impl<L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };

  public:
    using type = transform_impl<types>::type;
};

template <typename types, template <typename ...> typename F>
using transform_t = transform<types, F>::type;

template <typename types, typename QMF>
using transform_qmf_t = transform_t<types, QMF::template fn>;

// transform if
template <typename types, template <typename ...> typename F, template <typename...> typename P>
struct transform_if {
  private:

    template <typename L>
    struct transform_if_impl;

    template <template <typename...> typename L>
    struct transform_if_impl<L<>> {
      using type = L<>;
    };

    template <template <typename ...> typename L, typename T>
    struct transform_if_impl<L<T>> {
      using type = select_t<P<T>, L<F<T>>, L<>>;
    };

    template <template <typename ...> typename L, typename T, typename ...Ts>
    struct transform_if_impl<L<T, Ts...>> {
      using first = select_t<P<T>, L<F<T>>, L<>>;
      using rest = transform_if_impl<L<Ts...>>::type;

      using type = push_back_t<first, rest>;
    };
  public:
    using type = transform_if_impl<types>::type;
};

template <typename types, template <typename ...> typename F, template <typename...> typename P>
using transform_if_t = transform_if<types, F, P>::type;

template <typename types, typename QMFf, typename QMFp>
using transform_if_qmf_t = transform_if<types, QMFf::template fn, QMFp::template fn>::type;

} // namespace ctl
