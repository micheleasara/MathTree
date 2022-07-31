#ifndef MATHTREE_INFIXPARSELETS
#define MATHTREE_INFIXPARSELETS

#include <memory>
#include "Expression.hpp"
#include "Token.hpp"

namespace MathTree {
class AbstractPrattParser;

/// Represents an entity which can parse expressions having a non-prefix operator.
class InfixParselet {
public:
  /// Constructs an expression tree from the left subexpression given, and using the token and parser provided.
  virtual std::unique_ptr<Expression> parse(AbstractPrattParser& parser,
                                            std::unique_ptr<Expression> left,
                                            Token const& token) = 0;
  /// Returns the priority of the parselet.
  virtual int priority() const = 0;

  InfixParselet& operator=(InfixParselet const&) = delete;
  InfixParselet& operator=(InfixParselet&&) = delete;
  virtual ~InfixParselet() = default;
};

/// Represents a parselet that can parse binary expressions.
class BinaryOperatorParselet: public InfixParselet {
public:
  /// Constructs the parselet with the given priority.
  BinaryOperatorParselet(int priority);
  //! @copydoc InfixParselet::priorty()
  int priority() const override;

private:
  int m_priority{0};
};

/// Represents a parselet that can parse additions.
class AdditionParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  //! @copydoc InfixParselet::parse(AbstractPrattParser&,std::unique_ptr<Expression>,Token const&)
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

/// Represents a parselet that can parse subtractions.
class SubtractionParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  //! @copydoc InfixParselet::parse(AbstractPrattParser&,std::unique_ptr<Expression>,Token const&)
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

/// Represents a parselet that can parse multiplications.
class MultiplicationParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  //! @copydoc InfixParselet::parse(AbstractPrattParser&,std::unique_ptr<Expression>,Token const&)
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

/// Represents a parselet that can parse divisions.
class DivisionParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  //! @copydoc InfixParselet::parse(AbstractPrattParser&,std::unique_ptr<Expression>,Token const&)
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

/// Represents a parselet that can parse exponentiations.
class ExponentiationParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  //! @copydoc InfixParselet::parse(AbstractPrattParser&,std::unique_ptr<Expression>,Token const&)
  std::unique_ptr<Expression> parse(AbstractPrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

}
#endif // MATHTREE_INFIXPARSELETS