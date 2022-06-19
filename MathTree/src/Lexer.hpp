#ifndef MATHTREE_LEXER
#define MATHTREE_LEXER

#include <memory>
#include <string>
#include "Token.hpp"
#include "TokenMatchers.hpp"
#include <unordered_map>
#include <vector>

namespace MathTree {

class Lexer {
public:
  virtual Token next() = 0;
  virtual void reset() = 0;
  virtual void reset(std::string) = 0;

  Lexer& operator=(Lexer const&) = delete;
  Lexer& operator=(Lexer&&) = delete;
  virtual ~Lexer() = default;
};

class ArithmeticLexer: public Lexer {
public:
  ArithmeticLexer();
  ArithmeticLexer(std::string text);
  Token next() override;
  void reset(std::string newText) override;
  void reset() override;

private:
  std::string m_text;
  size_t m_currentIndex = 0;
  std::vector<std::unique_ptr<TokenMatcher>> m_matchers;
};

}

#endif // MATHTREE_LEXER