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

template <typename C>
using invert = invert_c<C::value>;

template <typename C>
using invert_t = typename invert<C>::type;

// valid
template <template <typename...> typename F, typename... Ts>
struct valid {
  private:
    template <typename, typename = void>
    struct helper : public std::false_type {};

    template <typename T>
    struct helper<T, std::void_t<typename T::type>> : public std::true_type {};

  public:
    using type = std::integral_constant<bool, helper<F<Ts...>>::value>;
};

template <template <typename...> typename F, typename... Ts>
using valid_t = typename valid<F, Ts...>::type;

template <typename QMF, typename... Ts>
using valid_qmf = valid<QMF::template fn, Ts...>;

template <typename QMF, typename... Ts>
using valid_qmf_t = typename valid_qmf<QMF, Ts...>::type;

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

}  // namespace ctl
