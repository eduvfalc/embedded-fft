/**
 * @file example.cpp
 * @author Eduardo Vieira Falcão
 * @brief Example of FFT calculation
 */

#include <chrono>
#include <iostream>
#include "../tests/utils/include/signal_generator.hpp"
#include "../tests/utils/include/testing_utils.hpp"
#include "dsp_utils.hpp"
#include "etl/vector.h"
#include "fft.hpp"
#include "fft_types.hpp"

using namespace fftemb;

/**
 * @brief The main
 *
 * @return 0
 */
int
main()
{
    // sine parameters (amplitude, frequency)
    std::vector<std::pair<double, double>> parameters{{10, 60}};

    // signal generator parameters (mind the trade-off in between frequency/time resolution)
    std::chrono::nanoseconds sampling_period
        = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(0.001));
    std::chrono::milliseconds duration(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(0.1)));

    // create signal container
    etl::vector<Complex, 128> signal(128);

    // create signal generator
    std::shared_ptr<SignalGenerator<Complex, etl::ivector>> generator
        = std::make_shared<SignalGenerator<Complex, etl::ivector>>(duration, sampling_period);

    // generate sine function
    generator->generate_sine_wave(signal, parameters);

    // normalize
    auto max_amplitude = fft_utils::normalize(signal);

    // apply the Hann window
    fft_utils::apply_hann_window(signal);

    // compute FFT
    compute(signal);

    // calculate peak
    auto peak_data = test_utils::find_peaks(signal, sampling_period, parameters.size());
    std::cout << "Peak: " << peak_data[0].first * max_amplitude << std::endl
              << "Peak freq: " << peak_data[0].second << std::endl;

    return 0;
}
