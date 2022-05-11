#ifndef MATHTREE_OPERATIONS_H
#define MATHTREE_OPERATIONS_H

#include "Expression.hpp"
#include <memory>

namespace MathTree {

class Addition: public BinaryExpression {
    public:
    using BinaryExpression::BinaryExpression;
    double evaluate() const override;
};

class Subtraction: public BinaryExpression {
    public:
    using BinaryExpression::BinaryExpression;
    double evaluate() const override;
};

class Multiplication: public BinaryExpression {
    public:
    using BinaryExpression::BinaryExpression;
    double evaluate() const override;
};

class Division: public BinaryExpression {
    public:
    using BinaryExpression::BinaryExpression;
    double evaluate() const override;
};

}

#endif // MATHTREE_OPERATIONS_H