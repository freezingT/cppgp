#include <cppgp/math/distance.hpp>

#include <gtest/gtest.h>


TEST(math_distance, dist2_test){
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
