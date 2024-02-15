#include <chrono>
#include <functional>
#include <memory>
#include <numeric>
#include "dsp_utils.hpp"
#include "fft.hpp"
#include "fft_types.hpp"
#include "gtest/gtest.h"
#include "signal_generator.hpp"
#include "utils/testing_utils.hpp"

using SignalParameters = std::vector<std::pair<double, double>>;

// error tolerances
constexpr auto kAmplitudeTolerance = 0.20;
constexpr auto kFrequencyTolerance = 0.05;

// signal generator
constexpr std::chrono::nanoseconds k_duration        = std::chrono::seconds(2);
constexpr std::chrono::nanoseconds k_sampling_period = std::chrono::milliseconds(1);
const SignalGenerator              g_generator       = SignalGenerator(k_duration, k_sampling_period);

// signal generator functions
auto pSineWavesGenerator
    = std::bind(&SignalGenerator::generate_sine_wave, &g_generator, std::placeholders::_1, std::placeholders::_2);

class TestFFT
  : public ::testing::TestWithParam<
        std::pair<std::function<void(std::vector<Complex>&, const SignalParameters&)>, SignalParameters>>
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
    std::shared_ptr<FFT>      fft       = std::make_shared<FFT>(dsp_utils);
};

TEST_P(TestFFT, computeSinusoidalSpectrum)
{
    std::vector<Complex> test_signal;
    auto                 test_params       = GetParam();
    auto                 signal_parameters = test_params.second;
    test_params.first(test_signal, signal_parameters);
    auto max_amplitude = dsp_utils->normalize(test_signal);
    dsp_utils->apply_hann_window(test_signal);

    fft->compute(test_signal);

    auto peak_data = dsp_utils->find_peaks(test_signal, k_sampling_period, signal_parameters.size());
    std::transform(peak_data.begin(), peak_data.end(), peak_data.begin(), [max_amplitude](const auto& peak) {
        return std::make_pair(peak.first * max_amplitude, peak.second);
    });
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