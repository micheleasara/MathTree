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

class PrattParser {
public:
  PrattParser(std::unique_ptr<Lexer> lexer);

  std::unique_ptr<Expression> parse();
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

}

#endif // MATHTREE_PARSER