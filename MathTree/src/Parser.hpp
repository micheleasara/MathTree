#ifndef MATHTREE_PARSER
#define MATHTREE_PARSER

#include <memory>
#include <optional>
#include "Expression.hpp"
#include "InfixParselets.hpp"
#include "Lexer.hpp"
#include <limits>
#include "PrefixParselets.hpp"
#include "Token.hpp"
#include <unordered_map>

namespace MathTree {

class AbstractPrattParser {
public:
  virtual std::unique_ptr<Expression> parse() = 0;
  virtual std::unique_ptr<Expression> parse(int priority) = 0;
  virtual Token consumeCurrentToken() = 0;
  
  AbstractPrattParser& operator=(AbstractPrattParser const&) = delete;
  AbstractPrattParser& operator=(AbstractPrattParser&&) = delete;
  virtual ~AbstractPrattParser() = default;
};

class PrattParser: public AbstractPrattParser {
public:
  static auto constexpr minAllowedPriority = std::numeric_limits<int>::min();
  PrattParser(std::unique_ptr<Lexer> lexer);

  std::unique_ptr<Expression> parse() override;
  std::unique_ptr<Expression> parse(int priority) override;
  std::unique_ptr<Expression> parse(std::string input,
                                    int priority = minAllowedPriority);

  void setPrefixParselet(TokenType token, std::unique_ptr<PrefixParselet> parselet);
  void setInfixParselet(TokenType token, std::unique_ptr<InfixParselet> parselet);

  Token consumeCurrentToken() override;
  
private:
  class ReferenceCountingResetter;

  void reset();
  int infixPriorityFor(Token const& token);
  Token const& currentToken();

  std::optional<Token> m_currentToken;
  std::unordered_map<TokenType, std::unique_ptr<PrefixParselet>> m_prefixParselets;
  std::unordered_map<TokenType, std::unique_ptr<InfixParselet>> m_infixParselets;
  std::unique_ptr<Lexer> m_lexer;
  int m_parseCallCount = 0;
};

class ArithmeticParser {
public:
  enum class Errors {
    UnpairedOpeningBracket,
    UnpairedClosingBracket,
    IncompleteOperation,
    MissingOperator,
    UnrecognisedSymbol,
    NothingBetweenBrackets
  };
  using IndexErrorPairs = std::vector<std::pair<size_t, Errors>>;

  enum class OperationPriority {
    Minimum = 0,
    Addition = 1,
    Subtraction = 1,
    Sign = 2,
    Multiplication = 2,
    Division = 2,
    Exponentiation = 3,
    SquareRoot = 3,
    Logarithm = 3
  };

  static IndexErrorPairs validate(std::string_view input);
  
  ArithmeticParser();
  std::unique_ptr<Expression> parse(std::string input);

private:
  PrattParser m_parser;
};

}

#endif // MATHTREE_PARSER