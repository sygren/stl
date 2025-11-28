#include <gtest/gtest.h>

#include "../core/result.hpp"
#include <cstdint>
#include <string>
#include <vector>

TEST(ResultTest, StaticSize) {
  struct foo {
    uint32_t _i;
    uint8_t _ii;
    uint8_t _iii;
  };

  auto t0 = result<uint32_t, uint32_t>::val(10);
  EXPECT_TRUE(sizeof(t0) == 8);

  auto t1 = result<uint32_t, bool>::err(true);
  EXPECT_TRUE(sizeof(t1) == 8);

  auto t2 = result<foo, bool>::err(true);
  std::cout << sizeof(t2) << "\n";
  EXPECT_TRUE(sizeof(t2) == 12);
}

// Test basic construction with value
TEST(ResultTest, ConstructWithValue) {
  result<int, const char *> r(42);

  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.val(), 42);
}

// Test basic construction with error
TEST(ResultTest, ConstructWithError) {
  result<int, const char *> r(
      result<int, const char *>::error("error message"));

  EXPECT_FALSE(r.has_value());
  EXPECT_STREQ(r.err(), "error message");
}

// Test copy constructor with value
TEST(ResultTest, CopyConstructorWithValue) {
  result<int, const char *> r1(100);
  result<int, const char *> r2(r1);

  EXPECT_TRUE(r1.has_value());
  EXPECT_TRUE(r2.has_value());
  EXPECT_EQ(r1.val(), 100);
  EXPECT_EQ(r2.val(), 100);
}

// Test copy constructor with error
TEST(ResultTest, CopyConstructorWithError) {
  result<int, const char *> r1(result<int, const char *>::error("error"));
  result<int, const char *> r2(r1);

  EXPECT_FALSE(r1.has_value());
  EXPECT_FALSE(r2.has_value());
  EXPECT_STREQ(r1.err(), "error");
  EXPECT_STREQ(r2.err(), "error");
}

// Test copy assignment with value
TEST(ResultTest, CopyAssignmentWithValue) {
  result<int, const char *> r1(50);
  result<int, const char *> r2(result<int, const char *>::error("error"));

  r2 = r1;

  EXPECT_TRUE(r2.has_value());
  EXPECT_EQ(r2.val(), 50);
}

// Test copy assignment with error
TEST(ResultTest, CopyAssignmentWithError) {
  result<int, const char *> r1(result<int, const char *>::error("new error"));
  result<int, const char *> r2(123);

  r2 = r1;

  EXPECT_FALSE(r2.has_value());
  EXPECT_STREQ(r2.err(), "new error");
}

// Test move constructor with value
TEST(ResultTest, MoveConstructorWithValue) {
  result<std::vector<int>, std::string> r1(std::vector<int>{1, 2, 3, 4, 5});

  EXPECT_TRUE(r1.has_value());
  EXPECT_EQ(r1.val().size(), 5);

  result<std::vector<int>, std::string> r2(move(r1));

  EXPECT_TRUE(r2.has_value());
  EXPECT_EQ(r2.val().size(), 5);
  EXPECT_EQ(r2.val()[0], 1);
  EXPECT_EQ(r2.val()[4], 5);

  // r1 is in valid-but-unspecified state, don't test its value
}

// Test move constructor with error
TEST(ResultTest, MoveConstructorWithError) {
  auto r1 = result<std::vector<int>, std::string>::err("error message");

  EXPECT_FALSE(r1.has_value());
  EXPECT_EQ(r1.err(), "error message");

  result<std::vector<int>, std::string> r2(move(r1));

  EXPECT_FALSE(r2.has_value());
  EXPECT_EQ(r2.err(), "error message");
}

// Test move assignment with value
TEST(ResultTest, MoveAssignmentWithValue) {
  result<std::vector<int>, std::string> r1(std::vector<int>{10, 20, 30});
  auto r2 = result<std::vector<int>, std::string>::err("error message");

  r2 = move(r1);

  EXPECT_TRUE(r2.has_value());
  EXPECT_EQ(r2.val().size(), 3);
  EXPECT_EQ(r2.val()[1], 20);
}

// Test move assignment with error
TEST(ResultTest, MoveAssignmentWithError) {
  auto r1 =
      result<std::vector<int>, std::string>::err(std::string("critical error"));
  result<std::vector<int>, std::string> r2(std::vector<int>{1, 2, 3});

  r2 = move(r1);

  EXPECT_FALSE(r2.has_value());
  EXPECT_EQ(r2.err(), "critical error");
}

// Test reference getters
TEST(ResultTest, ReferenceGetters) {
  result<int, std::string> r(42);

  // Test non-const reference
  r.val_ref() = 100;
  EXPECT_EQ(r.val(), 100);

  // Test const reference
  const result<int, std::string> &cr = r;
  EXPECT_EQ(cr.val(), 100);
}

// Test error reference getters
TEST(ResultTest, ErrorReferenceGetters) {
  auto r = result<int, std::string>::err(std::string("initial error"));

  // Test non-const reference
  r.err_ref() = "modified error";
  EXPECT_EQ(r.err(), "modified error");

  // Test const reference
  const result<int, std::string> &cr = r;
  EXPECT_EQ(cr.err(), "modified error");
}

// Test self-assignment (copy)
TEST(ResultTest, SelfCopyAssignment) {
  result<int, const char *> r(42);
  r = r;

  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.val(), 42);
}

// Test self-assignment (move)
TEST(ResultTest, SelfMoveAssignment) {
  result<std::vector<int>, std::string> r(std::vector<int>{1, 2, 3});
  r = move(r);

  EXPECT_TRUE(r.has_value());
  // Value is unspecified after self-move, just check it doesn't crash
}

// Test with complex types
TEST(ResultTest, ComplexTypes) {
  result<std::string, std::vector<int>> r1(std::string("hello world"));

  EXPECT_TRUE(r1.has_value());
  EXPECT_EQ(r1.val(), "hello world");

  auto r2 =
      result<std::string, std::vector<int>>::err(std::vector<int>{1, 2, 3});

  EXPECT_FALSE(r2.has_value());
  EXPECT_EQ(r2.err().size(), 3);
}

// Test multiple operations in sequence
TEST(ResultTest, SequentialOperations) {
  result<int, const char *> r1(10);
  result<int, const char *> r2(r1);
  result<int, const char *> r3(move(r2));

  r1 = r3;

  EXPECT_TRUE(r1.has_value());
  EXPECT_TRUE(r3.has_value());
  EXPECT_EQ(r1.val(), 10);
  EXPECT_EQ(r3.val(), 10);
}
