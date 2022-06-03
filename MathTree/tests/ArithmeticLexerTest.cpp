#include "Lexer.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Property;
using ::testing::Eq;
using MathTree::ArithmeticLexer;
using MathTree::Token;
using MathTree::TokenType;

class ArithmeticLexerTest: public ::testing::Test {
protected:
  ArithmeticLexer lexer;
};

TEST_F(ArithmeticLexerTest, resettingChangesTheInternalState) {
  auto original = lexer.next();
  lexer.reset("+1");
  EXPECT_NE(original, lexer.next());
}

TEST_F(ArithmeticLexerTest, canTokeniseAPositiveSign) {
  lexer.reset("+1");
  EXPECT_THAT(lexer.next(),
              Property(&Token::type, Eq(TokenType::PLUS)));
}

TEST_F(ArithmeticLexerTest, canTokeniseANegativeSign) {
  lexer.reset("-1");
  EXPECT_THAT(lexer.next(),
              Property(&Token::type, Eq(TokenType::MINUS)));
}

TEST_F(ArithmeticLexerTest, canTokeniseAnUnsignedNumber) {
  lexer.reset("100.2");
  auto token = lexer.next();
  EXPECT_THAT(token,
              Property(&Token::type, Eq(TokenType::NUMBER)));
  EXPECT_THAT(token,
              Property(&Token::text, Eq("100.2")));
}

TEST_F(ArithmeticLexerTest, canTokeniseAnOpeningBracket) {
  lexer.reset("(1+1)");
  EXPECT_THAT(lexer.next(),
              Property(&Token::type, Eq(TokenType::OPENING_BRACKET)));
}

TEST_F(ArithmeticLexerTest, canTokeniseAClosingBracket) {
  lexer.reset(")");
  EXPECT_THAT(lexer.next(),
              Property(&Token::type, Eq(TokenType::CLOSING_BRACKET)));
}

TEST_F(ArithmeticLexerTest, returnsASentinelValueWhenThereIsNoMoreInputThatNeedsToBeTokenised) {
  EXPECT_THAT(lexer.next(), Property(&Token::type, Eq(TokenType::STOP)));
}