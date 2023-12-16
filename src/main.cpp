#include <iostream>
#include <list.hpp>
#include <utils.hpp>
#include <show_type.hpp>
#include <fold_ops.hpp>

struct super_type {
  auto attendence() const {
    std::cout << "Present" << std::endl;
  }
};

struct type1 : public super_type {
  auto operator()() const {
    std::cout << "inside type1\n";
  }

  auto operator||(auto val) {
    return val || false;
  }

  auto operator&&(auto val) {
    return val && false;
  }
};

struct type2 : public super_type {
  auto operator()() const {
    std::cout << "inside type2\n";
  }

  auto operator||(auto val) {
    return val || true;
  }

  auto operator&&(auto val) {
    return val && true;
  }
};

int main() {

  std::cout << "This is CompileTimeLibrary main" << std::endl;

  using empty_list = ctl::list<>;
  using types1 = ctl::append_t<empty_list, type1>;
  using types_all = ctl::append_t<types1, type2>;

  static_assert(std::is_same_v<types_all, ctl::list<type1, type2> >, "Type1 or Type2 are not in list");

  // instantiable list
  using itypes_all = ctl::ilist<type1, type2>;
  itypes_all ita{};

  {
    //Callable list
    using ctypes_all = ctl::clist<type1, type2>;
    ctypes_all{}();

    // with predicate!
    constexpr auto fn = [](auto const& obj) {
      obj.attendence();
      decltype(obj){}();
    };
    ctypes_all{}(fn);

    // with fold operations
    if ( ctypes_all{}(ctl::fold_ops::logical_or{}) ) {
      std::cout << "fold op :: logical or :: returned true" << std::endl;
    } else {
      std::cout << "fold op :: logical or :: returned false" << std::endl;
    }

    if ( ctypes_all{}(ctl::fold_ops::logical_and{}) ) {
      std::cout << "fold op :: logical and :: returned true" << std::endl;
    } else {
      std::cout << "fold op :: logical and :: returned false" << std::endl;
    }

    // // with operator logical OR
    // if (ctypes_all{} || false) {
    //   std::cout << "Operation Logical OR returned true" << std::endl;
    // } else {
    //   std::cout << "Operation Logical OR returned false" << std::endl;
    // }
  }

  return 0;
}