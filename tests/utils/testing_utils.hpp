#include <algorithm>
#include <cmath>
#include <vector>

namespace test_utils
{
std::vector<std::pair<double, double>>
CalculateError(const std::vector<std::pair<double, double>>& data,
               const std::vector<std::pair<double, double>>& reference);

void
SortPairs(std::vector<std::pair<double, double>>& pairs_vector);
}  // namespace test_utils