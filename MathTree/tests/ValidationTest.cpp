#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Parser.hpp"

using MathTree::ArithmeticParser;
using ::testing::ElementsAre;
using ::testing::Pair;
using ::testing::IsEmpty;
using ::testing::Contains;

TEST(ValidationTest, canValidateCorrectInputs) {
  auto errors = ArithmeticParser::validateSyntax("1+(3-2)^2^0*4/2");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, canValidateCorrectInputsWithSpaces) {
  auto errors = ArithmeticParser::validateSyntax(" 1 + ( 3 - 2 ) ^ 2 ^ 0 * 4 / 2 ");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, unpairedOpeningBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2-(1 + (3");
  auto openingError = ArithmeticParser::SyntaxErrors::UnpairedOpeningBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(2, openingError),
                  Pair(7, openingError)));
}

TEST(ValidationTest, unpairedClosingBracketsAreReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("(2-1) )-3");
  auto closingError = ArithmeticParser::SyntaxErrors::UnpairedClosingBracket;
  EXPECT_THAT(errors, ElementsAre(Pair(6, closingError)));
}

TEST(ValidationTest, twoBinaryOperatorsInARowAreReportedAsErrorWithTheCorrespondingIndex) {
  auto operationError = ArithmeticParser::SyntaxErrors::IncompleteOperation;
  auto errors = ArithmeticParser::validateSyntax("1**1*/1*^1+*1-*1");
  EXPECT_THAT(errors, ElementsAre(Pair(2, operationError),
                                  Pair(5, operationError),
                                  Pair(8, operationError),
                                  Pair(11, operationError),
                                  Pair(14, operationError)));
}

TEST(ValidationTest, twoBinaryOperatorsInARowAreReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2* *3/ *5");
  auto operationError = ArithmeticParser::SyntaxErrors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operationError),
                                  Pair(7, operationError)));
}

TEST(ValidationTest, aBinaryOperatorAtTheStartIsReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("*3");
  EXPECT_THAT(errors, ElementsAre(Pair(0, ArithmeticParser::SyntaxErrors::IncompleteOperation)));
}

TEST(ValidationTest, aBinaryOperatorAtTheStartIsReportedAsAnErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax(" *3");
  EXPECT_THAT(errors, ElementsAre(Pair(1, ArithmeticParser::SyntaxErrors::IncompleteOperation)));
}

TEST(ValidationTest, aBinaryOperatorAfterAnOpeningBracketIsReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("(*3)");
  EXPECT_THAT(errors, ElementsAre(Pair(1, ArithmeticParser::SyntaxErrors::IncompleteOperation)));
}

TEST(ValidationTest, aBinaryOperatorAfterAnOpeningBracketIsReportedAsAnErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("( *3)");
  EXPECT_THAT(errors, ElementsAre(Pair(2, ArithmeticParser::SyntaxErrors::IncompleteOperation)));
}

TEST(ValidationTest, doesNotReportABinaryOperatorFollowedByAnyNumberOfSignsAsAnError) {
  auto errors = ArithmeticParser::validateSyntax("1++ 2+-3-+4--5* -6 ++++ 4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, anOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2+3-");
  auto operationError = ArithmeticParser::SyntaxErrors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operationError)));
}

TEST(ValidationTest, anOperatorAtTheEndIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2+3 - ");
  auto operationError = ArithmeticParser::SyntaxErrors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError)));
}

TEST(ValidationTest, anOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2+(3-)+(9+)+5");
  auto operationError = ArithmeticParser::SyntaxErrors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError),
                  Pair(9, operationError)));
}

TEST(ValidationTest, anOperatorBeforeAClosingBracketIsReportedAsIncompleteWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2+(3- )+(9+ )+5");
  auto operationError = ArithmeticParser::SyntaxErrors::IncompleteOperation;
  EXPECT_THAT(errors, ElementsAre(Pair(4, operationError),
                                  Pair(10, operationError)));
}

TEST(ValidationTest, aNumberBeforeAnOpeningBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2(3-1)");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, aNumberWithImpliedDecimalZeroBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2.(3-1)");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, aNumberBeforeAnOpeningBracketIsReportedAsErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2 (3-1)");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, aNumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2+(3-1)3");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(7, operatorsError)));
}

TEST(ValidationTest, aNumberAfterAClosingBracketIsReportedAsMissingOperatorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2+(3-1) 3");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(8, operatorsError)));
}

TEST(ValidationTest, repeatedDecimalsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2.2.2+3");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(3, operatorsError)));
}

TEST(ValidationTest, repeatedDecimalPointsAreReportedAsMissingOperatorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2..2+3");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, anUnrecognisedSymbolIsReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("2+a*3");
  auto symbolError = ArithmeticParser::SyntaxErrors::UnrecognisedSymbol;
  EXPECT_THAT(errors, Contains(Pair(2, symbolError)));
}

TEST(ValidationTest, sqrtCanBeAtTheBeginningOfAnExpression) {
  auto errors = ArithmeticParser::validateSyntax("sqrt4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, sqrtCanBeAtTheBeginningOfAnExpressionAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("    sqrt4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, sqrtCannotBeImmediatelyPrecededByANumber) {
  auto errors = ArithmeticParser::validateSyntax("2sqrt4");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, sqrtCannotBeImmediatelyPrecededByANumberAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2 sqrt4");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, sqrtCanAppearAfterAnOpeningBracket) {
  auto errors = ArithmeticParser::validateSyntax("(sqrt4)");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, sqrtCanAppearAfterAnOpeningBracketAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("( sqrt4)");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, emptyBracketsAreReportedAsAnErrorWithTheCorrespondingIndex) {
  auto errors = ArithmeticParser::validateSyntax("()");
  auto operatorsError = ArithmeticParser::SyntaxErrors::NothingBetweenBrackets;
  EXPECT_THAT(errors, ElementsAre(Pair(0, operatorsError)));
}

TEST(ValidationTest, emptyBracketsAreReportedAsAnErrorWithTheCorrespondingIndexAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("(    )");
  auto operatorsError = ArithmeticParser::SyntaxErrors::NothingBetweenBrackets;
  EXPECT_THAT(errors, ElementsAre(Pair(0, operatorsError)));
}

TEST(ValidationTest, sqrtCanBeCascaded) {
  auto errors = ArithmeticParser::validateSyntax("sqrtsqrt4");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, logCanBeAtTheBeginningOfAnExpression) {
  auto errors = ArithmeticParser::validateSyntax("log10");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, logCanBeAtTheBeginningOfAnExpressionAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("    log10");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, logCannotBeImmediatelyPrecededByANumber) {
  auto errors = ArithmeticParser::validateSyntax("2log10");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(1, operatorsError)));
}

TEST(ValidationTest, logCannotBeImmediatelyPrecededByANumberAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("2 log10");
  auto operatorsError = ArithmeticParser::SyntaxErrors::MissingOperator;
  EXPECT_THAT(errors, ElementsAre(Pair(2, operatorsError)));
}

TEST(ValidationTest, logCanAppearAfterAnOpeningBracket) {
  auto errors = ArithmeticParser::validateSyntax("(log10)");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, logCanAppearAfterAnOpeningBracketAndIgnoringSpaces) {
  auto errors = ArithmeticParser::validateSyntax("( log10)");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, canCascadeLogs) {
  auto errors = ArithmeticParser::validateSyntax("loglog_2(2^10)");
  EXPECT_THAT(errors, IsEmpty());
}

TEST(ValidationTest, canCascadeLogsWithSpaces) {
  auto errors = ArithmeticParser::validateSyntax("log log_2 (2^10)");
  EXPECT_THAT(errors, IsEmpty());
}