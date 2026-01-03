#pragma once

#include "log_config.hpp"
#include "log_message.hpp"
#include "metadata.hpp"

namespace udl {

template <typename ch, ch... val>
constexpr auto operator""_log() -> ::ctl::log_message_t<ch, val...> {
    return {};
}

}   // namespace udl

using udl::operator""_log;

namespace details {

template <typename ...Ts, typename ...Args>
constexpr static auto to_dispatcher(Args&& ...msg) {
    auto &dispConfig = ::ctl::dispatcherConfig<Ts...>;
    dispConfig.send_log_data(std::forward<Args>(msg)...);
}

}   // namespace details

// common log define
#define LOG(logLevel, MSG) []{ \
    if constexpr (std::is_base_of_v<logLevel, ::ctl::logLevelConfig>) { \
        details::to_dispatcher(MSG##_log); \
    } \
}();

#define LOG_INFO(MSG) LOG(::ctl::log_level::info_t, MSG);
#define LOG_DEBUG(MSG) LOG(::ctl::log_level::debug_t, MSG);
#define LOG_WARNING(MSG) LOG(::ctl::log_level::warning_t, MSG);
#define LOG_ERROR(MSG) LOG(::ctl::log_level::error_t, MSG);
#define LOG_CRITICAL(MSG) LOG(::ctl::log_level::critical_t, MSG);
