#include <charconv>
#include <cmath>
#include "Numbers.hpp"
#include <stdexcept>

namespace MathTree {

std::shared_ptr<RealNumber> RealNumber::parse(std::string_view input) {
    double number = 0;
    if (input.size() && input.front() == '+') {
        input.remove_prefix(1);
    }
    auto ret = std::from_chars(input.data(), input.data() + input.size(), number);
    return ret.ec == std::errc() ? std::make_shared<RealNumber>(number) : nullptr;
}

RealNumber::RealNumber(double value) {
    if (std::isinf(value) || std::isnan(value)) {
        throw std::domain_error("Cannot represent non-numbers or infinity as numbers.");
    }
    m_value = value;
};

double RealNumber::evaluate() const {
    return m_value;
}

}