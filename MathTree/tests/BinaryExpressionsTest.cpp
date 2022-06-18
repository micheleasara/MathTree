#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Expression.hpp"
#include "ExpressionMock.hpp"
#include <iostream>
#include "Token.hpp"

using MathTree::Expression;
using MathTree::AdditionExpression;
using MathTree::DivisionExpression;
using MathTree::MultiplicationExpression;
using MathTree::TokenType;

using ::testing::Return;

class BinaryExpressionsTest: public ::testing::Test {
  protected:
  std::unique_ptr<NiceExpressionMock> leftMock = std::make_unique<NiceExpressionMock>();
  std::unique_ptr<NiceExpressionMock> rightMock = std::make_unique<NiceExpressionMock>();
};

TEST_F(BinaryExpressionsTest, evaluatingAnAdditionReturnsTheSumOfTheValuesOfBothAddends) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(5.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(4.1));

  AdditionExpression adder{std::move(leftMock), TokenType::Plus, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(adder.evaluate(), 9.1);
}

TEST_F(BinaryExpressionsTest, evaluatingAMultiplicationReturnsTheProductOfTheValuesOfBothFactors) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(1.5));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(4.0));

  MultiplicationExpression multiplier{std::move(leftMock), TokenType::Asterisk, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(multiplier.evaluate(), 6.0);
}

TEST_F(BinaryExpressionsTest, evaluatingADivisionReturnsTheQuotientOfTheValuesOfTheDividendAndDivisor) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(10.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(5.0));

  DivisionExpression divider{std::move(leftMock), TokenType::Slash, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(divider.evaluate(), 2.0);
}

TEST_F(BinaryExpressionsTest, dividingByZeroThrows) {
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(0.0));

  DivisionExpression divider{std::move(leftMock), TokenType::Slash, std::move(rightMock)};
  EXPECT_ANY_THROW(divider.evaluate());
}