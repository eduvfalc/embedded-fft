#include "dsp_utils.hpp"
#include <chrono>
#include <cmath>
#include <complex>
#include <numbers>
#include <vector>
#include "fft_types.hpp"

using namespace fftemb;

void
DSPUtils::bit_reversal(std::vector<Complex>& signal)
{
    auto n      = signal.size();
    int  levels = log2(n);

    for (int i = 0; i < n; ++i) {
        int j = 0;
        for (int bit = 0; bit < levels; ++bit) {
            if (i & (1 << bit)) {
                j |= (1 << (levels - 1 - bit));
            }
        }
        if (j > i) {
            std::swap(signal[i], signal[j]);
        }
    }
}

void
DSPUtils::zero_padding(std::vector<Complex>& signal)
{
    int signalSize = signal.size();
    int nextPow2   = 1;
    while (nextPow2 < signalSize) {
        nextPow2 <<= 1;
    }

    if (nextPow2 > signalSize) {
        signal.resize(nextPow2, {0, 0});
    }
}

double
DSPUtils::normalize(std::vector<Complex>& signal)
{
    auto max_it        = std::max_element(signal.begin(), signal.end(), [](const Complex& b1, const Complex& b2) {
        return std::abs(b1) < std::abs(b2);
    });
    auto max_amplitude = std::abs(*max_it);
    std::transform(signal.begin(), signal.end(), signal.begin(), [max_amplitude](auto& bin) {
        return Complex(cnl::quotient(bin.real(), max_amplitude), cnl::quotient(bin.imag(), max_amplitude));
    });
    return static_cast<double>(max_amplitude);
}

std::vector<std::pair<double, double>>
DSPUtils::find_peaks(std::vector<Complex>& signal, std::chrono::nanoseconds sampling_period, int max_peaks)
{
    std::vector<std::pair<double, double>> peak_data(max_peaks);
    const auto t_s         = std::chrono::duration_cast<std::chrono::duration<double>>(sampling_period).count();
    const int  signal_size = signal.size();
    for (int i = 1; i <= signal_size / 2; ++i) {
        const auto amplitude = 2 * std::abs(signal[i]) / signal_size;
        auto       min_it    = std::min_element(peak_data.begin(), peak_data.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        if (min_it != peak_data.end() && amplitude > min_it->first) {
            const auto frequency = i * 1 / (signal_size * t_s);
            if (frequency > m_dc_leakage_frequency) {
                auto neighbor_it
                    = std::find_if(peak_data.begin(), peak_data.end(), [frequency, this](const auto& peak) {
                          return std::abs(1 - frequency / peak.second) < m_max_frequency_delta_pct;
                      });
                if (neighbor_it != peak_data.end()) {
                    if (amplitude > neighbor_it->first) {
                        *neighbor_it = {static_cast<double>(amplitude), frequency};
                    }
                }
                else {
                    *min_it = {static_cast<double>(amplitude), frequency};
                }
            }
        }
    }
    return peak_data;
}

void
DSPUtils::apply_hann_window(std::vector<Complex>& signal) const
{
    Complex correction_fator{2, 0};
    auto    signal_size = signal.size();
    for (int i = 0; i <= signal_size; ++i) {
        Complex hanning_bin = {0.5 - 0.5 * std::cos(2 * std::numbers::pi * i / signal_size), 0};
        signal[i]           = signal[i] * hanning_bin * correction_fator;
    }
}
