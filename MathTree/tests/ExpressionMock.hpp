#ifndef MATHTREE_EXPRESSIONMOCK
#define MATHTREE_EXPRESSIONMOCK
#include <gmock/gmock.h>
#include "Expression.hpp"

class ExpressionMock: public MathTree::Expression {
  public:
  MOCK_METHOD(double, evaluate, (), (const, override));
  MOCK_METHOD(void, print, (std::ostream&), (const, override));

};
using NiceExpressionMock = ::testing::NiceMock<ExpressionMock>;

#endif // MATHTREE_EXPRESSIONMOCK