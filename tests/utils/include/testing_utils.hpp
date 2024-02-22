/**
 * @file testing_utils.hpp
 * @author Eduardo Vieira Falcão
 * @brief Declares methods used to assit in the test verifications
 */

#include <algorithm>
#include <cmath>
#include <vector>

namespace fftemb::test_utils
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

/**
 * @brief Find the peaks from the spectrum informed
 *
 * @tparam T The complex type
 * @tparam Container The container type
 * @param[in,out] sequence The sequence of elements
 * @param sampling_period The sampling period used
 * @param max_peaks The number of peaks to be searched for
 * @return A vector of pairs containing peak value and frequency
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
std::vector<std::pair<double, double>>
find_peaks(Container<T>& sequence, std::chrono::nanoseconds sampling_period, int max_peaks)
{
    std::vector<std::pair<double, double>> peak_data(max_peaks);
    const auto t_s           = std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count();
    const int  sequence_size = sequence.size();
    for (int i = 1; i <= sequence_size / 2; ++i) {
        const auto amplitude = 2 * std::abs(sequence[i]) / sequence_size;
        auto       min_it    = std::min_element(peak_data.begin(), peak_data.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        if (min_it != peak_data.end() && amplitude > min_it->first) {
            const auto frequency = i * 1 / (sequence_size * t_s);
            // if the frequency is greater than 2Hz, peak is not a false positive from DC spectral leakage
            if (frequency > 2) {
                auto neighbor_it = std::find_if(peak_data.begin(), peak_data.end(), [frequency](const auto& peak) {
                    // check if peak frequency is not already mapped
                    return std::abs(1 - frequency / peak.second) < 0.05;
                });
                if (neighbor_it != peak_data.end()) {
                    if (amplitude > neighbor_it->first) {
                        *neighbor_it = {static_cast<double>(amplitude), frequency};
                    }
                }
                else {
                    *min_it = {static_cast<double>(amplitude), frequency};
                }
            }
        }
    }
    return peak_data;
}
}  // namespace fftemb::test_utils