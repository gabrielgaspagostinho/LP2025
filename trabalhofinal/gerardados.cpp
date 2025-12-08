#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>

int main() {
    // Configurações
    const std::string nome_arquivo = "temperaturas.csv";
    const int qtd_dados = 1000;
    const double temp_min = -10.0;
    const double temp_max = 45.0;

    // Abre o arquivo para escrita
    std::ofstream arquivo(nome_arquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao criar o arquivo " << nome_arquivo << "!" << std::endl;
        return 1;
    }

    // Configura o gerador de números aleatórios (Mersenne Twister)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(temp_min, temp_max);

    // Configura a formatação para 2 casas decimais
    arquivo << std::fixed << std::setprecision(2);

    // Gera e escreve os dados
    for (int i = 0; i < qtd_dados; ++i) {
        double temperatura = dis(gen);
        arquivo << temperatura << "\n";
    }

    arquivo.close();

    std::cout << "Sucesso! Arquivo '" << nome_arquivo 
            << "' gerado com " << qtd_dados << " registros." << std::endl;

    return 0;
}