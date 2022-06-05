#include "Parser.hpp"
#include <stdexcept>

namespace MathTree {

class ReferenceCountingResetter {
public:
  ReferenceCountingResetter(int& counter, PrattParser& parser): 
                         m_counter(++counter), m_parser(parser) {}
  ~ReferenceCountingResetter() { 
    if (--m_counter <= 0) {
      m_parser.reset();
    }
  }

private:
  int& m_counter;
  PrattParser& m_parser;
};

PrattParser::PrattParser(std::unique_ptr<Lexer> lexer): m_lexer(std::move(lexer)) {}

std::unique_ptr<Expression> PrattParser::parse() {
  return parse(0);
}

std::unique_ptr<Expression> PrattParser::parse(int priority) {
  ReferenceCountingResetter countingResetter(parseCallCount, *this);
  auto token = consumeCurrentToken();
  if (m_prefixParselets.count(token.type()) <= 0) {
    throw std::logic_error("Expected a prefix parselet while parsing.");
  }
  auto& prefix = *m_prefixParselets[token.type()];
  auto left = prefix.parse(*this, token);

  while (priority < currentTokenPriority()) {
    token = consumeCurrentToken();
    auto& infix = *m_infixParselets[token.type()];
    left = infix.parse(*this, std::move(left), token);
  }

  return left;
}

void PrattParser::setPrefixParselet(TokenType token, std::unique_ptr<PrefixParselet> parselet) {
  m_prefixParselets[token] = std::move(parselet);
}

void PrattParser::setInfixParselet(TokenType token, std::unique_ptr<InfixParselet> parselet) {
  m_infixParselets[token] = std::move(parselet);
}

Token PrattParser::consumeCurrentToken() {
  auto token = currentToken();
  m_currentToken = std::nullopt;
  return token;
}

int PrattParser::currentTokenPriority() {
  auto token = currentToken();
  if (m_infixParselets.count(token.type()) <= 0) {
    return 0;
  }
  InfixParselet& infix = *m_infixParselets[token.type()];
  return infix.priority();
}

Token const& PrattParser::currentToken() {
  if (!m_currentToken.has_value()) {
    m_currentToken = m_lexer->next();
  }
  return *m_currentToken;
}

void PrattParser::reset() {
  m_currentToken = std::nullopt;
  m_lexer->reset();
}

ArithmeticParser::IndexErrorPairs ArithmeticParser::validate(std::string_view input) {
  static auto constexpr operatorsList = {TokenType::PLUS, TokenType::MINUS,
                                          TokenType::SLASH, TokenType::ASTERISK,
                                          TokenType::CARET};
  static SymbolMatcher operatorMatcher(operatorsList);
  static UnsignedNumberMatcher numberMatcher;
  
  if (input.size() <= 0) {
    return IndexErrorPairs{};
  }

  IndexErrorPairs idxErrorPairs;
  std::vector<size_t> openBracketsIdx;
  int lastNonSpaceIdx = -1;
  auto wasNumber = false;
  auto wasOperator = false;
  for (size_t i = 0; i < input.size(); ++i) {
    if (std::isspace(input[i])) {
      continue;
    }

    auto increment = 0;
    std::optional<Token> operatorOpt, numberOpt;
    if (operatorOpt = operatorMatcher.match(input, i)) {
      increment += operatorOpt->text().size() - 1;
    } else if (numberOpt = numberMatcher.match(input, i)) {
      increment += numberOpt->text().size() - 1;
    }

    if (input[i] == '(') {
      openBracketsIdx.push_back(i);
      if (wasNumber) {
        idxErrorPairs.emplace_back(i, Errors::MissingOperator);
      }
    } else if (input[i] == ')') {
      if (openBracketsIdx.size() > 0) {
        openBracketsIdx.pop_back();
      } else {
        idxErrorPairs.emplace_back(i, Errors::UnpairedClosingBracket);
      }
      if (wasOperator) {
        idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
      }
    } else if (operatorOpt && wasOperator) {
      idxErrorPairs.emplace_back(i, Errors::TwoOperatorsInARow);
    } else if (numberOpt && (wasNumber || (lastNonSpaceIdx > -1 && input[lastNonSpaceIdx] == ')'))) {
      idxErrorPairs.emplace_back(i, Errors::MissingOperator);
    } else if (!operatorOpt && !numberOpt) {
      idxErrorPairs.emplace_back(i, Errors::UnrecognisedSymbol);
    }

    lastNonSpaceIdx = static_cast<int>(i);
    wasNumber = numberOpt.has_value();
    wasOperator = operatorOpt.has_value();
    i += increment;
  }

  if (wasOperator) {
    idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
  }

  for (auto const& idx: openBracketsIdx) {
    idxErrorPairs.emplace_back(idx, Errors::UnpairedOpeningBracket);
  }

  return idxErrorPairs;
}

ArithmeticParser::ArithmeticParser(std::string input): 
                       m_parser(PrattParser(std::make_unique<ArithmeticLexer>(std::move(input)))) {
  m_parser.setPrefixParselet(TokenType::PLUS, std::make_unique<PositiveSignParselet>(2));
  m_parser.setPrefixParselet(TokenType::MINUS, std::make_unique<NegativeSignParselet>(2));
  m_parser.setPrefixParselet(TokenType::NUMBER, std::make_unique<NumberParselet>());
  m_parser.setPrefixParselet(TokenType::OPENING_BRACKET, std::make_unique<GroupParselet>());

  m_parser.setInfixParselet(TokenType::PLUS, std::make_unique<AdditionParselet>(1));
  m_parser.setInfixParselet(TokenType::MINUS, std::make_unique<SubtractionParselet>(1));
  m_parser.setInfixParselet(TokenType::ASTERISK, std::make_unique<MultiplicationParselet>(2));
  m_parser.setInfixParselet(TokenType::SLASH, std::make_unique<DivisionParselet>(2));
  m_parser.setInfixParselet(TokenType::CARET, std::make_unique<ExponentiationParselet>(3));
}

std::unique_ptr<Expression> ArithmeticParser::parse() {
  return m_parser.parse();
}

}