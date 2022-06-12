#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Lexer.hpp"
#include "Parser.hpp"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::Const;
using ::testing::Ref;
using ::testing::DoDefault;
using ::testing::ByMove;
using ::testing::_;
using ::testing::Pointee;
using ::testing::InSequence;
using MathTree::Expression;
using MathTree::Token;
using MathTree::PrattParser;
using MathTree::TokenType;

class LexerMock: public MathTree::Lexer {
public:
  MOCK_METHOD(Token, next, (), ());
  MOCK_METHOD(void, reset, (), ());
};
using NiceLexerMock = NiceMock<LexerMock>;

class PrefixParseletMock: public MathTree::PrefixParselet {
public:
  MOCK_METHOD(std::unique_ptr<Expression>, parse, (PrattParser&, Token const&), ());
};
using NicePrefixParseletMock = NiceMock<PrefixParseletMock>;

class InfixParseletMock: public MathTree::InfixParselet {
public:
  MOCK_METHOD(std::unique_ptr<Expression>, 
                  parse, (PrattParser&, std::unique_ptr<Expression> left, Token const&), ());
  MOCK_METHOD(int, priority, (), (const));
};
using NiceInfixParseletMock = NiceMock<InfixParseletMock>;

class PrattParserTest: public ::testing::Test {
private:
  std::unique_ptr<NiceLexerMock> lexerMockPtr = std::make_unique<NiceLexerMock>();
  std::unique_ptr<NicePrefixParseletMock> prefixMockPtr = std::make_unique<NicePrefixParseletMock>();
  std::unique_ptr<NiceInfixParseletMock> infixMockPtr = std::make_unique<NiceInfixParseletMock>();

protected:
  NiceLexerMock& lexerMock{*lexerMockPtr};
  NicePrefixParseletMock& prefixMock{*prefixMockPtr};
  NiceInfixParseletMock& infixMock{*infixMockPtr};

  PrattParser parser{std::move(lexerMockPtr)};
  Token tokenNum{TokenType::NUMBER, "1"};
  Token tokenPlus{TokenType::PLUS, "+"};

  void SetUp() override {
    ON_CALL(lexerMock, next()).WillByDefault(
                                     Return(Token(TokenType::STOP, "")));
    ON_CALL(infixMock, priority()).WillByDefault(Return(1));
    parser.setPrefixParselet(TokenType::NUMBER, std::move(prefixMockPtr));
    parser.setInfixParselet(TokenType::PLUS, std::move(infixMockPtr));
  }
};

TEST_F(PrattParserTest, obtainsTokensFromTheGivenLexerDuringParsing) {
  EXPECT_CALL(lexerMock, next()).WillOnce(Return(tokenNum))
                                .WillRepeatedly(DoDefault()); 
  parser.parse();
}

TEST_F(PrattParserTest, callsTheCorrespondingPrefixParseletWithItselfAndTheCurrentToken) {  
  EXPECT_CALL(lexerMock, next()).WillOnce(Return(tokenNum))
                                .WillRepeatedly(DoDefault());
  EXPECT_CALL(prefixMock, parse(Ref(parser), Const(tokenNum)));
  parser.parse();
}

TEST_F(PrattParserTest, callsInfixParseletWithThePrefixBeforeItAsItsArgument) {
  auto numberExprPtr = std::make_unique<MathTree::RealNumberExpression>("1");
  auto& numberExpr = *numberExprPtr;

  EXPECT_CALL(lexerMock, next()).WillOnce(Return(tokenNum))
                                .WillOnce(Return(tokenPlus))
                                .WillOnce(Return(tokenNum))
                                .WillRepeatedly(DoDefault()); // 1+1 and then stop
  EXPECT_CALL(prefixMock, parse(_, _)).WillRepeatedly(
                                                    Return(
                                                      ByMove(std::move(numberExprPtr))));
  EXPECT_CALL(infixMock, parse(Ref(parser),
                               Pointee(Ref(numberExpr)),
                               Const(tokenPlus)));
  parser.parse();
}

TEST_F(PrattParserTest, theLexerIsResetAfterParsingIsCompleted) {
  EXPECT_CALL(lexerMock, next).WillOnce(Return(tokenNum))
                              .WillRepeatedly(DoDefault());
  EXPECT_CALL(lexerMock, reset);
  parser.parse();
}
