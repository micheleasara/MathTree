#include <cmath>
#include "Expression.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::IsEmpty;
using MathTree::RealNumberExpression;

TEST(NumbersTest, evaluatingARealNumberReturnsTheNumberItself) {
  RealNumberExpression number{"42.2"};
  EXPECT_DOUBLE_EQ(number.evaluate(), 42.2);
}

TEST(NumbersTest, printingARealNumberPrintsTheNumberItself) {
  std::string numStr{"42.2"};
  RealNumberExpression number{numStr};
  std::stringstream ss;
  number.print(ss);
  EXPECT_EQ(ss.str(), numStr);
}

TEST(NumbersTest, creatingARealNumberFromAStringWithLettersThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"a45"});
  EXPECT_ANY_THROW(RealNumberExpression{"4a5"});
  EXPECT_ANY_THROW(RealNumberExpression{"45a"});
}

TEST(NumbersTest, creatingARealNumberFromAStringWithSpacesThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{" 45"});
}

TEST(NumbersTest, creatingARealNumberWithInfinityThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"INFINITY"});
}

TEST(NumbersTest, creatingARealNumberWithNaNThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"NAN"});
}

TEST(NumbersTest, creatingARealNumberUsingScientificNotationThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"3e9"});
  EXPECT_ANY_THROW(RealNumberExpression{"3E9"});
}

TEST(NumbersTest, creatingARealNumberUsingHexadecimalThrows) {
  EXPECT_ANY_THROW(RealNumberExpression{"0x1"});
  EXPECT_ANY_THROW(RealNumberExpression{"0X1"});
}

TEST(NumbersTest, aRealNumberHasNoSubexpressions) {
  RealNumberExpression numberTen("10");
  EXPECT_THAT(numberTen.subexpressions(), IsEmpty());
}