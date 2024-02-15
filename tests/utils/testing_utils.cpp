#include <algorithm>
#include <cmath>
#include <vector>

namespace test_utils
{
std::vector<std::pair<double, double>>
calculate_error(const std::vector<std::pair<double, double>>& data,
                const std::vector<std::pair<double, double>>& reference)
{
    std::vector<std::pair<double, double>> error_vector;
    for (int i = 0; i <= data.size() - 1; ++i) {
        error_vector.emplace_back(std::make_pair(std::abs(1 - data[i].first / reference[i].first),
                                                 std::abs(1 - data[i].second / reference[i].second)));
    }
    return error_vector;
}

void
sort_pairs(std::vector<std::pair<double, double>>& pairs_vector)
{
    return std::sort(pairs_vector.begin(), pairs_vector.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
}
}  // namespace test_utils