#include <charconv>
#include "Numbers.hpp"

namespace MathTree {

std::shared_ptr<RealNumber> RealNumber::parse(std::string_view input) {
    double number = 0;
    if (input.size() && input.front() == '+') {
        input.remove_prefix(1);
    }
    auto ret = std::from_chars(input.data(), input.data() + input.size(), number);
    return ret.ec == std::errc() ? std::make_shared<RealNumber>(number) : nullptr;
}

RealNumber::RealNumber(double value): m_value(value) {};

double RealNumber::evaluate() const {
    return m_value;
}

}