#pragma once

#include <type_traits>

namespace ctl {

// quote
template <template <typename...> typename F>
struct quote {
    template <typename... Ts>
    using fn = F<Ts...>;
};

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
using valid_qmf_t = valid_t<QMF::template fn, Ts...>;

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
using select_t = select_c_t<C::value, T, F>;

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
using select_f_t = select_f_c_t<C::value, T, F, Ts...>;

template <bool C, typename T, typename QMF, typename... Ts>
using select_qmf_c_t = select_f_c_t<C, T, QMF::template fn, Ts...>;

template <typename C, typename T, typename QMF, typename... Ts>
using select_qmf_t = select_qmf_c_t<C::value, T, QMF, Ts...>;

}  // namespace ctl
