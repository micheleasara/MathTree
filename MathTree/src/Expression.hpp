#ifndef MATHTREE_EXPRESSION_H
#define MATHTREE_EXPRESSION_H

#include <memory>
#include <string_view>
#include "Token.hpp"
#include <utility>
#include <vector>

namespace MathTree {

class Expression {
public:
  virtual double evaluate() const = 0;
  virtual void print(std::ostream& stream) const = 0;

  Expression const& operator=(Expression const&) = delete;
  Expression&& operator=(Expression&&) = delete;
  virtual ~Expression() = default;
};

class BinaryExpression: public Expression {
public:
  BinaryExpression(std::unique_ptr<Expression> left,
                   TokenType tokenType,
                   std::unique_ptr<Expression> right);

  void print(std::ostream& stream) const override;
  Expression const& left() const;
  Expression const& right() const;

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
  TokenType m_tokenType;
};


class AdditionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
};

class SubtractionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
};

class MultiplicationExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
};

class DivisionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
};

class ExponentiationExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
};

class NegativeSignExpression: public Expression {
public:
  NegativeSignExpression(TokenType operatorToken, std::unique_ptr<Expression> right);
  void print(std::ostream& stream) const override;
  double evaluate() const override;

private:
  TokenType m_operator;
  std::unique_ptr<Expression> m_right;
};

class RealNumberExpression: public Expression {
public:
  RealNumberExpression(std::string_view num);
  double evaluate() const override;
  void print(std::ostream& stream) const override;

private:
  double m_value{0};
};

class SquareRootExpression: public Expression {
public:
  SquareRootExpression(std::unique_ptr<Expression> innerExpression,
                       TokenType tokenType);
  double evaluate() const override;
  void print(std::ostream& stream) const override;

private:
  std::unique_ptr<Expression> m_innerExpression;
  TokenType m_tokenType;
};

}

#endif // MATHTREE_EXPRESSION_H