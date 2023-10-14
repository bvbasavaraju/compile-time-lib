#include <gtest/gtest.h>

#include <type_traits>

#include "list.h"
#include "list_utils.hpp"
#include "show_type.hpp"

class list_test : public ::testing::Test {
  protected:
    void SetUp() override {
      //Add Test setup code here!
    }

    void TearDown() override {
      //Add Test teardown code here!
    }
};

//Tests
TEST_F(list_test, basic_type_list_test) {
  using heterogeneous_types = ctl::list::types<int, char, double>;

  constexpr auto expected_true = std::is_same_v<heterogeneous_types, ctl::list::types<int, char, double>>;
  EXPECT_TRUE(expected_true);

  constexpr auto expected_false = std::is_same_v<heterogeneous_types, ctl::list::types<int, char, float>>;
  EXPECT_FALSE(expected_false);
}

TEST_F(list_test, push_front_test) {
  using empty_list = ctl::list::types<>;
  constexpr auto is_empty_list = std::is_same_v<empty_list, ctl::list::types<>>;

  using int_added_to_empty_list = ctl::list::push_front_t<empty_list, int>;
  { //{int}
    constexpr auto expected_true = std::is_same_v<int_added_to_empty_list, ctl::list::types<int>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<int_added_to_empty_list, ctl::list::types<>>;
    EXPECT_TRUE(expected_true);
  }

  using int_in_list = ctl::list::types<int>;
  using char_int_list = ctl::list::push_front_t<char, int_in_list>;
  { //{char, int}
    constexpr auto expected_true = std::is_same_v<char_int_list, ctl::list::types<char, int>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<char_int_list, ctl::list::types<int, char>>;
    EXPECT_TRUE(expected_true);
  }

  using float_in_list = ctl::list::types<float>;
  using float_char_int_list = ctl::list::push_front_t<char_int_list, float_in_list>;
  { //{float, char, int}
    constexpr auto expected_true = std::is_same_v<float_char_int_list, ctl::list::types<float, char, int>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<float_char_int_list, ctl::list::types<int, char, float>>;
    EXPECT_TRUE(expected_true);
  }
}

TEST_F(list_test, push_back_test) {
  using empty_list = ctl::list::types<>;
  constexpr auto is_empty_list = std::is_same_v<empty_list, ctl::list::types<>>;

  using int_added_to_empty_list = ctl::list::push_back_t<empty_list, int>;
  { //{int}
    constexpr auto expected_true = std::is_same_v<int_added_to_empty_list, ctl::list::types<int>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<int_added_to_empty_list, ctl::list::types<>>;
    EXPECT_TRUE(expected_true);
  }

  using int_in_list = ctl::list::types<int>;
  using int_char_list = ctl::list::push_back_t<char, int_in_list>;
  { //{char, int}
    constexpr auto expected_true = std::is_same_v<int_char_list, ctl::list::types<int, char>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<int_char_list, ctl::list::types<char, int>>;
    EXPECT_TRUE(expected_true);
  }

  using char_int_list = ctl::list::types<char, int>;
  using float_in_list = ctl::list::types<float>;
  using char_int_float_list = ctl::list::push_back_t<char_int_list, float_in_list>;
  { //{char, int, float}
    constexpr auto expected_true = std::is_same_v<char_int_float_list, ctl::list::types<char, int, float>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<char_int_list, ctl::list::types<float, char, int>>;
    EXPECT_TRUE(expected_true);
  }
}

TEST_F(list_test, append_test) {
  using empty_list = ctl::list::types<>;
  constexpr auto is_empty_list = std::is_same_v<empty_list, ctl::list::types<>>;

  using int_added_to_empty_list = ctl::list::append_t<empty_list, int>;
  { //{int}
    constexpr auto expected_true = std::is_same_v<int_added_to_empty_list, ctl::list::types<int>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<int_added_to_empty_list, ctl::list::types<>>;
    EXPECT_TRUE(expected_true);
  }

  using int_in_list = ctl::list::types<int>;
  using int_char_list = ctl::list::append_t<char, int_in_list>;
  { //{char, int}
    constexpr auto expected_true = std::is_same_v<int_char_list, ctl::list::types<int, char>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<int_char_list, ctl::list::types<char, int>>;
    EXPECT_TRUE(expected_true);
  }

  using char_int_list = ctl::list::types<char, int>;
  using float_in_list = ctl::list::types<float>;
  using char_int_float_list = ctl::list::append_t<char_int_list, float_in_list>;
  { //{char, int, float}
    constexpr auto expected_true = std::is_same_v<char_int_float_list, ctl::list::types<char, int, float>>;
    EXPECT_TRUE(expected_true);

    constexpr auto expected_false = std::is_same_v<char_int_list, ctl::list::types<float, char, int>>;
    EXPECT_TRUE(expected_true);
  }
}

TEST_F(list_test, first_of_list) {

  // ctl::list::first_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<int, ctl::list::first_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::list::first_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<float, ctl::list::first_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<int, ctl::list::first_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<int, ctl::list::first_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::last_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::list::first_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list::types<>, ctl::list::first_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, last_of_list) {

  // ctl::list::last_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<double, ctl::list::last_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::list::last_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<double, ctl::list::last_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::last_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::list::last_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::last_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::list::last_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list::types<>, ctl::list::last_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, head_of_list) {

  // ctl::list::head_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<int, char>, ctl::list::head_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::list::head_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<float, unsigned int, int, char>, ctl::list::head_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::head_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<int, char, double, float>, ctl::list::head_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::head_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<>, ctl::list::head_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::head_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, front_of_list) {

  // ctl::list::front_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<int, char>, ctl::list::front_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::list::front_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<float, unsigned int, int, char>, ctl::list::front_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::front_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<int, char, double, float>, ctl::list::front_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::front_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<>, ctl::list::front_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::front_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, pop_back_list) {

  // ctl::list::pop_back_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<int, char>, ctl::list::pop_back_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::list::pop_back_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<float, unsigned int, int, char>, ctl::list::pop_back_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::pop_back_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<int, char, double, float>, ctl::list::pop_back_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::pop_back_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<>, ctl::list::pop_back_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::pop_back_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, tail_of_list) {

  // ctl::list::tail_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<char, double>, ctl::list::tail_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::tail_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<unsigned int, int, char, double>, ctl::list::tail_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::tail_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<char, double, float, unsigned int>, ctl::list::tail_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::tail_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<>, ctl::list::tail_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::tail_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, back_of_list) {

  // ctl::list::back_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<char, double>, ctl::list::back_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::back_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<unsigned int, int, char, double>, ctl::list::back_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::back_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<char, double, float, unsigned int>, ctl::list::back_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::back_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<>, ctl::list::back_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::back_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, pop_front_list) {

  // ctl::list::pop_front_t<ctl::list::types<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list::types<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<char, double>, ctl::list::pop_front_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::pop_front_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list::types<float, unsigned int>;
  using merged_list = ctl::list::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<unsigned int, int, char, double>, ctl::list::pop_front_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::pop_front_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::list::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<char, double, float, unsigned int>, ctl::list::pop_front_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::list::pop_front_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list::types<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list::types<>, ctl::list::pop_front_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::list::pop_front_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, list_size) {
  using empty_list = ctl::list::types<>;
  EXPECT_EQ(0, ctl::list::size_t<empty_list>::value);
  EXPECT_EQ(0, ctl::list::size_t<empty_list>());

  using single_element_list = ctl::list::types<int>;
  EXPECT_EQ(1, ctl::list::size_t<single_element_list>::value);
  EXPECT_EQ(1, ctl::list::size_t<single_element_list>());

  using two_elements_list = ctl::list::types<int, char>;
  EXPECT_EQ(2, ctl::list::size_t<two_elements_list>::value);
  EXPECT_EQ(2, ctl::list::size_t<two_elements_list>());

  using three_elements_list = ctl::list::types<int, char, float>;
  EXPECT_EQ(3, ctl::list::size_t<three_elements_list>::value);
  EXPECT_EQ(3, ctl::list::size_t<three_elements_list>());

  using merged_list = ctl::list::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_EQ(5, ctl::list::size_t<merged_list>::value);
  EXPECT_EQ(5, ctl::list::size_t<merged_list>());

  using merged_list2 = ctl::list::push_front_t<merged_list, single_element_list>;
  EXPECT_EQ(6, ctl::list::size_t<merged_list2>::value);
  EXPECT_EQ(6, ctl::list::size_t<merged_list2>());
}

TEST_F(list_test, list_count) {
  using empty_list = ctl::list::types<>;
  EXPECT_EQ(0, ctl::list::count_t<empty_list>::value);
  EXPECT_EQ(0, ctl::list::count_t<empty_list>());

  using single_element_list = ctl::list::types<int>;
  EXPECT_EQ(1, ctl::list::count_t<single_element_list>::value);
  EXPECT_EQ(1, ctl::list::count_t<single_element_list>());

  using two_elements_list = ctl::list::types<int, char>;
  EXPECT_EQ(2, ctl::list::count_t<two_elements_list>::value);
  EXPECT_EQ(2, ctl::list::count_t<two_elements_list>());

  using three_elements_list = ctl::list::types<int, char, float>;
  EXPECT_EQ(3, ctl::list::count_t<three_elements_list>::value);
  EXPECT_EQ(3, ctl::list::count_t<three_elements_list>());

  using merged_list = ctl::list::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_EQ(5, ctl::list::count_t<merged_list>::value);
  EXPECT_EQ(5, ctl::list::count_t<merged_list>());

  using merged_list2 = ctl::list::push_front_t<merged_list, single_element_list>;
  EXPECT_EQ(6, ctl::list::count_t<merged_list2>::value);
  EXPECT_EQ(6, ctl::list::count_t<merged_list2>());
}

TEST_F(list_test, empty_check) {
  using empty_list = ctl::list::types<>;
  EXPECT_TRUE(ctl::list::empty_t<empty_list>::value);
  EXPECT_TRUE(ctl::list::empty_t<empty_list>());

  using single_element_list = ctl::list::types<int>;
  EXPECT_FALSE(ctl::list::empty_t<single_element_list>::value);
  EXPECT_FALSE(ctl::list::empty_t<single_element_list>());

  using two_elements_list = ctl::list::types<int, char>;
  EXPECT_FALSE(ctl::list::empty_t<two_elements_list>::value);
  EXPECT_FALSE(ctl::list::empty_t<two_elements_list>());

  using three_elements_list = ctl::list::types<int, char, float>;
  EXPECT_FALSE(ctl::list::empty_t<three_elements_list>::value);
  EXPECT_FALSE(ctl::list::empty_t<three_elements_list>());

  using merged_list = ctl::list::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_FALSE(ctl::list::empty_t<merged_list>::value);
  EXPECT_FALSE(ctl::list::empty_t<merged_list>());

  using merged_list2 = ctl::list::push_front_t<merged_list, single_element_list>;
  EXPECT_FALSE(ctl::list::empty_t<merged_list2>::value);
  EXPECT_FALSE(ctl::list::empty_t<merged_list2>());
}

TEST_F(list_test, list_clear) {
  using empty_list = ctl::list::types<>;
  auto value = std::is_same_v<ctl::list::types<>, ctl::list::clear_t<empty_list>>;
  EXPECT_TRUE(value);

  value = false;
  using single_element_list = ctl::list::types<int>;
  value = std::is_same_v<ctl::list::types<>, ctl::list::clear_t<single_element_list>>;
  EXPECT_TRUE(value);

  value = false;
  using two_elements_list = ctl::list::types<int, char>;
  value = std::is_same_v<ctl::list::types<>, ctl::list::clear_t<two_elements_list>>;
  EXPECT_TRUE(value);

  value = false;
  using three_elements_list = ctl::list::types<int, char, float>;
  value = std::is_same_v<ctl::list::types<>, ctl::list::clear_t<three_elements_list>>;
  EXPECT_TRUE(value);

  value = false;
  using merged_list = ctl::list::push_front_t<three_elements_list, two_elements_list>;
  value = std::is_same_v<ctl::list::types<>, ctl::list::clear_t<merged_list>>;
  EXPECT_TRUE(value);

  value = false;
  using merged_list2 = ctl::list::push_front_t<merged_list, single_element_list>;
  value = std::is_same_v<ctl::list::types<>, ctl::list::clear_t<merged_list2>>;
  EXPECT_TRUE(value);
}
