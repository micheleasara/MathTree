#ifndef MATHTREE_TOKENMATCHERS
#define MATHTREE_TOKENMATCHERS

#include <optional>
#include <string_view>
#include "Token.hpp"
#include <unordered_map>
#include <vector>

namespace MathTree {

/// Responsible for constructing a token from a string.
class TokenMatcher {
public:
  /**
   * Returns a token from the input and the starting index given.
   * Returns std::nullopt if no token can be constructed by this matcher.
  **/
  virtual std::optional<Token> match(std::string_view source, size_t startIdx) = 0;

  TokenMatcher& operator=(TokenMatcher const&) = delete;
  TokenMatcher& operator=(TokenMatcher&&) = delete;
  virtual ~TokenMatcher() = default;
};

/// Token matcher which can construct a token if the input matches a list of known symbols.
class SymbolMatcher: public TokenMatcher {
public:
  /**
   * Constructs a symbol matcher using the token types found iterating through the given iterators.
   * The token types must have an associated symbol.
   **/
  template<typename InputIterator>
  SymbolMatcher(InputIterator tokenTypesBegin, InputIterator tokenTypesEnd);
  /**
   * Constructs a symbol matcher using the token types given.
   * The token types must have an associated symbol.
   **/
  SymbolMatcher(std::initializer_list<TokenType> initList);
  //! @copydoc TokenMatcher::match(std::string_view,size_t)
  std::optional<Token> match(std::string_view source, size_t startIdx) override;
  //! @copydoc TokenMatcher::match(std::string_view,size_t)
  std::optional<Token> match(std::string_view source, size_t startIdx) const;

private:
  std::vector<TokenType> m_tokenTypes;
};

/// Token matcher which can construct a token from a string containing an unsigned number.
class UnsignedNumberMatcher: public TokenMatcher {
public:
  //! @copydoc TokenMatcher::match(std::string_view,size_t)
  std::optional<Token> match(std::string_view source, size_t startIdx) override;
  //! @copydoc TokenMatcher::match(std::string_view,size_t)
  std::optional<Token> match(std::string_view source, size_t startIdx) const;
};

/// Token matcher which can construct a token from a string containing a logarithm with or without a base.
class LogarithmMatcher: public TokenMatcher {
public:
  LogarithmMatcher();
  //! @copydoc TokenMatcher::match(std::string_view,size_t)
  std::optional<Token> match(std::string_view source, size_t startIdx) override;
  //! @copydoc TokenMatcher::match(std::string_view,size_t)
  std::optional<Token> match(std::string_view source, size_t startIdx) const;
private:
  SymbolMatcher m_symbolMatcher;
  UnsignedNumberMatcher m_numberMatcher;
};

}

#endif // MATHTREE_TOKENMATCHERS