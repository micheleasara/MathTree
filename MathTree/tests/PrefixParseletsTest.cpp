#include "AbstractPrattParserMock.hpp"
#include "Expression.hpp"
#include "ExpressionMock.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include "Token.hpp"

using ::testing::ByMove;
using ::testing::Return;

using MathTree::GroupParselet;
using MathTree::LogarithmParselet;
using MathTree::NegativeSignParselet;
using MathTree::SquareRootParselet;
using MathTree::Token;
using MathTree::TokenType;

class PrefixParseletsTest: public ::testing::Test {  
protected:
  NiceAbstractPrattParserMock parserMock;
};

TEST_F(PrefixParseletsTest, groupParseletThrowsIfTheTokenAfterIsNotAClosingBracket) {
  GroupParselet group;
  EXPECT_CALL(parserMock, consumeCurrentToken).WillOnce(
                                                   Return(Token{TokenType::Minus, ""}));
  EXPECT_ANY_THROW(group.parse(parserMock, Token{TokenType::OpeningBracket, "("}));
}

TEST_F(PrefixParseletsTest, groupParseletDoesNotThrowIfTheTokenAfterIsAClosingBracket) {
  GroupParselet group;
  EXPECT_CALL(parserMock, consumeCurrentToken).WillOnce(
                                                   Return(Token{TokenType::ClosingBracket, ")"}));
  EXPECT_NO_THROW(group.parse(parserMock, Token{TokenType::OpeningBracket, "("}));
}

TEST_F(PrefixParseletsTest, squareRootParseletAsksForItsArgumentByParsingWithThePriorityUsedForItsConstruction) {
  int priority = 1;
  SquareRootParselet squareRoot(priority);
  EXPECT_CALL(parserMock, parse(priority));
  squareRoot.parse(parserMock, Token{TokenType::SquareRoot, "sqrt"});
}

TEST_F(PrefixParseletsTest, negativeSignParseletAsksForItsArgumentByParsingWithThePriorityUsedForItsConstruction) {
  int priority = 1;
  NegativeSignParselet negation(priority);
  EXPECT_CALL(parserMock, parse(priority));
  negation.parse(parserMock, Token{TokenType::Minus, "-"});
}

TEST_F(PrefixParseletsTest, logarithmParseletAsksForItsArgumentByParsingWithThePriorityUsedForItsConstruction) {
  int priority = 1;
  LogarithmParselet logarithm(priority);
  EXPECT_CALL(parserMock, parse(priority)).WillOnce(
                                               Return(ByMove(std::make_unique<ExpressionMock>())));
  logarithm.parse(parserMock, Token{TokenType::Log, "log"});
}

}