#include <gtest/gtest.h>

#include <type_traits>

#include "container/list.hpp"
#include "container/algorithms.hpp"
#include "debug/show_type.hpp"

class list_test : public ::testing::Test {
  protected:
    void SetUp() override {
      //Add Test setup code here!
    }

    void TearDown() override {
      //Add Test teardown code here!
    }
};

template <typename ...T> struct pred_int : public std::false_type {};
template <typename ...Ts> struct pred_int<int, Ts...> : public std::true_type {};

template <typename ...T> struct pred_int_char : public std::false_type {};
template <typename ...Ts> struct pred_int_char<int, Ts...> : public std::true_type {};
template <typename ...Ts> struct pred_int_char<char, Ts...> : public std::true_type {};

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

template <typename T1, typename T2>
struct comparator {
  constexpr static bool value = T1::value <= T2::value;
};

template <typename T>
struct is_same_indirection {
  constexpr static auto value = T::value;
};

template<class T1, class T2> using first_is_void = std::is_same<T1, void>;
template<class T1, class T2> using second = T2;

struct quoted_first_is_void {
  template <typename T1, typename T2>
  using fn = std::is_same<T1, void>;
};

struct quoted_second {
  template <typename T1, typename T2>
  using fn = T2;
};

//Tests
TEST_F(list_test, basic_type_list_test) {
  using heterogeneous_types = ctl::list<int, char, double>;

  constexpr auto expect_true = std::is_same_v<heterogeneous_types, ctl::list<int, char, double>>;
  EXPECT_TRUE(expect_true);

  constexpr auto expect_false = std::is_same_v<heterogeneous_types, ctl::list<int, char, float>>;
  EXPECT_FALSE(expect_false);
}

TEST_F(list_test, push_front_test) {
  using empty_list = ctl::list<>;
  constexpr auto is_empty_list = std::is_same_v<empty_list, ctl::list<>>;

  using int_added_to_empty_list = ctl::push_front_t<empty_list, int>;
  { //{int}
    constexpr auto expect_true = std::is_same_v<int_added_to_empty_list, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int_added_to_empty_list, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  using int_in_list = ctl::list<int>;
  using char_int_list = ctl::push_front_t<char, int_in_list>;
  { //{char, int}
    constexpr auto expect_true = std::is_same_v<char_int_list, ctl::list<char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char_int_list, ctl::list<int, char>>;
    EXPECT_FALSE(expect_false);
  }

  using float_in_list = ctl::list<float>;
  using float_char_int_list = ctl::push_front_t<char_int_list, float_in_list>;
  { //{float, char, int}
    constexpr auto expect_true = std::is_same_v<float_char_int_list, ctl::list<float, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<float_char_int_list, ctl::list<int, char, float>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, push_back_test) {
  using empty_list = ctl::list<>;
  constexpr auto is_empty_list = std::is_same_v<empty_list, ctl::list<>>;

  using int_added_to_empty_list = ctl::push_back_t<empty_list, int>;
  { //{int}
    constexpr auto expect_true = std::is_same_v<int_added_to_empty_list, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int_added_to_empty_list, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  using int_in_list = ctl::list<int>;
  using int_char_list = ctl::push_back_t<char, int_in_list>;
  { //{char, int}
    constexpr auto expect_true = std::is_same_v<int_char_list, ctl::list<int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int_char_list, ctl::list<char, int>>;
    EXPECT_FALSE(expect_false);
  }

  using char_int_list = ctl::list<char, int>;
  using float_in_list = ctl::list<float>;
  using char_int_float_list = ctl::push_back_t<char_int_list, float_in_list>;
  { //{char, int, float}
    constexpr auto expect_true = std::is_same_v<char_int_float_list, ctl::list<char, int, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char_int_list, ctl::list<float, char, int>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, append_test) {
  using empty_list = ctl::list<>;
  constexpr auto is_empty_list = std::is_same_v<empty_list, ctl::list<>>;

  using int_added_to_empty_list = ctl::append_t<empty_list, int>;
  { //{int}
    constexpr auto expect_true = std::is_same_v<int_added_to_empty_list, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int_added_to_empty_list, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  using int_in_list = ctl::list<int>;
  using int_char_list = ctl::append_t<char, int_in_list>;
  { //{char, int}
    constexpr auto expect_true = std::is_same_v<int_char_list, ctl::list<int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int_char_list, ctl::list<char, int>>;
    EXPECT_FALSE(expect_false);
  }

  using char_int_list = ctl::list<char, int>;
  using float_in_list = ctl::list<float>;
  using char_int_float_list = ctl::append_t<char_int_list, float_in_list>;
  { //{char, int, float}
    constexpr auto expect_true = std::is_same_v<char_int_float_list, ctl::list<char, int, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char_int_list, ctl::list<float, char, int>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, first_of_list) {

  // ctl::first_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<int, ctl::first_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::first_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<float, ctl::first_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int, ctl::first_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<int, ctl::first_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::last_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::first_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<>, ctl::first_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, last_of_list) {

  // ctl::last_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<double, ctl::last_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::last_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<double, ctl::last_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::last_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::last_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::last_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::last_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<>, ctl::last_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, head_of_list) {

  // ctl::head_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<int, char>, ctl::head_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::head_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<float, unsigned int, int, char>, ctl::head_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::head_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<int, char, double, float>, ctl::head_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::head_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::head_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::head_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, front_of_list) {

  // ctl::front_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<int, ctl::front_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<int, char>, ctl::front_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<float, ctl::front_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<float, unsigned int, int, char>, ctl::front_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<int, ctl::front_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<int, char, double, float>, ctl::front_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::front_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<>, ctl::front_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, pop_back_list) {

  // ctl::pop_back_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<int, char>, ctl::pop_back_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::pop_back_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<float, unsigned int, int, char>, ctl::pop_back_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::pop_back_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<int, char, double, float>, ctl::pop_back_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::pop_back_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::pop_back_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::pop_back_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, tail_of_list) {

  // ctl::tail_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<char, double>, ctl::tail_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::tail_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<unsigned int, int, char, double>, ctl::tail_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::tail_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<char, double, float, unsigned int>, ctl::tail_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::tail_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::tail_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::tail_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, back_of_list) {

  // ctl::back_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<double, ctl::back_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<char, double>, ctl::back_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<double, ctl::back_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<unsigned int, int, char, double>, ctl::back_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::back_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<char, double, float, unsigned int>, ctl::back_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::back_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<>, ctl::back_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, pop_front_list) {

  // ctl::pop_front_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<char, double>, ctl::pop_front_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::pop_front_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<unsigned int, int, char, double>, ctl::pop_front_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::pop_front_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<char, double, float, unsigned int>, ctl::pop_front_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::pop_front_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unsigned_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::pop_front_t<unsigned_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::pop_front_t<unsigned_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, list_size) {
  using empty_list = ctl::list<>;
  EXPECT_EQ(0, ctl::size_t<empty_list>::value);
  EXPECT_EQ(0, ctl::size_t<empty_list>());
  EXPECT_EQ(0, ctl::size_v<empty_list>);

  using single_element_list = ctl::list<int>;
  EXPECT_EQ(1, ctl::size_t<single_element_list>::value);
  EXPECT_EQ(1, ctl::size_t<single_element_list>());
  EXPECT_EQ(1, ctl::size_v<single_element_list>);

  using two_elements_list = ctl::list<int, char>;
  EXPECT_EQ(2, ctl::size_t<two_elements_list>::value);
  EXPECT_EQ(2, ctl::size_t<two_elements_list>());
  EXPECT_EQ(2, ctl::size_v<two_elements_list>);

  using three_elements_list = ctl::list<int, char, float>;
  EXPECT_EQ(3, ctl::size_t<three_elements_list>::value);
  EXPECT_EQ(3, ctl::size_t<three_elements_list>());
  EXPECT_EQ(3, ctl::size_v<three_elements_list>);

  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_EQ(5, ctl::size_t<merged_list>::value);
  EXPECT_EQ(5, ctl::size_t<merged_list>());
  EXPECT_EQ(5, ctl::size_v<merged_list>);

  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  EXPECT_EQ(6, ctl::size_t<merged_list2>::value);
  EXPECT_EQ(6, ctl::size_t<merged_list2>());
  EXPECT_EQ(6, ctl::size_v<merged_list2>);
}

TEST_F(list_test, list_count) {
  using empty_list = ctl::list<>;
  EXPECT_EQ(0, ctl::count_t<empty_list>::value);
  EXPECT_EQ(0, ctl::count_t<empty_list>());
  EXPECT_EQ(0, ctl::count_v<empty_list>);

  using single_element_list = ctl::list<int>;
  EXPECT_EQ(1, ctl::count_t<single_element_list>::value);
  EXPECT_EQ(1, ctl::count_t<single_element_list>());
  EXPECT_EQ(1, ctl::count_v<single_element_list>);

  using two_elements_list = ctl::list<int, char>;
  EXPECT_EQ(2, ctl::count_t<two_elements_list>::value);
  EXPECT_EQ(2, ctl::count_t<two_elements_list>());
  EXPECT_EQ(2, ctl::count_v<two_elements_list>);

  using three_elements_list = ctl::list<int, char, float>;
  EXPECT_EQ(3, ctl::count_t<three_elements_list>::value);
  EXPECT_EQ(3, ctl::count_t<three_elements_list>());
    EXPECT_EQ(3, ctl::count_v<three_elements_list>);

  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_EQ(5, ctl::count_t<merged_list>::value);
  EXPECT_EQ(5, ctl::count_t<merged_list>());
  EXPECT_EQ(5, ctl::count_v<merged_list>);

  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  EXPECT_EQ(6, ctl::count_t<merged_list2>::value);
  EXPECT_EQ(6, ctl::count_t<merged_list2>());
  EXPECT_EQ(6, ctl::count_v<merged_list2>);
}

TEST_F(list_test, list_count_predicate) {
  {
    using empty_list = ctl::list<>;
    EXPECT_EQ(0, (ctl::count_if_t<empty_list, pred_int>::value));
    EXPECT_EQ(0, (ctl::count_if_v<empty_list, pred_int>));

    EXPECT_EQ(0, (ctl::count_if_t<empty_list, pred_int_char>::value));
    EXPECT_EQ(0, (ctl::count_if_v<empty_list, pred_int_char>));

    EXPECT_EQ(0, (ctl::count_if_t<empty_list, pred_empty>::value));
    EXPECT_EQ(0, (ctl::count_if_v<empty_list, pred_empty>));

    EXPECT_EQ(0, (ctl::count_if_t<empty_list, pred_all>::value));
    EXPECT_EQ(0, (ctl::count_if_v<empty_list, pred_all>));
  }

  {
    using list = ctl::list<int>;
    EXPECT_EQ(1, (ctl::count_if_t<list, pred_int>::value));
    EXPECT_EQ(1, (ctl::count_if_v<list, pred_int>));

    EXPECT_EQ(1, (ctl::count_if_t<list, pred_int_char>::value));
    EXPECT_EQ(1, (ctl::count_if_v<list, pred_int_char>));

    EXPECT_EQ(0, (ctl::count_if_t<list, pred_empty>::value));
    EXPECT_EQ(0, (ctl::count_if_v<list, pred_empty>));

    EXPECT_EQ(1, (ctl::count_if_t<list, pred_all>::value));
    EXPECT_EQ(1, (ctl::count_if_v<list, pred_all>));
  }

  {
    using list = ctl::list<int, char>;
    EXPECT_EQ(1, (ctl::count_if_t<list, pred_int>::value));
    EXPECT_EQ(1, (ctl::count_if_v<list, pred_int>));

    EXPECT_EQ(2, (ctl::count_if_t<list, pred_int_char>::value));
    EXPECT_EQ(2, (ctl::count_if_v<list, pred_int_char>));

    EXPECT_EQ(0, (ctl::count_if_t<list, pred_empty>::value));
    EXPECT_EQ(0, (ctl::count_if_v<list, pred_empty>));

    EXPECT_EQ(2, (ctl::count_if_t<list, pred_all>::value));
    EXPECT_EQ(2, (ctl::count_if_v<list, pred_all>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ(2, (ctl::count_if_t<list, pred_int>::value));
    EXPECT_EQ(2, (ctl::count_if_v<list, pred_int>));

    EXPECT_EQ(3, (ctl::count_if_t<list, pred_int_char>::value));
    EXPECT_EQ(3, (ctl::count_if_v<list, pred_int_char>));

    EXPECT_EQ(0, (ctl::count_if_t<list, pred_empty>::value));
    EXPECT_EQ(0, (ctl::count_if_v<list, pred_empty>));

    EXPECT_EQ(4, (ctl::count_if_t<list, pred_all>::value));
    EXPECT_EQ(4, (ctl::count_if_v<list, pred_all>));
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    EXPECT_EQ(5, (ctl::count_if_t<list, pred_int>::value));
    EXPECT_EQ(5, (ctl::count_if_v<list, pred_int>));

    EXPECT_EQ(8, (ctl::count_if_t<list, pred_int_char>::value));
    EXPECT_EQ(8, (ctl::count_if_v<list, pred_int_char>));

    EXPECT_EQ(0, (ctl::count_if_t<list, pred_empty>::value));
    EXPECT_EQ(0, (ctl::count_if_v<list, pred_empty>));

    EXPECT_EQ(11, (ctl::count_if_t<list, pred_all>::value));
    EXPECT_EQ(11, (ctl::count_if_v<list, pred_all>));
  }
}

TEST_F(list_test, list_count_qmf) {
  {
    using empty_list = ctl::list<>;
    EXPECT_EQ(0, (ctl::count_if_qmf_v<empty_list, quoted_int_char>));
  }

  {
    using list = ctl::list<int>;
    EXPECT_EQ(1, (ctl::count_if_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<int, char>;
    EXPECT_EQ(2, (ctl::count_if_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ(3, (ctl::count_if_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    EXPECT_EQ(8, (ctl::count_if_qmf_v<list, quoted_int_char>));
  }
}

TEST_F(list_test, empty_check) {
  using empty_list = ctl::list<>;
  EXPECT_TRUE(ctl::empty_t<empty_list>::value);
  EXPECT_TRUE(ctl::empty_t<empty_list>());
  EXPECT_TRUE(ctl::empty_v<empty_list>);

  using single_element_list = ctl::list<int>;
  EXPECT_FALSE(ctl::empty_t<single_element_list>::value);
  EXPECT_FALSE(ctl::empty_t<single_element_list>());
  EXPECT_FALSE(ctl::empty_v<single_element_list>);

  using two_elements_list = ctl::list<int, char>;
  EXPECT_FALSE(ctl::empty_t<two_elements_list>::value);
  EXPECT_FALSE(ctl::empty_t<two_elements_list>());
  EXPECT_FALSE(ctl::empty_v<two_elements_list>);

  using three_elements_list = ctl::list<int, char, float>;
  EXPECT_FALSE(ctl::empty_t<three_elements_list>::value);
  EXPECT_FALSE(ctl::empty_t<three_elements_list>());
  EXPECT_FALSE(ctl::empty_v<three_elements_list>);

  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_FALSE(ctl::empty_t<merged_list>::value);
  EXPECT_FALSE(ctl::empty_t<merged_list>());
  EXPECT_FALSE(ctl::empty_v<merged_list>);

  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  EXPECT_FALSE(ctl::empty_t<merged_list2>::value);
  EXPECT_FALSE(ctl::empty_t<merged_list2>());
  EXPECT_FALSE(ctl::empty_v<merged_list2>);
}

TEST_F(list_test, list_clear) {
  using empty_list = ctl::list<>;
  auto value = std::is_same_v<ctl::list<>, ctl::clear_t<empty_list>>;
  EXPECT_TRUE(value);

  value = false;
  using single_element_list = ctl::list<int>;
  value = std::is_same_v<ctl::list<>, ctl::clear_t<single_element_list>>;
  EXPECT_TRUE(value);

  value = false;
  using two_elements_list = ctl::list<int, char>;
  value = std::is_same_v<ctl::list<>, ctl::clear_t<two_elements_list>>;
  EXPECT_TRUE(value);

  value = false;
  using three_elements_list = ctl::list<int, char, float>;
  value = std::is_same_v<ctl::list<>, ctl::clear_t<three_elements_list>>;
  EXPECT_TRUE(value);

  value = false;
  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  value = std::is_same_v<ctl::list<>, ctl::clear_t<merged_list>>;
  EXPECT_TRUE(value);

  value = false;
  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  value = std::is_same_v<ctl::list<>, ctl::clear_t<merged_list2>>;
  EXPECT_TRUE(value);
}

TEST_F(list_test, remove_type) {
  using list_with_repeated_type = ctl::list<int, char, double, int>;

  {
    using after_remove_int = ctl::remove_type_t<list_with_repeated_type, int>;
    constexpr auto expect_true = std::is_same_v<after_remove_int, ctl::list<char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_int, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  { 
    using after_remove_char = ctl::remove_type_t<list_with_repeated_type, char>;
    constexpr auto expect_true = std::is_same_v<after_remove_char, ctl::list<int, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_char, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  { 
    using after_remove_double = ctl::remove_type_t<list_with_repeated_type, double>;
    constexpr auto expect_true = std::is_same_v<after_remove_double, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_double, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  { 
    using after_remove_float = ctl::remove_type_t<list_with_repeated_type, float>;
    constexpr auto expect_true = std::is_same_v<after_remove_float, ctl::list<int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_float, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  using list_with_one_type = ctl::list<int>;
  { 
    using after_remove_int = ctl::remove_type_t<list_with_one_type, int>;
    constexpr auto expect_true = std::is_same_v<after_remove_int, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    EXPECT_TRUE(ctl::empty_t<after_remove_int>::value);

    constexpr auto expect_false = std::is_same_v<after_remove_int, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  using list_with_one_type = ctl::list<int>;
  { 
    using after_remove_char = ctl::remove_type_t<list_with_one_type, char>;
    constexpr auto expect_true = std::is_same_v<after_remove_char, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_char, ctl::list<>>;
    EXPECT_FALSE(expect_false);

    EXPECT_FALSE(ctl::empty_t<after_remove_char>::value);
  }

  using empty_list = ctl::list<>;
  { 
    using after_remove_char = ctl::remove_type_t<empty_list, char>;
    constexpr auto expect_true = std::is_same_v<after_remove_char, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_char, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, remove_duplicates) {
  {
    using one_repeated_type = ctl::list<int, char, double, int>;
    using after_remove_duplicate = ctl::remove_duplicates_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using one_repeated_type = ctl::list<int, char, int, double>;
    using after_remove_duplicate = ctl::remove_duplicates_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using one_repeated_type = ctl::list<int, int, char, double>;
    using after_remove_duplicate = ctl::remove_duplicates_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using one_repeated_type = ctl::list<int, int, int, char, int, double, int>;
    using after_remove_duplicate = ctl::remove_duplicates_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using multi_repeated_type = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using after_remove_duplicate = ctl::remove_duplicates_t<multi_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<char, int, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using multi_repeated_type = ctl::list<int, int, int, char, char, char, double, double, double>;
    using after_remove_duplicate = ctl::remove_duplicates_t<multi_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using no_repeated_type = ctl::list<double, float, int>;
    using after_remove_duplicate = ctl::remove_duplicates_t<no_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<double, float, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list_with_one_type = ctl::list<int>;
    using after_remove_duplicate = ctl::remove_duplicates_t<list_with_one_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using empty_list = ctl::list<>;
    using after_remove_duplicate = ctl::remove_duplicates_t<empty_list>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, reverse) {
  {
    using list = ctl::list<int, char, double, int>;
    using after_reverse = ctl::reverse_t<list>;
    constexpr auto expect_true = std::is_same_v<after_reverse, ctl::list<int, double, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_reverse, ctl::list<int, char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<double, int>;
    using after_reverse = ctl::reverse_t<list>;
    constexpr auto expect_true = std::is_same_v<after_reverse, ctl::list<int, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_reverse, ctl::list<double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, int, int, char, char, char, double, double, double>;
    using after_reverse = ctl::reverse_t<list>;
    constexpr auto expect_true = std::is_same_v<after_reverse, ctl::list<double, double, double, char, char, char, int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_reverse, ctl::list<int, int, int, char, char, char, double, double, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using after_reverse = ctl::reverse_t<list>;
    constexpr auto expect_true = std::is_same_v<after_reverse, ctl::list<double, int, double, int, char, int, double, int, char, int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_reverse, ctl::list<char, int, char, int, double, int, char, int, double, int, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list_with_one_type = ctl::list<int>;
    using after_reverse = ctl::reverse_t<list_with_one_type>;
    constexpr auto expect_true = std::is_same_v<after_reverse, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_reverse, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using empty_list = ctl::list<>;
    using after_reverse = ctl::reverse_t<empty_list>;
    constexpr auto expect_true = std::is_same_v<after_reverse, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_reverse, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, at) {
  using list = ctl::list<int, char, double, int>;

  {
    constexpr auto expect_true = std::is_same_v<ctl::at_c_t<list, 0>, int>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_c_t<list, 0>, char>;
    EXPECT_FALSE(expect_false);
  }

  {
    using index_type = std::integral_constant<std::size_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::at_t<list, index_type>, int>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_t<list, index_type>, char>;
    EXPECT_FALSE(expect_false);
  }

  {
    using index_type = std::integral_constant<std::uint64_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::at_t<list, index_type>, int>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_t<list, index_type>, char>;
    EXPECT_FALSE(expect_false);
  }

  {
    using index_type = std::integral_constant<std::uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::at_t<list, index_type>, int>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_t<list, index_type>, char>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::at_c_t<list, 1>, char>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_c_t<list, 1>, int>;
    EXPECT_FALSE(expect_false);
  }

  {
    using index_type = std::integral_constant<std::size_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::at_t<list, index_type>, char>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_t<list, index_type>, int>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::at_c_t<list, 2>, double>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_c_t<list, 2>, int>;
    EXPECT_FALSE(expect_false);
  }

  {
    using index_type = std::integral_constant<std::size_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::at_t<list, index_type>, double>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_t<list, index_type>, int>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::at_c_t<list, 3>, int>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_c_t<list, 3>, double>;
    EXPECT_FALSE(expect_false);
  }

  {
    using index_type = std::integral_constant<std::size_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::at_t<list, index_type>, int>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::at_t<list, index_type>, double>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, filter_only_ints) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int, list>, ctl::list<int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int, list>, ctl::list<int, int, int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int, list>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int, list>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
  
  {
    using L1 = ctl::list<int, char, double, int>;
    using L2 = ctl::list<float, double, void, char>;
    using L3 = ctl::list<void, int, char, char>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int, L1, L2, L3>, ctl::list<int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using L2 = ctl::list<void, int, void, int, int, int, void, int, char[2], int, double>;
    using L3 = ctl::list<double, int, char, int, float, int, char, int, double, char[3], double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int, L1, L2, L3>, ctl::list<int, int, int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<>;
    using L2 = ctl::list<>;
    using L3 = ctl::list<>;

    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int, L1, L2, L3>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int, L1, L2, L3>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }  
}

TEST_F(list_test, filter_only_ints_chars) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int_char, list>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int_char, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int_char, list>, ctl::list<char, int, char, int, int, char, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int_char, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int_char, list>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int_char, list>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<int, char, double, int>;
    using L2 = ctl::list<float, double, void, char>;
    using L3 = ctl::list<void, int, char, char>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int_char, L1, L2, L3>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int_char, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using L2 = ctl::list<void, int, void, int, int, int, void, int, char[2], int, double>;
    using L3 = ctl::list<double, int, char, int, float, int, char, int, double, char[3], double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int_char, L1, L2, L3>, ctl::list<char, int, char, int, int, char, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int_char, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<>;
    using L2 = ctl::list<>;
    using L3 = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_int_char, L1, L2, L3>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_int_char, L1, L2, L3>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, filter_none) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_empty, list>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_empty, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_empty, list>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_empty, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_empty, list>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_empty, list>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<int, char, double, int>;
    using L2 = ctl::list<float, double, void, char>;
    using L3 = ctl::list<void, int, char, char>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_empty, L1, L2, L3>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_empty, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using L2 = ctl::list<void, int, void, int, int, int, void, int, char[2], int, double>;
    using L3 = ctl::list<double, int, char, int, float, int, char, int, double, char[3], double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_empty, L1, L2, L3>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_empty, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<>;
    using L2 = ctl::list<>;
    using L3 = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_empty, L1, L2, L3>, L1>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_empty, L1, L2, L3>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, filter_all) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_all, list>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_all, list>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_all, list>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_all, list>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_all, list>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_all, list>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<int, char, double, int>;
    using L2 = ctl::list<float, double, void, char>;
    using L3 = ctl::list<void, int, char, char>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_all, L1, L2, L3>, L1>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_all, L1, L2, L3>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using L2 = ctl::list<void, int, void, int, int, int, void, int, char[2], int, double>;
    using L3 = ctl::list<double, int, char, int, float, int, char, int, double, char[3], double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_all, L1, L2, L3>, L1>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_all, L1, L2, L3>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<>;
    using L2 = ctl::list<>;
    using L3 = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_t<pred_all, L1, L2, L3>, L1>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_t<pred_all, L1, L2, L3>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, filter_using_quoted_meta_function) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, list>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, list>, ctl::list<char, int, char, int, int, char, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, list>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, list>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<int, char, double, int>;
    using L2 = ctl::list<float, double, void, char>;
    using L3 = ctl::list<void, int, char, char>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, L1, L2, L3>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using L2 = ctl::list<void, int, void, int, int, int, void, int, char[2], int, double>;
    using L3 = ctl::list<double, int, char, int, float, int, char, int, double, char[3], double>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, L1, L2, L3>, ctl::list<char, int, char, int, int, char, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, L1, L2, L3>, L1>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = ctl::list<>;
    using L2 = ctl::list<>;
    using L3 = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, L1, L2, L3>, L1>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::filter_if_qmf_t<quoted_int_char, L1, L2, L3>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, contains) {
  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_TRUE((ctl::contains_v<list, int>));
    EXPECT_TRUE((ctl::contains_v<list, double>));
    EXPECT_TRUE((ctl::contains_v<list, char>));
    EXPECT_FALSE((ctl::contains_v<list, float>));
  }

  {
    using list = ctl::list<char, int, float, int, float, int, char, int, int>;
    EXPECT_TRUE((ctl::contains_v<list, int>));
    EXPECT_TRUE((ctl::contains_v<list, float>));
    EXPECT_TRUE((ctl::contains_v<list, char>));
    EXPECT_FALSE((ctl::contains_v<list, double>));
  }

  {
    using list = ctl::list<>;
    EXPECT_FALSE((ctl::contains_v<list, int>));
    EXPECT_FALSE((ctl::contains_v<list, float>));
    EXPECT_FALSE((ctl::contains_v<list, char>));
    EXPECT_FALSE((ctl::contains_v<list, double>));
  }
}

TEST_F(list_test, drop) {
  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::drop_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::drop_t<list, count>, ctl::list<double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::drop_t<list, count>, ctl::list<double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::drop_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::drop_t<list, count>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::drop_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::drop_t<list, count>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::drop_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    using count = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::drop_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::drop_t<list, count>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using count = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::drop_t<list, count>, ctl::list<int, char, int, double, int, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::drop_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, take) {
  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::take_t<list, count>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::take_t<list, count>, ctl::list<double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::take_t<list, count>, ctl::list<int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::take_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::take_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::take_t<list, count>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::take_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::take_t<list, count>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    using count = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::take_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::take_t<list, count>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using count = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::take_t<list, count>, ctl::list<char, int, char, int, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::take_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, repeat) {
  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, char, double, int, int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, char, double, int, int, char, double, int, int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using count = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, char, double, int, int, char, double, int, int, char, double, int, int, char, double, int, int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    using count = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    using count = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    using count = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using count = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using count = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using count = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using count = std::integral_constant<uint32_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using count = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using count = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<int, int, int, int, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using count = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using count = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using count = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<char, int, char, int, double, int, char, int, double, int, double, char, int, char, int, double, int, char, int, double, int, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::repeat_t<list, count>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, from_integer_sequence) {
  
  {
    using seq = std::integer_sequence<unsigned int, 9, 2, 5, 1, 9, 1, 6>;
    using from_seq = ctl::from_integer_sequence_t<seq>;

    constexpr auto expect_true = std::is_same_v<from_seq, std::tuple<std::integral_constant<unsigned int, 9>, std::integral_constant<unsigned int, 2>, std::integral_constant<unsigned int, 5>, std::integral_constant<unsigned int, 1>, std::integral_constant<unsigned int, 9>, std::integral_constant<unsigned int, 1>, std::integral_constant<unsigned int, 6> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using seq = std::make_integer_sequence<int, 5>;
    using from_seq = ctl::from_integer_sequence_t<seq>;

    constexpr auto expect_true = std::is_same_v<from_seq, std::tuple<std::integral_constant<int, 0>, std::integral_constant<int, 1>, std::integral_constant<int, 2>, std::integral_constant<int, 3>, std::integral_constant<int, 4> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using seq = std::make_index_sequence<10>;
    using from_seq = ctl::from_integer_sequence_t<seq>;

    constexpr auto expect_true = std::is_same_v<from_seq, std::tuple<std::integral_constant<long unsigned int, 0>, std::integral_constant<long unsigned int, 1>, std::integral_constant<long unsigned int, 2>, std::integral_constant<long unsigned int, 3>, std::integral_constant<long unsigned int, 4>, std::integral_constant<long unsigned int, 5>, std::integral_constant<long unsigned int, 6>, std::integral_constant<long unsigned int, 7>, std::integral_constant<long unsigned int, 8>, std::integral_constant<long unsigned int, 9> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using seq = std::integer_sequence<unsigned int, 9, 2, 5, 1, 9, 1, 6>;
    using from_seq = ctl::from_integer_sequence_t<seq, ctl::list>;

    constexpr auto expect_true = std::is_same_v<from_seq, ctl::list<std::integral_constant<unsigned int, 9>, std::integral_constant<unsigned int, 2>, std::integral_constant<unsigned int, 5>, std::integral_constant<unsigned int, 1>, std::integral_constant<unsigned int, 9>, std::integral_constant<unsigned int, 1>, std::integral_constant<unsigned int, 6> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using seq = std::make_integer_sequence<int, 5>;
    using from_seq = ctl::from_integer_sequence_t<seq, ctl::list>;

    constexpr auto expect_true = std::is_same_v<from_seq, ctl::list<std::integral_constant<int, 0>, std::integral_constant<int, 1>, std::integral_constant<int, 2>, std::integral_constant<int, 3>, std::integral_constant<int, 4> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using seq = std::make_index_sequence<10>;
    using from_seq = ctl::from_integer_sequence_t<seq, ctl::list>;

    constexpr auto expect_true = std::is_same_v<from_seq, ctl::list<std::integral_constant<long unsigned int, 0>, std::integral_constant<long unsigned int, 1>, std::integral_constant<long unsigned int, 2>, std::integral_constant<long unsigned int, 3>, std::integral_constant<long unsigned int, 4>, std::integral_constant<long unsigned int, 5>, std::integral_constant<long unsigned int, 6>, std::integral_constant<long unsigned int, 7>, std::integral_constant<long unsigned int, 8>, std::integral_constant<long unsigned int, 9> >>;
    EXPECT_TRUE(expect_true);
  }
}

TEST_F(list_test, iota) {
  
  {
    using count = std::integral_constant<unsigned int, 7>;

    constexpr auto expect_true = std::is_same_v<ctl::iota_t<count>, std::tuple<std::integral_constant<unsigned int, 0>, std::integral_constant<unsigned int, 1>, std::integral_constant<unsigned int, 2>, std::integral_constant<unsigned int, 3>, std::integral_constant<unsigned int, 4>, std::integral_constant<unsigned int, 5>, std::integral_constant<unsigned int, 6> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using count = std::integral_constant<int, 5>;

    constexpr auto expect_true = std::is_same_v<ctl::iota_t<count>, std::tuple<std::integral_constant<int, 0>, std::integral_constant<int, 1>, std::integral_constant<int, 2>, std::integral_constant<int, 3>, std::integral_constant<int, 4> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using count = std::integral_constant<long unsigned int, 10>;

    constexpr auto expect_true = std::is_same_v<ctl::iota_t<count>, std::tuple<std::integral_constant<long unsigned int, 0>, std::integral_constant<long unsigned int, 1>, std::integral_constant<long unsigned int, 2>, std::integral_constant<long unsigned int, 3>, std::integral_constant<long unsigned int, 4>, std::integral_constant<long unsigned int, 5>, std::integral_constant<long unsigned int, 6>, std::integral_constant<long unsigned int, 7>, std::integral_constant<long unsigned int, 8>, std::integral_constant<long unsigned int, 9> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using count = std::integral_constant<unsigned int, 7>;

    constexpr auto expect_true = std::is_same_v<ctl::iota_t<count, ctl::list>, ctl::list<std::integral_constant<unsigned int, 0>, std::integral_constant<unsigned int, 1>, std::integral_constant<unsigned int, 2>, std::integral_constant<unsigned int, 3>, std::integral_constant<unsigned int, 4>, std::integral_constant<unsigned int, 5>, std::integral_constant<unsigned int, 6> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using count = std::integral_constant<int, 5>;

    constexpr auto expect_true = std::is_same_v<ctl::iota_t<count, ctl::list>, ctl::list<std::integral_constant<int, 0>, std::integral_constant<int, 1>, std::integral_constant<int, 2>, std::integral_constant<int, 3>, std::integral_constant<int, 4> >>;
    EXPECT_TRUE(expect_true);
  }

  {
    using count = std::integral_constant<long unsigned int, 10>;

    constexpr auto expect_true = std::is_same_v<ctl::iota_t<count, ctl::list>, ctl::list<std::integral_constant<long unsigned int, 0>, std::integral_constant<long unsigned int, 1>, std::integral_constant<long unsigned int, 2>, std::integral_constant<long unsigned int, 3>, std::integral_constant<long unsigned int, 4>, std::integral_constant<long unsigned int, 5>, std::integral_constant<long unsigned int, 6>, std::integral_constant<long unsigned int, 7>, std::integral_constant<long unsigned int, 8>, std::integral_constant<long unsigned int, 9> >>;
    EXPECT_TRUE(expect_true);
  }
}

TEST_F(list_test, insert) {
  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<float, int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, float, char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, float, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, char, float, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, char, double, float, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, char, double, int, float>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, char, double, int, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float>, ctl::list<int, char, double, float, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<float, void, int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, float, void, char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, float, void, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, char, float, void, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, char, double, float, void, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, char, double, int, float, void>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, char, double, int, float, void>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<int, char, double, float, void, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    using pos = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<float, void>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, float, void>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, erase) {
  // {
  //   using list = ctl::list<int, char, double, int>;
  //   using pos = std::integral_constant<uint32_t, 0>;

  //   constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos, pos>, ctl::list<char, double, int>>;
  //   EXPECT_TRUE(expect_true);

  //   constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos, pos>, ctl::list<int, char, double, int>>;
  //   EXPECT_FALSE(expect_false);
  // }

  // {
  //   using list = ctl::list<int, char, double, int>;
  //   using pos = std::integral_constant<uint32_t, 1>;
  //   constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos, pos>, ctl::list<int, double, int>>;
  //   EXPECT_TRUE(expect_true);

  //   constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos, pos>, ctl::list<int, char, int>>;
  //   EXPECT_FALSE(expect_false);
  // }

  // {
  //   using list = ctl::list<int, char, double, int>;
  //   using pos = std::integral_constant<uint32_t, 3>;
  //   constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos, pos>, ctl::list<int, char, double>>;
  //   EXPECT_TRUE(expect_true);

  //   constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos, pos>, ctl::list<int, char, int>>;
  //   EXPECT_FALSE(expect_false);
  // }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 0>;
    using pos2 = std::integral_constant<uint32_t, 1>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::insert_t<list, pos1, pos2>, ctl::list<double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 1>;
    using pos2 = std::integral_constant<uint32_t, 2>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 1>;
    using pos2 = std::integral_constant<uint32_t, 3>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 1>;
    using pos2 = std::integral_constant<uint32_t, 4>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 0>;
    using pos2 = std::integral_constant<uint32_t, 4>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 0>;
    using pos2 = std::integral_constant<uint32_t, 3>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 0>;
    using pos2 = std::integral_constant<uint32_t, 2>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 2>;
    using pos2 = std::integral_constant<uint32_t, 3>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos1 = std::integral_constant<uint32_t, 2>;
    using pos2 = std::integral_constant<uint32_t, 4>;

    constexpr auto expect_true = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::erase_t<list, pos1, pos2>, ctl::list<int, char, double>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace) {
  {
    using list = ctl::list<int, char, double, int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, int, float>, ctl::list<float, char, double, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, int, float>, ctl::list<float, char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, char, float>, ctl::list<int, float, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, char, float>, ctl::list<int, char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, double, float>, ctl::list<int, char, float, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, double, float>, ctl::list<int, char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, int, float>, ctl::list<float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, int, float>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, int, int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, int, float>, ctl::list<float, float, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, int, float>, ctl::list<int, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, char, float>, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, char, float>, ctl::list<float>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, int, int>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_t<list, char, float>, ctl::list<int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_t<list, char, float>, ctl::list<float, float, float>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace_at) {
  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 0>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float, char, double, float>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 1>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<int, float, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float, char, double, float>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 2>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<int, char, float, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float, char, double, float>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 3>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<int, char, double, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float, char, double, float>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int>;
    using pos = std::integral_constant<uint32_t, 0>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, int, int>;
    using pos = std::integral_constant<uint32_t, 0>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<float, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, int, int>;
    using pos = std::integral_constant<uint32_t, 1>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<int, float, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, int, int>;
    using pos = std::integral_constant<uint32_t, 2>;

    constexpr auto expect_true = std::is_same_v<ctl::replace_at_t<list, pos, float>, ctl::list<int, int, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_at_t<list, pos, float>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace_only_ints) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_int, float>, ctl::list<float, char, double, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_int, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_int, float>, ctl::list<char, float, char, float, double, float, char, float, double, float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_int, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_int, float>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_int, float>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace_only_ints_chars) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_int_char, float>, ctl::list<float, float, double, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_int_char, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_int_char, float>, ctl::list<float, float, float, float, double, float, float, float, double, float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_int_char, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_int_char, float>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_int_char, float>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace_none) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_empty, float>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_empty, float>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_empty, float>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_empty, float>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_empty, float>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_empty, float>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace_all) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_all, float>, ctl::list<float, float, float, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_all, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_all, float>, ctl::list<float, float, float, float, float, float, float, float, float, float, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_all, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_t<list, pred_all, float>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_t<list, pred_all, float>, ctl::list<float>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, replace_using_quoted_meta_function) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_qmf_t<list, quoted_int_char, float>, ctl::list<float, float, double, float>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_qmf_t<list, quoted_int_char, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_qmf_t<list, quoted_int_char, float>, ctl::list<float, float, float, float, double, float, float, float, double, float, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_qmf_t<list, quoted_int_char, float>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::replace_if_qmf_t<list, quoted_int_char, float>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::replace_if_qmf_t<list, quoted_int_char, float>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, rotate_left) {
  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<double, int, int, char>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<double, int, int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<int, int, char, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<int, int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_left_t<list, pos>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_left_t<list, pos>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_left_t<list, pos>, ctl::list<double, int, int, char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, rotate_right) {
  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 0>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<int, char, double, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 1>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<int, int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 2>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<double, int, int, char>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<int, int, char, double>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 3>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_right_t<list, pos>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 4>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_right_t<list, pos>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<int, char, double, int>;
    using pos = std::integral_constant<uint32_t, 5>;
    constexpr auto expect_true = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<int, int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::rotate_right_t<list, pos>, ctl::list<char, double, int, int>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, copy_if_only_ints) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_int>, ctl::list<int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_int>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_int>, ctl::list<int, int, int, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_int>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_int>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_int>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, copy_if_only_ints_chars) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_int_char>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_int_char>, ctl::list<char, int, char, int, int, char, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_int_char>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_int_char>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, copy_if_none) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_empty>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_empty>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_empty>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, copy_if_all) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_all>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_all>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_all>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_all>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_t<list, pred_all>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_t<list, pred_all>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, copy_if_using_quoted_meta_function) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_qmf_t<list, quoted_int_char>, ctl::list<int, char, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_qmf_t<list, quoted_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_qmf_t<list, quoted_int_char>, ctl::list<char, int, char, int, int, char, int, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_qmf_t<list, quoted_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::copy_if_qmf_t<list, quoted_int_char>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::copy_if_qmf_t<list, quoted_int_char>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, remove_if_only_ints) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_int>, ctl::list<char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_int>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_int>, ctl::list<char, char, double, char, double, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_int>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_int>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_int>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, remove_if_only_ints_chars) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_int_char>, ctl::list<double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_int_char>, ctl::list<double, double, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_int_char>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_int_char>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, remove_if_none) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_empty>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_empty>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_empty>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, remove_if_all) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_all>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_all>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_all>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_all>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_t<list, pred_all>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_t<list, pred_all>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, remove_if_using_quoted_meta_function) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_qmf_t<list, quoted_int_char>, ctl::list<double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_qmf_t<list, quoted_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_qmf_t<list, quoted_int_char>, ctl::list<double, double, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_qmf_t<list, quoted_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::remove_if_qmf_t<list, quoted_int_char>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::remove_if_qmf_t<list, quoted_int_char>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, find) {
  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_v<list, int>), 0);
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_v<list, char>), 1);
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_v<list, double>), 2);
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_v<list, float>), 4);
  }

  {
    using list = ctl::list<>;
    EXPECT_EQ((ctl::find_v<list, float>), 0);
  }

  {
    using list = ctl::list<float>;
    EXPECT_EQ((ctl::find_v<list, float>), 0);
  }

  {
    using list = ctl::list<int>;
    EXPECT_EQ((ctl::find_v<list, float>), 1);
  }
}

TEST_F(list_test, find_if) {
  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_if_v<list, pred_int>), 0);
  }

  {
    using list = ctl::list<double, int, char, double, int>;
    EXPECT_EQ((ctl::find_if_v<list, pred_int_char>), 1);
  }

  {
    using list = ctl::list<double, char, int, int>;
    EXPECT_EQ((ctl::find_if_v<list, pred_int_char>), 1);
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_if_v<list, pred_empty>), 4);
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_if_v<list, pred_all>), 0);
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 0);
  }

  {
    using list = ctl::list<double, char, int, int>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 1);
  }

  {
    using list = ctl::list<double, float, float, char>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 3);
  }

  {
    using list = ctl::list<double, float, char, int, char>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 2);
  }

  {
    using list = ctl::list<int>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 0);
  }

  {
    using list = ctl::list<float>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 1);
  }

  {
    using list = ctl::list<float, double, float, float>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 4);
  }

  {
    using list = ctl::list<>;
    EXPECT_EQ((ctl::find_if_qmf_v<list, quoted_int_char>), 0);
  }
}

TEST_F(list_test, unique) {
  {
    using one_repeated_type = ctl::list<int, char, double, int>;
    using after_remove_duplicate = ctl::unique_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using one_repeated_type = ctl::list<int, char, int, double>;
    using after_remove_duplicate = ctl::unique_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using one_repeated_type = ctl::list<int, int, char, double>;
    using after_remove_duplicate = ctl::unique_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using one_repeated_type = ctl::list<int, int, int, char, int, double, int>;
    using after_remove_duplicate = ctl::unique_t<one_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using multi_repeated_type = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    using after_remove_duplicate = ctl::unique_t<multi_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<char, int, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using multi_repeated_type = ctl::list<int, int, int, char, char, char, double, double, double>;
    using after_remove_duplicate = ctl::unique_t<multi_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int, char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using no_repeated_type = ctl::list<double, float, int>;
    using after_remove_duplicate = ctl::unique_t<no_repeated_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<double, float, int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char, double, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list_with_one_type = ctl::list<int>;
    using after_remove_duplicate = ctl::unique_t<list_with_one_type>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<int>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using empty_list = ctl::list<>;
    using after_remove_duplicate = ctl::unique_t<empty_list>;
    constexpr auto expect_true = std::is_same_v<after_remove_duplicate, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<after_remove_duplicate, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, unique_if_only_ints) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_int>, ctl::list<char, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_int>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_int>, ctl::list<char, char, double, char, double, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_int>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_int>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_int>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, unique_if_only_ints_chars) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_int_char>, ctl::list<double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_int_char>, ctl::list<double, double, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_int_char>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_int_char>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, unique_if_none) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_empty>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_empty>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_empty>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_empty>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, unique_if_all) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_all>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_all>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_all>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_all>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_t<list, pred_all>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_t<list, pred_all>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, unique_if_using_quoted_meta_function) {
  {
    using list = ctl::list<int, char, double, int>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_qmf_t<list, quoted_int_char>, ctl::list<double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_qmf_t<list, quoted_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<char, int, char, int, double, int, char, int, double, int, double>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_qmf_t<list, quoted_int_char>, ctl::list<double, double, double>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_qmf_t<list, quoted_int_char>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    using list = ctl::list<>;
    constexpr auto expect_true = std::is_same_v<ctl::unique_if_qmf_t<list, quoted_int_char>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::unique_if_qmf_t<list, quoted_int_char>, ctl::list<char>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, all_of) {
  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::all_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<>;
    EXPECT_TRUE((ctl::all_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<float>;
    EXPECT_FALSE((ctl::all_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<int, int>;
    EXPECT_TRUE((ctl::all_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<double, int, char, double, int>;
    EXPECT_FALSE((ctl::all_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<double, char, int, int>;
    EXPECT_FALSE((ctl::all_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<char, int>;
    EXPECT_TRUE((ctl::all_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<char, int, int>;
    EXPECT_TRUE((ctl::all_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::all_of_v<list, pred_empty>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_TRUE((ctl::all_of_v<list, pred_all>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int, int>;
    EXPECT_TRUE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int>;
    EXPECT_TRUE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char>;
    EXPECT_TRUE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<int>;
    EXPECT_TRUE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<float>;
    EXPECT_FALSE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<>;
    EXPECT_TRUE((ctl::all_of_qmf_v<list, quoted_int_char>));
  }
}

TEST_F(list_test, any_of) {
  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<>;
    EXPECT_FALSE((ctl::any_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<float>;
    EXPECT_FALSE((ctl::any_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<int, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<double, int, char, double, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<double, char, int, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<char, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<char, int, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::any_of_v<list, pred_empty>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_TRUE((ctl::any_of_v<list, pred_all>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_TRUE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int, int>;
    EXPECT_TRUE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int>;
    EXPECT_TRUE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char>;
    EXPECT_TRUE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<int>;
    EXPECT_TRUE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<float>;
    EXPECT_FALSE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<>;
    EXPECT_FALSE((ctl::any_of_qmf_v<list, quoted_int_char>));
  }
}

TEST_F(list_test, none_of) {
  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<>;
    EXPECT_TRUE((ctl::none_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<float>;
    EXPECT_TRUE((ctl::none_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<int, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_int>));
  }

  {
    using list = ctl::list<double, int, char, double, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<double, char, int, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<char, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<char, int, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_int_char>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_TRUE((ctl::none_of_v<list, pred_empty>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::none_of_v<list, pred_all>));
  }

  {
    using list = ctl::list<int, char, double, int>;
    EXPECT_FALSE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int, int>;
    EXPECT_FALSE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char, int>;
    EXPECT_FALSE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<char>;
    EXPECT_FALSE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<int>;
    EXPECT_FALSE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<float>;
    EXPECT_TRUE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }

  {
    using list = ctl::list<>;
    EXPECT_TRUE((ctl::none_of_qmf_v<list, quoted_int_char>));
  }
}

TEST_F(list_test, transform) {
  using list = ctl::list<int, char, double, int>;

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_t<add_pointer_t, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_t<add_pointer_t, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_t<add_pointer_t, ctl::list<>>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_t<add_pointer_t, ctl::list<>>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    // constexpr auto expect_true = std::is_same_v<ctl::transform_t<predicate_type, list>, ctl::list<std::integral_constant<bool, true>, std::integral_constant<bool, false>, std::integral_constant<bool, false>, std::integral_constant<bool, true> >>;
    constexpr auto expect_true = std::is_same_v<ctl::transform_t<predicate_type, list>, ctl::list<std::true_type, std::false_type, std::false_type, std::true_type>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_t<predicate_type, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_t<predicate_type, ctl::list<>>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_t<predicate_type, ctl::list<>>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, transform_multi_list) {
  {
    using list1 = ctl::list<int, char, double, int>;
    using list2 = ctl::list<int, char, double, int>;
    
    auto lists_compare = ctl::all_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::any_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::none_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);
  }

  {
    using list1 = ctl::list<int, char, double, int>;
    using list2 = ctl::list<int, char, double, float>;
    
    auto lists_compare = ctl::any_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::all_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);

    lists_compare = true;
    lists_compare = ctl::none_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);
  }

  {
    using list1 = ctl::list<int, char, double, int>;
    using list2 = ctl::list<char, double, int, float>;
    
    auto lists_compare = ctl::none_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::all_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);

    lists_compare = true;
    lists_compare = ctl::any_of_t<ctl::transform_t<std::is_same, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);
  }

  {
    using list1 = ctl::list<int, char, double, int>;
    using list2 = ctl::list<int, char, double, int>;
    
    auto lists_compare = ctl::all_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::any_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::none_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);
  }

  {
    using list1 = ctl::list<int, char, double, int>;
    using list2 = ctl::list<int, char, double, float>;
    
    auto lists_compare = ctl::all_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);

    lists_compare = ctl::any_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);

    lists_compare = ctl::none_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);
  }

  {
    using list1 = ctl::list<int, char, double, int>;
    using list2 = ctl::list<char, double, int, float>;
    
    auto lists_compare = ctl::all_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);

    lists_compare = true;
    lists_compare = ctl::any_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_FALSE(lists_compare);

    lists_compare = ctl::none_of_t<ctl::transform_qmf_t<ctl::quote<std::is_same>, list1, list2>, is_same_indirection>::value;
    EXPECT_TRUE(lists_compare);
  }

  {
    using type1 = ctl::list<int, char>;
    using type2 = ctl::list<char, double>;
    using type3 = ctl::list<double, float>;

    auto expect_true = std::is_same_v<ctl::transform_t<std::tuple, type1, type2, type3>, ctl::list<std::tuple<int, char, double>, std::tuple<char, double, float>>>;
    EXPECT_TRUE(expect_true);

    auto expect_false = std::is_same_v<ctl::transform_t<std::tuple, type1, type2, type3>, ctl::list<ctl::list<int, char, double>, ctl::list<char, double, float>>>;
    EXPECT_FALSE(expect_false);

    expect_false = std::is_same_v<ctl::transform_t<std::tuple, type1, type2, type3>, std::tuple<ctl::list<int, char, double>, ctl::list<char, double, float>>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, transform_quoted_meta_function) {
  using list = ctl::list<int, char, double, int>;

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_qmf_t<quoted_add_ptr, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_qmf_t<quoted_add_ptr, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_qmf_t<quoted_add_ptr, ctl::list<>>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_qmf_t<quoted_add_ptr, ctl::list<>>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_qmf_t<quoted_pred_type, list>, ctl::list<std::true_type, std::false_type, std::false_type, std::true_type>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_qmf_t<quoted_pred_type, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_qmf_t<quoted_pred_type, ctl::list<>>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_qmf_t<quoted_pred_type, ctl::list<>>, list>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, transform_if) {
  using list = ctl::list<int, char, double, int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<pred_int, add_pointer_t, list>, ctl::list<int*, char, double, int*>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<pred_int, add_pointer_t, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<pred_int_char, add_pointer_t, list>, ctl::list<int*, char*, double, int*>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<pred_int_char, add_pointer_t, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<pred_empty, add_pointer_t, list>, list>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<pred_int_char, add_pointer_t, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<pred_all, add_pointer_t, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<pred_int_char, add_pointer_t, list>, list>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_qmf_t<quoted_int_char, quoted_add_ptr, list>, ctl::list<int*, char*, double, int*>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_qmf_t<quoted_int_char, quoted_add_ptr, list>, ctl::list<int*, char*, double*, int*>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<pred_int, add_pointer_t, ctl::list<>>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<pred_int, add_pointer_t, ctl::list<>>, ctl::list<int*>>;
    EXPECT_FALSE(expect_false);
  }

  {
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<pred_int, predicate_type, ctl::list<>>, ctl::list<>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<pred_int, predicate_type, ctl::list<>>, ctl::list<int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = std::tuple<void, int, float, void, int>;
    using L2 = std::tuple<char[1], char[2], char[3], char[4], char[5]>;

    using expected = std::tuple<char[1], int, float, char[4], int>;
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_t<first_is_void, second, L1, L2>, expected>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_t<first_is_void, second, L1, L2>, std::tuple<void, int, float, void, int>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using L1 = std::tuple<void, int, float, void, int>;
    using L2 = std::tuple<char[1], char[2], char[3], char[4], char[5]>;

    using expected = std::tuple<char[1], int, float, char[4], int>;
    constexpr auto expect_true = std::is_same_v<ctl::transform_if_qmf_t<quoted_first_is_void, quoted_second, L1, L2>, expected>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::transform_if_qmf_t<quoted_first_is_void, quoted_second, L1, L2>, std::tuple<void, int, float, void, int>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, rename) {
  using list = ctl::list<int, char, float>;
  using expected_list = std::tuple<int, char, float>;

  EXPECT_TRUE((std::is_same_v<ctl::rename_t<list, std::tuple>, expected_list>));

  EXPECT_TRUE((std::is_same_v<ctl::rename_t<ctl::list<>, std::tuple>, std::tuple<>>));
}

TEST_F(list_test, apply) {
  using list = ctl::list<int, char, float>;
  using expected_list = std::tuple<int, char, float>;

  EXPECT_TRUE((std::is_same_v<ctl::apply_t<std::tuple, list>, expected_list>));

  EXPECT_TRUE((std::is_same_v<ctl::apply_t<std::tuple, ctl::list<>>, std::tuple<>>));
}

TEST_F(list_test, sort) {
  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 3, 1, 0, 2>>;

    using sorted_nums = ctl::sort_t<nums>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, ctl::iota_c_t<4>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, nums>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 13, 41, 450, 4562, 121, 34, 553, 2, 1, 6, 5689>>;
    using expected_nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 1, 2, 6, 13, 34, 41, 121, 450, 553, 4562, 5689>>;

    using sorted_nums = ctl::sort_t<nums>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, expected_nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, nums>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 0, 1, 2, 3>>;

    using sorted_nums = ctl::sort_t<nums>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, std::tuple<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::list<>;

    using sorted_nums = ctl::sort_t<nums>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, ctl::iota_c_t<1>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, sort_p) {
  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 3, 1, 0, 2>>;

    using sorted_nums = ctl::sort_p_t<nums, comparator>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, ctl::iota_c_t<4>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, nums>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 13, 41, 450, 4562, 121, 34, 553, 2, 1, 6, 5689>>;
    using expected_nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 1, 2, 6, 13, 34, 41, 121, 450, 553, 4562, 5689>>;

    using sorted_nums = ctl::sort_p_t<nums, comparator>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, expected_nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, nums>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 0, 1, 2, 3>>;

    using sorted_nums = ctl::sort_p_t<nums, comparator>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, std::tuple<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::list<>;

    using sorted_nums = ctl::sort_p_t<nums, comparator>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, ctl::iota_c_t<1>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, sort_qmf_p) {
  using comparator_quote = ctl::quote<comparator>;
  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 3, 1, 0, 2>>;

    using sorted_nums = ctl::sort_qmf_p_t<nums, comparator_quote>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, ctl::iota_c_t<4>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, nums>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 13, 41, 450, 4562, 121, 34, 553, 2, 1, 6, 5689>>;
    using expected_nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 1, 2, 6, 13, 34, 41, 121, 450, 553, 4562, 5689>>;

    using sorted_nums = ctl::sort_qmf_p_t<nums, comparator_quote>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, expected_nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, nums>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::from_integer_sequence_t<std::integer_sequence<uint32_t, 0, 1, 2, 3>>;

    using sorted_nums = ctl::sort_qmf_p_t<nums, comparator_quote>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, std::tuple<>>;
    EXPECT_FALSE(expect_false);
  }

  {
    using nums = ctl::list<>;

    using sorted_nums = ctl::sort_qmf_p_t<nums, comparator_quote>;
    constexpr auto expect_true = std::is_same_v<sorted_nums, nums>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<sorted_nums, ctl::iota_c_t<1>>;
    EXPECT_FALSE(expect_false);
  }
}
