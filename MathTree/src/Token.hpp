#ifndef MATHTREE_TOKEN
#define MATHTREE_TOKEN
#include <stdexcept>
#include <string>
#include <string_view>

namespace MathTree {

enum class TokenType {
  OpeningBracket,
  ClosingBracket,
  Plus,
  Minus,
  Asterisk,
  Slash,
  Caret,
  Number,
  SquareRoot,
  Log,
  Stop
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
  case TokenType::OpeningBracket:
    return "(";
  case TokenType::ClosingBracket:
    return ")";
  case TokenType::Plus:
    return "+";
  case TokenType::Minus:
    return "-";
  case TokenType::Asterisk:
    return "*";
  case TokenType::Slash:
    return "/";
  case TokenType::Caret:
    return "^";
  case TokenType::SquareRoot:
    return "sqrt";
  case TokenType::Log:
    return "log";
  default:
    throw std::logic_error("Token without an associated symbol.");
  }
}

}
#endif // MATHTREE_TOKEN