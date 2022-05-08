#include "Expression.hpp"
#include "Operations.hpp"

namespace MathTree {

Addition::Addition(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right): 
                                                BinaryExpression(std::move(left), std::move(right)) {}
double Addition::evaluate() const {
    return left().evaluate() + right().evaluate();
}

Subtraction::Subtraction(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right): 
                                                BinaryExpression(std::move(left), std::move(right)) {}
double Subtraction::evaluate() const {
    return left().evaluate() - right().evaluate();
}

Multiplication::Multiplication(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right): 
                                                BinaryExpression(std::move(left), std::move(right)) {}
double Multiplication::evaluate() const {
    return left().evaluate() * right().evaluate();
}

Division::Division(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right): 
                                                BinaryExpression(std::move(left), std::move(right)) {}
double Division::evaluate() const {
    return left().evaluate() / right().evaluate();
}

}