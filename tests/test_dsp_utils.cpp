#include <memory>
#include <numeric>
#include <vector>
#include "dsp_utils.hpp"
#include "fft_types.hpp"
#include "gtest/gtest.h"
#include "signal_generator.hpp"
#include "utils/testing_utils.hpp"

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

class Testbit_reversal : public ::testing::TestWithParam<std::vector<Complex>>
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

class Testzero_padding : public ::testing::TestWithParam<int>
{
protected:
    std::shared_ptr<DSPUtils> dsp_utils = std::make_shared<DSPUtils>();
};

TEST_P(Testbit_reversal, BitsAreReversed)
{
    const auto           input_signal = GetParam();
    const auto           signal_size  = input_signal.size();
    std::vector<Complex> test_signal;
    for (int i = 0; i <= signal_size - 1; ++i)
        test_signal.emplace_back(Complex(i, 0));

    dsp_utils->bit_reversal(test_signal);

    EXPECT_EQ(test_signal, input_signal);
}

TEST_P(Testzero_padding, SignalsArePowersOf2)
{
    const auto           signal_size = GetParam();
    std::vector<Complex> test_signal;
    for (int i = 0; i <= signal_size; ++i)
        test_signal.emplace_back(Complex(i, 0));
    const auto next_pow_2 = test_signal.capacity();

    dsp_utils->zero_padding(test_signal);

    EXPECT_EQ(test_signal.size(), next_pow_2);
}

INSTANTIATE_TEST_CASE_P(bit_reversal,
                        Testbit_reversal,
                        ::testing::Values(k_1_bit_reversed, k_2_bits_reversed, k_3_bits_reversed, k_4_bits_reversed));

INSTANTIATE_TEST_CASE_P(SignalsArePowersOf2, Testzero_padding, ::testing::Values(3, 5, 12, 100));