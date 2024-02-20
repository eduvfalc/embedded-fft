/**
 * @file dsp_utils.hpp
 * @author Eduardo Vieira Falcão
 * @brief Contains the definition of the template class DSPUtils
 */

#ifndef H_DSP_UTILS_HPP
#define H_DSP_UTILS_HPP

#include <chrono>
#include <cmath>
#include <complex>
#include <numbers>
#include <vector>
#include "etl/vector.h"
#include "fft_types.hpp"

namespace fftemb
{
/**
 * @brief Helper class for digital signal processing subtasks
 *
 * @tparam T The complex number type
 * @tparam Container The containe type
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
class DSPUtils
{
public:
    /**
     * @brief Construct a new DSPUtils object
     *
     */
    DSPUtils() = default;

    /**
     * @brief Construct a new DSPUtils object
     *
     * @param max_frequency_delta_pct Minimum percentual frequency difference used to qualify two adjacent peaks as a
     * different peak
     * @param min_dc_leakage_frequency_hz Maximum frequency value that defines a DC component and its spectral leakage
     */
    DSPUtils(double max_frequency_delta_pct, double max_dc_leakage_frequency_hz)
      : m_min_frequency_delta_pct(max_frequency_delta_pct)
      , m_max_dc_leakage_frequency_hz(min_dc_leakage_frequency_hz){};

    /**
     * @brief Apply the bit reversal permutation to the container
     *
     * @param[in,out] sequence The sequence of elements
     */
    void
    bit_reversal(Container<T>& sequence);

    /**
     * @brief Append zeros to the sequence until the next power of 2, if applicable
     *
     * @param[in,out] sequence The sequence of elements
     */
    void
    zero_padding(Container<T>& sequence);

    /**
     * @brief Find the peaks from the spectrum informed
     *
     * @param[in,out] sequence The sequence of elements
     * @param sampling_period The sampling period used
     * @param max_peaks The number of peaks to be searched for
     * @return A vector of pairs containing peak value and frequency
     */
    std::vector<std::pair<double, double>>
    find_peaks(Container<T>& signal, std::chrono::nanoseconds sampling_period, int max_peaks);

    /**
     * @brief Normalize a sequence of complex numbers
     *
     * @param[in,out] sequence The sequence of elements
     * @return The longest norm of the sequence
     */
    double
    normalize(Container<T>& sequence);

    /**
     * @brief Sequence to mulitply by the Hann window
     *
     * @param[in,out] sequence The sequence of elements
     */
    void
    apply_hann_window(Container<T>& sequence) const;

private:
    /// @brief Minimum percentual frequency difference used to qualify two adjacent peaks as a different peak
    double m_min_frequency_delta_pct = 0.05;
    /// @brief Maximum frequency value that defines a DC component and its spectral leakage
    double m_max_dc_leakage_frequency_hz = 2;
};

template <typename T, template <class...> class Container>
void
DSPUtils<T, Container>::bit_reversal(Container<T>& signal)
{
    auto n      = signal.size();
    int  levels = log2(n);

    for (int i = 0; i < n; ++i) {
        int j = 0;
        for (int bit = 0; bit < levels; ++bit) {
            if (i & (1 << bit)) {
                j |= (1 << (levels - 1 - bit));
            }
        }
        if (j > i) {
            std::swap(signal[i], signal[j]);
        }
    }
}

template <typename T, template <class...> class Container>
void
DSPUtils<T, Container>::zero_padding(Container<T>& signal)
{
    int signalSize = signal.size();
    int nextPow2   = 1;
    while (nextPow2 < signalSize) {
        nextPow2 <<= 1;
    }

    if (nextPow2 > signalSize) {
        signal.resize(nextPow2, {0, 0});
    }
}

template <typename T, template <class...> class Container>
double
DSPUtils<T, Container>::normalize(Container<T>& signal)
{
    auto max_it        = std::max_element(signal.begin(), signal.end(), [](const T& b1, const T& b2) {
        return std::abs(b1) < std::abs(b2);
    });
    auto max_amplitude = std::abs(*max_it);
    std::transform(signal.begin(), signal.end(), signal.begin(), [max_amplitude](auto& bin) {
        return T(cnl::quotient(bin.real(), max_amplitude), cnl::quotient(bin.imag(), max_amplitude));
    });
    return static_cast<double>(max_amplitude);
}

template <typename T, template <class...> class Container>
std::vector<std::pair<double, double>>
DSPUtils<T, Container>::find_peaks(Container<T>& signal, std::chrono::nanoseconds sampling_period, int max_peaks)
{
    std::vector<std::pair<double, double>> peak_data(max_peaks);
    const auto t_s         = std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count();
    const int  signal_size = signal.size();
    for (int i = 1; i <= signal_size / 2; ++i) {
        const auto amplitude = 2 * std::abs(signal[i]) / signal_size;
        auto       min_it    = std::min_element(peak_data.begin(), peak_data.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        if (min_it != peak_data.end() && amplitude > min_it->first) {
            const auto frequency = i * 1 / (signal_size * t_s);
            if (frequency > m_max_dc_leakage_frequency_hz) {
                auto neighbor_it
                    = std::find_if(peak_data.begin(), peak_data.end(), [frequency, this](const auto& peak) {
                          return std::abs(1 - frequency / peak.second) < m_min_frequency_delta_pct;
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

template <typename T, template <class...> class Container>
void
DSPUtils<T, Container>::apply_hann_window(Container<T>& signal) const
{
    T    correction_fator{2, 0};
    auto signal_size = signal.size();
    for (int i = 0; i <= signal_size; ++i) {
        T hanning_bin = {0.5 - 0.5 * std::cos(2 * std::numbers::pi * i / signal_size), 0};
        signal[i]     = signal[i] * hanning_bin * correction_fator;
    }
}

}  // namespace fftemb

#endif  // H_DSP_UTILS_HPP
