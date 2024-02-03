#include <chrono>
#include <functional>
#include <memory>
#include <numeric>
#include "DSPUtils.hpp"
#include "FFT.hpp"
#include "SignalGenerator.hpp"
#include "gtest/gtest.h"

using Complex          = std::complex<double>;
using SignalParameters = std::vector<std::pair<double, double>>;

// error tolerances
constexpr auto kAmplitudeTolerance = 0.10;
constexpr auto kFrequencyTolerance = 0.05;

// signal generator
constexpr std::chrono::nanoseconds kDuration       = std::chrono::seconds(2);
constexpr std::chrono::nanoseconds kSamplingPeriod = std::chrono::milliseconds(1);
const SignalGenerator              gGenerator      = SignalGenerator(kDuration, kSamplingPeriod);

// signal generator functions
auto kSineWaves = std::bind(&SignalGenerator::GenerateSines, &gGenerator, std::placeholders::_1, std::placeholders::_2);

class TestFFT
  : public ::testing::TestWithParam<
        std::pair<std::function<void(std::vector<Complex>&, const SignalParameters&)>, SignalParameters>>
{
protected:
    std::shared_ptr<IDSPUtils> fft_utils = std::make_shared<DSPUtils>();
    std::shared_ptr<IFFT>      fft       = std::make_shared<FFT>(fft_utils);

    std::pair<double, double>
    FindPeak(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period);

    std::pair<double, double>
    CalculateError(const std::pair<double, double>& peak_data, const std::pair<double, double>& parameters);
};

std::pair<double, double>
TestFFT::FindPeak(std::vector<Complex>& signal, const std::chrono::nanoseconds& sampling_period)
{
    std::pair<double, double> peakData{0, 0};
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
    return std::make_pair(1 - peak_data.first / parameters.first, 1 - peak_data.second / parameters.second);
}

TEST_P(TestFFT, ComputeSinusoidSpectra)
{
    std::vector<Complex> test_signal;
    auto                 test_params = GetParam();
    test_params.first(test_signal, test_params.second);
    gGenerator.ApplyHannWindow(test_signal);

    fft->Compute(test_signal);

    const auto peak_data  = FindPeak(test_signal, kSamplingPeriod);
    const auto peak_error = CalculateError(peak_data, test_params.second[0]);

    EXPECT_LE(peak_error.first, kAmplitudeTolerance);
    EXPECT_LE(peak_error.second, kFrequencyTolerance);
}

INSTANTIATE_TEST_CASE_P(TestSpectraComputation,
                        TestFFT,
                        ::testing::Values(std::make_pair(kSineWaves, SignalParameters{{5, 60}}),
                                          std::make_pair(kSineWaves, SignalParameters{{10, 100}})));