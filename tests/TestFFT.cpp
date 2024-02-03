#include <chrono>
#include <functional>
#include <memory>
#include <numeric>
#include "FFT.hpp"
#include "FFTUtils.hpp"
#include "SignalGenerator.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

constexpr auto                     kAmplitudeTolerance = 0.10;
constexpr auto                     kFrequencyTolerance = 0.05;
constexpr std::chrono::nanoseconds kSamplingPeriod     = std::chrono::milliseconds(1);
constexpr std::chrono::nanoseconds kDuration           = std::chrono::seconds(2);

using Complex = std::complex<double>;

class TestFFT : public ::testing::Test
{
protected:
    std::shared_ptr<IFFTUtils>       fft_utils = std::make_shared<FFTUtils>();
    std::shared_ptr<IFFT>            fft       = std::make_shared<FFT>(fft_utils);
    std::shared_ptr<SignalGenerator> sig_gen   = std::make_shared<SignalGenerator>(kDuration, kSamplingPeriod);

    std::pair<double, double>
    FindPeak(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period);

    std::pair<double, double>
    CalculateError(const std::pair<double, double>& peak_data, const std::pair<double, double>& parameters);
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

std::pair<double, double>
TestFFT::CalculateError(const std::pair<double, double>& peak_data, const std::pair<double, double>& parameters)
{
    return std::pair<double, double>{1 - peak_data.first / parameters.first, 1 - peak_data.second / parameters.second};
}

TEST_F(TestFFT, ComputeSineWaveSpectrum)
{
    std::vector<Complex>                         test_signal;
    const std::vector<std::pair<double, double>> parameters{{5, 60}};
    sig_gen->GenerateSines(test_signal, parameters);
    sig_gen->ApplyHannWindow(test_signal);

    fft->Compute(test_signal);
    const auto peak_data  = FindPeak(test_signal, kSamplingPeriod);
    const auto peak_error = CalculateError(peak_data, parameters[0]);

    EXPECT_LE(peak_error.first, kAmplitudeTolerance);
    EXPECT_LE(peak_error.second, kFrequencyTolerance);
}