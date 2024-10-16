#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/extrema.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

int main() {
    thrust::host_vector<float> h_stock_prices;

    // Função para remover espaços em branco de ambos os lados de uma string
    auto trim = [](std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    };

    // Passo 1: Leitura de dados a partir de std::cin (entrada padrão redirecionada)
    std::string line;
    while (std::getline(std::cin, line)) {
        // Remover o caractere de carriage return (\r) e espaços extras
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        trim(line);

        // Ignorar linhas vazias ou que só contenham espaços
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        float price;

        // Verificar se o formato da linha está correto
        if (!(ss >> price)) {
            std::cerr << "Erro ao ler preço das ações! Linha lida: '" << line << "'" << std::endl;
            return 1;
        }

        h_stock_prices.push_back(price);
    }

    // Verificar se lemos algum dado
    if (h_stock_prices.size() == 0) {
        std::cerr << "Nenhum dado foi lido." << std::endl;
        return 1;
    }

    // Passo 2: Transferir para a GPU usando device_vector
    thrust::device_vector<float> d_stock_prices = h_stock_prices;

    // Passo 3: Cálculo da média total (últimos 10 anos)
    float total_sum = thrust::reduce(d_stock_prices.begin(), d_stock_prices.end(), 0.0f);
    float total_avg = total_sum / d_stock_prices.size();
    std::cout << "Preço médio total (últimos 10 anos): " << total_avg << std::endl;

    // Passo 4: Cálculo da média dos últimos 365 dias
    int n_last_year = 365;
    if (d_stock_prices.size() >= n_last_year) {
        float last_year_sum = thrust::reduce(d_stock_prices.end() - n_last_year, d_stock_prices.end(), 0.0f);
        float last_year_avg = last_year_sum / n_last_year;
        std::cout << "Preço médio último ano: " << last_year_avg << std::endl;
    } else {
        std::cerr << "Erro: Não há dados suficientes para calcular a média dos últimos 365 dias." << std::endl;
    }

    // Passo 5: Encontrar o maior e menor preço no período total
    auto result_total = thrust::minmax_element(d_stock_prices.begin(), d_stock_prices.end());
    std::cout << "Menor preço total (últimos 10 anos): " << *result_total.first << std::endl;
    std::cout << "Maior preço total (últimos 10 anos): " << *result_total.second << std::endl;

    // Passo 6: Encontrar o maior e menor preço no último ano
    if (d_stock_prices.size() >= n_last_year) {
        auto result_last_year = thrust::minmax_element(d_stock_prices.end() - n_last_year, d_stock_prices.end());
        std::cout << "Menor preço último ano: " << *result_last_year.first << std::endl;
        std::cout << "Maior preço último ano: " << *result_last_year.second << std::endl;
    } else {
        std::cerr << "Erro: Não há dados suficientes para calcular o maior e menor preço do último ano." << std::endl;
    }

    return 0;
}
