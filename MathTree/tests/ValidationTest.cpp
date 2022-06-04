#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Expression.hpp"

using MathTree::Validator;
using ::testing::ElementsAre;
using ::testing::Pair;

TEST(ValidationTest, CanValidateCorrectInputs) {
  auto errors = Validator::validate("1+(3-2)^2^0*4/2");
  EXPECT_EQ(errors.size(), 0);
}

TEST(ValidationTest, CanValidateCorrectInputsWithSpaces) {
  auto errors = Validator::validate(" 1 + ( 3 - 2 ) ^ 2 ^ 0 * 4 / 2 ");
  EXPECT_EQ(errors.size(), 0);
}

TEST(ValidationTest, UnpairedOpeningBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2-(1 + (3");
  auto openingError = Validator::Errors::UnpairedOpeningBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(2, openingError),
                  Pair(7, openingError)));
}

TEST(ValidationTest, UnpairedClosingBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("(2-1) )-3");
  auto closingError = Validator::Errors::UnpairedClosingBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(6, closingError)));
}

TEST(ValidationTest, TwoOperatorsInARowAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2++3--5**3//4^^5");
  auto operatorsError = Validator::Errors::TwoOperatorsInARow;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError),
                                  Pair(5, operatorsError),
                                  Pair(8, operatorsError),
                                  Pair(11, operatorsError),
                                  Pair(14, operatorsError)));
}

TEST(ValidationTest, TwoOperatorsInARowAreReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = Validator::validate("2+ +3- -5");
  auto operatorsError = Validator::Errors::TwoOperatorsInARow;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operatorsError),
                  Pair(7, operatorsError)));
}

TEST(ValidationTest, AnOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2+3-");
  auto operatorsError = Validator::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operatorsError)));
}

TEST(ValidationTest, AnOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = Validator::validate("2+3 - ");
  auto operatorsError = Validator::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operatorsError)));
}

TEST(ValidationTest, AnOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2+(3-)+(9+)+5");
  auto operatorsError = Validator::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operatorsError),
                  Pair(9, operatorsError)));
}

TEST(ValidationTest, AnOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = Validator::validate("2+(3- )+(9+ )+5");
  auto operatorsError = Validator::Errors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operatorsError),
                                  Pair(10, operatorsError)));
}

TEST(ValidationTest, ANumberBeforeAnOpeningBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2(3-1)");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, ANumberWithImpliedDecimalZeroBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2.(3-1)");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, ANumberBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = Validator::validate("2 (3-1)");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, ANumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2+(3-1)3");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(7, operatorsError)));
}

TEST(ValidationTest, ANumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = Validator::validate("2+(3-1) 3");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(8, operatorsError)));
}

TEST(ValidationTest, RepeatedDecimalsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2.2.2+3");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operatorsError)));
}

TEST(ValidationTest, RepeatedDecimalPointsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2..2+3");
  auto operatorsError = Validator::Errors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}
TEST(ValidationTest, AnUnrecognisedSymbolIsReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = Validator::validate("2+a*3");
  auto operatorsError = Validator::Errors::UnrecognisedSymbol;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}
