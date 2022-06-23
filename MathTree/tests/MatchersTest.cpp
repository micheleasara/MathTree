#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TokenMatchers.hpp"

using ::testing::Property;
using ::testing::Optional;
using ::testing::Eq;
using MathTree::LogarithmMatcher;
using MathTree::TokenType;
using MathTree::SymbolMatcher;
using MathTree::UnsignedNumberMatcher;


TEST(MatchersTest, symbolMatcherReturnsAnEmptyOptionalWhenTheIndexIsOutOfBounds) {
  SymbolMatcher matcher({TokenType::Plus});
  EXPECT_EQ(matcher.match("", 0), std::nullopt);
}

TEST(MatchersTest, symbolMatcherMatchesOperatorsUsedForItsInitialisation) {
  SymbolMatcher matcher({TokenType::Plus});
  EXPECT_THAT(matcher.match("1+1", 1), 
              Optional(Property(&MathTree::Token::text, "+")));
}

TEST(MatchersTest, symbolMatcherDoesNotMatchOperatorsNotUsedForItsInitialisation) {
  SymbolMatcher matcher({TokenType::Plus});
  EXPECT_EQ(matcher.match("1*1", 1), std::nullopt);
}

TEST(MatchersTest, unsignedNumberMatcherReturnsAnEmptyOptionalWhenTheIndexIsOutOfBounds) {
  UnsignedNumberMatcher matcher;
  EXPECT_EQ(matcher.match("", 1), std::nullopt);
}

TEST(MatchersTest, unsignedNumberMatcherMatchesFirstInteger) {
  UnsignedNumberMatcher matcher;
  EXPECT_THAT(matcher.match("100+3", 0), 
              Optional(Property(&MathTree::Token::text, Eq("100"))));
}

TEST(MatchersTest, unsignedNumberMatcherMatchesFirstFloatingPointNumber) {
  UnsignedNumberMatcher matcher;
  EXPECT_THAT(matcher.match("100.39+0.1", 0), 
              Optional(Property(&MathTree::Token::text, Eq("100.39"))));
}

TEST(MatchersTest, unsignedNumberMatcherDoesNotMatchSignedNumbers) {
  UnsignedNumberMatcher matcher;
  EXPECT_EQ(matcher.match("+100", 0), std::nullopt);
  EXPECT_EQ(matcher.match("-100", 0), std::nullopt);
}

TEST(MatchersTest, logMatcherMatchesLogWithImpliedBase10) {
  LogarithmMatcher matcher;
  EXPECT_NE(matcher.match("log100", 0), std::nullopt);
  EXPECT_NE(matcher.match("log(100)", 0), std::nullopt);
}