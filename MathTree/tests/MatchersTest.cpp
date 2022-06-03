#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TokenMatchers.hpp"

using ::testing::Property;
using ::testing::Optional;
using ::testing::Eq;
using MathTree::TokenType;
using MathTree::SymbolMatcher;
using MathTree::UnsignedNumberMatcher;


TEST(MatchersTest, SymbolMatcherReturnsAnEmptyOptionalWhenTheIndexIsOutOfBounds) {
  SymbolMatcher matcher({TokenType::PLUS});
  EXPECT_EQ(matcher.match("", 0), std::nullopt);
}

TEST(MatchersTest, SymbolMatcherMatchesOperatorsUsedForItsInitialisation) {
  SymbolMatcher matcher({TokenType::PLUS});
  EXPECT_THAT(matcher.match("1+1", 1), 
              Optional(Property(&MathTree::Token::text, "+")));
}

TEST(MatchersTest, SymbolMatcherDoesNotMatchOperatorsNotUsedForItsInitialisation) {
  SymbolMatcher matcher({TokenType::PLUS});
  EXPECT_EQ(matcher.match("1*1", 1), std::nullopt);
}

TEST(MatchersTest, UnsignedNumberMatcherReturnsAnEmptyOptionalWhenTheIndexIsOutOfBounds) {
  UnsignedNumberMatcher matcher;
  EXPECT_EQ(matcher.match("", 1), std::nullopt);
}

TEST(MatchersTest, UnsignedNumberMatcherMatchesFirstInteger) {
  UnsignedNumberMatcher matcher;
  EXPECT_THAT(matcher.match("100+3", 0), 
              Optional(Property(&MathTree::Token::text, Eq("100"))));
}

TEST(MatchersTest, UnsignedNumberMatcherMatchesFirstFloatingPointNumber) {
  UnsignedNumberMatcher matcher;
  EXPECT_THAT(matcher.match("100.39+0.1", 0), 
              Optional(Property(&MathTree::Token::text, Eq("100.39"))));
}

TEST(MatchersTest, UnsignedNumberMatcherDoesNotMatchSignedNumbers) {
  UnsignedNumberMatcher matcher;
  EXPECT_EQ(matcher.match("+100", 0), std::nullopt);
  EXPECT_EQ(matcher.match("-100", 0), std::nullopt);
}