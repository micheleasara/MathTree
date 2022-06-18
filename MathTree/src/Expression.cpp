#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include "Expression.hpp"
#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <string_view>
#include "TokenMatchers.hpp"
#include "Utils.hpp"
#include <vector>

namespace MathTree {
BinaryExpression::BinaryExpression(std::unique_ptr<Expression> left,
                                   TokenType tokenType,
                                   std::unique_ptr<Expression> right):
                                      m_left(std::move(left)), m_right(std::move(right)), m_tokenType(tokenType) {}

void BinaryExpression::print(std::ostream& stream) const {
  stream << '(';
  left().print(stream);
  stream << ' ' << symboliseTokenType(m_tokenType) << ' ';
  right().print(stream);
  stream << ')';
}

Expression const& BinaryExpression::left() const {
  return *m_left;
}
Expression const& BinaryExpression::right() const {
  return *m_right;
}

NegativeSignExpression::NegativeSignExpression(TokenType operatorToken, std::unique_ptr<Expression> right) {
  m_operator = operatorToken;
  m_right = std::move(right);
}

void NegativeSignExpression::print(std::ostream& stream) const {
  stream << "(" << symboliseTokenType(m_operator);
  m_right->print(stream);
  stream << ")";
}

double NegativeSignExpression::evaluate() const {
  return -(m_right->evaluate());
}

RealNumberExpression::RealNumberExpression(std::string_view num) {
  auto numberOpt = Utils::parseDouble(num);
  if (!numberOpt.has_value() || std::isinf(*numberOpt) || std::isnan(*numberOpt)) {
    throw std::logic_error("Failed to parse \"" + std::string(num) + "\" as a number.");
  }
  m_value = *numberOpt;
}

void RealNumberExpression::print(std::ostream& stream) const {
  stream << m_value;
}

double RealNumberExpression::evaluate() const {
  return m_value;
}

double AdditionExpression::evaluate() const {
  return left().evaluate() + right().evaluate();
}

double SubtractionExpression::evaluate() const {
  return left().evaluate() - right().evaluate();
}

double MultiplicationExpression::evaluate() const {
  return left().evaluate() * right().evaluate();
}

double DivisionExpression::evaluate() const {
  auto divisor = right().evaluate();
  if (divisor == 0.0) {
    throw std::domain_error("Detected a divide-by-zero operation.");
  }
  return left().evaluate() / divisor;
}

double ExponentiationExpression::evaluate() const {
  auto leftEval = left().evaluate();
  auto rightEval = right().evaluate();
  if ((leftEval == 0.0 && rightEval <= 0.0) ||
      (leftEval < 0.0 && std::floor(rightEval) != std::ceil(rightEval))) {
    // 0^0 is not defined for real numbers
    // 0^a with a < 0 implies 1/0, which is not a real number
    // a^b with a < 0 is only a real number when b is an integer
    throw std::logic_error("Cannot compute " + std::to_string(leftEval) +
                           " to the power of " + std::to_string(rightEval) + ".");
  }
  return std::pow(leftEval, rightEval);
}

SquareRootExpression::SquareRootExpression(std::unique_ptr<Expression> innerExpression, 
                                          TokenType tokenType):
                                            m_innerExpression(std::move(innerExpression)),
                                            m_tokenType(tokenType) {}

double SquareRootExpression::evaluate() const {
  auto innerResult = m_innerExpression->evaluate();
  if (innerResult < 0 || std::isinf(innerResult) || std::isnan(innerResult)) {
    throw std::domain_error("Cannot compute the square root of " + std::to_string(innerResult) + ".");
  }
  return std::sqrt(innerResult);
}

void SquareRootExpression::print(std::ostream& stream) const {
  stream << symboliseTokenType(m_tokenType) << "(";
  m_innerExpression->print(stream);
  stream << ")";
}

}