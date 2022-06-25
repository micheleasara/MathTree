#include "Parser.hpp"
#include <stdexcept>
#include <limits>

namespace MathTree {

class PrattParser::ReferenceCountingResetter {
public:
  ReferenceCountingResetter(PrattParser& parser): m_parser(parser) {
    ++m_parser.m_parseCallCount;
  }
  ~ReferenceCountingResetter() { 
    if (--m_parser.m_parseCallCount <= 0) {
      m_parser.reset();
    }
  }

private:
  PrattParser& m_parser;
};

PrattParser::PrattParser(std::unique_ptr<Lexer> lexer): m_lexer(std::move(lexer)) {}

std::unique_ptr<Expression> PrattParser::parse() {
  return parse(std::numeric_limits<int>::min());
}

std::unique_ptr<Expression> PrattParser::parse(int priority) {
  ReferenceCountingResetter countingResetter(*this);
  auto token = consumeCurrentToken();
  if (m_prefixParselets.count(token.type()) <= 0) {
    throw std::logic_error("Expected a prefix parselet while parsing.");
  }
  auto& prefix = *m_prefixParselets[token.type()];
  auto left = prefix.parse(*this, token);

  while (priority < infixPriorityFor(currentToken())) {
    token = consumeCurrentToken();
    auto& infix = *m_infixParselets[token.type()];
    left = infix.parse(*this, std::move(left), token);
  }

  return left;
}

std::unique_ptr<Expression> PrattParser::parse(std::string input, int priority) {
  m_lexer->reset(std::move(input));
  return parse(priority);
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

int PrattParser::infixPriorityFor(Token const& token) {
  if (m_infixParselets.count(token.type()) <= 0) {
    return std::numeric_limits<int>::min();
  }
  InfixParselet const& infix = *m_infixParselets[token.type()];
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
  static SymbolMatcher const symbolMatcher({TokenType::Plus, TokenType::Minus,
                                              TokenType::Slash, TokenType::Asterisk,
                                              TokenType::Caret, TokenType::SquareRoot});
  static SymbolMatcher const signMatcher({TokenType::Plus, TokenType::Minus});
  static UnsignedNumberMatcher const numberMatcher;
  // logMatcher is needed as it can match the base as well (so more than just the "log" symbol)
  static LogarithmMatcher const logMatcher;
  
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
    if (operatorOpt = symbolMatcher.match(input, i)) {
      increment += operatorOpt->text().size() - 1;
    } else if (operatorOpt = logMatcher.match(input, i)) {
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
        auto openingIdx = openBracketsIdx.back();
        openBracketsIdx.pop_back();
        if (static_cast<size_t>(lastNonSpaceIdx) <= openingIdx) {
          idxErrorPairs.emplace_back(openingIdx, Errors::NothingBetweenBrackets);
        }
      } else {
        idxErrorPairs.emplace_back(i, Errors::UnpairedClosingBracket);
      }
      if (wasOperator) {
        idxErrorPairs.emplace_back(lastNonSpaceIdx, Errors::IncompleteOperation);
      }
    } else if (operatorOpt) {
      auto isSqrtOrLog = operatorOpt->type() == TokenType::SquareRoot ||
                         operatorOpt->type() == TokenType::Log;
      if (isSqrtOrLog && !wasOperator && lastNonSpaceIdx != -1 && input[lastNonSpaceIdx] != '(') {
        // sqrt and log must be preceded by an operator, or by an opening bracket,
        // or they must be at the start of the expression
        idxErrorPairs.emplace_back(i, Errors::MissingOperator);
      } else if ((wasOperator || lastNonSpaceIdx == -1 || input[lastNonSpaceIdx] == '(') && 
                                           !isSqrtOrLog && !signMatcher.match(operatorOpt->text(), 0)) {
        // allow expressions like 1+-2, 2++3, sqrtsqrt3, 2-sqrt2 and so on, but disallow
        // other operators from appearing in a row without numbers between them.
        // Also ensure no binary operator is at the start of the expression.
        idxErrorPairs.emplace_back(i, Errors::IncompleteOperation);
      }
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

ArithmeticParser::ArithmeticParser(): m_parser(PrattParser(std::make_unique<ArithmeticLexer>())) {
  m_parser.setPrefixParselet(TokenType::Plus,
                             std::make_unique<PositiveSignParselet>(static_cast<int>(OperationPriority::Sign)));
  m_parser.setPrefixParselet(TokenType::Minus,
                             std::make_unique<NegativeSignParselet>(static_cast<int>(OperationPriority::Sign)));
  m_parser.setPrefixParselet(TokenType::Number,
                             std::make_unique<NumberParselet>());
  m_parser.setPrefixParselet(TokenType::OpeningBracket,
                             std::make_unique<GroupParselet>());
  m_parser.setPrefixParselet(TokenType::SquareRoot,
                             std::make_unique<SquareRootParselet>(static_cast<int>(OperationPriority::SquareRoot)));
  m_parser.setPrefixParselet(TokenType::Log,
                             std::make_unique<LogarithmParselet>(static_cast<int>(OperationPriority::Logarithm)));

  m_parser.setInfixParselet(TokenType::Plus,
                            std::make_unique<AdditionParselet>(static_cast<int>(OperationPriority::Addition)));
  m_parser.setInfixParselet(TokenType::Minus,
                            std::make_unique<SubtractionParselet>(static_cast<int>(OperationPriority::Subtraction)));
  m_parser.setInfixParselet(TokenType::Asterisk,
                            std::make_unique<MultiplicationParselet>(static_cast<int>(OperationPriority::Multiplication)));
  m_parser.setInfixParselet(TokenType::Slash,
                            std::make_unique<DivisionParselet>(static_cast<int>(OperationPriority::Division)));
  m_parser.setInfixParselet(TokenType::Caret,
                            std::make_unique<ExponentiationParselet>(static_cast<int>(OperationPriority::Exponentiation)));
}

std::unique_ptr<Expression> ArithmeticParser::parse(std::string input) {
  return m_parser.parse(std::move(input));
}

}