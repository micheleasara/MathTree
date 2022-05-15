#include <algorithm>
#include <cctype>
#include <iostream>
#include "Expression.hpp"
#include <unordered_set>
#include <stack>
#include <string>
#include <vector>

int main() {
    using MathTree::ExpressionFactory;

    while(true) {
        std::cout << "\n";
        std::string input;
        std::cout << "Enter an expression:\n";
        std::getline(std::cin, input);

        auto idxErrorPairs = ExpressionFactory::validate(input);
        for (auto const& [idx, error]: idxErrorPairs) {
            switch (error) {
                case ExpressionFactory::ValidationErrors::UnpairedClosingBracket:
                    std::cerr << "Unpaired ')' at index " << idx << "\n"; break;
                case ExpressionFactory::ValidationErrors::UnpairedOpeningBracket:
                    std::cerr << "Unpaired '(' at index " << idx << "\n"; break;
                case ExpressionFactory::ValidationErrors::MissingOperator:
                    std::cerr << "Missing operator at index " << idx << "\n"; break;
                case ExpressionFactory::ValidationErrors::TwoOperatorsInARow:
                    std::cerr << "Invalid second operator at index " << idx << "\n"; break;
                case ExpressionFactory::ValidationErrors::IncompleteOperation:
                    std::cerr << "Operation with a missing operand at index " << idx << "\n"; break;
                default:
                    std::cerr << "Unknown error at index " << idx << "\n"; break;
            }
        }
        if (!idxErrorPairs.empty()) {
            continue;
        }

        auto it = std::remove_if(input.begin(), input.end(),
                            [](char c) { return std::isspace(c); });
        input.erase(it, input.end());

        if (auto expression = ExpressionFactory::parse(input)) {
            std::cout << "Result is " << expression->evaluate() << "\n";
        } else {
            std::cerr << "Invalid input\n";
            continue;
        }
        std::cout << std::endl;
    }

    return 0;
}