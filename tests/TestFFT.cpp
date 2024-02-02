#include <chrono>
#include <functional>
#include <memory>
#include <numeric>
#include "FFT.hpp"
#include "FFTUtils.hpp"
#include "SignalGenerator.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"


using Complex = std::complex<double>;

class TestFFT : public ::testing::Test
{
protected:
    std::shared_ptr<IFFTUtils> fft_utils = std::make_shared<FFTUtils>();
    std::shared_ptr<IFFT>      fft       = std::make_shared<FFT>(fft_utils);
    std::pair<double, double>
    FindPeak(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period);
};

std::pair<double, double>
TestFFT::FindPeak(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period)
{
    std::pair<double, double> peakData    = {0, 0};
    int                       signal_size = signal.size();
    for (int i = 1; i <= signal_size / 2; ++i) {
        auto amplitude = 2 * std::abs(signal[i]) / signal_size;
        if (amplitude > peakData.first) {
            peakData = {amplitude,
                        i * 1
                            / (signal_size
                               * std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count())};
        }
    }
    return peakData;
}

TEST_F(TestFFT, ComputeSineWaveSpectrum)
{
    std::vector<Complex>             test_signal;
    std::pair<double, double>        parameters{5, 60};
    const std::chrono::nanoseconds   sampling_period = std::chrono::milliseconds(1);
    const std::chrono::nanoseconds   duration        = std::chrono::seconds(2);
    std::shared_ptr<SignalGenerator> sig_gen         = std::make_shared<SignalGenerator>(duration, sampling_period);
    sig_gen->GenerateSine(test_signal, parameters);
    sig_gen->ApplyHannWindow(test_signal);

    fft->Compute(test_signal);
    auto         peak_data       = FindPeak(test_signal, sampling_period);
    const double amplitude_error = 1 - peak_data.first / parameters.first;
    const double frequency_error = 1 - peak_data.second / parameters.second;

    EXPECT_LE(amplitude_error, 0.25);
    EXPECT_LE(frequency_error, 0.05);
}