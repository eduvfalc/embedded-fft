#include <memory>
#include <numeric>
#include <vector>
#include "DSPUtils.hpp"
#include "SignalGenerator.hpp"
#include "gtest/gtest.h"
#include "utils/TestUtils.hpp"

using Complex          = std::complex<double>;
using SignalParameters = std::vector<std::pair<double, double>>;

// peak detection error tolerances
constexpr auto kAmplitudeTolerance = 0.15;
constexpr auto kFrequencyTolerance = 0.05;

// signal generator
constexpr std::chrono::nanoseconds kDuration       = std::chrono::seconds(2);
constexpr std::chrono::nanoseconds kSamplingPeriod = std::chrono::milliseconds(1);

// bit reversal test data
const std::vector<Complex> k1bit{0, 1};
const std::vector<Complex> k2bits{0, 2, 1, 3};
const std::vector<Complex> k3bits{0, 4, 2, 6, 1, 5, 3, 7};
const std::vector<Complex> k4bits{0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

class TestBitReversal : public ::testing::TestWithParam<std::vector<Complex>>
{
protected:
    std::shared_ptr<IDSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

class TestZeroPadding : public ::testing::TestWithParam<int>
{
protected:
    std::shared_ptr<IDSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

TEST_P(TestBitReversal, BitsAreReversed)
{
    const auto           input_signal = GetParam();
    const auto           signal_size  = input_signal.size();
    std::vector<Complex> test_signal;
    for (int i = 0; i <= signal_size - 1; ++i)
        test_signal.emplace_back(Complex(i, 0));

    dsp_utils->BitReversal(test_signal);

    EXPECT_EQ(test_signal, input_signal);
}

TEST_P(TestZeroPadding, SignalsArePowersOf2)
{
    const auto           signal_size = GetParam();
    std::vector<Complex> test_signal;
    for (int i = 0; i <= signal_size; ++i)
        test_signal.emplace_back(Complex(i, 0));
    const auto next_pow_2 = test_signal.capacity();

    dsp_utils->ZeroPadding(test_signal);

    EXPECT_EQ(test_signal.size(), next_pow_2);
}

INSTANTIATE_TEST_CASE_P(BitReversal, TestBitReversal, ::testing::Values(k1bit, k2bits, k3bits, k4bits));

INSTANTIATE_TEST_CASE_P(SignalsArePowersOf2, TestZeroPadding, ::testing::Values(3, 5, 12, 100));