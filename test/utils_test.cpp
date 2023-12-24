#include <gtest/gtest.h>

#include <type_traits>

#include "utils.hpp"
#include "container/algorithms.hpp"
#include "container/list.hpp"
#include "debug/show_type.hpp"

class utils_test : public ::testing::Test {
  protected:
    void SetUp() override {
      //Add Test setup code here!
    }

    void TearDown() override {
      //Add Test teardown code here!
    }
};

template <typename ...T> struct pred_int : public std::false_type {};
template <> struct pred_int<int> : public std::true_type {};

template <typename ...T> struct pred_int_char : public std::false_type {};
template <> struct pred_int_char<int> : public std::true_type {};
template <> struct pred_int_char<char> : public std::true_type {};

template <typename ...T> struct pred_empty : public std::false_type {};
template <> struct pred_empty<> : public std::true_type {};

template <typename ...T> struct pred_all : public std::true_type {};
template <> struct pred_all<> : public std::false_type {};

struct quoted_int_char {
  public:
    template <typename ...T>
    using fn = pred_int_char<T...>;
};

template <typename ...T>
struct member_type {};

template <> struct member_type<int> {
  using type = int;
};

struct quoted_member_type {
  template <typename ...T>
  using fn = member_type<T...>;
};

template<typename T> 
using add_pointer_t = typename std::add_pointer<T>::type;

struct quoted_add_ptr {
  template <typename T>
  using fn = add_pointer_t<T>;
};

template <typename T>
using predicate_type = std::integral_constant<bool, pred_int<T>::value>;

struct quoted_pred_type {
  template <typename T>
  using fn = predicate_type<T>;
};

//Tests
TEST_F(utils_test, select) {
  using list = ctl::list<int, char, double, int>;
  using empty = ctl::list<>;
  
  {
    constexpr auto expect_true = std::is_same_v<ctl::select_c_t<true, list, empty>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_c_t<true, list, empty>, empty>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_c_t<false, list, empty>, empty>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_c_t<false, list, empty>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_t<std::true_type, list, empty>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_t<std::true_type, list, empty>, empty>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_t<std::false_type, list, empty>, empty>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_t<std::false_type, list, empty>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(utils_test, select_f) {
  using list = ctl::list<int, char, double, int>;
  
  {
    constexpr auto expect_true = std::is_same_v<ctl::select_f_c_t<true, list, ctl::list, float, double>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_f_c_t<true, list, ctl::list, float, double>, ctl::list<float, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_f_c_t<false, list, ctl::list, float, double>, ctl::list<float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_f_c_t<false, list, ctl::list, float, double>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_f_t<std::true_type, list, ctl::list, float, double>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_f_t<std::true_type, list, ctl::list, float, double>, ctl::list<float, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_f_t<std::false_type, list, ctl::list, float, double>, ctl::list<float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_f_t<std::false_type, list, ctl::list, float, double>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(utils_test, select_qmf) {
  using list = ctl::list<int, char, double, int>;
  
  {
    constexpr auto expect_true = std::is_same_v<ctl::select_qmf_c_t<true, list, quoted_member_type, float, double>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_qmf_c_t<true, list, quoted_member_type, float, double>, member_type<float, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_qmf_c_t<false, list, quoted_member_type, float, double>, member_type<float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_qmf_c_t<false, list, quoted_member_type, float, double>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_qmf_t<std::true_type, list, quoted_member_type, float, double>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_qmf_t<std::true_type, list, quoted_member_type, float, double>, member_type<float, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_qmf_t<std::false_type, list, quoted_member_type, float, double>, member_type<float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::select_qmf_t<std::false_type, list, quoted_member_type, float, double>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(utils_test, valid) {
  EXPECT_FALSE((ctl::valid_t<member_type>::value));
  EXPECT_FALSE((ctl::valid_t<member_type, void>::value));
  EXPECT_FALSE((ctl::valid_t<member_type, void, int>::value));
  EXPECT_TRUE((ctl::valid_t<member_type, int>::value));

  EXPECT_FALSE((ctl::valid_qmf_t<quoted_member_type>::value));
  EXPECT_FALSE((ctl::valid_qmf_t<quoted_member_type, void>::value));
  EXPECT_FALSE((ctl::valid_qmf_t<quoted_member_type, void, int>::value));
  EXPECT_TRUE((ctl::valid_qmf_t<quoted_member_type, int>::value));
}

TEST_F(utils_test, quote) {
  { // valid check using ctl::quote
    EXPECT_FALSE((ctl::valid_qmf_t<ctl::quote<member_type>>::value));
    EXPECT_FALSE((ctl::valid_qmf_t<ctl::quote<member_type>, void>::value));
    EXPECT_FALSE((ctl::valid_qmf_t<ctl::quote<member_type>, void, int>::value));
    EXPECT_TRUE((ctl::valid_qmf_t<ctl::quote<member_type>, int>::value));
  }

  { // count if check using ctl::quote
    {
      using empty_list = ctl::list<>;
      auto val = ctl::count_if_qmf_t<empty_list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(0, val);
    }

    {
      using list = ctl::list<int>;
      auto val = ctl::count_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(1, val);
    }

    {
      using list = ctl::list<int, char>;
      auto val = ctl::count_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(2, val);
    }

    {
      using list = ctl::list<int, char, double, int>;
      auto val = ctl::count_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(3, val);
    }

    {
      using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
      auto val = ctl::count_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(8, val);
    }
  }

  { // find if check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 0);
    }

    {
      using list = ctl::list<double, char, int, int>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 1);
    }

    {
      using list = ctl::list<double, float, float, char>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 3);
    }

    {
      using list = ctl::list<double, float, char, int, char>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 2);
    }

    {
      using list = ctl::list<int>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 0);
    }

    {
      using list = ctl::list<float>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 1);
    }

    {
      using list = ctl::list<float, double, float, float>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 4);
    }

    {
      using list = ctl::list<>;
      constexpr auto pos = ctl::find_if_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_EQ(pos, 0);
    }
  }

  { // all of check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<char, int, int>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<char, int>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<char>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<int>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<float>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<>;
      constexpr auto result = ctl::all_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }
  }

  { // any of check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<char, int, int>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<char, int>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<char>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<int>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<float>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<>;
      constexpr auto result = ctl::any_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }
  }

  { // none of check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<char, int, int>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<char, int>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<char>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<int>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_FALSE(result);
    }

    {
      using list = ctl::list<float>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }

    {
      using list = ctl::list<>;
      constexpr auto result = ctl::none_of_qmf_t<list, ctl::quote<pred_int_char>>::value;
      EXPECT_TRUE(result);
    }
  }

  { // select check using ctl::quote
    using list = ctl::list<int, char, double, int>;
    
    {
      constexpr auto expect_true = std::is_same_v<ctl::select_qmf_c_t<true, list, ctl::quote<member_type>, float, double>, list>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::select_qmf_c_t<true, list, ctl::quote<member_type>, float, double>, member_type<float, double>>;
      EXPECT_FALSE(expect_false);
    }

    {
      constexpr auto expect_true = std::is_same_v<ctl::select_qmf_c_t<false, list, ctl::quote<member_type>, float, double>, member_type<float, double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::select_qmf_c_t<false, list, ctl::quote<member_type>, float, double>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      constexpr auto expect_true = std::is_same_v<ctl::select_qmf_t<std::true_type, list, ctl::quote<member_type>, float, double>, list>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::select_qmf_t<std::true_type, list, ctl::quote<member_type>, float, double>, member_type<float, double>>;
      EXPECT_FALSE(expect_false);
    }

    {
      constexpr auto expect_true = std::is_same_v<ctl::select_qmf_t<std::false_type, list, ctl::quote<member_type>, float, double>, member_type<float, double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::select_qmf_t<std::false_type, list, ctl::quote<member_type>, float, double>, list>;
      EXPECT_FALSE(expect_false);
    }
  }

  { // filter check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<int, char, int>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
      constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<char, int, char, int, int, char, int, int>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<>;
      constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<char>>;
      EXPECT_FALSE(expect_false);
    }
  }

  { // replace if check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto expect_true = std::is_same_v<ctl::replace_if_qmf_t<list, ctl::quote<pred_int_char>, float>, ctl::list<float, float, double, float>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::replace_if_qmf_t<list, ctl::quote<pred_int_char>, float>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
      constexpr auto expect_true = std::is_same_v<ctl::replace_if_qmf_t<list, ctl::quote<pred_int_char>, float>, ctl::list<float, float, float, float, double, float, float, float, double, float, double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::replace_if_qmf_t<list, ctl::quote<pred_int_char>, float>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<>;
      constexpr auto expect_true = std::is_same_v<ctl::replace_if_qmf_t<list, ctl::quote<pred_int_char>, float>, ctl::list<>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::replace_if_qmf_t<list, ctl::quote<pred_int_char>, float>, ctl::list<char>>;
      EXPECT_FALSE(expect_false);
    }
  }

  { // copy if check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto expect_true = std::is_same_v<ctl::copy_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<int, char, int>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::copy_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
      constexpr auto expect_true = std::is_same_v<ctl::copy_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<char, int, char, int, int, char, int, int>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::copy_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<>;
      constexpr auto expect_true = std::is_same_v<ctl::copy_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::copy_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<char>>;
      EXPECT_FALSE(expect_false);
    }
  }

  { // remove if check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto expect_true = std::is_same_v<ctl::remove_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::remove_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
      constexpr auto expect_true = std::is_same_v<ctl::remove_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<double, double, double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::remove_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<>;
      constexpr auto expect_true = std::is_same_v<ctl::remove_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::remove_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<char>>;
      EXPECT_FALSE(expect_false);
    }
  }

  { // unique if check using ctl::quote
    {
      using list = ctl::list<int, char, double, int>;
      constexpr auto expect_true = std::is_same_v<ctl::unique_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::unique_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
      constexpr auto expect_true = std::is_same_v<ctl::unique_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<double, double, double>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::unique_if_qmf_t<list, ctl::quote<pred_int_char>>, list>;
      EXPECT_FALSE(expect_false);
    }

    {
      using list = ctl::list<>;
      constexpr auto expect_true = std::is_same_v<ctl::unique_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<>>;
      EXPECT_TRUE(expect_true);

      constexpr auto expect_false = std::is_same_v<ctl::unique_if_qmf_t<list, ctl::quote<pred_int_char>>, ctl::list<char>>;
      EXPECT_FALSE(expect_false);
    }
  }
}

TEST_F(utils_test, not) {
  EXPECT_TRUE((ctl::invert_t<std::integral_constant<bool, false>>::value));
  EXPECT_FALSE((ctl::invert_t<std::integral_constant<bool, true>>::value));

  EXPECT_TRUE((ctl::invert_t<std::false_type>::value));
  EXPECT_FALSE((ctl::invert_t<std::true_type>::value));

  EXPECT_TRUE((ctl::invert_c_t<false>::value));
  EXPECT_FALSE((ctl::invert_c_t<true>::value));
}
