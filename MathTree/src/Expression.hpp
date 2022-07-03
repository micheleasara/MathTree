#ifndef MATHTREE_EXPRESSION_H
#define MATHTREE_EXPRESSION_H

#include <memory>
#include <optional>
#include <string_view>
#include "Token.hpp"
#include <utility>
#include <vector>

namespace MathTree {

class Expression {
public:
  virtual double evaluate() const = 0;
  virtual void print(std::ostream& stream) const = 0;
  virtual std::vector<Expression const*> subexpressions() const = 0;

  Expression& operator=(Expression const&) = delete;
  Expression& operator=(Expression&&) = delete;
  virtual ~Expression() = default;
};
std::ostream& operator<<(std::ostream& left, Expression const& right);

class BinaryExpression: public Expression {
public:
  BinaryExpression(std::unique_ptr<Expression> left,
                   TokenType tokenType,
                   std::unique_ptr<Expression> right);

  void print(std::ostream& stream) const override;
  Expression const& left() const;
  Expression const& right() const;
  std::vector<Expression const*> subexpressions() const override;

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
  TokenType m_tokenType;
};


class AdditionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

class SubtractionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

class MultiplicationExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

class DivisionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

class ExponentiationExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

class NegativeSignExpression: public Expression {
public:
  NegativeSignExpression(TokenType operatorToken, std::unique_ptr<Expression> right);
  void print(std::ostream& stream) const override;
  double evaluate() const override;
  std::vector<Expression const*> subexpressions() const override;

private:
  TokenType m_operator;
  std::unique_ptr<Expression> m_right;
  mutable std::optional<double> m_cache;
};

class RealNumberExpression: public Expression {
public:
  RealNumberExpression(std::string_view num);
  double evaluate() const override;
  void print(std::ostream& stream) const override;
  std::vector<Expression const*> subexpressions() const override;

private:
  double m_value{0};
};

class SquareRootExpression: public Expression {
public:
  SquareRootExpression(std::unique_ptr<Expression> innerExpression,
                       TokenType tokenType);
  double evaluate() const override;
  void print(std::ostream& stream) const override;
  std::vector<Expression const*> subexpressions() const override;

private:
  std::unique_ptr<Expression> m_innerExpression;
  TokenType m_tokenType;
  mutable std::optional<double> m_cache;
};

class LogarithmExpression: public Expression {
public:
  LogarithmExpression(std::unique_ptr<Expression> innerExpression,
                       double base,
                       TokenType tokenType);
  double evaluate() const override;
  void print(std::ostream& stream) const override;
  std::vector<Expression const*> subexpressions() const override;

private:
  std::unique_ptr<Expression> m_innerExpression;
  double m_base{0};
  TokenType m_tokenType;
  mutable std::optional<double> m_cache;
};

}

#endif // MATHTREE_EXPRESSION_H