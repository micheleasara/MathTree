#include <cmath>
#include "Numbers.hpp"
#include <gtest/gtest.h>

using MathTree::RealNumber;

TEST(NumbersTest, EvaluatingReturnsTheNumberItself) {
    RealNumber number{42};
    EXPECT_EQ(number.evaluate(), 42);
}

TEST(NumbersTest, CreatingARealNumberWithInfinityThrows) {
    EXPECT_ANY_THROW(RealNumber{INFINITY});
}

TEST(NumbersTest, CreatingARealNumberWithNaNThrows) {
    EXPECT_ANY_THROW(RealNumber{NAN});
}

TEST(NumbersTest, ReturnsNullWhenParsingFails) {
    EXPECT_EQ(RealNumber::parse(""), nullptr);
    EXPECT_EQ(RealNumber::parse("a"), nullptr);
    EXPECT_EQ(RealNumber::parse("+"), nullptr);
}

TEST(NumbersTest, CanParseAPositiveIntegerWithImplicitSign) {
    auto parsed = RealNumber::parse("42");
    ASSERT_NE(parsed, nullptr);
    EXPECT_EQ(parsed->evaluate(), 42);
}

TEST(NumbersTest, CanParseAPositiveIntegerWithExplicitSign) {
    auto parsed = RealNumber::parse("+42");
    ASSERT_NE(parsed, nullptr);
    EXPECT_EQ(parsed->evaluate(), 42);
}

TEST(NumbersTest, CanParseANegativeInteger) {
    auto parsed = RealNumber::parse("-42");
    ASSERT_NE(parsed, nullptr);
    EXPECT_EQ(parsed->evaluate(), -42);
}

TEST(NumbersTest, CanParseFloatingPointNumbers) {
    auto parsed = RealNumber::parse("42.78");
    ASSERT_NE(parsed, nullptr);
    EXPECT_EQ(parsed->evaluate(), 42.78);
}