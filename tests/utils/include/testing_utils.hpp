/**
 * @file testing_utils.hpp
 * @author Eduardo Vieira Falcáo
 * @brief Declares methods used to assit in the test verifications
 */

#include <algorithm>
#include <cmath>
#include <vector>

namespace test_utils
{
/**
 * @brief Calculate the accuracy error in between the peak values detected and the reference values
 *
 * @param data The peak values detected as a vector of [amplitude, frequency]
 * @param reference The reference values as a vector of [amplitude, frequency]
 * @return A vector of errors [amplitude error, frequency error]
 */
std::vector<std::pair<double, double>>
calculate_error(const std::vector<std::pair<double, double>>& data,
                const std::vector<std::pair<double, double>>& reference);

/**
 * @brief Sorts a pair based on its first element
 *
 * @param pairs_vector The vector of pairs
 */
void
sort_pairs(std::vector<std::pair<double, double>>& pairs_vector);
}  // namespace test_utils