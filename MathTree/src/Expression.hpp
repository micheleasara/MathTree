#ifndef MATHTREE_EXPRESSION_H
#define MATHTREE_EXPRESSION_H

#include <memory>
#include <string_view>

namespace MathTree {
class Expression {
    public:
    virtual double evaluate() const = 0;

    Expression const& operator=(Expression const&) = delete;
    Expression&& operator=(Expression&&) = delete;
    virtual ~Expression() = default;

};

class BinaryExpression: public Expression {
    public:
    BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);

    Expression const& left() const;
    Expression const& right() const;

    private:
    std::unique_ptr<Expression> m_left;
    std::unique_ptr<Expression> m_right;
};

class ExpressionFactory final {
    public:
    static std::unique_ptr<Expression> parse(std::string_view input);
    static std::unique_ptr<BinaryExpression> makeBinary(std::unique_ptr<Expression> left,
                                                        std::unique_ptr<Expression> right,
                                                        char operation);

    private:
    ExpressionFactory() {}
};

}

#endif // MATHTREE_EXPRESSION_H