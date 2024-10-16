#include <thrust/device_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <vector>

struct square {
    __host__ __device__
    float operator()(const float& x) const {
        return x * x;
    }
};

float magnitude_gpu(thrust::device_vector<float>& v) {
    float sum_of_squares = thrust::transform_reduce(v.begin(), v.end(), square(), 0.0f, thrust::plus<float>());
    return std::sqrt(sum_of_squares);
}

float magnitude_cpu(const std::vector<float>& v) {
    float sum_of_squares = 0.0f;
    for (const float& x : v) {
        sum_of_squares += x * x;
    }
    return std::sqrt(sum_of_squares);
}

int main() {
    // Gerando vetor aleatório
    const int N = 1000000;

    std::vector<float> h_v(N);
    for (int i = 0; i < N; i++) {
        h_v[i] = rand() % 100;
    }

    thrust::device_vector<float> d_v(h_v);

    // Cálculo na CPU
    auto start_cpu = std::chrono::high_resolution_clock::now();
    float mag_cpu = magnitude_cpu(h_v);
    auto end_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cpu_time = end_cpu - start_cpu;

    // Cálculo na GPU
    auto start_gpu = std::chrono::high_resolution_clock::now();
    float mag_gpu = magnitude_gpu(d_v);
    auto end_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> gpu_time = end_gpu - start_gpu;

    std::cout << "Magnitude (CPU): " << mag_cpu << " em " << cpu_time.count() << " segundos\n";
    std::cout << "Magnitude (GPU): " << mag_gpu << " em " << gpu_time.count() << " segundos\n";

    return 0;
}
