#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono> // Inclui a biblioteca para medição de tempo

int main() {
    const int N = 100000000; // Número de pontos
    int count = 0; // Contagem de pontos dentro do círculo

    // Semente para o gerador de números aleatórios
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Inicia a contagem de tempo
    auto start = std::chrono::high_resolution_clock::now();

    // Loop para gerar N pontos
    for (int i = 0; i < N; ++i) {
        // Gera coordenadas x e y entre 0 e 1
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;

        // Verifica se o ponto está dentro do círculo
        if (x * x + y * y <= 1) {
            ++count;
        }
    }

    // Estimativa de Pi
    double pi_estimate = 4.0 * static_cast<double>(count) / N;

    // Finaliza a contagem de tempo
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Exibe o valor estimado de Pi e o tempo de execução
    std::cout << "Valor estimado de Pi: " << pi_estimate << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
