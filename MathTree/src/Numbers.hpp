#ifndef MATHTREE_NUMBERS_H
#define MATHTREE_NUMBERS_H

#include "Expression.hpp"
#include <string_view>

namespace MathTree {

class RealNumber: public Expression 
{
    public:
    static std::unique_ptr<RealNumber> parse(std::string_view input);

    RealNumber(double value);

    double evaluate() const override;

    private:
    double m_value;
};

}

#endif // MATHTREE_NUMBERS_H