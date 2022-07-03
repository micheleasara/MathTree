#ifndef MATHTREE_ABSTRACT_PARSER_MOCK_H
#define MATHTREE_ABSTRACT_PARSER_MOCK_H

#include "Expression.hpp"
#include "gmock/gmock.h"
#include "Parser.hpp"
#include "Token.hpp"

class AbstractPrattParserMock: public MathTree::AbstractPrattParser {
public:
  MOCK_METHOD(std::unique_ptr<MathTree::Expression>, parse, ());
  MOCK_METHOD(std::unique_ptr<MathTree::Expression>, parse, (int));
  MOCK_METHOD(MathTree::Token, consumeCurrentToken, ());
};
using NiceAbstractPrattParserMock = ::testing::NiceMock<AbstractPrattParserMock>;

#endif // MATHTREE_ABSTRACT_PARSER_MOCK_H