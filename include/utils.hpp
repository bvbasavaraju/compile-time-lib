#pragma once

#include <type_traits>

namespace ctl {

// quote
template <template <typename...> typename F>
struct quote {
    template <typename... Ts>
    using fn = F<Ts...>;
};

// not
template <bool C>
struct invert_c {
    using type = std::integral_constant<bool, !C>;
};

template <bool C>
using invert_c_t = typename invert_c<C>::type;

template <bool C>
constexpr static bool invert_c_v = invert_c_t<C>::value;

template <typename C>
using invert = invert_c<C::value>;

template <typename C>
using invert_t = typename invert<C>::type;

template <typename C>
constexpr static bool invert_v = invert_t<C>::value;

// valid
template <template <typename...> typename F, typename... Ts>
struct valid {
  private:
    template <typename, typename = void>
    struct helper : public std::false_type {};

    template <typename T>
    struct helper<T, std::void_t<typename T::type>> : public std::true_type {};

  public:
    using type = helper<F<Ts...>>;
};

template <template <typename...> typename F, typename... Ts>
using valid_t = typename valid<F, Ts...>::type;

template <template <typename...> typename F, typename... Ts>
constexpr static bool valid_v = valid_t<F, Ts...>::value;

template <typename QMF, typename... Ts>
using valid_qmf = valid<QMF::template fn, Ts...>;

template <typename QMF, typename... Ts>
using valid_qmf_t = typename valid_qmf<QMF, Ts...>::type;

template <typename QMF, typename... Ts>
constexpr static bool valid_qmf_v = valid_qmf_t<QMF, Ts...>::value;

// select
template <bool C, typename T, typename F>
struct select_c {
  private:
    template <bool COND, typename TT>
    struct select_c_impl {
        using type = F;
    };

    template <typename TT>
    struct select_c_impl<true, TT> {
        using type = TT;
    };

  public:
    using type = typename select_c_impl<C, T>::type;
};

template <bool C, typename T, typename F>
using select_c_t = typename select_c<C, T, F>::type;

template <typename C, typename T, typename F>
using select = select_c<C::value, T, F>;

template <typename C, typename T, typename F>
using select_t = typename select<C, T, F>::type;

template <bool C, typename T, template <typename...> typename F, typename... Ts>
struct select_f_c {
  private:
    template <bool COND, typename TT>
    struct select_c_impl {
        using type = F<Ts...>;
    };

    template <typename TT>
    struct select_c_impl<true, TT> {
        using type = TT;
    };

  public:
    using type = typename select_c_impl<C, T>::type;
};

template <bool C, typename T, template <typename...> typename F, typename... Ts>
using select_f_c_t = typename select_f_c<C, T, F, Ts...>::type;

template <typename C, typename T, template <typename...> typename F, typename... Ts>
using select_f = select_f_c<C::value, T, F, Ts...>;

template <typename C, typename T, template <typename...> typename F, typename... Ts>
using select_f_t = typename select_f<C, T, F, Ts...>::type;

template <bool C, typename T, typename QMF, typename... Ts>
using select_qmf_c = select_f_c<C, T, QMF::template fn, Ts...>;

template <bool C, typename T, typename QMF, typename... Ts>
using select_qmf_c_t = typename select_qmf_c<C, T, QMF, Ts...>::type;

template <typename C, typename T, typename QMF, typename... Ts>
using select_qmf = select_f<C, T, QMF::template fn, Ts...>;

template <typename C, typename T, typename QMF, typename... Ts>
using select_qmf_t = typename select_qmf<C, T, QMF, Ts...>::type;

// is_function
template <typename T>
struct is_function {
  private:
    struct is_function_impl {
        constexpr static bool value = !std::is_reference_v<T> && !std::is_const_v<std::add_const_t<T>>;
    };

  public:
    using type = std::integral_constant<bool, is_function_impl::value>;
};

template <typename T>
using is_function_t = is_function<T>::type;

template <typename T>
constexpr static auto is_function_v = is_function_t<T>::value;

// is invocable
/*
    using member detection idiom:
    https://en.wikibooks.org/wiki/More_C++_Idioms/Member_Detector
*/
template <typename T>
struct is_invocable {
  private:
    // member detection idiom. member to detect here is 'operator()'
    struct call_operator {
        auto operator()() -> void {};
    };

    template <typename F, bool> 
    struct derived : call_operator {};

    template <typename F>
    struct derived <F, true> : F, call_operator {};

    //invocable impl
    //if T also has operator() defined, then there will be ambiguity in the std::void_t<> will be ill-formed, so only true_type is accepted.
    template <typename, typename = void>
    struct is_invocable_impl : public std::true_type {};

    template <typename U>
    struct is_invocable_impl<U, std::void_t< decltype(& derived<U, std::is_class_v<U>>::operator()) > > : public std::false_type {};

  public:
    using type = std::integral_constant<bool, is_function_v<T> || is_invocable_impl<T>::value>;
};

template <typename T>
using is_invocable_t = is_invocable<T>::type;

template <typename T>
constexpr static auto is_invocable_v = is_invocable_t<T>::value;

}  // namespace ctl
