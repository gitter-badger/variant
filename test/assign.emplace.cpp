// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <string>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

using namespace std::string_literals;

TEST(Assign_Emplace, IndexDirect) {
  std_exp::variant<int, std::string> v;
  v.emplace<1>("42"s);
  EXPECT_EQ("42"s, std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexDirectRef) {
  int x = 42;
  std::ostringstream y;
  std_exp::variant<int &, std::ostream &> v(std_exp::in_place_index<0>, x);
  EXPECT_EQ(&x, &std_exp::get<0>(v));
  v.emplace<1>(y);
  EXPECT_EQ(&y, &std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexDirectDuplicate) {
  std_exp::variant<int, int> v;
  v.emplace<1>(42);
  EXPECT_EQ(42, std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexDirectDuplicateRef) {
  int x, y;
  std_exp::variant<int &, int &> v(std_exp::in_place_index<1>, y);
  EXPECT_EQ(&y, &std_exp::get<1>(v));
  v.emplace<0>(x);
  EXPECT_EQ(&x, &std_exp::get<0>(v));
}

TEST(Assign_Emplace, IndexConversion) {
  std_exp::variant<int, std::string> v;
  v.emplace<1>("42");
  EXPECT_EQ("42"s, std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexConversionRef) {
  int x = 42;
  std::ostringstream y;
  std_exp::variant<int &, std::ostream &> v(std_exp::in_place_index<0>, x);
  EXPECT_EQ(&x, &std_exp::get<0>(v));
  v.emplace<1>(y);
  EXPECT_EQ(&y, &std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexConversionDuplicate) {
  std_exp::variant<int, int> v;
  v.emplace<1>(1.1);
  EXPECT_EQ(1, std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexConversionDuplicateRef) {
  std::ostringstream x, y;
  std_exp::variant<std::ostream &, std::ostream &> v(std_exp::in_place_index<0>,
                                                     x);
  EXPECT_EQ(&x, &std_exp::get<0>(v));
  v.emplace<1>(y);
  EXPECT_EQ(&y, &std_exp::get<1>(v));
}

TEST(Assign_Emplace, IndexInitializerList) {
  std_exp::variant<int, std::string> v;
  v.emplace<1>({'4', '2'});
  EXPECT_EQ("42"s, std_exp::get<1>(v));
}

TEST(Assign_Emplace, TypeDirect) {
  std_exp::variant<int, std::string> v;
  v.emplace<std::string>("42"s);
  EXPECT_EQ("42"s, std_exp::get<std::string>(v));
}

TEST(Assign_Emplace, TypeDirectRef) {
  int x = 42;
  std::ostringstream y;
  std_exp::variant<int &, std::ostream &> v(std_exp::in_place_type<int &>, x);
  EXPECT_EQ(&x, &std_exp::get<int &>(v));
  v.emplace<std::ostream &>(y);
  EXPECT_EQ(&y, &std_exp::get<std::ostream &>(v));
}

TEST(Assign_Emplace, TypeConversion) {
  std_exp::variant<int, std::string> v;
  v.emplace<int>(1.1);
  EXPECT_EQ(1, std_exp::get<int>(v));
}

TEST(Assign_Emplace, TypeConversionRef) {
  int x = 42;
  std::ostringstream y;
  std_exp::variant<int &, std::ostream &> v(x);
  EXPECT_EQ(&x, &std_exp::get<int &>(v));
  v.emplace<std::ostream &>(y);
  EXPECT_EQ(&y, &std_exp::get<std::ostream &>(v));
}

TEST(Assign_Emplace, TypeInitializerList) {
  std_exp::variant<int, std::string> v;
  v.emplace<std::string>({'4', '2'});
  EXPECT_EQ("42"s, std_exp::get<std::string>(v));
}
