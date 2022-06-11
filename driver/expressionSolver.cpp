#include <algorithm>
#include <cctype>
#include <iostream>
#include "Expression.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <unordered_set>
#include <stack>
#include <string>
#include <vector>

void printError(size_t idx, MathTree::ArithmeticParser::Errors error);

int main() {
    using namespace MathTree;

    while(true) {
        std::string input;
        std::cout << "Enter an expression:\n";
        std::getline(std::cin, input);

        auto idxErrorPairs = ArithmeticParser::validate(input);
        std::sort(idxErrorPairs.begin(), idxErrorPairs.end(), [](auto const& leftPair, 
                                                                 auto const& rightPair) {
            return leftPair.first < rightPair.first;
        });
        for (auto const& [idx, error]: idxErrorPairs) {
            printError(idx, error);
        }
        if (!idxErrorPairs.empty()) {
            std::cout << std::endl;
            continue;
        }

        ArithmeticParser parser(std::move(input));
        try {
            auto expression = parser.parse();
            std::cout << "Expression parsed as ";
            expression->print(std::cout);
            std::cout << "\n";
            auto result = expression->evaluate();
            std::cout << "Result is " << result;
        } catch (std::logic_error const& ex) {
            std::cerr << "Logic error. " << ex.what();
        }
        std::cout << "\n" << std::endl;
    }

    return 0;
}

void printError(size_t idx, MathTree::ArithmeticParser::Errors error) {
    using MathTree::ArithmeticParser;
    switch (error) {
        case ArithmeticParser::Errors::UnpairedClosingBracket:
            std::cerr << "Unpaired ')' at index " << idx << ".\n"; break;
        case ArithmeticParser::Errors::UnpairedOpeningBracket:
            std::cerr << "Unpaired '(' at index " << idx << ".\n"; break;
        case ArithmeticParser::Errors::MissingOperator:
            std::cerr << "Missing operator at index " << idx << ".\n"; break;
        case ArithmeticParser::Errors::IncompleteOperation:
            std::cerr << "Operation with a missing operand at index " << idx << ".\n"; break;
        case ArithmeticParser::Errors::UnrecognisedSymbol:
            std::cerr << "Unrecognised symbol at index " << idx << ".\n"; break;
        case ArithmeticParser::Errors::NothingBetweenBrackets:
            std::cerr << "Nothing between brackets starting at index " << idx << ".\n"; break;
        default:
            std::cerr << "Unknown error at index " << idx << ".\n"; break;
    }
}