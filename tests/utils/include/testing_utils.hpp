#include <algorithm>
#include <cmath>
#include <vector>

namespace test_utils
{
std::vector<std::pair<double, double>>
calculate_error(const std::vector<std::pair<double, double>>& data,
                const std::vector<std::pair<double, double>>& reference);

void
sort_pairs(std::vector<std::pair<double, double>>& pairs_vector);
}  // namespace test_utils