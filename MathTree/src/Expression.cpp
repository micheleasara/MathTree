#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include "Expression.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include "TokenMatchers.hpp"
#include "Utils.hpp"
#include <vector>

namespace MathTree {
std::ostream& operator<<(std::ostream& left, Expression const& right) {
  right.print(left);
  return left;
}

BinaryExpression::BinaryExpression(std::unique_ptr<Expression> left,
                                   TokenType tokenType,
                                   std::unique_ptr<Expression> right):
                                      m_left(std::move(left)), m_right(std::move(right)), m_tokenType(tokenType) {
  if (m_left == nullptr || m_right == nullptr) {
    throw std::logic_error("A binary expression cannot be constructed if one or more of its subexpressions are null.");
  }
}

void BinaryExpression::print(std::ostream& stream) const {
  stream << '(' << left() << ' ';
  stream << symboliseTokenType(m_tokenType) << ' ';
  stream << right() << ')';
}

Expression const& BinaryExpression::left() const {
  return *m_left;
}
Expression const& BinaryExpression::right() const {
  return *m_right;
}

std::vector<Expression const*> BinaryExpression::subexpressions() const {
  return {m_left.get(), m_right.get()};
}

NegativeSignExpression::NegativeSignExpression(TokenType operatorToken, std::unique_ptr<Expression> right) {
  m_operator = operatorToken;
  m_right = std::move(right);
}

void NegativeSignExpression::print(std::ostream& stream) const {
  stream << "(" << symboliseTokenType(m_operator);
  stream << *m_right << ")";
}

double NegativeSignExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  m_cache = -(m_right->evaluate());
  return *m_cache;
}

std::vector<Expression const*> NegativeSignExpression::subexpressions() const {
  return {m_right.get()};
}

RealNumberExpression::RealNumberExpression(std::string_view num) {
  auto numberOpt = Utils::parseDouble(num);
  if (!numberOpt.has_value() || std::isinf(*numberOpt) || std::isnan(*numberOpt)) {
    throw std::logic_error("Failed to parse \"" + std::string(num) + "\" as a number.");
  }
  m_value = *numberOpt;
}

std::vector<Expression const*> RealNumberExpression::subexpressions() const {
  return {};
}

void RealNumberExpression::print(std::ostream& stream) const {
  stream << m_value;
}

double RealNumberExpression::evaluate() const {
  return m_value;
}

double AdditionExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  m_cache = left().evaluate() + right().evaluate();
  return *m_cache;
}

double SubtractionExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  m_cache = left().evaluate() - right().evaluate();
  return *m_cache;
}

double MultiplicationExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  m_cache = left().evaluate() * right().evaluate();
  return *m_cache;
}

double DivisionExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  auto divisor = right().evaluate();
  if (divisor == 0.0) {
    throw std::domain_error("Detected a divide-by-zero operation.");
  }
  m_cache = left().evaluate() / divisor;
  return *m_cache;
}

double ExponentiationExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  auto leftEval = left().evaluate();
  auto rightEval = right().evaluate();
  if ((leftEval == 0.0 && rightEval <= 0.0) ||
      (leftEval < 0.0 && std::floor(rightEval) != std::ceil(rightEval))) {
    // 0^0 is not defined for real numbers
    // 0^a with a < 0 implies 1/0, which is not a real number
    // a^b with a < 0 is only a real number when b is an integer
    throw std::domain_error("Cannot compute " + std::to_string(leftEval) +
                           " to the power of " + std::to_string(rightEval) + ".");
  }
  m_cache = std::pow(leftEval, rightEval);
  return *m_cache;
}

SquareRootExpression::SquareRootExpression(std::unique_ptr<Expression> innerExpression, 
                                          TokenType tokenType):
                                            m_innerExpression(std::move(innerExpression)),
                                            m_tokenType(tokenType) {}

double SquareRootExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  auto innerResult = m_innerExpression->evaluate();
  if (innerResult < 0 || std::isinf(innerResult) || std::isnan(innerResult)) {
    throw std::domain_error("Cannot compute the square root of " + std::to_string(innerResult) + ".");
  }
  m_cache = std::sqrt(innerResult);
  return *m_cache;
}

void SquareRootExpression::print(std::ostream& stream) const {
  stream << symboliseTokenType(m_tokenType) << "(";
  stream << *m_innerExpression << ")";
}

std::vector<Expression const*> SquareRootExpression::subexpressions() const {
  return {m_innerExpression.get()};
}

LogarithmExpression::LogarithmExpression(std::unique_ptr<Expression> innerExpression,
                                         double base,
                                         TokenType tokenType):
                                            m_innerExpression(std::move(innerExpression)),
                                            m_base(base),
                                            m_tokenType(tokenType) {
  if (m_innerExpression == nullptr) {
    throw std::logic_error("Cannot instantiate a LogarithmExpression with an empty inner expression.");
  }

  if (std::isnan(base) || std::isinf(base) || base <= 0) {
    throw std::domain_error("Cannot have a logarithm with base " + std::to_string(base) +
                            ". The base must be a finite, positive number.");
  }
}

double LogarithmExpression::evaluate() const {
  if (m_cache.has_value()) {
    return *m_cache;
  }

  auto inner = m_innerExpression->evaluate();
  if (inner <= 0) {
    throw std::domain_error("Cannot compute the logarithm of " + std::to_string(inner) +
                            ". The argument must be a positive number.");
  }
  m_cache = std::log2(inner) / std::log2(m_base);
  return *m_cache;
}

void LogarithmExpression::print(std::ostream& stream) const {
  stream << symboliseTokenType(m_tokenType);
  stream << delimeterFor(TokenType::Log) << m_base << "(";
  stream << *m_innerExpression << ")";
}

std::vector<Expression const*> LogarithmExpression::subexpressions() const {
  return {m_innerExpression.get()};
}

}