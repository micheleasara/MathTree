#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Expression.hpp"
#include "ExpressionMock.hpp"
#include <iostream>
#include "Token.hpp"

using ::testing::Return;
using ::testing::ElementsAreArray;
using MathTree::AdditionExpression;
using MathTree::BinaryExpression;
using MathTree::DivisionExpression;
using MathTree::ExponentiationExpression;
using MathTree::Expression;
using MathTree::MultiplicationExpression;
using MathTree::TokenType;


class BinaryExpressionsTest: public ::testing::Test {
protected:
  std::unique_ptr<NiceExpressionMock> leftMock = std::make_unique<NiceExpressionMock>();
  std::unique_ptr<NiceExpressionMock> rightMock = std::make_unique<NiceExpressionMock>();
};

class BinaryExpressionStub: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const {
    return 0.0;
  }
  void print(std::ostream&) {}
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

TEST_F(BinaryExpressionsTest, zeroToTheZerothPowerThrows) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(0.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(0.0));

  ExponentiationExpression power{std::move(leftMock), TokenType::Caret, std::move(rightMock)};
  EXPECT_ANY_THROW(power.evaluate());
}

TEST_F(BinaryExpressionsTest, zeroToANegativePowerThrows) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(0.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(-3.0));

  ExponentiationExpression power{std::move(leftMock), TokenType::Caret, std::move(rightMock)};
  EXPECT_ANY_THROW(power.evaluate());
}

TEST_F(BinaryExpressionsTest, evaluatingThePowerOfANegativeNumberThrowsWhenTheExponentIsNotAnInteger) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(-3));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(0.5));

  ExponentiationExpression power{std::move(leftMock), TokenType::Caret, std::move(rightMock)};
  EXPECT_ANY_THROW(power.evaluate());
}

TEST_F(BinaryExpressionsTest, evaluatingAPowerReturnsTheBaseRaisedToTheExponent) {
  EXPECT_CALL(*leftMock, evaluate()).WillOnce(Return(2.0));
  EXPECT_CALL(*rightMock, evaluate()).WillOnce(Return(3.0));

  ExponentiationExpression power{std::move(leftMock), TokenType::Caret, std::move(rightMock)};
  EXPECT_DOUBLE_EQ(power.evaluate(), 8.0);
}

TEST_F(BinaryExpressionsTest, aBinaryExpressionThrowsIfConstructedWithEitherArgumentNull) {
  EXPECT_ANY_THROW(BinaryExpressionStub binary(nullptr, TokenType::Asterisk, std::move(rightMock)));
  EXPECT_ANY_THROW(BinaryExpressionStub binary(std::move(leftMock), TokenType::Asterisk, nullptr));
}

TEST_F(BinaryExpressionsTest, aBinaryExpressionThrowsIfConstructedWithBothArgumentsNull) {
  EXPECT_ANY_THROW(BinaryExpressionStub binary(nullptr, TokenType::Asterisk, nullptr));
}

TEST_F(BinaryExpressionsTest, aBinaryExpressionReturnsTheTwoSubexpressionsUsedToConstructItOrderedLeftToRight) {
  auto leftMockPtr = leftMock.get();
  auto rightMockPtr = rightMock.get();
  BinaryExpressionStub binary(std::move(leftMock), TokenType::Asterisk, std::move(rightMock));
  EXPECT_THAT(binary.subexpressions(), ElementsAreArray({leftMockPtr, rightMockPtr}));
}