#ifndef MATHTREE_LEXER
#define MATHTREE_LEXER

#include <string>
#include "Token.hpp"
#include <unordered_map>

namespace MathTree {

class Lexer {
public:
  virtual Token next() = 0;
  
  Lexer const& operator=(Lexer const&) = delete;
  Lexer&& operator=(Lexer&&) = delete;
  virtual ~Lexer() = default;
};

class ArithmeticLexer: public Lexer {
public:
  ArithmeticLexer();
  ArithmeticLexer(std::string text);
  Token next() override;
  void reset(std::string newText);

private:
  std::unordered_map<char, TokenType> m_symbols;
  std::string m_text;
  size_t m_currentIndex = 0;
};

}

#endif // MATHTREE_LEXER