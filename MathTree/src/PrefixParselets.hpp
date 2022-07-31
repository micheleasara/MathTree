#ifndef MATHTREE_PREFIXPARSELETS
#define MATHTREE_PREFIXPARSELETS

#include "Expression.hpp"
#include <memory>
#include "Token.hpp"

namespace MathTree {
class AbstractPrattParser;

/// Responsible for parsing an expression identified by a prefix.
class PrefixParselet {
public:
  /// Constructs an expression using the Pratt parser and the token given.
  virtual std::unique_ptr<Expression> parse(AbstractPrattParser& parser, Token const& token) = 0;

  PrefixParselet& operator=(PrefixParselet const&) = delete;
  PrefixParselet& operator=(PrefixParselet&&) = delete;
  virtual ~PrefixParselet() = default;
};

/// Responsible for building real number expressions from a token.
class NumberParselet: public PrefixParselet {
public:
  /// Builds a real number expression using the token provided. 
  std::unique_ptr<Expression> parse(AbstractPrattParser&, Token const& token) override;
};

/// Responsible for grouping together multiple subexpressions into one cohesive expression.
class GroupParselet: public PrefixParselet {
public:
  /// Parses an expression from a group of subexpressions. Expects the group to be terminated by a closing bracket.
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser, Token const&) override;
};

/// Responsible for building a square root expression from a token.
class SquareRootParselet: public PrefixParselet {
public:
  /// Constructs a square root parselet with the given priority.
  SquareRootParselet(int priority);
  /// Returns a square root expression applied to the parser's next expression and using the token type of the given token.
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser, Token const& token) override;
private:
  int m_priority{0};
};

/// Responsible for building an expression that negates another.
class NegativeSignParselet: public PrefixParselet {
public:
  /// Constructs a negative sign parselet with the given priority.
  NegativeSignParselet(int priority);
  /// Returns a negative sign expression applied to the parser's next expression and using the token type of the given token.
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser, Token const& token) override;

private:
  int m_priority{0};
};

/// Responsible for building an expression corresponding to the next one in sequence.
class PositiveSignParselet: public PrefixParselet {
public:
  /// Constructs a positive sign parselet with the given priority.
  PositiveSignParselet(int priority);
  /// Returns the parser's next expression associated to the token type of the given token.
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser, Token const& token) override;

private:
  int m_priority{0};
};

/// Responsible for building a logarithm expression.
class LogarithmParselet: public PrefixParselet {
public:
  /// Constructs a logarithm parselet with the given priority.
  LogarithmParselet(int priority);
  /**
   * Returns a logarithm expression applied to the parser's next expression, and associated to
   * the token type of the given token.
   * If no base is specified, it is assumed to be 10.
   **/
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser, Token const& token) override;
private:
  int m_priority{0};
};

}

#endif // MATHTREE_PREFIXPARSELETS