#include "Expression.hpp"
#include "Parser.hpp"
#include "PrefixParselets.hpp"
#include <stdexcept>

namespace MathTree {

std::unique_ptr<Expression> NumberParselet::parse(Parser&, Token const& token) {
  return std::make_unique<RealNumberExpression>(token.text());
}

std::unique_ptr<Expression> GroupParselet::parse(Parser& parser, Token const&) {
  auto expression = parser.parse();
  auto nextToken = parser.consumeCurrentToken();
  if (nextToken.type() != TokenType::CLOSING_BRACKET) {
    throw std::logic_error("Expected a closing bracket during parsing.");
  }
  return expression;
}

NegativeSignParselet::NegativeSignParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> NegativeSignParselet::parse(Parser& parser, Token const& token) {
  return std::make_unique<NegativeSignExpression>(token.type(), parser.parse(m_priority));
}

PositiveSignParselet::PositiveSignParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> PositiveSignParselet::parse(Parser& parser, Token const&) {
  return parser.parse(m_priority);
}

}