#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Operations.hpp"

using MathTree::Expression;
using MathTree::Addition;
using MathTree::Division;
using MathTree::Multiplication;
using ::testing::Return;
using ::testing::NiceMock;

class ExpressionMock: public Expression {
    public:
    MOCK_METHOD(double, evaluate, (), (const, override));
};
using NiceExpressionMock = NiceMock<ExpressionMock>;

class OperationsTest: public ::testing::Test {
    protected:
    std::shared_ptr<NiceExpressionMock> mock = std::make_shared<NiceExpressionMock>();
};

TEST_F(OperationsTest, EvaluatingAnAdditionReturnsTheSumOfTheValuesOfBothAddends) {
    EXPECT_CALL(*mock, evaluate())
            .WillOnce(Return(5))
            .WillOnce(Return(4.1));

    Addition adder{mock, std::move(mock)};
    EXPECT_DOUBLE_EQ(adder.evaluate(), 9.1);
}

TEST_F(OperationsTest, EvaluatingAMultiplicationReturnsTheProductOfTheValuesOfBothFactors) {
    EXPECT_CALL(*mock, evaluate())
                .WillOnce(Return(1.5))
                .WillOnce(Return(4));

    Multiplication multiplier{mock, std::move(mock)};
    EXPECT_DOUBLE_EQ(multiplier.evaluate(), 6);
}

TEST_F(OperationsTest, EvaluatingADivisionReturnsTheQuotientOfTheValuesOfTheDividendAndDivisor) {
    EXPECT_CALL(*mock, evaluate())
                .WillOnce(Return(0.6))
                .WillOnce(Return(2));

    Division divider{mock, std::move(mock)};
    EXPECT_DOUBLE_EQ(divider.evaluate(), 0.3);
}