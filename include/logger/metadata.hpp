#pragma once

#include <cstdint>

template <typename T, T... ch>
struct string_variadic_t;

template <typename T>
struct log_metadata_t{};

using log_token_t = std::uint32_t;

template <typename T>
extern auto get_token() -> log_token_t;
