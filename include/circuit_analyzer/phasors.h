#include <vector>
#include <complex>

// Parallel Phasor API
std::complex<double> simplify_series(const std::vector<std::complex<double>>& elements);
std::complex<double> simplify_parallel(const std::vector<std::complex<double>>& elements);
std::complex<double> simplify_parallel(const std::complex<double>& one, const std::complex<double>& two);

// Series Phasor API
std::complex<float> simplify_series(const std::vector<std::complex<float>>& elements);
std::complex<float> simplify_parallel(const std::vector<std::complex<float>>& elements);
std::complex<float> simplify_parallel(const std::complex<float>& one, const std::complex<float>& two);

