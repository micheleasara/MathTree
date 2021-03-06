#include <cmath>
#include "Lexer.hpp"
#include <stdexcept>
#include "Utils.hpp"

namespace MathTree {

auto constexpr symbolsList = {TokenType::Plus, TokenType::Minus,
                                    TokenType::Slash, TokenType::Asterisk,
                                    TokenType::Caret, TokenType::SquareRoot,
                                    TokenType::OpeningBracket, TokenType::ClosingBracket};

ArithmeticLexer::ArithmeticLexer(): ArithmeticLexer("") {}

ArithmeticLexer::ArithmeticLexer(std::string text): m_text(std::move(text)) {
  m_matchers.reserve(2);
  m_matchers.push_back(std::make_unique<SymbolMatcher>(symbolsList));
  m_matchers.push_back(std::make_unique<UnsignedNumberMatcher>());
  m_matchers.push_back(std::make_unique<LogarithmMatcher>());
}

Token ArithmeticLexer::next() {
  while (m_currentIndex < m_text.length()) {
    for (auto const& matcher: m_matchers) {
      if (auto tokenOpt = matcher->match(m_text, m_currentIndex)) {
        m_currentIndex += tokenOpt->text().size();
        return *tokenOpt;
      }
    }
    if (std::isspace(m_text[m_currentIndex])) {
      ++m_currentIndex;
      continue;
    }
    throw std::logic_error("Lexer could not extract token at index " +
                                   std::to_string(m_currentIndex) + ".");
  }
  return {TokenType::Stop, ""};
}

void ArithmeticLexer::reset(std::string newText) {
  m_text = std::move(newText);
  reset();
}

void ArithmeticLexer::reset() {
  m_currentIndex = 0;
}  

}