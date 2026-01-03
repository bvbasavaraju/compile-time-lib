#include <array>

namespace ctl {

template<typename ch, ch... val>
struct log_message_t{
public:
    using value_type = std::basic_string_view<ch>;

private:
    constexpr static std::array<char, sizeof...(val)> storage{val...};
    constexpr static value_type value{storage.data(), sizeof(storage)};

public:
    constexpr static auto size() -> uint32_t {
        return sizeof(storage);
    }

    constexpr auto data() {
        return value;
    }
};

}   // namespace ctl
