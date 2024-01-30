#include "FFT.hpp"
#include "FFTUtils.hpp"
#include "SignalGenerator.hpp"
#include <algorithm>
#include <chrono>
#include <complex>
#include <iostream>
#include <memory>
#include <vector>

using Complex = std::complex<double>;

int main()
{
    // signal parameters
    std::vector<std::pair<double, double>> parameters = {{2, 30}};

    // signal generator parameters
    double t_s = 1 / (120 * parameters[0].second);
    std::chrono::nanoseconds sampling_period =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double>(t_s));
    std::chrono::milliseconds duration(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<double>(0.5)));

    // create signal container
    std::vector<Complex> signal = {};

    // create signal generator
    std::shared_ptr<SignalGenerator> generator =
        std::make_shared<SignalGenerator>(signal, duration, sampling_period);

    // generate sine function
    generator->GenerateSine(parameters);

    // keep actual signal size (before eventual zero padding)
    int signal_size = signal.size();

    // compute FFT
    std::shared_ptr<IFFTUtils> fft_utils = std::make_shared<FFTUtils>();
    std::shared_ptr<IFFT> fft = std::make_shared<FFT>(fft_utils);
    fft->Compute(signal);

    // calculate peak
    std::pair<double, double> peakData = {0, 0};
    for (int i = 1; i <= signal_size / 2; ++i)
    {
        if ((2 * std::abs(signal[i]) / signal_size) > peakData.first)
        {
            peakData.first = 2 * std::abs(signal[i]) / signal_size;
            peakData.second =
                i * 1 /
                (signal_size *
                 std::chrono::duration_cast<std::chrono::duration<double>>(
                     sampling_period)
                     .count());
        }
    }

    std::cout << "Peak: " << peakData.first << std::endl
              << "Peak freq: " << peakData.second;

    return 0;
}
