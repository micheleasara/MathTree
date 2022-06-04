#ifndef MATHTREE_PARSER
#define MATHTREE_PARSER

#include <memory>
#include <optional>
#include "Expression.hpp"
#include "InfixParselets.hpp"
#include "Lexer.hpp"
#include "PrefixParselets.hpp"
#include "Token.hpp"
#include <unordered_map>

namespace MathTree {
class Parser {
public:
  virtual std::unique_ptr<Expression> parse() = 0;
  Parser const& operator=(Parser const&) = delete;
  Parser&& operator=(Parser&&) = delete;
  virtual ~Parser() = default;
};

class PrattParser: public Parser {
public:
  PrattParser(std::unique_ptr<Lexer> lexer);

  std::unique_ptr<Expression> parse() override;
  std::unique_ptr<Expression> parse(int priority);

  void setPrefixParselet(TokenType token, std::unique_ptr<PrefixParselet> parselet);
  void setInfixParselet(TokenType token, std::unique_ptr<InfixParselet> parselet);

  Token consumeCurrentToken();
  
private:
  int currentTokenPriority();
  Token const& currentToken();

  std::optional<Token> m_currentToken;
  std::unordered_map<TokenType, std::unique_ptr<PrefixParselet>> m_prefixParselets;
  std::unordered_map<TokenType, std::unique_ptr<InfixParselet>> m_infixParselets;
  std::unique_ptr<Lexer> m_lexer;
};

class ArithmeticParser: public Parser {
public:
  ArithmeticParser(std::string input);
  std::unique_ptr<Expression> parse() override;

private:
  PrattParser m_parser;
};

}

#endif // MATHTREE_PARSER