#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <complex>

const double PI = 3.14159265358979323846;

using Complex = std::complex<double>;

/*void fft(std::vector<Complex> &a, bool invert = false) {
    int n = a.size();
    if (n <= 1)
        return;

    std::vector<Complex> a0(n / 2), a1(n / 2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    fft(a0, invert);
    fft(a1, invert);

    double angle = 2 * M_PI / n * (invert ? -1 : 1);
    Complex w(1), wn(cos(angle), sin(angle));

    for (int i = 0; i < n / 2; ++i) {
        Complex t = w * a1[i];
        a[i] = a0[i] + t;
        a[i + n / 2] = a0[i] - t;
        if (invert) {
            a[i] /= 2;
            a[i + n / 2] /= 2;
        }
        w *= wn;
    }
}*/


void fft(std::vector<Complex> &a, bool invert = false) {
    
    int n = a.size();
    int levels = log2(n);

    // Bit-reversal permutation
    for (int i = 0; i < n; ++i) {
        int j = 0;
        for (int bit = 0; bit < levels; ++bit) {
            if (i & (1 << bit)) {
                j |= (1 << (levels - 1 - bit));
            }
        }
        if (j > i) {
            std::swap(a[i], a[j]);
        }
    }

    // Iterative FFT
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * PI / len * (invert ? -1 : 1);
        Complex wlen(cos(angle), sin(angle));

        for (int i = 0; i < n; i += len) {
            Complex w(1);
            for (int j = 0; j < len / 2; ++j) {
                Complex u = a[i + j];
                Complex v = w * a[i + j + len / 2];
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (int i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

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
        Complex sample = {std::sin(2  * PI * frequency * t), 0};
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
    fft(signal);

    // print abs() of fft bins
    std::cout << "FFT bins:" << std::endl;
    for(int i = 0; i <= signal.size()/2; ++i) std::cout << std::abs(signal[i]) << ",";

    return 0;
}
