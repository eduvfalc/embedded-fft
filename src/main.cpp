#include <memory>
#include <vector>
#include <complex>
#include <iostream>
#include <algorithm>
#include "FFT.hpp"
#include "FFTUtils.hpp"

const double PI = 2 * std::acos(0.0);

using Complex = std::complex<double>;

int main() {
    // signal parameters
    auto frequency = 30.;
    auto samplingPeriod = 1 / ( 120 * frequency);
    auto duration = 1;
   
    // signal bins
    std::vector<Complex> signal = {};

    // generate sine function
    for(double t = 0; t <= duration; t+= samplingPeriod) {
        Complex sample = {std::sin(2  * PI * frequency * t), 0};
        signal.emplace_back(sample);
    }
    
    // print signal bins
    /* std::cout << "signal :";
    for(auto& bin : signal) {
        std::cout << bin.real() << ",";
    }*/
    
    std::cout << std::endl;

    // compute FFT
    std::shared_ptr<IFFTUtils> fft_utils = std::make_shared<FFTUtils>();
    std::shared_ptr<IFFT> fft = std::make_shared<FFT>(fft_utils);
    fft->Compute(signal);

    // print abs() of fft bins
    std::pair<double, double> peakData = {0, 0};
    // std::cout << "FFT bins:" << std::endl;
    // std::cout << std::abs(signal[0]) / signalSize << ",";
    for(int i = 1; i <= signalSize / 2; ++i) {
        // std::cout << 2 * std::abs(signal[i]) / signalSize << ",";
        if ( ( 2 * std::abs(signal[i]) / signalSize ) > peakData.first) {
            peakData.first = 2 * std::abs(signal[i]) / signalSize;
            peakData.second = i * 1 / (signalSize * samplingPeriod);
        }
    }

    std::cout << std::endl;

    std::cout << "Peak: " << peakData.first << std::endl << "Peak freq: " << peakData.second;

    return 0;
}
