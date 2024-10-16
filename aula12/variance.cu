#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <iostream>
#include <cmath>
#include <chrono>  // Para medir o tempo de execução

// Functor para calcular a diferença ao quadrado em relação à média
struct variance_op {
    float mean;
    variance_op(float mean_) : mean(mean_) {}

    __host__ __device__
    float operator()(const float& x) const {
        float diff = x - mean;
        return diff * diff;
    }
};

// Função para calcular a variância usando a técnica de Fusion Kernel
float calculate_variance_fusion_kernel(const thrust::device_vector<float>& d_vec, float mean) {
    return thrust::transform_reduce(d_vec.begin(), d_vec.end(), variance_op(mean), 0.0f, thrust::plus<float>()) / d_vec.size();
}

// Função para calcular a variância em etapas separadas
float calculate_variance_separate_steps(const thrust::device_vector<float>& d_vec) {
    // Etapa 1: Cálculo da média
    float mean = thrust::reduce(d_vec.begin(), d_vec.end(), 0.0f, thrust::plus<float>()) / d_vec.size();

    // Etapa 2: Cálculo das diferenças ao quadrado e soma
    thrust::device_vector<float> diff_squares(d_vec.size());
    thrust::transform(d_vec.begin(), d_vec.end(), diff_squares.begin(), variance_op(mean));

    // Reduzir para obter a soma das diferenças ao quadrado
    float variance = thrust::reduce(diff_squares.begin(), diff_squares.end(), 0.0f, thrust::plus<float>()) / d_vec.size();
    
    return variance;
}

int main() {
    const int sizes[] = {1000000, 5000000, 10000000};  // Testar com tamanhos maiores

    for (int size : sizes) {
        thrust::host_vector<float> h_v(size);
        for (int i = 0; i < size; i++) {
            h_v[i] = rand() % 100;
        }
        thrust::device_vector<float> d_v = h_v;

        std::cout << "Tamanho do vetor: " << size << std::endl;

        // 1. Cálculo da variância usando Fusion Kernel
        auto start_fusion = std::chrono::high_resolution_clock::now();
        cudaDeviceSynchronize();  // Sincronizar antes da medição
        float mean_fusion = thrust::reduce(d_v.begin(), d_v.end(), 0.0f, thrust::plus<float>()) / d_v.size();
        float variance_fusion = calculate_variance_fusion_kernel(d_v, mean_fusion);
        cudaDeviceSynchronize();  // Garantir que o cálculo terminou
        auto end_fusion = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> fusion_time = end_fusion - start_fusion;

        std::cout << "  [Fusion Kernel] Variância: " << variance_fusion << ", Tempo: " << fusion_time.count() << " segundos" << std::endl;

        // 2. Cálculo da variância em etapas separadas
        auto start_separate = std::chrono::high_resolution_clock::now();
        cudaDeviceSynchronize();  // Sincronizar antes da medição
        float variance_separate = calculate_variance_separate_steps(d_v);
        cudaDeviceSynchronize();  // Garantir que o cálculo terminou
        auto end_separate = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> separate_time = end_separate - start_separate;

        std::cout << "  [Etapas Separadas] Variância: " << variance_separate << ", Tempo: " << separate_time.count() << " segundos" << std::endl;

        std::cout << std::endl;
    }

    return 0;
}
