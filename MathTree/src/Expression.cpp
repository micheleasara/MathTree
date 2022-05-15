#include <algorithm>
#include <cassert>
#include <cctype>
#include "Expression.hpp"
#include "Operations.hpp"
#include "Numbers.hpp"
#include <iostream>
#include <memory>
#include <unordered_set>
#include <string>
#include <string_view>
#include <vector>

namespace MathTree {
// temporary dirty hack
std::unordered_map<char, int> operatorsPriority{{'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}};

BinaryExpression::BinaryExpression(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right): 
                                                                        m_left(std::move(left)), m_right(std::move(right)) {}

Expression const& BinaryExpression::left() const {
    return *m_left;
}
Expression const& BinaryExpression::right() const {
    return *m_right;
}

ExpressionFactory::IndexErrorPairs ExpressionFactory::validate(std::string_view input) {
    if (input.size() <= 0) {
        return IndexErrorPairs{};
    }

    IndexErrorPairs idxErrorPairs;
    std::vector<size_t> openBracketsIdx;
    bool error = false;
    int lastNonSpaceIdx = -1;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '(') {
            openBracketsIdx.push_back(i);
            if (lastNonSpaceIdx > -1 && std::isdigit(input[lastNonSpaceIdx])) {
                idxErrorPairs.emplace_back(i, ValidationErrors::MissingOperator);
            }
        } else if (input[i] == ')') {
            if (openBracketsIdx.size() > 0) {
                openBracketsIdx.pop_back();
            } else {
                idxErrorPairs.emplace_back(i, ValidationErrors::UnpairedClosingBracket);
            }
            if (lastNonSpaceIdx > -1 && operatorsPriority.count(input[lastNonSpaceIdx])) {
                idxErrorPairs.emplace_back(lastNonSpaceIdx, ValidationErrors::IncompleteOperation);
            }
        } else if (operatorsPriority.count(input[i]) && 
                                lastNonSpaceIdx > -1 && operatorsPriority.count(input[lastNonSpaceIdx])) {
            idxErrorPairs.emplace_back(i, ValidationErrors::TwoOperatorsInARow);
        } else if (std::isdigit(input[i]) && lastNonSpaceIdx > -1 && input[lastNonSpaceIdx] == ')') {
            idxErrorPairs.emplace_back(i, ValidationErrors::MissingOperator);
        }

        if (!std::isspace(input[i])) {
            lastNonSpaceIdx = static_cast<int>(i);
        }
    }

    if (lastNonSpaceIdx > -1 && operatorsPriority.count(input[lastNonSpaceIdx])) {
        idxErrorPairs.emplace_back(lastNonSpaceIdx, ValidationErrors::IncompleteOperation);
    }

    for (size_t i = 0; i < openBracketsIdx.size(); ++i) {
        idxErrorPairs.emplace_back(openBracketsIdx[i], ValidationErrors::UnpairedOpeningBracket);
    }

    return idxErrorPairs;
}

std::shared_ptr<Expression> buildExpression(std::string_view expression) {
    static std::unordered_set<char> signs{'+','-'};

    std::cout << "Processing expression " << expression << "\n";
    std::vector<size_t> openBracketsIdx;

    if (expression.size() <= 0) {
        return nullptr;
    }

    std::string modifiedExpression;
    if (signs.count(expression.front()) && 
                                expression.size() > 1 && expression[1] == '(') {
        // deal with signs in front of parentheses
        auto sign = expression.front();
        expression.remove_prefix(1);
        if (sign == '-') {
            modifiedExpression = sign + std::string("1*") + std::string(expression);
            expression = modifiedExpression;
        }
    }

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
        if (openBracketsIdx.empty() && operatorsPriority.count(expression[i])) {
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
        return ExpressionFactory::makeBinary(buildExpression(left),
                                                buildExpression(right),
                                                expression[lowestPriorityOperatorIdx]);
    }
}

std::string wrapNonAssociativeOperators(std::string_view input) {
    static int MAX_INSERTION_PER_OPERATOR = 3;
    static std::unordered_map<char, std::string> nonAssocOperMap{{'-', "+("}, {'/', "*(1"}};
    
    // contains the number of opening brackets which still need to be closed and that were
    // added during pre-processing, with the index indicating the level in the unprocessed input
    // a level begins with an opening bracket and ends with a closing bracket
    std::vector<size_t> pendingClosuresPerLevel{0};

    std::string processedInput;
    processedInput.reserve(MAX_INSERTION_PER_OPERATOR * input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        assert(pendingClosuresPerLevel.size() > 0); // at least one level at all times
        if (pendingClosuresPerLevel.back() > 0 &&
                                    (input[i] == ')' || operatorsPriority.count(input[i]))) {
            processedInput.append(pendingClosuresPerLevel.back(), ')');
            pendingClosuresPerLevel.back() = 0;
        }

        if (input[i] == ')') {
            pendingClosuresPerLevel.pop_back();
        } else if (input[i] == '(') {
            pendingClosuresPerLevel.push_back(0);
        } else if (nonAssocOperMap.count(input[i]) && pendingClosuresPerLevel.back() <= 0) {
            processedInput.append(nonAssocOperMap[input[i]]);
            ++pendingClosuresPerLevel.back();
        }
        processedInput.append(1, input[i]);
    }
    if (pendingClosuresPerLevel.back() > 0) {
        processedInput.append(pendingClosuresPerLevel.back(), ')');
    }

    return processedInput;
}

std::shared_ptr<BinaryExpression> ExpressionFactory::makeBinary(std::shared_ptr<Expression> left,
                                                                    std::shared_ptr<Expression> right,
                                                                    char operation) {
    if (left == nullptr || right == nullptr) {
        return nullptr;
    }

    if (operation == '+') {
        return std::make_shared<Addition>(std::move(left), std::move(right));
    } else if (operation == '-') {
        return std::make_shared<Subtraction>(std::move(left), std::move(right));
    } else if (operation == '*') {
        return std::make_shared<Multiplication>(std::move(left), std::move(right));
    } else if (operation == '/') {
        return std::make_shared<Division>(std::move(left), std::move(right));
    }

    return nullptr;
}

std::shared_ptr<Expression> ExpressionFactory::parse(std::string_view input) {
    auto processedInput = wrapNonAssociativeOperators(input);
    return buildExpression(processedInput);
}
}