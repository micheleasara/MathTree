#include "Token.hpp"
#include <stdexcept>

namespace MathTree {

char symbolise(TokenType type) {
  switch (type) {
  case TokenType::OPENING_BRACKET:
    return '(';
  case TokenType::CLOSING_BRACKET:
    return ')';
  case TokenType::PLUS:
    return '+';
  case TokenType::MINUS:
    return '-';
  case TokenType::ASTERISK:
    return '*';
  case TokenType::SLASH:
    return '/';
  case TokenType::CARET:
    return '^';
  default:
    throw std::logic_error("Token without associated symbol.");
  }
}

Token::Token(TokenType type, std::string text): m_type(type), m_text(std::move(text)) {}

TokenType Token::type() const {
  return m_type;
}

std::string const& Token::text() const {
  return m_text;
}

bool Token::operator==(Token const& other) const {
  return other.m_text == m_text && other.m_type == m_type;
}

bool Token::operator!=(Token const& other) const {
  return !operator==(other);
}
}