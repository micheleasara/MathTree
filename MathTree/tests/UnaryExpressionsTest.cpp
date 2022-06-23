#include <gtest/gtest.h>
#include "Expression.hpp"
#include "ExpressionMock.hpp"
#include <limits>
#include <memory>

using MathTree::RealNumberExpression;
using MathTree::SquareRootExpression;
using MathTree::NegativeSignExpression;
using MathTree::TokenType;
using MathTree::LogarithmExpression;
using ::testing::Return;

class UnaryExpressionsTest: public ::testing::Test {
protected:
  std::unique_ptr<NiceExpressionMock> exprMock = std::make_unique<NiceExpressionMock>();
};

TEST_F(UnaryExpressionsTest, evaluatingASquareRootOfANegativeNumberThrows) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(-1));
  SquareRootExpression sqrt(std::move(exprMock), TokenType::SquareRoot);
  EXPECT_ANY_THROW(sqrt.evaluate());
}

TEST_F(UnaryExpressionsTest, evaluatingASquareRootReturnsTheSquareRootOfItsRadicand) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(16));
  SquareRootExpression sqrt(std::move(exprMock), TokenType::SquareRoot);
  EXPECT_DOUBLE_EQ(sqrt.evaluate(), 4.0);
}

TEST_F(UnaryExpressionsTest, evaluatingTheNegationOfAPositiveNumberReturnsANegativeNumberWithTheSameMagnitude) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(42));
  NegativeSignExpression minusFortyTwo(TokenType::Minus, std::move(exprMock));
  EXPECT_DOUBLE_EQ(minusFortyTwo.evaluate(), -42.0);
}

TEST_F(UnaryExpressionsTest, evaluatingTheNegationOfANegativeNumberReturnsAPositiveNumberWithTheSameMagnitude) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(-42));
  NegativeSignExpression plusFortyTwo(TokenType::Minus, std::move(exprMock));
  EXPECT_DOUBLE_EQ(plusFortyTwo.evaluate(), 42.0);
}

TEST_F(UnaryExpressionsTest, constructingALogarithmWithNonPositiveOrInfiniteBaseThrows) {
  EXPECT_ANY_THROW(LogarithmExpression logarithm(std::move(exprMock), -1.0, TokenType::Log));
  EXPECT_ANY_THROW(LogarithmExpression logarithm(std::move(exprMock), 0.0, TokenType::Log));

  if (std::numeric_limits<double>::has_infinity) {
    EXPECT_ANY_THROW(LogarithmExpression logarithm(std::move(exprMock),
                                       std::numeric_limits<double>::infinity(),
                                       TokenType::Log));
  }

  if (std::numeric_limits<double>::has_quiet_NaN) {
    EXPECT_ANY_THROW(LogarithmExpression logarithm(std::move(exprMock),
                     std::numeric_limits<double>::quiet_NaN(), TokenType::Log));
  }

  if (std::numeric_limits<double>::has_signaling_NaN) {
    EXPECT_ANY_THROW(LogarithmExpression logarithm(std::move(exprMock),
                     std::numeric_limits<double>::signaling_NaN(), TokenType::Log));
  }
}

TEST_F(UnaryExpressionsTest, evaluatingLogOfANonPositiveNumberThrows) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(0.0))
                                  .WillOnce(Return(-1.0));
  LogarithmExpression logarithm(std::move(exprMock), 10.0, TokenType::Log);
  EXPECT_ANY_THROW(logarithm.evaluate());
  EXPECT_ANY_THROW(logarithm.evaluate());
}

TEST_F(UnaryExpressionsTest, canComputeLog10) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(100.0));
  LogarithmExpression logarithm(std::move(exprMock), 10.0, TokenType::Log);
  EXPECT_DOUBLE_EQ(logarithm.evaluate(), 2.0);
}

TEST_F(UnaryExpressionsTest, canComputeNaturalLog) {
  auto const nepero = std::exp(1.0);
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(nepero));
  LogarithmExpression logarithm(std::move(exprMock), nepero, TokenType::Log);
  EXPECT_DOUBLE_EQ(logarithm.evaluate(), 1.0);
}

TEST_F(UnaryExpressionsTest, canComputeLogWithArbitraryBases) {
  EXPECT_CALL(*exprMock, evaluate).WillOnce(Return(42.0 * 42.0 * 42.0));
  LogarithmExpression logarithm(std::move(exprMock), 42.0, TokenType::Log);
  EXPECT_DOUBLE_EQ(logarithm.evaluate(), 3.0);
}