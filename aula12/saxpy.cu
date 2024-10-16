#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/host_vector.h>
#include <iostream>
#include <cstdlib>

struct saxpy {
    int a;
    saxpy(int a_) : a(a_) {}

    __host__ __device__
    float operator()(const float& x, const float& y) const {
        return a * x + y;
    }
};

int main() {
    // Gerando vetores aleatórios
    const int N = 1000000;
    int a = 5; // Constante para Saxpy

    thrust::host_vector<float> h_x(N), h_y(N), h_z(N);
    for (int i = 0; i < N; i++) {
        h_x[i] = rand() % 100;
        h_y[i] = rand() % 100;
    }

    thrust::device_vector<float> d_x = h_x;
    thrust::device_vector<float> d_y = h_y;
    thrust::device_vector<float> d_z(N);

    // Aplicando Saxpy
    thrust::transform(d_x.begin(), d_x.end(), d_y.begin(), d_z.begin(), saxpy(a));

    // Copiando de volta para a CPU
    thrust::copy(d_z.begin(), d_z.end(), h_z.begin());

    // Mostrando alguns resultados
    for (int i = 0; i < 10; i++) {
        std::cout << "Saxpy(" << h_x[i] << ", " << h_y[i] << ") = " << h_z[i] << std::endl;
    }

    return 0;
}
