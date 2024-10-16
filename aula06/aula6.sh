#!/bin/bash

# Verificar se o número correto de argumentos foi fornecido
if [ $# -ne 1 ]; then
    echo "Uso: $0 <numero_de_iteracoes>"
    exit 1
fi

# Obter o número de iterações do primeiro argumento
iteracoes=$1

# Compilar o código
g++ -o aula6 aula6.cpp

# Verificar se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida!"

    # Inicializar o arquivo CSV com o cabeçalho
    echo "arquivo,n_iteracoes,n_itens,valor_maximo,peso_ocupado,n_combinacoes,tempo_medio" > resultados.csv

    # Executar o programa para cada arquivo que corresponda ao padrão entrada*.txt
    for arquivo in entrada*.txt; do
        if [ -f "$arquivo" ]; then
            # Executar o programa e capturar a saída
            resultado=$(./aula6 "$arquivo" "$iteracoes")

            # Extrair informações da saída do programa
            numeroDeItens=$(echo "$resultado" | grep -oP 'Numero medio de combinacoes avaliadas: \K[0-9]+')
            valorMaximo=$(echo "$resultado" | grep -oP 'Valor maximo: \K[0-9]+')
            pesoOcupado=$(echo "$resultado" | grep -oP 'Peso ocupado: \K[0-9]+')
            mediaCombinacoes=$(echo "$resultado" | grep -oP 'Numero medio de combinacoes avaliadas: \K[0-9]+')
            tempoMedioExecucao=$(echo "$resultado" | grep -oP 'Tempo medio de execucao: \K[0-9]+')

            # Adicionar os dados no arquivo CSV
            echo "$arquivo,$iteracoes,$numeroDeItens,$valorMaximo,$pesoOcupado,$mediaCombinacoes,$tempoMedioExecucao" >> resultados.csv
        else
            echo "Nenhum arquivo de entrada encontrado com o padrão entrada*.txt"
            break
        fi
    done

    echo "Resultados salvos em resultados.csv"
else
    echo "Erro na compilação."
fi
