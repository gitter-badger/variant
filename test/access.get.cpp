// Copyright Michael Park 2015
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <variant.hpp>

#include <gtest/gtest.h>

namespace std_exp = std::experimental;

enum Qual { Ptr, ConstPtr, LRef, ConstLRef, RRef, ConstRRef };

constexpr Qual get_qual(int *) { return Ptr; }
constexpr Qual get_qual(const int *) { return ConstPtr; }
constexpr Qual get_qual(int &) { return LRef; }
constexpr Qual get_qual(const int &) { return ConstLRef; }
constexpr Qual get_qual(int &&) { return RRef; }
constexpr Qual get_qual(const int &&) { return ConstRRef; }

struct move_thrower_t {
  move_thrower_t() = default;
  move_thrower_t(const move_thrower_t &) = default;
  move_thrower_t(move_thrower_t &&) { throw std::runtime_error(""); }
  move_thrower_t &operator=(const move_thrower_t &) = default;
  move_thrower_t &operator=(move_thrower_t &&) { throw std::runtime_error(""); }
};  // move_thrower_t

TEST(Access_Get, MutVarMutType) {
  std_exp::variant<int> v(42);
  EXPECT_EQ(42, std_exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, get_qual(std_exp::get<int>(v)));
  EXPECT_EQ(RRef, get_qual(std_exp::get<int>(std::move(v))));
}

TEST(Access_GetIf, MutVarMutTypeRef) {
  int expected = 42;
  std_exp::variant<int &> v(expected);
  EXPECT_EQ(expected, std_exp::get<int &>(v));
  EXPECT_EQ(&expected, &std_exp::get<int &>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, get_qual(std_exp::get<int &>(v)));
  EXPECT_EQ(LRef, get_qual(std_exp::get<int &>(std::move(v))));
}

TEST(Access_Get, MutVarConstType) {
  std_exp::variant<const int> v(42);
  EXPECT_EQ(42, std_exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<const int>(v)));
  EXPECT_EQ(ConstRRef, get_qual(std_exp::get<const int>(std::move(v))));
}

TEST(Access_Get, MutVarConstTypeRef) {
  int expected = 42;
  std_exp::variant<const int &> v(expected);
  EXPECT_EQ(expected, std_exp::get<const int &>(v));
  EXPECT_EQ(&expected, &std_exp::get<const int &>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<const int &>(v)));
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<const int &>(std::move(v))));
}

TEST(Access_Get, ConstVarMutType) {
  const std_exp::variant<int> v(42);
  EXPECT_EQ(42, std_exp::get<int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<int>(v)));
  EXPECT_EQ(ConstRRef, get_qual(std_exp::get<int>(std::move(v))));

  /* constexpr */ {
    constexpr std_exp::variant<int> v(42);
    static_assert(42 == std_exp::get<int>(v), "");
    // Check qualifier.
    static_assert(ConstLRef == get_qual(std_exp::get<int>(v)), "");
    static_assert(ConstRRef == get_qual(std_exp::get<int>(std::move(v))), "");
  }
}

TEST(Access_Get, ConstVarMutTypeRef) {
  int expected = 42;
  const std_exp::variant<int &> v(expected);
  EXPECT_EQ(expected, std_exp::get<int &>(v));
  EXPECT_EQ(&expected, &std_exp::get<int &>(v));
  // Check qualifier.
  EXPECT_EQ(LRef, get_qual(std_exp::get<int &>(v)));
  EXPECT_EQ(LRef, get_qual(std_exp::get<int &>(std::move(v))));
}

TEST(Access_Get, ConstVarConstType) {
  const std_exp::variant<const int> v(42);
  EXPECT_EQ(42, std_exp::get<const int>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<const int>(v)));
  EXPECT_EQ(ConstRRef, get_qual(std_exp::get<const int>(std::move(v))));

  /* constexpr */ {
    constexpr std_exp::variant<const int> v(42);
    static_assert(42 == std_exp::get<const int>(v), "");
    // Check qualifier.
    static_assert(ConstLRef == get_qual(std_exp::get<const int>(v)), "");
    static_assert(ConstRRef == get_qual(std_exp::get<const int>(std::move(v))),
                  "");
  }
}

TEST(Access_Get, ConstVarConstTypeRef) {
  int expected = 42;
  const std_exp::variant<const int &> v(expected);
  EXPECT_EQ(expected, std_exp::get<const int &>(v));
  EXPECT_EQ(&expected, &std_exp::get<const int &>(v));
  // Check qualifier.
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<const int &>(v)));
  EXPECT_EQ(ConstLRef, get_qual(std_exp::get<const int &>(std::move(v))));
}

TEST(Access_Get, MoveCorruptedByException) {
  std_exp::variant<int, move_thrower_t> v(42);
  try {
    v = move_thrower_t{};
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    EXPECT_THROW(std_exp::get<int>(v), std_exp::bad_variant_access);
    EXPECT_THROW(std_exp::get<move_thrower_t>(v), std_exp::bad_variant_access);
  }  // try
}

TEST(Access_Get, CopyCorruptedByException) {
  std_exp::variant<int, move_thrower_t> v(42);
  try {
    move_thrower_t move_thrower;
    v = move_thrower;
  } catch (const std::exception &) {
    EXPECT_TRUE(v.corrupted_by_exception());
    EXPECT_THROW(std_exp::get<int>(v), std_exp::bad_variant_access);
    EXPECT_THROW(std_exp::get<move_thrower_t>(v), std_exp::bad_variant_access);
  }  // try
}
