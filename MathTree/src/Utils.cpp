#include <charconv>
#include <cmath>
#include "Utils.hpp"

namespace MathTree {

namespace Utils {

std::optional<double> parseDouble(std::string_view input) {
  double number = 0;

  if (input.size() && input.front() == '+') {
    // std::from_chars does not accept '+' outside of the exponent
    input.remove_prefix(1);
  }

  auto [ptr, ec] = std::from_chars(input.data(), input.data() + input.size(), number,
                    std::chars_format::fixed | std::chars_format::hex);
  if (ec != std::errc() || ptr != &(input.back())+1) {
    return std::nullopt;
  }

  return number;
}

}

}