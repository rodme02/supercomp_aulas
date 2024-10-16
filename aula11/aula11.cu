#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/constant_iterator.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <thrust/count.h>
#include <thrust/copy.h>
#include <iostream>
#include <fstream>
#include <cmath>

struct diff_functor {
    __host__ __device__
    float operator()(const float& x, const float& y) const {
        return y - x;
    }
};

struct positive_functor {
    __host__ __device__
    bool operator()(const float& x) const {
        return x > 0;
    }
};

struct replace_negative {
    __host__ __device__
    float operator()(const float& x) const {
        return x < 0 ? 0 : x;
    }
};

int main() {
    // Leitura dos dados do arquivo de entrada
    std::ifstream file("stocks-google.txt");
    thrust::host_vector<float> h_stocks;
    float value;

    while (file >> value) {
        h_stocks.push_back(value);
    }
    file.close();

    int n = h_stocks.size();
    if (n <= 1) {
        std::cerr << "Dados insuficientes." << std::endl;
        return 1;
    }

    // Copiando os dados para o device
    thrust::device_vector<float> d_stocks = h_stocks;
    thrust::device_vector<float> d_gain_daily(n - 1);

    // Cálculo da diferença diária entre os preços das ações
    thrust::transform(d_stocks.begin(), d_stocks.end() - 1,
                      d_stocks.begin() + 1,
                      d_gain_daily.begin(), diff_functor());

    // Cálculo da média das diferenças diárias (variância)
    float mean = thrust::reduce(d_gain_daily.begin(), d_gain_daily.end()) / (n - 1);

    // Usando iteradores constantes para calcular a variância
    thrust::device_vector<float> d_variance(n - 1);
    thrust::transform(d_gain_daily.begin(), d_gain_daily.end(),
                      thrust::make_constant_iterator(mean),
                      d_variance.begin(),
                      thrust::minus<float>());

    // Quadrado das diferenças
    thrust::transform(d_variance.begin(), d_variance.end(),
                      d_variance.begin(), d_variance.begin(),
                      thrust::multiplies<float>());

    // Calculando a variância final
    float variance = thrust::reduce(d_variance.begin(), d_variance.end()) / (n - 1);

    std::cout << "Variância das diferenças diárias: " << variance << std::endl;

    // Contando os dias com aumento no preço
    int days_up = thrust::count_if(d_gain_daily.begin(), d_gain_daily.end(), positive_functor());
    std::cout << "Número de dias com aumento de preço: " << days_up << std::endl;

    // Substituindo os valores negativos por zero
    thrust::transform(d_gain_daily.begin(), d_gain_daily.end(), d_gain_daily.begin(), replace_negative());

    // Soma dos aumentos
    float total_up = thrust::reduce(d_gain_daily.begin(), d_gain_daily.end());

    // Cálculo do aumento médio nos dias em que o preço subiu
    float average_up = total_up / days_up;
    std::cout << "Aumento médio nos dias em que o preço subiu: " << average_up << std::endl;

    return 0;
}
