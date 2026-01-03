#pragma once

namespace ctl {

namespace dummy {

struct dispatcher_t {
    constexpr auto send_log_data(auto && ...) -> void {
        // Nothing to do here!!
    }
}; 

}   // namespace dummy

namespace log_level {

struct none_t {};
struct critical_t : none_t {};
struct error_t : critical_t {};
struct warning_t : error_t {};
struct debug_t : warning_t {};
struct info_t : debug_t {};

}   // log_level

//  Global definition
template <typename ...>
inline auto dispatcherConfig = dummy::dispatcher_t{};

using logLevelConfig = log_level::info_t;

}   // namespace ctl
