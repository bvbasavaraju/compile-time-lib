#include <iostream>
#include <list.hpp>
#include <utils.hpp>
#include <show_type.hpp>

struct super_type {
  auto attendence() const {
    std::cout << "Present" << std::endl;
  }
};

struct type1 : public super_type {
  auto operator()() {
    std::cout << "inside type1\n";
  }
};

struct type2 : public super_type {
  auto operator()() {
    std::cout << "inside type2\n";
  }
};

int main() {

  std::cout << "This is CompileTimeLibrary main" << std::endl;

  using empty_list = ctl::list<>;
  using types1 = ctl::append_t<empty_list, type1>;
  using types_all = ctl::append_t<types1, type2>;

  static_assert(std::is_same_v<types_all, ctl::list<type1, type2> >, "Type1 or Type2 are not in list");

  using itypes_all = ctl::ilist<type1, type2>;
  itypes_all ita{};

  using ctypes_all = ctl::clist<type1, type2>;
  ctypes_all{}();

  constexpr auto fn = [](auto const& obj) {
    obj.attendence();
  };
  ctypes_all{}(fn);

  return 0;
}