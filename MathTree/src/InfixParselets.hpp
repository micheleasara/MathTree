#ifndef MATHTREE_INFIXPARSELETS
#define MATHTREE_INFIXPARSELETS

#include <memory>
#include "Expression.hpp"
#include "Token.hpp"

namespace MathTree {
class PrattParser;

class InfixParselet {
public:
  virtual std::unique_ptr<Expression> parse(PrattParser& parser,
                                            std::unique_ptr<Expression> left,
                                            Token const& token) = 0;
  virtual int priority() const = 0;

  InfixParselet const& operator=(InfixParselet const&) = delete;
  InfixParselet&& operator=(InfixParselet&&) = delete;
  virtual ~InfixParselet() = default;
};

class BinaryOperatorParselet: public InfixParselet {
public:
  BinaryOperatorParselet(int priority);

  int priority() const override;

private:
  int m_priority{0};
};

class AdditionParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  std::unique_ptr<Expression> parse(PrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

class SubtractionParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  std::unique_ptr<Expression> parse(PrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

class MultiplicationParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  std::unique_ptr<Expression> parse(PrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

class DivisionParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  std::unique_ptr<Expression> parse(PrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

class ExponentiationParselet: public BinaryOperatorParselet {
public:
  using BinaryOperatorParselet::BinaryOperatorParselet;
  std::unique_ptr<Expression> parse(PrattParser& parser,
                                    std::unique_ptr<Expression> left,
                                    Token const& token) override;
};

}
#endif // MATHTREE_INFIXPARSELETS