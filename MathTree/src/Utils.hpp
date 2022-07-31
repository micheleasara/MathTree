#ifndef MATHTREE_UTILS
#define MATHTREE_UTILS

#include <optional>
#include <string_view>

namespace MathTree {

namespace Utils {

/// Returns a signed double if the input string only contains that. Otherwise returns std::nullopt.
std::optional<double> parseDouble(std::string_view input);
/// Returns the first signed double if the input string contains one. Otherwise returns std::nullopt.
std::optional<double> parseFirstDouble(std::string_view input);

}

}

#endif // MATHTREE_UTILS