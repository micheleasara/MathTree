#include "TokenMatchers.hpp"
#include "Utils.hpp"

namespace MathTree {

template<typename InputIterator>
SymbolMatcher::SymbolMatcher(InputIterator tokenTypesBegin, InputIterator tokenTypesEnd) {
  for (auto it = tokenTypesBegin; it != tokenTypesEnd; ++it) {
    m_symbolToOperator.emplace(symbolise(*it), *it);
  }
}

SymbolMatcher::SymbolMatcher(std::initializer_list<TokenType> initList): 
                                                        SymbolMatcher(initList.begin(), initList.end()) {}

std::optional<Token> SymbolMatcher::match(std::string_view source, size_t startIdx) {
  if (startIdx < source.size()) {
    auto chr = source[startIdx];
    if (m_symbolToOperator.count(chr) > 0) {
      return Token{m_symbolToOperator[chr], std::string(1, chr)};
    }
  }
  return std::nullopt;
}

std::optional<Token> UnsignedNumberMatcher::match(std::string_view source, size_t startIdx) {
  if (startIdx >= source.size()) {
    return std::nullopt;
  }

  auto endIdx = startIdx;
  bool decimalPlace = false;
  while(endIdx < source.size() && 
        (std::isdigit(source[endIdx]) || source[endIdx] == '.')) {
    if (source[endIdx] == '.') {
      if (decimalPlace) {
        // allow at most one decimal place
        break;
      }
      decimalPlace = true;
    }
    ++endIdx;
  }

  auto numberStr = source.substr(startIdx, endIdx - startIdx);
  if (Utils::parseDouble(numberStr).has_value()) {
    return Token{TokenType::NUMBER, std::string(numberStr)};
  }
  return std::nullopt;
}

}