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
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::PLUS);
}

TEST_F(ArithmeticLexerTest, canTokeniseANegativeSign) {
  lexer.reset("-1");
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::MINUS);
}

TEST_F(ArithmeticLexerTest, canTokeniseAnUnsignedNumber) {
  lexer.reset("100.2");
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::NUMBER);
  EXPECT_EQ(token.text(), "100.2");
}

TEST_F(ArithmeticLexerTest, canTokeniseAnOpeningBracket) {
  lexer.reset("(1+1)");
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::OPENING_BRACKET);
}

TEST_F(ArithmeticLexerTest, canTokeniseAClosingBracket) {
  lexer.reset(")");
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::CLOSING_BRACKET);
}

TEST_F(ArithmeticLexerTest, ignoresWhiteSpaces) {
  lexer.reset("    \n");
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::STOP);
}

TEST_F(ArithmeticLexerTest, returnsASentinelValueWhenThereIsNoMoreInputThatNeedsToBeTokenised) {
  auto token = lexer.next();
  EXPECT_EQ(token.type(), TokenType::STOP);
}

TEST_F(ArithmeticLexerTest, throwsWhenACharacterIsNotRecognised) {
  lexer.reset("\1");
  EXPECT_ANY_THROW(lexer.next());
}