#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Expression.hpp"
#include "Parser.hpp"

using MathTree::ArithmeticParser;
using ::testing::ElementsAre;
using ::testing::Pair;

class ArithmeticParserTest: public ::testing::Test {
protected:
  ArithmeticParser parser;
};

TEST_F(ArithmeticParserTest, IgnoresUselessParentheses) {
    auto result = parser.parse("((-2.3))");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), -2.3);
}

TEST_F(ArithmeticParserTest, CanParseTwoAdditions) {
    auto result = parser.parse("1+3+10");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 14.0);
}

TEST_F(ArithmeticParserTest, CanParseTwoMultiplications) {
    auto result = parser.parse("2*4.5*5");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 45.0);
}

TEST_F(ArithmeticParserTest, ParsesDivisionsWithLeftToRightAssociativity) {
    auto result = parser.parse("15/3/5");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 1.0);
}

TEST_F(ArithmeticParserTest, ParsesSubtractionsWithLeftToRightAssociativity) {
    auto result = parser.parse("1-3-10");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), -12.0);
}

TEST_F(ArithmeticParserTest, CanParseOperatorsWithDifferentPrioritiesAndGoesLeftToRightIfThePriorityIsTheSame) {
    auto result = parser.parse("10+2*5-20/2");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 10.0);
}

TEST_F(ArithmeticParserTest, ExpressionsInParenthesesHavePriorityOverExpressionsOutsideOfParentheses) {
    auto result = parser.parse("1+2*(3-1)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 5.0);
}

TEST_F(ArithmeticParserTest, ExpressionsInInnerMostParenthesesHavePriorityOverExpressionsInOuterParentheses) {
    auto result = parser.parse("2*(3*(1+1)+(3*2))");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 24.0);
}

TEST_F(ArithmeticParserTest, CanInterpretNonAssociativeOperatorsInNestedExpressions) {
    auto result = parser.parse("100-(30/3/(3-1)+0-3-2)-10/5");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 98.0);
}

TEST_F(ArithmeticParserTest, CanInterpretPositiveSignOutsideOfParentheses) {
    auto result = parser.parse("+(3-1)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 2.0);
}

TEST_F(ArithmeticParserTest, CanInterpretNegativeSignOutsideOfParentheses) {
    auto result = parser.parse("-(3-1)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), -2.0);
}

TEST_F(ArithmeticParserTest, CanInterpretSignInteractionBetweenSignsAcrossParentheses) {
    auto result = parser.parse("-(-3)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 3.0);

    result = parser.parse("+(+3)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 3.0);

    result = parser.parse("-(+3)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), -3.0);

    result = parser.parse("+(-3)");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), -3.0);
}

TEST_F(ArithmeticParserTest, CanParseTwoPowersWithRightToLeftAssociativity) {
    auto result = parser.parse("2^3^0");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 2.0);
}

TEST_F(ArithmeticParserTest, CanParseTwoSquareRoots) {
    auto result = parser.parse("sqrtsqrt16");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 2.0);
}