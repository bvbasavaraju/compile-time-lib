#pragma once

namespace ctl {

struct debug{
  private:
    template <typename T>
    constexpr static bool always_false = false;

  public:
    template <typename T>
    constexpr static auto show_type() -> void {
      static_assert(always_false<T>, "--- Compile time deduced type is: ---");
    }

    template <typename T>
    constexpr static auto show_type(T&&) -> void {
      static_assert(always_false<T>, "--- Compile time deduced type is: ---");
    }
};

} // namespace ctl