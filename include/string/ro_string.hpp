#pragma once

#include <string/hash.hpp>

namespace ctl::string {

// Read Only String
template <typename ch, ch... chs>
struct ro_string {
    private:
        constexpr static char storage[] = {chs..., 0};
    public:
        constexpr static auto size() noexcept -> std::size_t {
            return sizeof...(chs);
        }

        constexpr static auto c_str() noexcept -> const char* {
            return storage;
        }

        constexpr static auto hash() noexcept -> std::uint64_t {
            constexpr char str[] = {chs..., 0};
            return ::ctl::string::hash{}(str, sizeof...(chs));
        }
};

// User-defined literal for ro_string
template <typename Char, Char... chs>
constexpr auto operator""_ros() -> ro_string<Char, chs...> {
    return {};
}

// Note: As #define will replace during preprocessing stage operator""_ros might not be available if the namespace is not used.
// So, make sure to forward declare the operator""_ros in your namespace or use the ctl namespace to access it.
// ex: using ctl::operator""_ros;
// OR
// using namespace ctl;
#define ROST(data) decltype(data##_ros)
#define HASH(data) ROST(data)::hash()

}   // namespace ctl::string

// export operator""_ros to global namespace for convenience
using ctl::string::operator""_ros;
