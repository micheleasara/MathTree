#include "Expression.hpp"
#include "Operations.hpp"
#include <stdexcept>

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
    auto divisor = right().evaluate();
    if (divisor == 0.0) {
        throw std::domain_error("Detected a divide-by-zero operation.");
    }
    return left().evaluate() / divisor;
}

}