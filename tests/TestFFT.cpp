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
    std::shared_ptr<IDSPUtils> mDspUtils = std::make_shared<DSPUtils>();
    std::shared_ptr<IFFT>      mFFT      = std::make_shared<FFT>(mDspUtils);

    std::vector<std::pair<double, double>>
    CalculateError(const std::vector<std::pair<double, double>>& peak_data,
                   const std::vector<std::pair<double, double>>& parameters);

    void
    SortSignalParameters(std::vector<std::pair<double, double>>& signal_parameters);
};

std::vector<std::pair<double, double>>
TestFFT::CalculateError(const std::vector<std::pair<double, double>>& peak_data,
                        const std::vector<std::pair<double, double>>& parameters)
{
    std::vector<std::pair<double, double>> error_vector;
    for (int i = 0; i <= peak_data.size() - 1; ++i) {
        error_vector.emplace_back(std::make_pair(std::abs(1 - peak_data[i].first / parameters[i].first),
                                                 std::abs(1 - peak_data[i].second / parameters[i].second)));
    }
    return error_vector;
}

void
TestFFT::SortSignalParameters(std::vector<std::pair<double, double>>& signal_parameters)
{
    return std::sort(signal_parameters.begin(), signal_parameters.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
}

TEST_P(TestFFT, ComputeSinusoidalSpectrum)
{
    std::vector<Complex> test_signal;
    auto                 test_params       = GetParam();
    auto                 signal_parameters = test_params.second;
    test_params.first(test_signal, signal_parameters);
    gGenerator.ApplyHannWindow(test_signal);

    mFFT->Compute(test_signal);

    const auto peak_data = mDspUtils->FindPeaks(test_signal, kSamplingPeriod, signal_parameters.size());
    SortSignalParameters(signal_parameters);
    const auto peak_errors = CalculateError(peak_data, signal_parameters);
    for (const auto& error : peak_errors) {
        EXPECT_LE(error.first, kAmplitudeTolerance);
        EXPECT_LE(error.second, kFrequencyTolerance);
    }
}

INSTANTIATE_TEST_CASE_P(TestSpectraComputation,
                        TestFFT,
                        ::testing::Values(std::make_pair(kSineWaves, SignalParameters{{5, 60}}),
                                          std::make_pair(kSineWaves, SignalParameters{{10, 100}})));