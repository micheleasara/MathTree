#include "AbstractPrattParserMock.hpp"
#include "ExpressionMock.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Token.hpp"

using ::testing::Return;
using ::testing::ByMove;

using MathTree::AdditionParselet;
using MathTree::DivisionParselet;
using MathTree::ExponentiationParselet;
using MathTree::MultiplicationParselet;
using MathTree::SubtractionParselet;
using MathTree::Token;
using MathTree::TokenType;

class InfixParseletsTest: public ::testing::Test {  
protected:
  NiceAbstractPrattParserMock parserMock;
  std::unique_ptr<NiceExpressionMock> expressionMock = std::make_unique<NiceExpressionMock>();
};

TEST_F(InfixParseletsTest, additionParseletAsksForTheRHSExpressionUsingThePriorityGivenOnConstruction) {
  int priority = 1;
  EXPECT_CALL(parserMock, parse(priority)).WillOnce(
                                                Return(ByMove(std::make_unique<NiceExpressionMock>())));
  AdditionParselet addition{priority};
  addition.parse(parserMock, std::move(expressionMock), Token{TokenType::Plus, "+"});
}

TEST_F(InfixParseletsTest, subtractionParseletAsksForTheRHSExpressionUsingThePriorityGivenOnConstruction) {
  int priority = 1;
  EXPECT_CALL(parserMock, parse(priority)).WillOnce(
                                                Return(ByMove(std::make_unique<NiceExpressionMock>())));
  SubtractionParselet subtraction{priority};
  subtraction.parse(parserMock, std::move(expressionMock), Token{TokenType::Minus, "-"});
}

TEST_F(InfixParseletsTest, multiplicationParseletAsksForTheRHSExpressionUsingThePriorityGivenOnConstruction) {
  int priority = 1;
  EXPECT_CALL(parserMock, parse(priority)).WillOnce(
                                                Return(ByMove(std::make_unique<NiceExpressionMock>())));
  MultiplicationParselet multiplication{priority};
  multiplication.parse(parserMock, std::move(expressionMock), Token{TokenType::Asterisk, "*"});
}

TEST_F(InfixParseletsTest, divisionParseletAsksForTheRHSExpressionUsingThePriorityGivenOnConstruction) {
  int priority = 1;
  EXPECT_CALL(parserMock, parse(priority)).WillOnce(
                                                Return(ByMove(std::make_unique<NiceExpressionMock>())));
  DivisionParselet division{priority};
  division.parse(parserMock, std::move(expressionMock), Token{TokenType::Slash, "/"});
}

TEST_F(InfixParseletsTest, exponentiationParseletAsksForTheRHSExpressionUsingThePriorityGivenOnConstructionMinusOne) {
  int priority = 1;
  EXPECT_CALL(parserMock, parse(priority-1)).WillOnce(
                                                Return(ByMove(std::make_unique<NiceExpressionMock>())));
  ExponentiationParselet exponentiation{priority};
  exponentiation.parse(parserMock, std::move(expressionMock), Token{TokenType::Caret, "^"});
}