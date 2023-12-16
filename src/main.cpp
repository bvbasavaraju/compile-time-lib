#include <iostream>
#include <list.hpp>
#include <utils.hpp>
#include <show_type.hpp>
#include <fold_ops.hpp>

struct super_type {
  auto attendence() const {
    std::cout << "Present" << std::endl;
  }

  virtual void name() const = 0;
};

struct type1 : public super_type {
  void name() const {
    std::cout << "type1: ";
  }

  auto operator()() const {
    return false;
  }
};

struct type2 : public super_type {
  void name() const {
    std::cout << "type2: ";
  }

  auto operator()() const {
    return true;
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
    ctypes_all{};

    // with predicate!
    constexpr auto fn = [](auto const& obj) {
      obj.name();
      obj.attendence();
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
  }

  return 0;
}