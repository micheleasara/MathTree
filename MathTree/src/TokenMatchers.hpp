#ifndef MATHTREE_TOKENMATCHERS
#define MATHTREE_TOKENMATCHERS

#include <optional>
#include <string_view>
#include "Token.hpp"
#include <unordered_map>
#include <vector>

namespace MathTree {

class TokenMatcher {
public:
  virtual std::optional<Token> match(std::string_view source, size_t startIdx) = 0;

  TokenMatcher const& operator=(TokenMatcher const&) = delete;
  TokenMatcher&& operator=(TokenMatcher&&) = delete;
  virtual ~TokenMatcher() = default;
};

class SymbolMatcher: public TokenMatcher {
public:
  template<typename InputIterator>
  SymbolMatcher(InputIterator tokenTypesBegin, InputIterator tokenTypesEnd);
  
  SymbolMatcher(std::initializer_list<TokenType> initList);

  std::optional<Token> match(std::string_view source, size_t startIdx) override;

private:
  std::vector<TokenType> m_tokenTypes;
};

class UnsignedNumberMatcher: public TokenMatcher {
public:
    std::optional<Token> match(std::string_view source, size_t startIdx) override;
};

}

#endif // MATHTREE_TOKENMATCHERS