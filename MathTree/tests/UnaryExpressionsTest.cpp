#include <gtest/gtest.h>
#include "Expression.hpp"
#include "ExpressionMock.hpp"
#include <memory>

using MathTree::RealNumberExpression;
using MathTree::SquareRootExpression;
using MathTree::NegativeSignExpression;
using MathTree::TokenType;

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