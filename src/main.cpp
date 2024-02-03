#include <algorithm>
#include <chrono>
#include <complex>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "DSPUtils.hpp"
#include "FFT.hpp"
#include "SignalGenerator.hpp"

using Complex = std::complex<double>;

std::pair<double, double>
ComputePeakData(std::vector<Complex>& signal, std::chrono::nanoseconds sampling_period)
{
    std::pair<double, double> peak_data   = {0, 0};
    auto                      signal_size = signal.size();
    for (int i = 1; i <= signal_size / 2; ++i) {
        auto amplitude = 2 * std::abs(signal[i]) / signal_size;
        if (amplitude > peak_data.first) {
            peak_data = {amplitude,
                         i * 1
                             / (signal_size
                                * std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count())};
        }
    }
    return peak_data;
}

int
main()
{
    // sine parameters (amplitude, frequency)
    std::vector<std::pair<double, double>> parameters{{5, 60}};

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

    // apply the Hann window
    generator->ApplyHannWindow(signal);

    // compute FFT
    std::shared_ptr<IDSPUtils> dsp_utils = std::make_shared<DSPUtils>();
    std::shared_ptr<IFFT>      fft       = std::make_shared<FFT>(dsp_utils);
    fft->Compute(signal);

    // calculate peak
    auto peak_data = ComputePeakData(signal, sampling_period);
    std::cout << "Peak: " << peak_data.first << std::endl << "Peak freq: " << peak_data.second << std::endl;

    return 0;
}
