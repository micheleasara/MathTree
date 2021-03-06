#include "InfixParselets.hpp"
#include <optional>
#include "Parser.hpp"

namespace MathTree {

BinaryOperatorParselet::BinaryOperatorParselet(int priority): m_priority(priority) {};

int BinaryOperatorParselet::priority() const {
  return m_priority;
}

std::unique_ptr<Expression> AdditionParselet::parse(AbstractPrattParser& parser,
                                                    std::unique_ptr<Expression> left,
                                                    Token const& token) {
  auto right = parser.parse(priority());
  return std::make_unique<AdditionExpression>(std::move(left), token.type(), std::move(right));
}

std::unique_ptr<Expression> SubtractionParselet::parse(AbstractPrattParser& parser,
                                                       std::unique_ptr<Expression> left,
                                                       Token const& token) {
  auto right = parser.parse(priority());
  return std::make_unique<SubtractionExpression>(std::move(left), token.type(), std::move(right));
}

std::unique_ptr<Expression> MultiplicationParselet::parse(AbstractPrattParser& parser,
                                                          std::unique_ptr<Expression> left,
                                                          Token const& token) {
  auto right = parser.parse(priority());
  return std::make_unique<MultiplicationExpression>(std::move(left), token.type(), std::move(right));
}

std::unique_ptr<Expression> DivisionParselet::parse(AbstractPrattParser& parser,
                                                    std::unique_ptr<Expression> left,
                                                    Token const& token) {
  auto right = parser.parse(priority());
  return std::make_unique<DivisionExpression>(std::move(left), token.type(), std::move(right));
}

std::unique_ptr<Expression> ExponentiationParselet::parse(AbstractPrattParser& parser,
                                                          std::unique_ptr<Expression> left,
                                                          Token const& token) {
  auto right = parser.parse(priority() - 1); // right associativity needs a lower priority
  return std::make_unique<ExponentiationExpression>(std::move(left), token.type(), std::move(right));
}

}