#ifndef MATHTREE_EXPRESSION_H
#define MATHTREE_EXPRESSION_H

#include <memory>
#include <string_view>
#include <utility>
#include <vector>

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
    BinaryExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);

    Expression const& left() const;
    Expression const& right() const;

    private:
    std::shared_ptr<Expression> m_left;
    std::shared_ptr<Expression> m_right;
};

class ExpressionFactory final {
    public:

    enum class ValidationErrors {
        UnpairedOpeningBracket,
        UnpairedClosingBracket,
        TwoOperatorsInARow,
        IncompleteOperation
    };
    using IndexErrorPairs = std::vector<std::pair<size_t, ValidationErrors>>;

    static IndexErrorPairs validate(std::string_view input);
    static std::shared_ptr<Expression> parse(std::string_view input);
    static std::shared_ptr<BinaryExpression> makeBinary(std::shared_ptr<Expression> left,
                                                        std::shared_ptr<Expression> right,
                                                        char operation);

    private:
    ExpressionFactory() {}
};

}

#endif // MATHTREE_EXPRESSION_H