#include "Expression.hpp"
#include "Parser.hpp"
#include "PrefixParselets.hpp"
#include <stdexcept>
#include "Utils.hpp"

namespace MathTree {

std::unique_ptr<Expression> NumberParselet::parse(AbstractPrattParser&, Token const& token) {
  return std::make_unique<RealNumberExpression>(token.text());
}

std::unique_ptr<Expression> GroupParselet::parse(AbstractPrattParser& parser, Token const&) {
  auto expression = parser.parse();
  auto nextToken = parser.consumeCurrentToken();
  if (nextToken.type() != TokenType::ClosingBracket) {
    throw std::logic_error("Expected a closing bracket during parsing.");
  }
  return expression;
}

SquareRootParselet::SquareRootParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> SquareRootParselet::parse(AbstractPrattParser& parser, Token const& token) {
  return std::make_unique<SquareRootExpression>(parser.parse(m_priority), token.type());
}

NegativeSignParselet::NegativeSignParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> NegativeSignParselet::parse(AbstractPrattParser& parser, Token const& token) {
  return std::make_unique<NegativeSignExpression>(token.type(), parser.parse(m_priority));
}

PositiveSignParselet::PositiveSignParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> PositiveSignParselet::parse(AbstractPrattParser& parser, Token const&) {
  return parser.parse(m_priority);
}

LogarithmParselet::LogarithmParselet(int priority): m_priority(priority) {}

std::unique_ptr<Expression> LogarithmParselet::parse(AbstractPrattParser& parser, Token const& token) {
  double base = 10.0;
  auto const logSymbol = symboliseTokenType(token.type());
  if (token.text().size() > logSymbol.size()) {
    auto const baseStr = token.text().substr(logSymbol.size() +
                                             delimeterFor(TokenType::Log).size());
    if (auto baseOpt = Utils::parseFirstDouble(baseStr)) {
      base = *baseOpt;
    }
  }
  return std::make_unique<LogarithmExpression>(parser.parse(m_priority), base, token.type());
}

}