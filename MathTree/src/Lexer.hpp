#ifndef MATHTREE_LEXER
#define MATHTREE_LEXER

#include <memory>
#include <string>
#include "Token.hpp"
#include "TokenMatchers.hpp"
#include <unordered_map>
#include <vector>

namespace MathTree {

/// Represents an entity which produces a stream of tokens from a string input.
class Lexer {
public:
  /// Returns the next token.
  virtual Token next() = 0;
  /// Resets the lexer to its initial state.
  virtual void reset() = 0;
  /// Resets the lexer to point to the beginning of the given string.
  virtual void reset(std::string) = 0;

  Lexer& operator=(Lexer const&) = delete;
  Lexer& operator=(Lexer&&) = delete;
  virtual ~Lexer() = default;
};

/// Represents a lexer which can tokenise an arithmetic expression in string form.
class ArithmeticLexer: public Lexer {
public:
  /// Constructs a lexer with an empty string.
  ArithmeticLexer();
  /// Constructs a lexer with the given string.
  ArithmeticLexer(std::string text);
  //! @copydoc Lexer::next()
  Token next() override;
  //! @copydoc Lexer::reset(std::string)
  void reset(std::string newText) override;
  //! @copydoc Lexer::reset()
  void reset() override;

private:
  std::string m_text;
  size_t m_currentIndex = 0;
  std::vector<std::unique_ptr<TokenMatcher>> m_matchers;
};

}

#endif // MATHTREE_LEXER