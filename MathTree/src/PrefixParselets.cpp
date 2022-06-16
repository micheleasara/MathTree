#include "Expression.hpp"
#include "Parser.hpp"
#include "PrefixParselets.hpp"
#include <stdexcept>

namespace MathTree {

std::unique_ptr<Expression> NumberParselet::parse(PrattParser&, Token const& token) {
  return std::make_unique<RealNumberExpression>(token.text());
}

std::unique_ptr<Expression> GroupParselet::parse(PrattParser& parser, Token const&) {
  auto expression = parser.parse();
  auto nextToken = parser.consumeCurrentToken();
  if (nextToken.type() != TokenType::ClosingBracket) {
    throw std::logic_error("Expected a closing bracket during parsing.");
  }
  return expression;
}

SquareRootParselet::SquareRootParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> SquareRootParselet::parse(PrattParser& parser, Token const& token) {
  auto expression = parser.parse(m_priority - 1); // right associativity requires lower priority
  return std::make_unique<SquareRootExpression>(std::move(expression), token.type());
}

NegativeSignParselet::NegativeSignParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> NegativeSignParselet::parse(PrattParser& parser, Token const& token) {
  return std::make_unique<NegativeSignExpression>(token.type(), parser.parse(m_priority));
}

PositiveSignParselet::PositiveSignParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> PositiveSignParselet::parse(PrattParser& parser, Token const&) {
  return parser.parse(m_priority);
}

}