#include "Numbers.hpp"
#include <gtest/gtest.h>

using MathTree::RealNumber;

class NumbersTest: public ::testing::Test {
    protected:
    RealNumber number{42};
};

TEST_F(NumbersTest, EvaluatingReturnsTheNumberItself) {
    EXPECT_EQ(number.evaluate(), 42);
}