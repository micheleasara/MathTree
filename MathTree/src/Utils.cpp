#include <charconv>
#include <cmath>
#include "Utils.hpp"

namespace MathTree {

namespace Utils {

auto signedFromChars(std::string_view input, double& number,
                                             std::chars_format format = std::chars_format::fixed) {
  if (input.size() && input.front() == '+') {
    // std::from_chars does not accept '+' outside of the exponent
    input.remove_prefix(1);
  }

  return std::from_chars(input.data(), input.data() + input.size(), number, format);
}

std::optional<double> parseDouble(std::string_view input) {
  double number = 0;
  auto [ptr, ec] = signedFromChars(input, number);
  if (ec != std::errc() || ptr != input.data() + input.size()) {
    return std::nullopt;
  }

  return number;
}

std::optional<double> parseFirstDouble(std::string_view input) {
  double number = 0;
  auto ret = signedFromChars(input, number);
  if (ret.ec != std::errc()) {
    return std::nullopt;
  }

  return number;
}

}

}