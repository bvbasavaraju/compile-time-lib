#pragma once

#include <tuple>

namespace ctl {

namespace details {

template <typename Msg>
constexpr static auto to_log_metadata(Msg msg) {
    constexpr auto m = msg.data();
    using type = typename std::remove_cv_t<decltype(m)>::value_type;

    return [&]<std::size_t ... Is>(std::integer_sequence<std::size_t, Is...>) {
        return log_metadata_t< string_variadic_t<type, m[Is]...> >{};
    }(std::make_integer_sequence<std::size_t, msg.size()>{});
}

template <typename T>
inline auto log_token(T) -> log_token_t {
    return get_token<T>();
}

}   // namespace details

template <typename T>
concept HasDispatch = requires(T t) {
    { t.dispatch(std::uint32_t{}) } -> std::same_as<void>;
};

template <typename T>
concept DestinationType = HasDispatch<T>
    && std::is_move_constructible_v<T>
    && std::is_move_assignable_v<T>;


template <DestinationType ... dests>
struct log_destinations_t {
    using DestinationsAsTuple_t = std::tuple<dests...>;

    // Instantiating tuple of destinations
    DestinationsAsTuple_t destinations;

    constexpr auto send_log_data(auto && msg) -> void {
        auto token = details::log_token(details::to_log_metadata(msg));

        std::apply([&](auto & ... dest) -> void {
            ((dest.dispatch(token)), ...);
        }, destinations);
    }
};

}   // namespace ctl
