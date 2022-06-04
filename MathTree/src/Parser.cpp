#include "Parser.hpp"
#include <stdexcept>

namespace MathTree {

PrattParser::PrattParser(std::unique_ptr<Lexer> lexer): m_lexer(std::move(lexer)) {}

std::unique_ptr<Expression> PrattParser::parse() {
  return parse(0);
}

std::unique_ptr<Expression> PrattParser::parse(int priority) {
  auto token = consumeCurrentToken();
  if (m_prefixParselets.count(token.type()) <= 0) {
    throw std::logic_error("Expected a prefix parselet while parsing.");
  }
  auto& prefix = *m_prefixParselets[token.type()];
  auto left = prefix.parse(*this, token);

  while (priority < currentTokenPriority()) {
    token = consumeCurrentToken();
    auto& infix = *m_infixParselets[token.type()];
    left = infix.parse(*this, std::move(left), token);
  }

  return left;
}

void PrattParser::setPrefixParselet(TokenType token, std::unique_ptr<PrefixParselet> parselet) {
  m_prefixParselets[token] = std::move(parselet);
}

void PrattParser::setInfixParselet(TokenType token, std::unique_ptr<InfixParselet> parselet) {
  m_infixParselets[token] = std::move(parselet);
}

Token PrattParser::consumeCurrentToken() {
  auto token = currentToken();
  m_currentToken = std::nullopt;
  return token;
}

int PrattParser::currentTokenPriority() {
  auto token = currentToken();
  if (m_infixParselets.count(token.type()) <= 0) {
    return 0;
  }
  InfixParselet& infix = *m_infixParselets[token.type()];
  return infix.priority();
}

Token const& PrattParser::currentToken() {
  if (!m_currentToken.has_value()) {
    m_currentToken = m_lexer->next();
  }
  return *m_currentToken;
}

}