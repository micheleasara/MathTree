#include <cmath>
#include "Expression.hpp"
#include <gtest/gtest.h>

using MathTree::RealNumberExpression;

TEST(NumbersTest, EvaluatingARealNumberReturnsTheNumberItself) {
  RealNumberExpression number{"42.2"};
  EXPECT_DOUBLE_EQ(number.evaluate(), 42.2);
}

TEST(NumbersTest, PrintingARealNumberPrintsTheNumberItself) {
  std::string numStr{"42.2"};
  RealNumberExpression number{numStr};
  std::stringstream ss;
  number.print(ss);
  EXPECT_EQ(ss.str(), numStr);
}

TEST(NumbersTest, CreatingARealNumberFromAStringWithLettersThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"4a5"});
}

TEST(NumbersTest, CreatingARealNumberFromAStringWithSpacesThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{" 45"});
}

TEST(NumbersTest, CreatingARealNumberWithInfinityThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"INFINITY"});
}

TEST(NumbersTest, CreatingARealNumberWithNaNThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"NAN"});
}

TEST(NumbersTest, CreatingARealNumberUsingScientificNotationThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"3e9"});
  EXPECT_ANY_THROW(RealNumberExpression{"3E9"});
}

TEST(NumbersTest, CreatingARealNumberUsingHexadecimalThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"0x1"});
  EXPECT_ANY_THROW(RealNumberExpression{"0X1"});
}