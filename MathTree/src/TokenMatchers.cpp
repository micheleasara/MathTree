#include "TokenMatchers.hpp"
#include "Utils.hpp"

namespace MathTree {

template<typename InputIterator>
SymbolMatcher::SymbolMatcher(InputIterator tokenTypesBegin, InputIterator tokenTypesEnd) {
  for (auto it = tokenTypesBegin; it != tokenTypesEnd; ++it) {
    m_tokenTypes.push_back(*it);
  }
}

SymbolMatcher::SymbolMatcher(std::initializer_list<TokenType> initList): 
                                                        SymbolMatcher(initList.begin(), initList.end()) {}

std::optional<Token> SymbolMatcher::match(std::string_view source, size_t startIdx) {
  return static_cast<SymbolMatcher const&>(*this).match(source, startIdx);
}

std::optional<Token> SymbolMatcher::match(std::string_view source, size_t startIdx) const {
  for (auto const& tokenType: m_tokenTypes) {
    auto symbol = symboliseTokenType(tokenType);
    if (source.substr(startIdx, symbol.size()) == symbol) {
      return Token{tokenType, std::string(symbol)};
    }
  }
  return std::nullopt;   
}

std::optional<Token> UnsignedNumberMatcher::match(std::string_view source, size_t startIdx) {
  return static_cast<UnsignedNumberMatcher const&>(*this).match(source, startIdx);
}

std::optional<Token> UnsignedNumberMatcher::match(std::string_view source, size_t startIdx) const {
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
    return Token{TokenType::Number, std::string(numberStr)};
  }
  return std::nullopt;
}

std::optional<Token> LogarithmMatcher::match(std::string_view source, size_t startIdx) const {
  return static_cast<LogarithmMatcher const&>(*this).match(source, startIdx);
}

std::optional<Token> LogarithmMatcher::match(std::string_view source, size_t startIdx) {
  if (auto logOpt = m_symbolMatcher.match(source, startIdx)) {
    return logOpt;
  }
  return std::nullopt;
}

LogarithmMatcher::LogarithmMatcher(): m_symbolMatcher({TokenType::Log}) {}

}