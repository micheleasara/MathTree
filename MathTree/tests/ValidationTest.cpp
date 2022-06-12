#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Parser.hpp"

using MathTree::ArithmeticParser;
using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::IsEmpty;

TEST(ValidationTest, canValidateCorrectInputs) {
  auto errors = ArithmeticParser::validate("1+(3-2)^2^0*4/2");
  EXPECT_EQ(errors.size(), 0);
}

TEST(ValidationTest, canValidateCorrectInputsWithSpaces) {
  auto errors = ArithmeticParser::validate(" 1 + ( 3 - 2 ) ^ 2 ^ 0 * 4 / 2 ");
  EXPECT_EQ(errors.size(), 0);
}

TEST(ValidationTest, unpairedOpeningBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2-(1 + (3");
  auto openingError = ArithmeticParser::Errors::UnpairedOpeningBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(2, openingError),
                  Pair(7, openingError)));
}

TEST(ValidationTest, unpairedClosingBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("(2-1) )-3");
  auto closingError = ArithmeticParser::Errors::UnpairedClosingBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(6, closingError)));
}

TEST(ValidationTest, twoBinaryOperatorsInARowAreReportedAsErrorWithTheCorrespondingIndex) {
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  auto errors = ArithmeticParser::validate("1**1*/1*^1+*1-*1");
  EXPECT_THAT(errors, ElementsAre(Pair(2, operationError),
                                  Pair(5, operationError),
                                  Pair(8, operationError),
                                  Pair(11, operationError),
                                  Pair(14, operationError)));
}

TEST(ValidationTest, twoBinaryOperatorsInARowAreReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2* *3/ *5");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operationError),
                  Pair(7, operationError)));
}

TEST(ValidationTest, doesNotReportABinaryOperatorFollowedByAnyNumberOfSignsAsAnError) {
  auto errors = ArithmeticParser::validate("1++ 2+-3-+4--5* -6 ++++ 4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, anOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+3-");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operationError)));
}

TEST(ValidationTest, anOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2+3 - ");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError)));
}

TEST(ValidationTest, anOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+(3-)+(9+)+5");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError),
                  Pair(9, operationError)));
}

TEST(ValidationTest, anOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2+(3- )+(9+ )+5");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError),
                                  Pair(10, operationError)));
}

TEST(ValidationTest, aNumberBeforeAnOpeningBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2(3-1)");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, aNumberWithImpliedDecimalZeroBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2.(3-1)");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, aNumberBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2 (3-1)");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, aNumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+(3-1)3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(7, operatorsError)));
}

TEST(ValidationTest, aNumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2+(3-1) 3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(8, operatorsError)));
}

TEST(ValidationTest, repeatedDecimalsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2.2.2+3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operatorsError)));
}

TEST(ValidationTest, repeatedDecimalPointsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2..2+3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, anUnrecognisedSymbolIsReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+a*3");
  auto symbolError = ArithmeticParser::Errors::UnrecognisedSymbol;
  EXPECT_THAT(errors, ElementsAre(Pair(2, symbolError)));
}

TEST(ValidationTest, sqrtCanBeAtTheBeginningOfAnExpression) {
  auto errors = ArithmeticParser::validate("sqrt4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, sqrtCanBeAtTheBeginningOfAnExpressionAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("    sqrt4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, sqrtCannotBeImmediatelyPrecededByANumber) {
  auto errors = ArithmeticParser::validate("2sqrt4");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, sqrtCannotBeImmediatelyPrecededByANumberAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2 sqrt4");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, emptyBracketsAreReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("()");
  auto operatorsError = ArithmeticParser::Errors::NothingBetweenBrackets;
  EXPECT_THAT(errors, ElementsAre(Pair(0, operatorsError)));
}

TEST(ValidationTest, emptyBracketsAreReportedAsAnErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("(    )");
  auto operatorsError = ArithmeticParser::Errors::NothingBetweenBrackets;
  EXPECT_THAT(errors, ElementsAre(Pair(0, operatorsError)));
}
