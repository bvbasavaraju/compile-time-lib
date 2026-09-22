#pragma once

namespace ctl::string {

// Hash
constexpr static std::uint64_t PRIME_NUM = 0x100000001b3ULL;
constexpr static std::uint64_t OFFSET_BASIS = 0xcbf29ce484222325ULL;

struct hash {
    constexpr auto operator()(const char* str, std::size_t len) const noexcept -> std::uint64_t {
        if(str == nullptr) {
            return 0;
        }

        std::uint64_t hash = OFFSET_BASIS;
        for (std::size_t i = 0; i < len; ++i) {
            hash ^= static_cast<std::uint64_t>(str[i]);
            hash *= PRIME_NUM;
        }
        return hash;
    }

    constexpr auto operator()(const char* str) const noexcept -> std:: uint64_t {
        return operator()(str, std::strlen(str));
    }
};

inline constexpr auto getHash(const char* str) -> std::uint64_t {
    return hash{}(str);
}

}   // namespace ctl::string
