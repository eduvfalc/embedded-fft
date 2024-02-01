#include <iostream>
#include <memory>
#include <numeric>
#include "FFTUtils.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using Complex = std::complex<double>;

const std::vector<Complex> k1bit{0, 1};
const std::vector<Complex> k2bits{0, 2, 1, 3};
const std::vector<Complex> k3bits{0, 4, 2, 6, 1, 5, 3, 7};
const std::vector<Complex> k4bits{0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

class TestFFTUtils : public ::testing::TestWithParam<std::vector<Complex>>
{
protected:
    std::shared_ptr<IFFTUtils> fft_utils = std::make_shared<FFTUtils>();
};

TEST_P(TestFFTUtils, BitsAreSuccessfullyReversed)
{
    auto                 input_vector = GetParam();
    auto                 signal_size  = input_vector.size();
    std::vector<Complex> test_vector  = {};
    for (int i = 0; i <= signal_size - 1; ++i)
        test_vector.emplace_back(Complex(i, 0));

    fft_utils->BitReversal(test_vector);

    EXPECT_EQ(test_vector, input_vector);
}

INSTANTIATE_TEST_CASE_P(BitReversalTests, TestFFTUtils, ::testing::Values(k1bit, k2bits, k3bits, k4bits));