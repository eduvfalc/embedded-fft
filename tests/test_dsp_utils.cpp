#include <memory>
#include <numeric>
#include <vector>
#include "dsp_utils.hpp"
#include "fft_types.hpp"
#include "gtest/gtest.h"
#include "signal_generator.hpp"
#include "utils/testing_utils.hpp"

using SignalParameters = std::vector<std::pair<double, double>>;

// signal generator
constexpr std::chrono::nanoseconds kDuration       = std::chrono::seconds(2);
constexpr std::chrono::nanoseconds kSamplingPeriod = std::chrono::milliseconds(1);

// bit reversal test data
const std::vector<Complex> k1bit{{0, 0},{1, 0}};
const std::vector<Complex> k2bits{{0, 0}, {2, 0}, {1, 0}, {3, 0}};
const std::vector<Complex> k3bits{{0, 0}, {4, 0}, {2, 0}, {6, 0}, {1, 0}, {5, 0}, {3, 0}, {7, 0}};
const std::vector<Complex> k4bits{{0, 0}, {8, 0}, {4, 0}, {12, 0}, {2, 0}, {10, 0}, {6, 0}, {14, 0}, {1, 0}, {9, 0}, {5, 0}, {13, 0}, {3, 0}, {11, 0}, {7, 0}, {15, 0}};

class TestBitReversal : public ::testing::TestWithParam<std::vector<Complex>>
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

class TestZeroPadding : public ::testing::TestWithParam<int>
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
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