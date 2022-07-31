#ifndef MATHTREE_TOKEN
#define MATHTREE_TOKEN
#include <stdexcept>
#include <string>
#include <string_view>

namespace MathTree {

/// Represents the possible types of a MathTree token.
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

/// Represents the unit of information that is used to determine how to parse an expression.
class Token {
public:
  /// Constructs a token with the given type and text.
  Token(TokenType type, std::string text);
  /// Returns the type of this token.
  TokenType type() const;
  /// Returns the string corresponding to this token.
  std::string const& text() const;

  /// Returns true if the two tokens have the same type and text, false otherwise.
  bool operator==(Token const& other) const;
  /// The negation of operator==(Token const&).
  bool operator!=(Token const& other) const;
  
private:
  TokenType m_type;
  std::string m_text;
};

/// Returns the string representation of a token type. Throws if no string representation exists.
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
    throw std::logic_error("Token type without an associated symbol.");
  }
}

/// Returns the delimeter associated to a token type. Throws if no such delimeter exists.
constexpr std::string_view delimeterFor(TokenType type) {
  if (type == TokenType::Log) {
    return "_";
  }
  throw std::logic_error("Token type without an associated delimeter.");
}

}
#endif // MATHTREE_TOKEN