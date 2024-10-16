#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/reduce.h>
#include <iostream>
#include <sstream>

int main() {
    thrust::host_vector<float> h_apple_prices;
    thrust::host_vector<float> h_microsoft_prices;

    // Passo 1: Leitura de dados a partir de std::cin (entrada padrão redirecionada)
    std::string line;
    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        float apple_price, microsoft_price;

        // Verificar se o formato da linha está correto
        if (!(ss >> apple_price)) {
            std::cerr << "Erro ao ler preço da Apple!" << std::endl;
            return 1;
        }

        // Ignorar a vírgula e tentar ler o próximo valor
        if (ss.peek() == ',') {
            ss.ignore();
        }

        if (!(ss >> microsoft_price)) {
            std::cerr << "Erro ao ler preço da Microsoft!" << std::endl;
            return 1;
        }

        // Adicionar os preços ao host_vector
        h_apple_prices.push_back(apple_price);
        h_microsoft_prices.push_back(microsoft_price);
    }

    // Verificar se ambos os vetores têm o mesmo tamanho
    if (h_apple_prices.size() != h_microsoft_prices.size()) {
        std::cerr << "Número de registros entre Apple e Microsoft não corresponde!" << std::endl;
        return 1;
    }

    // Passo 2: Transferir para a GPU
    thrust::device_vector<float> d_apple_prices = h_apple_prices;
    thrust::device_vector<float> d_microsoft_prices = h_microsoft_prices;

    // Passo 3: Calcular a diferença ponto a ponto (Apple - Microsoft)
    thrust::device_vector<float> d_diff(d_apple_prices.size());
    thrust::transform(d_apple_prices.begin(), d_apple_prices.end(),
                      d_microsoft_prices.begin(), d_diff.begin(), thrust::minus<float>());

    // Passo 4: Calcular a soma dos preços para cada empresa
    float apple_sum = thrust::reduce(d_apple_prices.begin(), d_apple_prices.end(), 0.0f);
    float microsoft_sum = thrust::reduce(d_microsoft_prices.begin(), d_microsoft_prices.end(), 0.0f);

    // Cálculo da média dos preços para Apple e Microsoft
    float apple_avg = apple_sum / d_apple_prices.size();
    float microsoft_avg = microsoft_sum / d_microsoft_prices.size();

    // Verificar qual empresa teve, em média, o preço maior
    if (apple_avg > microsoft_avg) {
        std::cout << "A Apple teve um preço médio maior.\n";
        std::cout << "Diferença média de preço: " << (apple_avg - microsoft_avg) << std::endl;
    } else if (microsoft_avg > apple_avg) {
        std::cout << "A Microsoft teve um preço médio maior.\n";
        std::cout << "Diferença média de preço: " << (microsoft_avg - apple_avg) << std::endl;
    } else {
        std::cout << "Os preços médios de Apple e Microsoft foram iguais.\n";
    }

    return 0;
}
