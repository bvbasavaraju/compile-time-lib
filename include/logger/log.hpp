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

}   // namespace 


#define LOG(MSG) details::to_dispatcher(MSG##_log);
