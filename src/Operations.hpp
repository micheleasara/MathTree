#ifndef MATHTREE_OPERATIONS_H
#define MATHTREE_OPERATIONS_H

#include "Expression.hpp"
#include <memory>

namespace MathTree {

class Addition: public BinaryExpression {
    public:
    Addition(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    double evaluate() const override;
};

class Subtraction: public BinaryExpression {
    public:
    Subtraction(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    double evaluate() const override;
};

class Multiplication: public BinaryExpression {
    public:
    Multiplication(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    double evaluate() const override;
};

class Division: public BinaryExpression {
    public:
    Division(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
    double evaluate() const override;
};

}

#endif // MATHTREE_OPERATIONS_H