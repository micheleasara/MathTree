#ifndef MATHTREE_TOKEN
#define MATHTREE_TOKEN
#include <string>

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
  
std::string symbolise(TokenType type);

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

}
#endif // MATHTREE_TOKEN