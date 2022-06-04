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

void printError(size_t idx, MathTree::Validator::Errors error);

int main() {
    using namespace MathTree;

    while(true) {
        std::string input;
        std::cout << "Enter an expression:\n";
        std::getline(std::cin, input);

        auto idxErrorPairs = Validator::validate(input);
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

        Parser parser(std::make_unique<ArithmeticLexer>(input));

        parser.setPrefixParselet(TokenType::PLUS, std::make_unique<PositiveSignParselet>(2));
        parser.setPrefixParselet(TokenType::MINUS, std::make_unique<NegativeSignParselet>(2));
        parser.setPrefixParselet(TokenType::NUMBER, std::make_unique<NumberParselet>());
        parser.setPrefixParselet(TokenType::OPENING_BRACKET, std::make_unique<GroupParselet>());

        parser.setInfixParselet(TokenType::PLUS, std::make_unique<AdditionParselet>(1));
        parser.setInfixParselet(TokenType::MINUS, std::make_unique<SubtractionParselet>(1));
        parser.setInfixParselet(TokenType::ASTERISK, std::make_unique<MultiplicationParselet>(2));
        parser.setInfixParselet(TokenType::SLASH, std::make_unique<DivisionParselet>(2));
        parser.setInfixParselet(TokenType::CARET, std::make_unique<ExponentiationParselet>(3));

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

void printError(size_t idx, MathTree::Validator::Errors error) {
    using MathTree::Validator;
    switch (error) {
        case Validator::Errors::UnpairedClosingBracket:
            std::cerr << "Unpaired ')' at index " << idx << ".\n"; break;
        case Validator::Errors::UnpairedOpeningBracket:
            std::cerr << "Unpaired '(' at index " << idx << ".\n"; break;
        case Validator::Errors::MissingOperator:
            std::cerr << "Missing operator at index " << idx << ".\n"; break;
        case Validator::Errors::TwoOperatorsInARow:
            std::cerr << "Invalid second operator at index " << idx << ".\n"; break;
        case Validator::Errors::IncompleteOperation:
            std::cerr << "Operation with a missing operand at index " << idx << ".\n"; break;
        case Validator::Errors::UnrecognisedSymbol:
            std::cerr << "Unrecognised symbol at index " << idx << ".\n"; break;
        default:
            std::cerr << "Unknown error at index " << idx << ".\n"; break;
    }
}