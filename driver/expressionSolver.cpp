#include <algorithm>
#include <cctype>
#include <iostream>
#include "Expression.hpp"
#include <unordered_set>
#include <stack>
#include <string>
#include <vector>

int main() {
    using namespace MathTree;

    while(true) {
        std::string input;
        std::cout << "Enter an expression:\n";
        std::getline(std::cin, input);

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

        auto it = std::remove_if(input.begin(), input.end(),
                            [](char c) { return std::isspace(c); });
        input.erase(it, input.end());

        if (auto expression = ExpressionFactory::parse(input)) {
            std::cout << "Result is " << expression->evaluate() << "\n";
        } else {
            std::cerr << "Invalid input\n";
            return -1;
        }
        std::cout << std::endl;
    }

    return 0;
}