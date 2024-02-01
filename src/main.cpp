#include <algorithm>
#include <chrono>
#include <complex>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "FFT.hpp"
#include "FFTUtils.hpp"
#include "SignalGenerator.hpp"

using Complex = std::complex<double>;

int
main()
{
    // sine parameters (amplitude, frequency)
    std::pair<double, double> parameters = {10, 30};

    // signal generator parameters (mind the trade-off in between frequency/time resolution)
    std::chrono::nanoseconds sampling_period
        = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(0.001));
    std::chrono::milliseconds duration(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1)));

    // create signal container
    std::vector<Complex> signal = {};

    // create signal generator
    std::shared_ptr<SignalGenerator> generator = std::make_shared<SignalGenerator>(duration, sampling_period);

    // generate sine function
    generator->GenerateSine(signal, parameters);

    // apply the Hann window
    generator->ApplyHannWindow(signal);

    // keep actual signal size (before eventual zero padding)
    int signal_size = signal.size();

    // compute FFT
    std::shared_ptr<IFFTUtils> fft_utils = std::make_shared<FFTUtils>();
    std::shared_ptr<IFFT>      fft       = std::make_shared<FFT>(fft_utils);
    fft->Compute(signal);

    // calculate peak
    std::pair<double, double> peakData = {0, 0};
    for (int i = 1; i <= signal_size / 2; ++i) {
        auto amplitude = 2 * std::abs(signal[i]) / signal_size;
        if (amplitude > peakData.first) {
            peakData = {amplitude,
                        i * 1
                            / (signal_size
                               * std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count())};
        }
    }

    std::cout << "Peak: " << peakData.first << std::endl << "Peak freq: " << peakData.second << std::endl;

    return 0;
}
