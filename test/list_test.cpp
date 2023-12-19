#include <gtest/gtest.h>

#include <type_traits>

#include "list.hpp"
#include "utils.hpp"
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

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::first_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<>, ctl::first_t<unisnged_int_in_list>>;
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

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<unsigned int, ctl::last_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<ctl::list<>, ctl::last_t<unisnged_int_in_list>>;
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

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::head_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::head_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, front_of_list) {

  // ctl::front_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<int, char>, ctl::front_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<char, ctl::front_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<float, unsigned int, int, char>, ctl::front_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::front_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<int, char, double, float>, ctl::front_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::front_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::front_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::front_t<unisnged_int_in_list>>;
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

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::pop_back_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::pop_back_t<unisnged_int_in_list>>;
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

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::tail_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::tail_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, back_of_list) {

  // ctl::back_t<ctl::list<>>; // - resulting behaviour is compiler error

  using heterogeneous_list_1 = ctl::list<int, char, double>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<char, double>, ctl::back_t<heterogeneous_list_1>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::back_t<heterogeneous_list_1>>;
    EXPECT_FALSE(expect_false);
  }

  using heterogeneous_list_2 = ctl::list<float, unsigned int>;
  using merged_list = ctl::push_front_t<heterogeneous_list_1, heterogeneous_list_2>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<unsigned int, int, char, double>, ctl::back_t<merged_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::back_t<merged_list>>;
    EXPECT_FALSE(expect_false);
  }

  using merged_list2 = ctl::push_front_t<heterogeneous_list_2, heterogeneous_list_1>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<char, double, float, unsigned int>, ctl::back_t<merged_list2>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<double, ctl::back_t<merged_list2>>;
    EXPECT_FALSE(expect_false);
  }

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::back_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::back_t<unisnged_int_in_list>>;
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

  using unisnged_int_in_list = ctl::list<unsigned int>;
  {
    constexpr auto expect_true = std::is_same_v<ctl::list<>, ctl::pop_front_t<unisnged_int_in_list>>;
    EXPECT_TRUE(expect_true);

    constexpr auto expect_false = std::is_same_v<unsigned int, ctl::pop_front_t<unisnged_int_in_list>>;
    EXPECT_FALSE(expect_false);
  }
}

TEST_F(list_test, list_size) {
  using empty_list = ctl::list<>;
  EXPECT_EQ(0, ctl::size_t<empty_list>::value);
  EXPECT_EQ(0, ctl::size_t<empty_list>());

  using single_element_list = ctl::list<int>;
  EXPECT_EQ(1, ctl::size_t<single_element_list>::value);
  EXPECT_EQ(1, ctl::size_t<single_element_list>());

  using two_elements_list = ctl::list<int, char>;
  EXPECT_EQ(2, ctl::size_t<two_elements_list>::value);
  EXPECT_EQ(2, ctl::size_t<two_elements_list>());

  using three_elements_list = ctl::list<int, char, float>;
  EXPECT_EQ(3, ctl::size_t<three_elements_list>::value);
  EXPECT_EQ(3, ctl::size_t<three_elements_list>());

  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_EQ(5, ctl::size_t<merged_list>::value);
  EXPECT_EQ(5, ctl::size_t<merged_list>());

  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  EXPECT_EQ(6, ctl::size_t<merged_list2>::value);
  EXPECT_EQ(6, ctl::size_t<merged_list2>());
}

TEST_F(list_test, list_count) {
  using empty_list = ctl::list<>;
  EXPECT_EQ(0, ctl::count_t<empty_list>::value);
  EXPECT_EQ(0, ctl::count_t<empty_list>());

  using single_element_list = ctl::list<int>;
  EXPECT_EQ(1, ctl::count_t<single_element_list>::value);
  EXPECT_EQ(1, ctl::count_t<single_element_list>());

  using two_elements_list = ctl::list<int, char>;
  EXPECT_EQ(2, ctl::count_t<two_elements_list>::value);
  EXPECT_EQ(2, ctl::count_t<two_elements_list>());

  using three_elements_list = ctl::list<int, char, float>;
  EXPECT_EQ(3, ctl::count_t<three_elements_list>::value);
  EXPECT_EQ(3, ctl::count_t<three_elements_list>());

  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_EQ(5, ctl::count_t<merged_list>::value);
  EXPECT_EQ(5, ctl::count_t<merged_list>());

  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  EXPECT_EQ(6, ctl::count_t<merged_list2>::value);
  EXPECT_EQ(6, ctl::count_t<merged_list2>());
}

TEST_F(list_test, empty_check) {
  using empty_list = ctl::list<>;
  EXPECT_TRUE(ctl::empty_t<empty_list>::value);
  EXPECT_TRUE(ctl::empty_t<empty_list>());

  using single_element_list = ctl::list<int>;
  EXPECT_FALSE(ctl::empty_t<single_element_list>::value);
  EXPECT_FALSE(ctl::empty_t<single_element_list>());

  using two_elements_list = ctl::list<int, char>;
  EXPECT_FALSE(ctl::empty_t<two_elements_list>::value);
  EXPECT_FALSE(ctl::empty_t<two_elements_list>());

  using three_elements_list = ctl::list<int, char, float>;
  EXPECT_FALSE(ctl::empty_t<three_elements_list>::value);
  EXPECT_FALSE(ctl::empty_t<three_elements_list>());

  using merged_list = ctl::push_front_t<three_elements_list, two_elements_list>;
  EXPECT_FALSE(ctl::empty_t<merged_list>::value);
  EXPECT_FALSE(ctl::empty_t<merged_list>());

  using merged_list2 = ctl::push_front_t<merged_list, single_element_list>;
  EXPECT_FALSE(ctl::empty_t<merged_list2>::value);
  EXPECT_FALSE(ctl::empty_t<merged_list2>());
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

TEST_F(list_test, select) {
  using list = ctl::list<int, char, double, int>;
  using empty = ctl::list<>;

  {
    constexpr auto expect_true = std::is_same_v<ctl::select_c_t<true, list>, list>;
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
}

// template <typename T>
// struct pred : public std::false_type {};
// template <> struct pred<int> : public std::true_type {};

// TEST_F(list_test, filter) {
//   using list = ctl::list<int, char, double, int>;

//   {
//     using filtered = ctl::filter_t<pred<int>, list>;
//     // ctl::debug::show_type<pred>();

//     constexpr auto expect_true = std::is_same_v<ctl::filter_t<pred<int>, list>, ctl::list<int, int>>;
//     EXPECT_TRUE(expect_true);
//   }
// }
