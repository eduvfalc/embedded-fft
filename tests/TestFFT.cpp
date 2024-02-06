#include <chrono>
#include <functional>
#include <memory>
#include <numeric>
#include "DSPUtils.hpp"
#include "FFT.hpp"
#include "SignalGenerator.hpp"
#include "gtest/gtest.h"
#include "utils/TestUtils.hpp"

using Complex          = std::complex<double>;
using SignalParameters = std::vector<std::pair<double, double>>;

// error tolerances
constexpr auto kAmplitudeTolerance = 0.15;
constexpr auto kFrequencyTolerance = 0.05;

// signal generator
constexpr std::chrono::nanoseconds kDuration       = std::chrono::seconds(2);
constexpr std::chrono::nanoseconds kSamplingPeriod = std::chrono::milliseconds(1);
const SignalGenerator              gGenerator      = SignalGenerator(kDuration, kSamplingPeriod);

// signal generator functions
auto pSineWavesGenerator
    = std::bind(&SignalGenerator::GenerateSines, &gGenerator, std::placeholders::_1, std::placeholders::_2);

class TestFFT
  : public ::testing::TestWithParam<
        std::pair<std::function<void(std::vector<Complex>&, const SignalParameters&)>, SignalParameters>>
{
protected:
    std::shared_ptr<IDSPUtils> mDspUtils = std::make_shared<DSPUtils>();
    std::shared_ptr<IFFT>      mFFT      = std::make_shared<FFT>(mDspUtils);
};

TEST_P(TestFFT, ComputeSinusoidalSpectrum)
{
    std::vector<Complex> test_signal;
    auto                 test_params       = GetParam();
    auto                 signal_parameters = test_params.second;
    test_params.first(test_signal, signal_parameters);
    gGenerator.ApplyHannWindow(test_signal);

    mFFT->Compute(test_signal);

    auto peak_data = mDspUtils->FindPeaks(test_signal, kSamplingPeriod, signal_parameters.size());
    test_utils::SortPairs(peak_data);
    test_utils::SortPairs(signal_parameters);
    const auto peak_errors = test_utils::CalculateError(peak_data, signal_parameters);
    for (const auto& error : peak_errors) {
        EXPECT_LE(error.first, kAmplitudeTolerance);
        EXPECT_LE(error.second, kFrequencyTolerance);
    }
}

INSTANTIATE_TEST_CASE_P(TestSpectraComputation,
                        TestFFT,
                        ::testing::Values(std::make_pair(pSineWavesGenerator, SignalParameters{{5, 60}}),
                                          std::make_pair(pSineWavesGenerator, SignalParameters{{5, 60}, {10, 100}}),
                                          std::make_pair(pSineWavesGenerator,
                                                         SignalParameters{{8, 30}, {3, 60}, {12, 90}})));