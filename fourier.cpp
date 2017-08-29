#include "fourier.h"
#include <vector>
#include <complex>

namespace jos {
std::vector<double> fourier(const std::vector<short> &samples) {

  using namespace std;

  // Initialise twiddle matrix
  auto *twiddle = new complex<double>[bins][bins]();

  // Populate twiddle matrix. The "exp" is the important bit.
  for (unsigned int k = 0; k < bins; ++k)
    for (unsigned int n = 0; n < bins; ++n)
      twiddle[n][k] =
          exp(complex<double>(0, 2) * M_PI * static_cast<double>(k) *
              static_cast<double>(n) / static_cast<double>(bins));

  // The Fourier transform is the dot product of the twiddle matrix and the
  // original samples. Only run over the first half of the matrix as the other
  // half is a mirror image.
  vector<double> results;
  for (unsigned int k = 0; k < bins; ++k) {

    complex<double> sum;
    for (unsigned int n = 0; n < bins; ++n)
      sum += twiddle[n][k] * complex<double>(samples.at(n), 0);

    // Store the absolute value of the complex average
    results.push_back(abs(sum / static_cast<double>(bins)));
  }

  // Free up the twiddles
  delete[] twiddle;

  return results;
}
}
