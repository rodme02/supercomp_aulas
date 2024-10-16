#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <numeric>

using namespace std;
using namespace chrono;

typedef pair<int, int> Resultado;

// Função para calcular o valor e peso máximo (igual ao original)
Resultado calcularValorPesoMaximo(int capacidadeRestante, const vector<int>& pesos, const vector<int>& valores, int indiceItem, int& combinacoes) {
    combinacoes++;
    if (indiceItem == 0 || capacidadeRestante == 0) return {0, 0};

    if (pesos[indiceItem - 1] > capacidadeRestante) {
        return calcularValorPesoMaximo(capacidadeRestante, pesos, valores, indiceItem - 1, combinacoes);
    } else {
        Resultado incluirItem = calcularValorPesoMaximo(capacidadeRestante - pesos[indiceItem - 1], pesos, valores, indiceItem - 1, combinacoes);
        incluirItem.first += valores[indiceItem - 1];
        incluirItem.second += pesos[indiceItem - 1];
        Resultado excluirItem = calcularValorPesoMaximo(capacidadeRestante, pesos, valores, indiceItem - 1, combinacoes);
        return (incluirItem.first > excluirItem.first) ? incluirItem : excluirItem;
    }
}

// Função para imprimir resultados
void imprimirResultados(const string& metodo, int valorTotal, int pesoTotal, long long duracao) {
    cout << metodo << ":" << endl;
    cout << "Valor total: " << valorTotal << endl;
    cout << "Peso total: " << pesoTotal << endl;
    cout << "Tempo de execucao: " << duracao << " ns" << endl << endl;
}

// Função para Mochila Cheia (retorna itens selecionados)
vector<int> mochilaCheia(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores, int& valorTotal, int& pesoTotal) {
    int n = pesos.size();
    vector<int> selecionados(n, 0); // Mantém quais itens foram selecionados
    valorTotal = 0;
    pesoTotal = 0;
    
    // Percorre todos os itens e adiciona o que couber
    for (int i = 0; i < n; ++i) {
        if (pesoTotal + pesos[i] <= capacidadeDaMochila) {
            pesoTotal += pesos[i];
            valorTotal += valores[i];
            selecionados[i] = 1;
        }
    }

    return selecionados;
}

// Função para Substituição de Objeto
void substituicaoDeObjeto(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores) {
    int n = pesos.size();
    int valorTotal = 0, pesoTotal = 0;

    // Mochila cheia primeiro para obter a solução inicial
    vector<int> selecionados = mochilaCheia(capacidadeDaMochila, pesos, valores, valorTotal, pesoTotal);

    auto inicio = high_resolution_clock::now();

    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        // Percorre todos os itens que já estão na mochila
        for (int i = 0; i < n; ++i) {
            if (selecionados[i] == 1) {  // Se o item está na mochila
                // Tenta substituir por outros itens
                for (int j = 0; j < n; ++j) {
                    if (selecionados[j] == 0) {  // Se o item não está na mochila
                        int novoPeso = pesoTotal - pesos[i] + pesos[j];
                        int novoValor = valorTotal - valores[i] + valores[j];
                        if (novoPeso <= capacidadeDaMochila && novoValor > valorTotal) {
                            // Faz a troca
                            pesoTotal = novoPeso;
                            valorTotal = novoValor;
                            selecionados[i] = 0;
                            selecionados[j] = 1;
                            melhorou = true;
                            break;  // Volta para o início do loop para verificar novas substituições
                        }
                    }
                }
            }
        }
    }

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<nanoseconds>(fim - inicio).count();

    imprimirResultados("Metodo Substituicao de Objeto", valorTotal, pesoTotal, duracao);
}

// Função para Hill Climbing
void hillClimbing(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores) {
    int n = pesos.size();
    vector<int> selecionados(n, 0);
    int pesoAtual = 0, valorTotal = 0;

    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(0, n - 1);

    // Solução inicial aleatória
    for (int i = 0; i < n; ++i) {
        selecionados[i] = dist(gen) % 2;
        if (selecionados[i] == 1 && pesoAtual + pesos[i] <= capacidadeDaMochila) {
            pesoAtual += pesos[i];
            valorTotal += valores[i];
        } else {
            selecionados[i] = 0;
        }
    }

    bool melhorou = true;
    auto inicio = high_resolution_clock::now();

    while (melhorou) {
        melhorou = false;
        for (int i = 0; i < n; ++i) {
            selecionados[i] = 1 - selecionados[i]; // Alterna o bit (inclui/exclui item)
            int novoPeso = pesoAtual + (selecionados[i] ? pesos[i] : -pesos[i]);
            int novoValor = valorTotal + (selecionados[i] ? valores[i] : -valores[i]);
            
            if (novoPeso <= capacidadeDaMochila && novoValor > valorTotal) {
                pesoAtual = novoPeso;
                valorTotal = novoValor;
                melhorou = true;
            } else {
                selecionados[i] = 1 - selecionados[i]; // Reverte a alteração se não foi uma melhoria
            }
        }
    }

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<nanoseconds>(fim - inicio).count();

    imprimirResultados("Metodo Hill Climbing", valorTotal, pesoAtual, duracao);
}

// Função de Embaralhar e Preencher (Aula 7)
void embaralharEPreencher(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores, mt19937& gen) {
    int n = pesos.size();
    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);

    int melhorValor = 0, melhorPeso = 0;
    long long melhorDuracao = 0;

    for (int repeticoes = 0; repeticoes < 5; ++repeticoes) {
        shuffle(indices.begin(), indices.end(), gen);

        int pesoAtual = 0;
        int valorTotal = 0;
        auto inicio = high_resolution_clock::now();

        for (int i : indices) {
            if (pesoAtual + pesos[i] <= capacidadeDaMochila) {
                pesoAtual += pesos[i];
                valorTotal += valores[i];
            }
        }

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<nanoseconds>(fim - inicio).count();

        // Verifica se este resultado é o melhor
        if (valorTotal > melhorValor) {
            melhorValor = valorTotal;
            melhorPeso = pesoAtual;
            melhorDuracao = duracao;
        }
    }

    // Imprime o melhor resultado
    imprimirResultados("Metodo Embaralhar e Preencher (Melhor de 5)", melhorValor, melhorPeso, melhorDuracao);
}

// Função de Seleção Aleatória (Aula 7)
void selecaoAleatoria(int capacidadeDaMochila, vector<int>& pesos, vector<int>& valores, mt19937& gen) {
    int n = pesos.size();
    uniform_real_distribution<double> distribution(0.0, 1.0);
    double threshold = 0.5;

    int melhorValor = 0, melhorPeso = 0;
    long long melhorDuracao = 0;

    for (int repeticoes = 0; repeticoes < 5; ++repeticoes) {
        int pesoAtual = 0;
        int valorTotal = 0;
        auto inicio = high_resolution_clock::now();

        for (int i = 0; i < n; ++i) {
            if (distribution(gen) > threshold && pesoAtual + pesos[i] <= capacidadeDaMochila) {
                pesoAtual += pesos[i];
                valorTotal += valores[i];
            }
        }

        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<nanoseconds>(fim - inicio).count();

        // Verifica se este resultado é o melhor
        if (valorTotal > melhorValor) {
            melhorValor = valorTotal;
            melhorPeso = pesoAtual;
            melhorDuracao = duracao;
        }
    }

    // Imprime o melhor resultado
    imprimirResultados("Metodo Selecao Aleatoria (Melhor de 5)", melhorValor, melhorPeso, melhorDuracao);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " <arquivo de entrada>" << endl;
        return 1;
    }

    ifstream arquivoEntrada(argv[1]);
    if (!arquivoEntrada) {
        cout << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }

    int numeroDeItens, capacidadeDaMochila;
    arquivoEntrada >> numeroDeItens >> capacidadeDaMochila;

    vector<int> pesos(numeroDeItens), valores(numeroDeItens);
    for (int i = 0; i < numeroDeItens; ++i) {
        arquivoEntrada >> pesos[i] >> valores[i];
    }

    mt19937 gen(random_device{}()); // Gera números aleatórios para os algoritmos probabilísticos

    // Executar os algoritmos da aula 07
    embaralharEPreencher(capacidadeDaMochila, pesos, valores, gen);
    selecaoAleatoria(capacidadeDaMochila, pesos, valores, gen);

    // Executar os algoritmos da aula 08
    int valorTotal = 0, pesoTotal = 0;
    mochilaCheia(capacidadeDaMochila, pesos, valores, valorTotal, pesoTotal);  // Agora retorna valores corretos
    substituicaoDeObjeto(capacidadeDaMochila, pesos, valores);  // Executa corretamente a substituição de objetos
    hillClimbing(capacidadeDaMochila, pesos, valores);  // Executa corretamente o Hill Climbing

    return 0;
}
