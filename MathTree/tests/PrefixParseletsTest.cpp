#include "AbstractPrattParserMock.hpp"
#include "Expression.hpp"
#include "ExpressionMock.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>
#include "Token.hpp"

using ::testing::ByMove;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::WhenDynamicCastTo;

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

TEST_F(PrefixParseletsTest, groupParseletReturnsTheExpressionItObtainsFromTheParser) {
  GroupParselet group;
  std::unique_ptr<NiceExpressionMock> expressionMockPtr{std::make_unique<NiceExpressionMock>()};
  auto& expressionMock = *expressionMockPtr;
  ON_CALL(parserMock, consumeCurrentToken).WillByDefault(
                                                 Return(Token{TokenType::ClosingBracket, ")"}));
  EXPECT_CALL(parserMock, parse()).WillOnce(
                                      Return(ByMove(std::move(expressionMockPtr))));

  auto parseResult = group.parse(parserMock, Token{TokenType::OpeningBracket, "("});
  EXPECT_EQ(parseResult.get(), &expressionMock);
}

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

TEST_F(PrefixParseletsTest, squareRootParseletReturnsAnObjectOfTheCorrespondingExpressionType) {
  SquareRootParselet squareRoot(1);
  auto parseResult = squareRoot.parse(parserMock, Token{TokenType::SquareRoot, "sqrt"});
  ASSERT_THAT(parseResult, NotNull());
  EXPECT_THAT(parseResult.get(), WhenDynamicCastTo<MathTree::SquareRootExpression*>(NotNull()));
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

TEST_F(PrefixParseletsTest, negativeSignParseletReturnsAnObjectOfTheCorrespondingExpressionType) {
  NegativeSignParselet parselet(1);
  auto parseResult = parselet.parse(parserMock, Token{TokenType::Minus, "-"});
  ASSERT_THAT(parseResult, NotNull());
  EXPECT_THAT(parseResult.get(), WhenDynamicCastTo<MathTree::NegativeSignExpression*>(NotNull()));
}

TEST_F(PrefixParseletsTest, logarithmParseletAsksForItsArgumentByParsingWithThePriorityUsedForItsConstruction) {
  int priority = 1;
  LogarithmParselet logarithm(priority);
  EXPECT_CALL(parserMock, parse(priority)).WillOnce(
                                               Return(ByMove(std::make_unique<ExpressionMock>())));
  logarithm.parse(parserMock, Token{TokenType::Log, "log"});
}

TEST_F(PrefixParseletsTest, logarithmSignParseletReturnsAnObjectOfTheCorrespondingExpressionType) {
  LogarithmParselet parselet(1);
  ON_CALL(parserMock, parse(1)).WillByDefault(
                                        Return(ByMove(std::make_unique<NiceExpressionMock>())));
  auto parseResult = parselet.parse(parserMock, Token{TokenType::Log, "log"});
  ASSERT_THAT(parseResult, NotNull());
  EXPECT_THAT(parseResult.get(), WhenDynamicCastTo<MathTree::LogarithmExpression*>(NotNull()));
}