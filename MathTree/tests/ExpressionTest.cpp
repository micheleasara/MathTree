#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Expression.hpp"

using MathTree::ExpressionFactory;
using ::testing::ElementsAre;
using ::testing::Pair;

TEST(ExpressionTest, IgnoresUselessParentheses) {
    auto result = ExpressionFactory::parse("((-2.3))");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), -2.3);
}

TEST(ExpressionTest, CanParseTwoAdditions) {
    auto result = ExpressionFactory::parse("1+3+10");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 14);
}

TEST(ExpressionTest, CanParseTwoMultiplications) {
    auto result = ExpressionFactory::parse("2*4.5*5");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 45);
}

TEST(ExpressionTest, ParsesDivisionsWithLeftToRightAssociativity) {
    auto result = ExpressionFactory::parse("15/3/5");
    ASSERT_NE(result, nullptr);
    EXPECT_DOUBLE_EQ(result->evaluate(), 1);
}

TEST(ExpressionTest, ParsesSubtractionsWithLeftToRightAssociativity) {
    auto result = ExpressionFactory::parse("1-3-10");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), -12);
}

TEST(ExpressionTest, CanParseOperatorsWithDifferentPrioritiesAndGoesLeftToRightIfThePriorityIsTheSame) {
    auto result = ExpressionFactory::parse("10+2*5-20/2");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 10);
}

TEST(ExpressionTest, ExpressionsInParenthesesHavePriorityOverExpressionsOutsideOfParentheses) {
    auto result = ExpressionFactory::parse("1+2*(3-1)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 5);
}

TEST(ExpressionTest, ExpressionsInInnerMostParenthesesHavePriorityOverExpressionsInOuterParentheses) {
    auto result = ExpressionFactory::parse("2*(3*(1+1)+(3*2))");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 24);
}

TEST(ExpressionTest, CanInterpretNonAssociativeOperatorsInNestedExpressions) {
    auto result = ExpressionFactory::parse("100-(30/3/(3-1)+0-3-2)-10/5");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 98);
}

TEST(ExpressionTest, CanInterpretPositiveSignOutsideOfParentheses) {
    auto result = ExpressionFactory::parse("+(3-1)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 2);
}

TEST(ExpressionTest, CanInterpretNegativeSignOutsideOfParentheses) {
    auto result = ExpressionFactory::parse("-(3-1)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), -2);
}

TEST(ExpressionTest, CanInterpretSignInteractionBetweenSignsAcrossParentheses) {
    auto result = ExpressionFactory::parse("-(-3)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 3);

    result = ExpressionFactory::parse("+(+3)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), 3);

    result = ExpressionFactory::parse("-(+3)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), -3);

    result = ExpressionFactory::parse("+(-3)");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->evaluate(), -3);
}

TEST(ExpressionTest, CanValidateCorrectInputs) {
    auto errors = ExpressionFactory::validate("1+(3-2)");
    EXPECT_EQ(errors.size(), 0);
}

TEST(ExpressionTest, UnpairedOpeningBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
    auto errors = ExpressionFactory::validate("2-(1 + (3");
    auto openingError = ExpressionFactory::ValidationErrors::UnpairedOpeningBracket;
    EXPECT_THAT(errors, ElementsAre(Pair(2, openingError),
                                    Pair(7, openingError)));
}

TEST(ExpressionTest, UnpairedClosingBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
    auto errors = ExpressionFactory::validate("(2-1) )-3");
    auto closingError = ExpressionFactory::ValidationErrors::UnpairedClosingBracket;
    EXPECT_THAT(errors, ElementsAre(Pair(6, closingError)));
}