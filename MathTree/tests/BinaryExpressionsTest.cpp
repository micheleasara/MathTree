#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Expression.hpp"
#include <iostream>
#include "Token.hpp"

using MathTree::Expression;
using MathTree::AdditionExpression;
using MathTree::DivisionExpression;
using MathTree::MultiplicationExpression;
using MathTree::TokenType;

using ::testing::Return;
using ::testing::NiceMock;

class ExpressionMock: public Expression {
  public:
  MOCK_METHOD(double, evaluate, (), (const, override));
  MOCK_METHOD(void, print, (std::ostream&), (const, override));

};
using NiceExpressionMock = NiceMock<ExpressionMock>;

class BinaryExpressionsTest: public ::testing::Test {
  protected:
  std::unique_ptr<NiceExpressionMock> leftMock = std::make_unique<NiceExpressionMock>();
  std::unique_ptr<NiceExpressionMock> rightMock = std::make_unique<NiceExpressionMock>();
};

TEST_F(BinaryExpressionsTest, evaluatingAnAdditionReturnsTheSumOfTheValuesOfBothAddends) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(5.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(4.1));

  AdditionExpression adder{std::move(leftMock), TokenType::PLUS, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(adder.evaluate(), 9.1);
}

TEST_F(BinaryExpressionsTest, evaluatingAMultiplicationReturnsTheProductOfTheValuesOfBothFactors) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(1.5));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(4.0));

  MultiplicationExpression multiplier{std::move(leftMock), TokenType::ASTERISK, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(multiplier.evaluate(), 6.0);
}

TEST_F(BinaryExpressionsTest, evaluatingADivisionReturnsTheQuotientOfTheValuesOfTheDividendAndDivisor) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(10.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(5.0));

  DivisionExpression divider{std::move(leftMock), TokenType::SLASH, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(divider.evaluate(), 2.0);
}

TEST_F(BinaryExpressionsTest, dividingByZeroThrows) {
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(0.0));

  DivisionExpression divider{std::move(leftMock), TokenType::SLASH, std::move(rightMock)};
  EXPECT_ANY_THROW(divider.evaluate());
}