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
auto constexpr operatorsList = {TokenType::PLUS, TokenType::MINUS,
                                    TokenType::SLASH, TokenType::ASTERISK,
                                    TokenType::CARET};

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

  SymbolMatcher operatorMatcher(operatorsList);
  UnsignedNumberMatcher numberMatcher;
  IndexErrorPairs idxErrorPairs;
  std::vector<size_t> openBracketsIdx;
  int lastNonSpaceIdx = -1;
  auto wasNumber = false;
  auto wasOperator = false;
  for (size_t i = 0; i < input.size(); ++i) {
    if (std::isspace(input[i])) {
      continue;
    }

    auto increment = 0;
    std::optional<Token> operatorOpt, numberOpt;
    if (operatorOpt = operatorMatcher.match(input, i)) {
      increment += operatorOpt->text().size() - 1;
    } else if (numberOpt = numberMatcher.match(input, i)) {
      increment += numberOpt->text().size() - 1;
    }

    if (input[i] == '(') {
      openBracketsIdx.push_back(i);
      if (wasNumber) {
        idxErrorPairs.emplace_back(i, Errors::MissingOperator);
      }
    } else if (input[i] == ')') {
      if (openBracketsIdx.size() > 0) {
        openBracketsIdx.pop_back();
      } else {
        idxErrorPairs.emplace_back(i, Errors::UnpairedClosingBracket);
      }
      if (wasOperator) {
        idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
      }
    } else if (operatorOpt && wasOperator) {
      idxErrorPairs.emplace_back(i, Errors::TwoOperatorsInARow);
    } else if (numberOpt && (wasNumber || (lastNonSpaceIdx > -1 && input[lastNonSpaceIdx] == ')'))) {
      idxErrorPairs.emplace_back(i, Errors::MissingOperator);
    } else if (!operatorOpt && !numberOpt) {
      idxErrorPairs.emplace_back(i, Errors::UnrecognisedSymbol);
    }

    lastNonSpaceIdx = static_cast<int>(i);
    wasNumber = numberOpt.has_value();
    wasOperator = operatorOpt.has_value();
    i += increment;
  }

  if (wasOperator) {
    idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
  }

  for (auto const& idx: openBracketsIdx) {
    idxErrorPairs.emplace_back(idx, Errors::UnpairedOpeningBracket);
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