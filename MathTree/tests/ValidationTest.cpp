#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Parser.hpp"

using MathTree::ArithmeticParser;
using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::IsEmpty;

TEST(ValidationTest, CanValidateCorrectInputs) {
  auto errors = ArithmeticParser::validate("1+(3-2)^2^0*4/2");
  EXPECT_EQ(errors.size(), 0);
}

TEST(ValidationTest, CanValidateCorrectInputsWithSpaces) {
  auto errors = ArithmeticParser::validate(" 1 + ( 3 - 2 ) ^ 2 ^ 0 * 4 / 2 ");
  EXPECT_EQ(errors.size(), 0);
}

TEST(ValidationTest, UnpairedOpeningBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2-(1 + (3");
  auto openingError = ArithmeticParser::Errors::UnpairedOpeningBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(2, openingError),
                  Pair(7, openingError)));
}

TEST(ValidationTest, UnpairedClosingBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("(2-1) )-3");
  auto closingError = ArithmeticParser::Errors::UnpairedClosingBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(6, closingError)));
}

TEST(ValidationTest, TwoOperatorsInARowExcludingSignsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  auto errors = ArithmeticParser::validate("1**1*/1*^1+*1-*1");
  EXPECT_THAT(errors, ElementsAre(Pair(2, operationError),
                                  Pair(5, operationError),
                                  Pair(8, operationError),
                                  Pair(11, operationError),
                                  Pair(14, operationError)));
}

TEST(ValidationTest, TwoOperatorsInARowExcludingSignsAreReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2* *3/ *5");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operationError),
                  Pair(7, operationError)));
}

TEST(ValidationTest, DoesNotReportAnOperatorFollowedByAnyNumberOfSignsAsAnError) {
  auto errors = ArithmeticParser::validate("1++ 2+-3-+4--5* -6 ++++ 4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, AnOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+3-");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operationError)));
}

TEST(ValidationTest, AnOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2+3 - ");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError)));
}

TEST(ValidationTest, AnOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+(3-)+(9+)+5");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError),
                  Pair(9, operationError)));
}

TEST(ValidationTest, AnOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2+(3- )+(9+ )+5");
  auto operationError = ArithmeticParser::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError),
                                  Pair(10, operationError)));
}

TEST(ValidationTest, ANumberBeforeAnOpeningBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2(3-1)");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, ANumberWithImpliedDecimalZeroBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2.(3-1)");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, ANumberBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2 (3-1)");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, ANumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+(3-1)3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(7, operatorsError)));
}

TEST(ValidationTest, ANumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validate("2+(3-1) 3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(8, operatorsError)));
}

TEST(ValidationTest, RepeatedDecimalsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2.2.2+3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operatorsError)));
}

TEST(ValidationTest, RepeatedDecimalPointsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2..2+3");
  auto operatorsError = ArithmeticParser::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, AnUnrecognisedSymbolIsReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validate("2+a*3");
  auto symbolError = ArithmeticParser::Errors::UnrecognisedSymbol;
  EXPECT_THAT(errors, ElementsAre(Pair(2, symbolError)));
}
}
