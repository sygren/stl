#include <gtest/gtest.h>

#include "../core/optional.hpp"
#include <optional>
#include <string>
#include <vector>

// Test construction with value
TEST(OptionalTest, ConstructWithValue) {
  optional<int> opt(42);

  EXPECT_TRUE(opt.has_value());
  EXPECT_EQ(opt.val(), 42);
}

// Test construction with none
TEST(OptionalTest, ConstructWithNone) {
  optional<int> opt(optional<int>::none());

  EXPECT_FALSE(opt.has_value());
}

// Test static none factory
TEST(OptionalTest, StaticNoneFactory) {
  auto opt = optional<int>::none();

  EXPECT_FALSE(opt.has_value());
}

// Test static wrap factory
TEST(OptionalTest, StaticWrapFactory) {
  auto opt = optional<int>::wrap(100);

  EXPECT_TRUE(opt.has_value());
  EXPECT_EQ(opt.val(), 100);
}

// Test copy constructor with value
TEST(OptionalTest, CopyConstructorWithValue) {
  optional<int> opt1(50);
  optional<int> opt2(opt1);

  EXPECT_TRUE(opt1.has_value());
  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt1.val(), 50);
  EXPECT_EQ(opt2.val(), 50);
}

// Test copy constructor with none
TEST(OptionalTest, CopyConstructorWithNone) {
  optional<int> opt1(optional<int>::none());
  optional<int> opt2(opt1);

  EXPECT_FALSE(opt1.has_value());
  EXPECT_FALSE(opt2.has_value());
}

// Test copy assignment from value to value
TEST(OptionalTest, CopyAssignmentValueToValue) {
  optional<int> opt1(100);
  optional<int> opt2(200);

  opt2 = opt1;

  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt2.val(), 100);
}

// Test copy assignment from value to none
TEST(OptionalTest, CopyAssignmentValueToNone) {
  optional<int> opt1(75);
  optional<int> opt2(optional<int>::none());

  opt2 = opt1;

  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt2.val(), 75);
}

// Test copy assignment from none to value
TEST(OptionalTest, CopyAssignmentNoneToValue) {
  optional<int> opt1(optional<int>::none());
  optional<int> opt2(123);

  opt2 = opt1;

  EXPECT_FALSE(opt2.has_value());
}

// Test copy assignment from none to none
TEST(OptionalTest, CopyAssignmentNoneToNone) {
  optional<int> opt1(optional<int>::none());
  optional<int> opt2(optional<int>::none());

  opt2 = opt1;

  EXPECT_FALSE(opt2.has_value());
}

// Test self-assignment
TEST(OptionalTest, SelfCopyAssignment) {
  optional<int> opt(42);
  opt = opt;

  EXPECT_TRUE(opt.has_value());
  EXPECT_EQ(opt.val(), 42);
}

// Test move constructor with value
TEST(OptionalTest, MoveConstructorWithValue) {
  optional<std::vector<int>> opt1(std::vector<int>{1, 2, 3, 4, 5});

  EXPECT_TRUE(opt1.has_value());
  EXPECT_EQ(opt1.val().size(), 5);

  optional<std::vector<int>> opt2(std::move(opt1));

  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt2.val().size(), 5);
  EXPECT_EQ(opt2.val()[0], 1);
  EXPECT_EQ(opt2.val()[4], 5);
}

// Test move constructor with none
TEST(OptionalTest, MoveConstructorWithNone) {
  optional<std::vector<int>> opt1(optional<std::vector<int>>::none());
  optional<std::vector<int>> opt2(std::move(opt1));

  EXPECT_FALSE(opt2.has_value());
}

// Test move assignment from value to value
TEST(OptionalTest, MoveAssignmentValueToValue) {
  optional<std::string> opt1(std::string("hello world"));
  optional<std::string> opt2(std::string("goodbye"));

  opt2 = std::move(opt1);

  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt2.val(), "hello world");
}

// Test move assignment from value to none
TEST(OptionalTest, MoveAssignmentValueToNone) {
  optional<std::string> opt1(std::string("test string"));
  optional<std::string> opt2(optional<std::string>::none());

  opt2 = std::move(opt1);

  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt2.val(), "test string");
}

// Test move assignment from none to value
TEST(OptionalTest, MoveAssignmentNoneToValue) {
  optional<std::vector<int>> opt1(optional<std::vector<int>>::none());
  optional<std::vector<int>> opt2(std::vector<int>{1, 2, 3});

  opt2 = std::move(opt1);

  EXPECT_FALSE(opt2.has_value());
}

// Test self-move assignment
TEST(OptionalTest, SelfMoveAssignment) {
  optional<std::vector<int>> opt(std::vector<int>{1, 2, 3});
  opt = std::move(opt);

  EXPECT_TRUE(opt.has_value());
  // Value is in valid-but-unspecified state after self-move
}

// Test mutable reference access
TEST(OptionalTest, MutableReferenceAccess) {
  optional<int> opt(10);

  opt.val_ref() = 99;

  EXPECT_EQ(opt.val(), 99);
}

// Test const reference access
TEST(OptionalTest, ConstReferenceAccess) {
  const optional<int> opt(42);

  EXPECT_TRUE(opt.has_value());
  EXPECT_EQ(opt.val(), 42);
}

// Test with complex types
TEST(OptionalTest, ComplexTypes) {
  optional<std::string> opt1(std::string("hello"));

  EXPECT_TRUE(opt1.has_value());
  EXPECT_EQ(opt1.val(), "hello");

  optional<std::vector<int>> opt2(std::vector<int>{10, 20, 30});

  EXPECT_TRUE(opt2.has_value());
  EXPECT_EQ(opt2.val().size(), 3);
  EXPECT_EQ(opt2.val()[1], 20);
}

// Test with types without default constructor
struct NoDefaultConstructor {
  int value;
  explicit NoDefaultConstructor(int v) : value(v) {}
  // No default constructor
};

TEST(OptionalTest, TypeWithoutDefaultConstructor) {
  optional<NoDefaultConstructor> opt1(NoDefaultConstructor(42));

  EXPECT_TRUE(opt1.has_value());
  EXPECT_EQ(opt1.val().value, 42);

  optional<NoDefaultConstructor> opt2(optional<NoDefaultConstructor>::none());

  EXPECT_FALSE(opt2.has_value());
}

// Test sequential operations
TEST(OptionalTest, SequentialOperations) {
  optional<int> opt1(10);
  optional<int> opt2(opt1);
  optional<int> opt3(std::move(opt2));

  opt1 = opt3;

  EXPECT_TRUE(opt1.has_value());
  EXPECT_TRUE(opt3.has_value());
  EXPECT_EQ(opt1.val(), 10);
  EXPECT_EQ(opt3.val(), 10);
}

// Test assignment chains
TEST(OptionalTest, AssignmentChain) {
  optional<int> opt1(100);
  optional<int> opt2(200);
  optional<int> opt3(300);

  opt3 = opt2 = opt1;

  EXPECT_TRUE(opt1.has_value());
  EXPECT_TRUE(opt2.has_value());
  EXPECT_TRUE(opt3.has_value());
  EXPECT_EQ(opt1.val(), 100);
  EXPECT_EQ(opt2.val(), 100);
  EXPECT_EQ(opt3.val(), 100);
}

// Test destructor properly cleans up
TEST(OptionalTest, DestructorCleanup) {
  static int destructor_count = 0;

  struct CountDestructor {
    ~CountDestructor() { destructor_count++; }
  };

  destructor_count = 0;
  {
    optional<CountDestructor> opt1(CountDestructor{});
    optional<CountDestructor> opt2(optional<CountDestructor>::none());
  }

  EXPECT_EQ(destructor_count, 2); // Only opt1 should destroy its value
}

// Test multiple copies don't interfere
TEST(OptionalTest, MultipleCopies) {
  optional<std::string> opt1(std::string("original"));
  optional<std::string> opt2(opt1);
  optional<std::string> opt3(opt2);

  opt1.val_ref() = "modified";

  EXPECT_EQ(opt1.val(), "modified");
  EXPECT_EQ(opt2.val(), "original");
  EXPECT_EQ(opt3.val(), "original");
}
