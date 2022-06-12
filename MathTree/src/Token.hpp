#ifndef MATHTREE_TOKEN
#define MATHTREE_TOKEN
#include <stdexcept>
#include <string>
#include <string_view>

namespace MathTree {

enum class TokenType {
  OPENING_BRACKET,
  CLOSING_BRACKET,
  COMMA,
  PLUS,
  MINUS,
  ASTERISK,
  SLASH,
  CARET,
  NUMBER,
  SQUARE_ROOT,
  STOP
};

class Token {
public:
  Token(TokenType type, std::string text);

  TokenType type() const;
  std::string const& text() const;

  bool operator==(Token const& other) const;
  bool operator!=(Token const& other) const;
  
private:
  TokenType m_type;
  std::string m_text;
};

constexpr std::string_view symboliseTokenType(TokenType type) {
  switch (type) {
  case TokenType::OPENING_BRACKET:
    return "(";
  case TokenType::CLOSING_BRACKET:
    return ")";
  case TokenType::PLUS:
    return "+";
  case TokenType::MINUS:
    return "-";
  case TokenType::ASTERISK:
    return "*";
  case TokenType::SLASH:
    return "/";
  case TokenType::CARET:
    return "^";
  case TokenType::SQUARE_ROOT:
    return "sqrt";
  default:
    throw std::logic_error("Token without an associated symbol.");
  }
}

}
#endif // MATHTREE_TOKEN