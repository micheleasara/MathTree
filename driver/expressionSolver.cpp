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
bool userWantsToContinue();

int main() {
  using namespace MathTree;
  ArithmeticParser parser;

  do {
    std::string input;
    std::cout << "Parentheses and the following operators are supported:\n";
    std::cout << "+ (addition), - (subtraction), * (multiplication), / (division)\n";
    std::cout << "^ (exponentiation), sqrt (square root), log (logarithm base 10) and log_n (logarithm base n).\n\n";
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

    try {
      auto expression = parser.parse(std::move(input));
      std::cout << "Expression parsed as " << *expression << "\n";
      auto result = expression->evaluate();
      std::cout << "Result is " << result;
    } catch (std::logic_error const& ex) {
      std::cerr << "Logic error. " << ex.what();
    }
    std::cout << "\n\n";
  } while (userWantsToContinue());

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

bool userWantsToContinue() {
  std::string input;
  do {
    std::cout << "Continue? (y/n)\n";
    std::getline(std::cin, input);
  } while (input != "y" && input != "n");
  
  auto wantsToContinue = input == "y";
  if (wantsToContinue) {
    std::cout << "\n";
  }
  return wantsToContinue;
}