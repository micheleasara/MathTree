#ifndef MATHTREE_EXPRESSION_H
#define MATHTREE_EXPRESSION_H

#include <memory>
#include <optional>
#include <string_view>
#include "Token.hpp"
#include <utility>
#include <vector>

namespace MathTree {

/// Represents a mathematical expression of real numbers.
class Expression {
public:
  /// Returns the real valued number obtained by solving the expression.
  virtual double evaluate() const = 0;
  /// Prints the expression to the output stream provided.
  virtual void print(std::ostream& stream) const = 0;
  /// Returns the list of subexpressions that make up this expression, if there are any, ordered left to right.
  virtual std::vector<Expression const*> subexpressions() const = 0;

  Expression& operator=(Expression const&) = delete;
  Expression& operator=(Expression&&) = delete;
  virtual ~Expression() = default;
};
std::ostream& operator<<(std::ostream& left, Expression const& right);

/// Represents an expression composed of two subexpressions.
class BinaryExpression: public Expression {
public:
  /**
   * Constructs a binary expression from a left and right subexpressions,
   * and from the token that symbolises its operation.
   **/
  BinaryExpression(std::unique_ptr<Expression> left,
                   TokenType tokenType,
                   std::unique_ptr<Expression> right);

  /**
   * Prints the binary expression to the output stream provided.
   * It first prints the left subexpression, then the token, and
   * finally the right subexpression.
   **/
  void print(std::ostream& stream) const override;
  /// Returns the left subexpression.
  Expression const& left() const;
  /// Returns the right subexpression.
  Expression const& right() const;
  /// Returns the left and right subexpressions, in this order.
  std::vector<Expression const*> subexpressions() const override;

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
  TokenType m_tokenType;
};

/// Represents an addition of two subexpressions.
class AdditionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  /// Returns the addition of the two subexpressions.
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

/// Represents a subtraction of two subexpressions.
class SubtractionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  /// Returns the subtraction of the right subexpression from the left subexpression.
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

/// Represents a multiplication of two subexpressions.
class MultiplicationExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  /// Returns the multiplication of the two subexpressions.
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

/// Represents a division of two subexpressions.
class DivisionExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  /**
   * Returns the division of the left subexpression by the right subexpression.
   * Throws if the divisor evaluates to zero.
   **/
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

/// Represents the exponentiation of a base subexpression with an exponent subexpression.
class ExponentiationExpression: public BinaryExpression {
public:
  using BinaryExpression::BinaryExpression;
  /**
   * Returns the left subexpression exponentiated to the result of the right subexpression.
   * Throws if the result is not real. Namely, with a and b denoting real numbers:
   * 0^0 for any a and b;
   * 0^a with a < 0;
   * a^b with a < 0 and b non-integer.
   **/
  double evaluate() const override;
private:
  mutable std::optional<double> m_cache;
};

/// Represents the negation of a subexpression.
class NegativeSignExpression: public Expression {
public:
  /**
   * Constructs a negative sign expression using the given token type as its negation symbol.
   * The negation will be applied to the right subexpression provided.
   **/
  NegativeSignExpression(TokenType operatorToken, std::unique_ptr<Expression> right);
  /**
   * Prints the expression to the output stream given, by first printing the negation symbol
   * and then the right subexpression.
   **/
  void print(std::ostream& stream) const override;
  /// Returns the negation of the right subexpression.
  double evaluate() const override;
  /// Returns the only subexpression.
  std::vector<Expression const*> subexpressions() const override;

private:
  TokenType m_operator;
  std::unique_ptr<Expression> m_right;
  mutable std::optional<double> m_cache;
};

/// Represents a finite real number limited by double precision.
class RealNumberExpression: public Expression {
public:
  /// Constructs a real number from the string provided, with double precision.
  RealNumberExpression(std::string_view num);
  /// Returns the real number.
  double evaluate() const override;
  /// Prints the real number to the output stream.
  void print(std::ostream& stream) const override;
  /// Returns an empty container.
  std::vector<Expression const*> subexpressions() const override;

private:
  double m_value{0};
};

/// Represents the square root of a subexpression.
class SquareRootExpression: public Expression {
public:
  /**
   * Constructs the square root of the inner expression provided,
   * and using the given token type as its symbol.
   **/
  SquareRootExpression(std::unique_ptr<Expression> innerExpression,
                       TokenType tokenType);
  /**
   * Returns the square root of the subexpression.
   * Throws if the subexpression is infinite, negative or non-real.
   **/
  double evaluate() const override;
  /**
   * Prints the symbol of this expression, followed by an opening bracket,
   * then the subexpression, and finally a closing bracket.
   **/
  void print(std::ostream& stream) const override;
  /// Returns the only subexpression.
  std::vector<Expression const*> subexpressions() const override;

private:
  std::unique_ptr<Expression> m_innerExpression;
  TokenType m_tokenType;
  mutable std::optional<double> m_cache;
};

/// Represents the logarithm of a subexpression with an arbitrary base.
class LogarithmExpression: public Expression {
public:
  /**
   * Constructs a logarithm expression which is applied to the given subexpression
   * using the base provided, and with the token type given as its symbol.
   * Throws if the base is infinite or negative, or if the subexpression is null.
   **/
  LogarithmExpression(std::unique_ptr<Expression> innerExpression,
                       double base,
                       TokenType tokenType);
  /**
   * Returns the logarithm of the given subexpression using the base specified on construction.
   * Throws if the subexpression is non-positive.
   **/
  double evaluate() const override;
  /// Prints to the output stream provided using a log_b(a) format.
  void print(std::ostream& stream) const override;
  /// Returns the only subexpression.
  std::vector<Expression const*> subexpressions() const override;

private:
  std::unique_ptr<Expression> m_innerExpression;
  double m_base{0};
  TokenType m_tokenType;
  mutable std::optional<double> m_cache;
};

}

#endif // MATHTREE_EXPRESSION_H