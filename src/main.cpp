#include <algorithm>
#include <chrono>
#include <complex>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "dsp_utils.hpp"
#include "fft.hpp"
#include "fft_types.hpp"
#include "signal_generator.hpp"

int
main()
{
    // sine parameters (amplitude, frequency)
    std::vector<std::pair<double, double>> parameters{{10, 60}};

    // signal generator parameters (mind the trade-off in between frequency/time resolution)
    std::chrono::nanoseconds sampling_period
        = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(0.001));
    std::chrono::milliseconds duration(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(2)));

    // create signal container
    std::vector<Complex> signal;

    // create signal generator
    std::shared_ptr<SignalGenerator> generator = std::make_shared<SignalGenerator>(duration, sampling_period);

    // generate sine function
    generator->GenerateSines(signal, parameters);

    // normalize
    std::shared_ptr<DSPUtils> dsp_utils     = std::make_shared<DSPUtils>();
    auto                      max_amplitude = dsp_utils->Normalize(signal);

    // apply the Hann window
    generator->ApplyHannWindow(signal);

    // compute FFT
    std::shared_ptr<FFT> fft = std::make_shared<FFT>(dsp_utils);
    fft->Compute(signal);

    // calculate peak
    auto peak_data = dsp_utils->FindPeaks(signal, sampling_period, parameters.size());
    std::cout << "Peak: " << peak_data[0].first * max_amplitude << std::endl
              << "Peak freq: " << peak_data[0].second << std::endl;

    return 0;
}
