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
  STOP
};
  
char symbolise(TokenType type);

class Token {
public:
  Token(TokenType type, std::string text);

  TokenType type() const;
  std::string const& text() const;
  
private:
  TokenType m_type;
  std::string m_text;
};

}
#endif // MATHTREE_TOKEN