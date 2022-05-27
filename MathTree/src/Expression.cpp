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
#include "Utils.hpp"
#include <vector>

namespace MathTree {
// temporary dirty hack
std::unordered_map<char, int> operatorsPriority{{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}};

BinaryExpression::BinaryExpression(std::unique_ptr<Expression> left,
                                   TokenType tokenType,
                                   std::unique_ptr<Expression> right):
                                      m_left(std::move(left)), m_right(std::move(right)), m_tokenType(tokenType) {}

void BinaryExpression::print(std::ostream& stream) const {
  stream << '(';
  left().print(stream);
  stream << ' ' << symbolise(m_tokenType) << ' ';
  right().print(stream);
  stream << ')';
}

Expression const& BinaryExpression::left() const {
  return *m_left;
}
Expression const& BinaryExpression::right() const {
  return *m_right;
}

Validator::IndexErrorPairs Validator::validate(std::string_view input) {
  if (input.size() <= 0) {
    return IndexErrorPairs{};
  }

  IndexErrorPairs idxErrorPairs;
  std::vector<size_t> openBracketsIdx;
  int lastNonSpaceIdx = -1;
  for (size_t i = 0; i < input.size(); ++i) {
    if (input[i] == '(') {
      openBracketsIdx.push_back(i);
      if (lastNonSpaceIdx > -1 && std::isdigit(input[lastNonSpaceIdx])) {
        idxErrorPairs.emplace_back(i, Errors::MissingOperator);
      }
    } else if (input[i] == ')') {
      if (openBracketsIdx.size() > 0) {
        openBracketsIdx.pop_back();
      } else {
        idxErrorPairs.emplace_back(i, Errors::UnpairedClosingBracket);
      }
      if (lastNonSpaceIdx > -1 && operatorsPriority.count(input[lastNonSpaceIdx])) {
        idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
      }
    } else if (operatorsPriority.count(input[i]) &&
                lastNonSpaceIdx > -1 && operatorsPriority.count(input[lastNonSpaceIdx])) {
      idxErrorPairs.emplace_back(i, Errors::TwoOperatorsInARow);
    } else if (std::isdigit(input[i]) && lastNonSpaceIdx > -1 && input[lastNonSpaceIdx] == ')') {
      idxErrorPairs.emplace_back(i, Errors::MissingOperator);
    }

    if (!std::isspace(input[i])) {
      lastNonSpaceIdx = static_cast<int>(i);
    }
  }

  if (lastNonSpaceIdx > -1 && operatorsPriority.count(input[lastNonSpaceIdx])) {
    idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
  }

  for (size_t i = 0; i < openBracketsIdx.size(); ++i) {
    idxErrorPairs.emplace_back(openBracketsIdx[i], Errors::UnpairedOpeningBracket);
  }

  return idxErrorPairs;
}

NegativeSignExpression::NegativeSignExpression(TokenType operatorToken, std::unique_ptr<Expression> right) {
  m_operator = operatorToken;
  m_right = std::move(right);
}

void NegativeSignExpression::print(std::ostream& stream) const {
  stream << "(" << symbolise(m_operator);
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
  return std::pow(left().evaluate(), right().evaluate());
}

}