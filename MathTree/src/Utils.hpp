#ifndef MATHTREE_UTILS
#define MATHTREE_UTILS

#include <optional>
#include <string_view>

namespace MathTree {

namespace Utils {

std::optional<double> parseDouble(std::string_view input);

}

}

#endif // MATHTREE_UTILS