#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <complex>
#include "FFT.hpp"

using Complex = std::complex<double>;

int main() {
    // sine frequency
    auto frequency = 30.;
    
    // sampling period
    auto samplingPeriod = 1 / ( 120 * frequency);
    
    // window size (s)
    auto duration = 1;
   
    // signal bins
    std::vector<Complex> signal = {};

    // generate sine function
    for(double t = 0; t <= duration; t+= samplingPeriod) {
        Complex sample = {std::sin(2  * M_PI * frequency * t), 0};
        signal.emplace_back(sample);
    }
    
    // print signal bins
    std::cout << "signal :";
    std::for_each(signal.begin(), signal.end(), [](const Complex& bin) {
        std::cout << bin.real() << ",";
    });
    
    std::cout << std::endl;

    // ensure the size of the input signal is a power of 2
    int signalSize = signal.size();
    int nextPow2 = 1;
    while (nextPow2 < signalSize) {
        nextPow2 <<= 1;
    }

    // pad the signal with zeros if necessary
    if (nextPow2 > signalSize) {
        signal.resize(nextPow2, 0.0);
    }

    // compute FFT
    std::shared_ptr<IFFT> fft = std::make_shared<FFT>();
    fft->Compute(signal, false);

    // print abs() of fft bins
    std::cout << "FFT bins:" << std::endl;
    for(int i = 0; i <= signal.size()/2; ++i) std::cout << std::abs(signal[i]) << ",";

    return 0;
}
