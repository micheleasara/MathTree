#include "Token.hpp"

namespace MathTree {

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