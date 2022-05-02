#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <stack>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>

class Expression {
    public:
    virtual double evaluate() const = 0;

    Expression const& operator=(Expression const&) = delete;
    Expression&& operator=(Expression&&) = delete;
    virtual ~Expression() = default;

};

class BinaryExpression: public Expression {
    public:
    BinaryExpression(std::unique_ptr<Expression> left,  std::unique_ptr<Expression> right): 
                                                m_left(std::move(left)), m_right(std::move(right)) {}

    virtual double evaluate() const = 0;

    Expression const& left() const {
        return *m_left;
    }
    Expression const& right() const {
        return *m_right;
    }

    private:
    std::unique_ptr<Expression> m_left;
    std::unique_ptr<Expression> m_right;
};

class Addition: public BinaryExpression {
    public:
    Addition(std::unique_ptr<Expression> left,  std::unique_ptr<Expression> right): 
                                                    BinaryExpression(std::move(left), std::move(right)) {}
    double evaluate() const override {
        return left().evaluate() + right().evaluate();
    }
};

class Subtraction: public BinaryExpression {
    public:
    Subtraction(std::unique_ptr<Expression> left,  std::unique_ptr<Expression> right): 
                                                    BinaryExpression(std::move(left), std::move(right)) {}
    double evaluate() const override {
        return left().evaluate() - right().evaluate();
    }
};

class Multiplication: public BinaryExpression {
    public:
    Multiplication(std::unique_ptr<Expression> left,  std::unique_ptr<Expression> right): 
                                                    BinaryExpression(std::move(left), std::move(right)) {}
    double evaluate() const override {
        return left().evaluate() * right().evaluate();
    }
};

class Division: public BinaryExpression {
    public:
    Division(std::unique_ptr<Expression> left,  std::unique_ptr<Expression> right): 
                                                    BinaryExpression(std::move(left), std::move(right)) {}
    double evaluate() const override {
        return left().evaluate() / right().evaluate();
    }
};

class RealNumber: public Expression {
    public:
    static std::unique_ptr<RealNumber> parse(std::string_view input) {
        double number = 0;
        auto ret = std::from_chars(input.data(), input.data() + input.size(), number);
        return ret.ec == std::errc() ? std::make_unique<RealNumber>(number) : nullptr;
    }

    RealNumber(double value): m_value(value) {};

    double evaluate() const override {
        return m_value;
    }

    private:
    double m_value;
};

class BinaryExpressionFactory final {
    public:
    static std::unique_ptr<BinaryExpression> make(std::unique_ptr<Expression> left,
                                        std::unique_ptr<Expression> right,
                                        char operation) {
        if (left == nullptr || right == nullptr) {
            return nullptr;
        }

        if (operation == '+') {
            return std::make_unique<Addition>(std::move(left), std::move(right));
        } else if (operation == '-') {
            return std::make_unique<Subtraction>(std::move(left), std::move(right));
        } else if (operation == '*') {
            return std::make_unique<Multiplication>(std::move(left), std::move(right));
        } else if (operation == '/') {
            return std::make_unique<Division>(std::move(left), std::move(right));
        }

        return nullptr;
    }

    private:
    BinaryExpressionFactory() {}
};

std::unique_ptr<Expression> buildExpression(std::string_view expression) {
    static std::unordered_set<char> operators{'+','*','/','-'};
    // temporary dirty hack 
    static std::unordered_map<char, int> operatorsPriority{{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}};

    std::cout << "Processing expression " << expression << "\n";
    std::vector<size_t> openBracketsIdx;
    // flag to check if we need to remove extra brackets e.g. (a+b) to a+b
    bool needsParenthesisPruning = (expression.front() == '(');

    for (size_t i = 0; i < expression.size(); ++i) {
        if (expression[i] == '(') {
            openBracketsIdx.push_back(i);
        } else if (expression[i] == ')') {
            assert(openBracketsIdx.size() > 0);
            openBracketsIdx.pop_back();
            if (openBracketsIdx.size() == 0 && i < expression.size() - 1) {
                needsParenthesisPruning = false;
            }
        }
    }
    assert(openBracketsIdx.size() == 0);

    if (needsParenthesisPruning) {
        expression.remove_prefix(1);
        expression.remove_suffix(1);
        std::cout << "Pruned expression " << expression << "\n";
        // handle cases where multiple pruning steps are required e.g. (((a+b)))
        return buildExpression(expression);
    }

    size_t lowestPriorityOperatorIdx = 0;
    size_t topLevelOperatorsCount = 0;
    for (size_t i = 0; i < expression.size(); ++i) {
        if (openBracketsIdx.empty() && operators.count(expression[i])) {
            ++topLevelOperatorsCount;
            if (lowestPriorityOperatorIdx <= 0 ||
                        operatorsPriority[expression[lowestPriorityOperatorIdx]] > operatorsPriority[expression[i]]) {
                lowestPriorityOperatorIdx = i;
            }
        } else if (expression[i] == '(') {
            openBracketsIdx.push_back(i);
        } else if (expression[i] == ')') {
            openBracketsIdx.pop_back();
        }
    }
    if (topLevelOperatorsCount <= 0 || (topLevelOperatorsCount == 1 && lowestPriorityOperatorIdx == 0)) {
        return RealNumber::parse(expression);
    } else {
        auto left = std::string_view(expression.data(), lowestPriorityOperatorIdx);
        auto right = std::string_view(expression.data() + lowestPriorityOperatorIdx + 1,
                                      expression.size() - lowestPriorityOperatorIdx - 1);
        return BinaryExpressionFactory::make(buildExpression(left),
                                             buildExpression(right),
                                             expression[lowestPriorityOperatorIdx]);
    }
}

int main() {
    while(true) {
        std::string input;
        std::cout << "Enter an expression:\n";
        std::getline(std::cin, input);
        auto it = std::remove_if(input.begin(), input.end(),
                                 [](char c) { return std::isspace(c); });
        input.erase(it, input.end());

        std::vector<size_t> openBracketsIdx;
        bool error = false;
        for (size_t i = 0; i < input.size(); ++i) {
            if (input[i] == '(') {
                openBracketsIdx.push_back(i);
            } else if (input[i] == ')') {
                if (openBracketsIdx.size() > 0) {
                    openBracketsIdx.pop_back();
                } else {
                    std::cerr << "Unpaired ')' at index " << i << "\n";
                    error = true;
                }
            }
        }

        error = error || openBracketsIdx.size() > 0;
        for (size_t i = 0; i < openBracketsIdx.size(); ++i) {
            std::cerr << "Unpaired '(' at index " << openBracketsIdx[i] << "\n";
        }
        if (error) {
            return -1;
        }
        
        if (auto expression = buildExpression(input)) {
            std::cout << "Result is " << expression->evaluate() << "\n";
        } else {
            std::cerr << "Invalid input\n";
            return -1;
        }
        std::cout << std::endl;
    }

    return 0;
}