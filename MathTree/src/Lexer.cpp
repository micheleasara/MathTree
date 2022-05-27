#include <cmath>
#include "Lexer.hpp"
#include <stdexcept>
#include "Utils.hpp"

namespace MathTree {

auto constexpr initializer = {TokenType::PLUS, TokenType::MINUS,
                                    TokenType::SLASH, TokenType::ASTERISK,
                                    TokenType::CARET,
                                    TokenType::OPENING_BRACKET, TokenType::CLOSING_BRACKET};

ArithmeticLexer::ArithmeticLexer(std::string text) {
  m_text = std::move(text);

  for (auto const& type: initializer) {
    m_symbols[symbolise(type)] = type;
  }
}

Token ArithmeticLexer::next() {
  while (m_currentIndex < m_text.length()) {
    auto chr = m_text[m_currentIndex++];
    
    if (m_symbols.count(chr)) {
      return {m_symbols[chr], std::string(1, chr)};
    } else if (std::isdigit(chr) || chr == '.') {
      int start = m_currentIndex - 1;
      while (m_currentIndex < m_text.length()) {
        if (std::isdigit(m_text[m_currentIndex]) || m_text[m_currentIndex] == '.') {
          ++m_currentIndex;
        } else {
          break;
        }
      }
      std::string number = m_text.substr(start, m_currentIndex - start);
      if (!Utils::parseDouble(number).has_value()) {
        throw std::logic_error("Expected a number, but failed to extract it from \"" + 
                                                           number + std::string("\"."));
      }
      return {TokenType::NUMBER, number};
    }
  }

  return {TokenType::STOP, ""};
}

}