/**
 * @file signal_generator.hpp
 * @author Eduardo Vieira Falcão
 * @brief Implements the Signal Generator class
 */

#ifndef H_SIGNAL_GENERATOR_HPP
#define H_SIGNAL_GENERATOR_HPP

#include <math.h>
#include <numbers>
#include <vector>
#include "etl/vector.h"
#include "fft_types.hpp"

namespace fftemb
{
/**
 * @brief Contain methods that generate different time-valued signals
 *
 * @tparam T The complex number type
 * @tparam Container The containe type
 */
template <typename T = Complex, template <class...> class Container = etl::ivector>
class SignalGenerator
{
public:
    SignalGenerator() = delete;

    /**
     * @brief Construct a new Signal Generator object
     *
     * @param duration The duration of the signal
     * @param sampling_period The sampling period of the signal
     */
    SignalGenerator(std::chrono::nanoseconds duration, std::chrono::nanoseconds sampling_period)
      : m_duration(duration)
      , m_sampling_period(sampling_period){};

    /**
     * @brief Generate a sine wave
     *
     * @param[in,out] signal The signal
     * @param parameters The pairs of [amplitude, frequency]
     */
    void
    generate_sine_wave(Container<T>& signal, const std::vector<std::pair<double, double>>& parameters) const;

    /**
     * @brief Generate a square wave
     *
     * @param signal The signal
     * @param frequency The square wave frequency
     */
    void
    generate_square_wave(Container<T>& signal, double frequency) const;

private:
    /// @brief The duration of the signals generated
    std::chrono::nanoseconds m_duration;
    /// @brief The sampling period of the signal
    std::chrono::nanoseconds m_sampling_period;
};

template <typename T, template <class...> class Container>
void
SignalGenerator<T, Container>::generate_sine_wave(Container<T>&                                 signal,
                                                  const std::vector<std::pair<double, double>>& parameters) const
{
    int i = 0;
    for (std ::chrono::nanoseconds dt(0); dt <= m_duration; dt += m_sampling_period, ++i) {
        double signal_value = 0;
        for (const auto& signal_params : parameters) {
            signal_value
                += signal_params.first
                   * std::sin(2 * std::numbers::pi * signal_params.second * std::chrono::duration<double>(dt).count());
        }
        signal[i] = T{signal_value, 0};
    }
}

template <typename T, template <class...> class Container>
void
SignalGenerator<T, Container>::generate_square_wave(Container<T>& signal, double frequency) const
{
    int i = 0;
    for (std::chrono::nanoseconds dt(0); dt <= m_duration; dt += m_sampling_period, ++i) {
        double signal_value = 0;
        signal_value
            = std::sin(2 * std::numbers::pi * frequency * std::chrono::duration<double>(dt).count()) >= 0 ? 1 : -1;
        signal[i] = T{signal_value, 0};
    }
}

}  // namespace fftemb

#endif  // H_SIGNAL_GENERATOR_HPP