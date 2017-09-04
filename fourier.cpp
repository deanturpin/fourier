#include "fourier.h"
#include "omp.h"
#include <chrono>
#include <complex>
#include <iostream>
#include <vector>

namespace jos {

auto *twiddle = new std::complex<double>[bins][bins]();

void dot_product(const std::vector<short> &samples, std::vector<double> &results) {

  for (unsigned long k = 0; k < results.size(); ++k) {
    for (unsigned long n = 0; n < bins; ++n) {

      std::complex<double> sum;
      for (unsigned int n = 0; n < bins; ++n)
        sum += twiddle[n][k] * std::complex<double>(samples.at(n), 0);

      results.at(k) = abs(sum);
    }
  }
}

std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  auto start = chrono::steady_clock::now();

  // Initialise twiddle matrix
  // auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
// #pragma omp parallel for
  for (unsigned int k = 0; k < bins; ++k)
// #pragma omp parallel for
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] =
        exp(complex<double>(0, 2) * M_PI * static_cast<double>(k) *
            static_cast<double>(n) / static_cast<double>(bins));

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.

  auto end = chrono::steady_clock::now();
  cout << "FT twiddle time " << (end - start).count() / 1e9 << endl;
  start = chrono::steady_clock::now();

  vector<double> results(bins / 2);
  dot_product(samples, results);

  // Enabling OMP for either of the first for loops halves the processing time
  // #pragma omp parallel for

  end = chrono::steady_clock::now();
  cout << "FT dot pro time " << (end - start).count() / 1e9 << endl;

  return results;
}
}
