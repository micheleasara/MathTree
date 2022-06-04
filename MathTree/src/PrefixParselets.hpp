#ifndef MATHTREE_PREFIXPARSELETS
#define MATHTREE_PREFIXPARSELETS

#include "Expression.hpp"
#include <memory>
#include "Token.hpp"

namespace MathTree {
class PrattParser;

class PrefixParselet {
public:
    virtual std::unique_ptr<Expression> parse(PrattParser& parser, Token const& token) = 0;

    PrefixParselet const& operator=(PrefixParselet const&) = delete;
    PrefixParselet&& operator=(PrefixParselet&&) = delete;
    virtual ~PrefixParselet() = default;
};

class NumberParselet: public PrefixParselet {
public:
    std::unique_ptr<Expression> parse(PrattParser&, Token const& token) override;
};

class GroupParselet: public PrefixParselet {
public:
    std::unique_ptr<Expression> parse(PrattParser& parser, Token const&) override;
};

class NegativeSignParselet: public PrefixParselet {
public:
    NegativeSignParselet(int priority);
    std::unique_ptr<Expression> parse(PrattParser& parser, Token const& token) override;

private:
    int m_priority{0}; 
};

class PositiveSignParselet: public PrefixParselet {
public:
    PositiveSignParselet(int priority);
    std::unique_ptr<Expression> parse(PrattParser& parser, Token const& token) override;

private:
    int m_priority{0}; 
};

}

#endif // MATHTREE_PREFIXPARSELETS