#pragma once

namespace ctl {

namespace dummy {

struct dispatcher_t {
    constexpr auto send_log_data(auto && ...) -> void {
        // Nothing to do here!!
    }
}; 

}   // namespace dummy

//  Global definition
template <typename ...>
inline auto dispatcherConfig = dummy::dispatcher_t{};

}   // namespace ctl
