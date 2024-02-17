#include <memory>
#include <numeric>
#include <vector>
#include "dsp_utils.hpp"
#include "etl/vector.h"
#include "fft_types.hpp"
#include "gtest/gtest.h"
#include "utils/include/signal_generator.hpp"
#include "utils/include/testing_utils.hpp"

using SignalParameters = std::vector<std::pair<double, double>>;
using namespace fftemb;

// bit reversal test data
const std::vector<Complex> k_1_bit_reversed{{0, 0}, {1, 0}};
const std::vector<Complex> k_2_bits_reversed{{0, 0}, {2, 0}, {1, 0}, {3, 0}};
const std::vector<Complex> k_3_bits_reversed{{0, 0}, {4, 0}, {2, 0}, {6, 0}, {1, 0}, {5, 0}, {3, 0}, {7, 0}};
const std::vector<Complex> k_4_bits_reversed{{0, 0},
                                             {8, 0},
                                             {4, 0},
                                             {12, 0},
                                             {2, 0},
                                             {10, 0},
                                             {6, 0},
                                             {14, 0},
                                             {1, 0},
                                             {9, 0},
                                             {5, 0},
                                             {13, 0},
                                             {3, 0},
                                             {11, 0},
                                             {7, 0},
                                             {15, 0}};

class TestBitReversal : public ::testing::Test
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

class TestZeroPadding : public ::testing::TestWithParam<int>
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

TEST_F(TestBitReversal, Reverse1Bit)
{
    const auto              input_signal = k_1_bit_reversed;
    const auto              signal_size  = input_signal.size();
    etl::vector<Complex, 2> test_signal(2);
    for (int i = 0; i <= signal_size - 1; ++i)
        test_signal[i] = Complex(i, 0);

    dsp_utils->bit_reversal(test_signal);

    for (int i = 0; i <= signal_size - 1; ++i) {
        EXPECT_EQ(test_signal[i], input_signal[i]);
    }
}

TEST_F(TestBitReversal, Reverse2Bits)
{
    const auto              input_signal = k_2_bits_reversed;
    const auto              signal_size  = input_signal.size();
    etl::vector<Complex, 4> test_signal(4);
    for (int i = 0; i <= signal_size - 1; ++i)
        test_signal[i] = Complex(i, 0);

    dsp_utils->bit_reversal(test_signal);

    for (int i = 0; i <= signal_size - 1; ++i) {
        EXPECT_EQ(test_signal[i], input_signal[i]);
    }
}

TEST_F(TestBitReversal, Reverse3Bits)
{
    const auto              input_signal = k_3_bits_reversed;
    const auto              signal_size  = input_signal.size();
    etl::vector<Complex, 8> test_signal(8);
    for (int i = 0; i <= signal_size - 1; ++i)
        test_signal[i] = Complex(i, 0);

    dsp_utils->bit_reversal(test_signal);

    for (int i = 0; i <= signal_size - 1; ++i) {
        EXPECT_EQ(test_signal[i], input_signal[i]);
    }
}

TEST_F(TestBitReversal, Reverse4Bits)
{
    const auto               input_signal = k_4_bits_reversed;
    const auto               signal_size  = input_signal.size();
    etl::vector<Complex, 16> test_signal(16);
    for (int i = 0; i <= signal_size - 1; ++i)
        test_signal[i] = Complex(i, 0);

    dsp_utils->bit_reversal(test_signal);

    for (int i = 0; i <= signal_size - 1; ++i) {
        EXPECT_EQ(test_signal[i], input_signal[i]);
    }
}

/* TEST_P(TestZeroPadding, SignalsArePowersOf2)
{
    const auto                        signal_size = GetParam();
    etl::vector<Complex, signal_size> test_signal(signal_size);
    for (int i = 0; i <= signal_size; ++i)
        test_signal[i] = Complex(i, 0);
    const auto next_pow_2 = test_signal.capacity();

    dsp_utils->zero_padding(test_signal);

    EXPECT_EQ(test_signal.size(), next_pow_2);
}

INSTANTIATE_TEST_CASE_P(SignalsArePowersOf2, TestZeroPadding, ::testing::Values(3, 5, 12, 100)); */