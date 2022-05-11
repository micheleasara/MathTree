#include "Expression.hpp"
#include "Operations.hpp"

namespace MathTree {

double Addition::evaluate() const {
    return left().evaluate() + right().evaluate();
}

double Subtraction::evaluate() const {
    return left().evaluate() - right().evaluate();
}

double Multiplication::evaluate() const {
    return left().evaluate() * right().evaluate();
}

double Division::evaluate() const {
    return left().evaluate() / right().evaluate();
}

}