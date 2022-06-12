#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TokenMatchers.hpp"

using ::testing::Property;
using ::testing::Optional;
using ::testing::Eq;
using MathTree::TokenType;
using MathTree::SymbolMatcher;
using MathTree::UnsignedNumberMatcher;


TEST(MatchersTest, symbolMatcherReturnsAnEmptyOptionalWhenTheIndexIsOutOfBounds) {
  SymbolMatcher matcher({TokenType::PLUS});
  EXPECT_EQ(matcher.match("", 0), std::nullopt);
}

TEST(MatchersTest, symbolMatcherMatchesOperatorsUsedForItsInitialisation) {
  SymbolMatcher matcher({TokenType::PLUS});
  EXPECT_THAT(matcher.match("1+1", 1), 
              Optional(Property(&MathTree::Token::text, "+")));
}

TEST(MatchersTest, symbolMatcherDoesNotMatchOperatorsNotUsedForItsInitialisation) {
  SymbolMatcher matcher({TokenType::PLUS});
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