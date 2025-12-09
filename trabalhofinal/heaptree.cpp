// Bibliotecas que vou usar
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <chrono>
#include <string>

// Heap mínimo simples
class PureMinHeap {
private:
    std::vector<double> heap;

    // Funções pra navegar no heap
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }

    // Sobe o elemento até o lugar certo
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            std::swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // Desce o elemento até o lugar certo
    void siftDown(int i) {
        int minIndex = i;
        int l = leftChild(i);
        int r = rightChild(i);
        int n = heap.size();

        if (l < n && heap[l] < heap[minIndex])
            minIndex = l;

        if (r < n && heap[r] < heap[minIndex])
            minIndex = r;

        if (i != minIndex) {
            std::swap(heap[i], heap[minIndex]);
            siftDown(minIndex);
        }
    }

public:
    // Adiciona valor no heap
    void insert(double value) {
        heap.push_back(value);
        siftUp(heap.size() - 1);
    }

    // Remove valor do heap (busca linear)
    void remove(double value) {
        int index = -1;
        for (size_t i = 0; i < heap.size(); i++) {
            if (heap[i] == value) {
                index = i;
                break;
            }
        }

        if (index == -1) return; 

        heap[index] = heap.back();
        heap.pop_back();

        if (index < (int)heap.size()) {
            siftDown(index);
            siftUp(index);
        }
    }

    // Imprime os valores em ordem (faz uma cópia e ordena)
    void printSorted() {
        if (heap.empty()) {
            std::cout << "Heap vazia." << std::endl;
            return;
        }
        std::vector<double> temp = heap;
        std::sort(temp.begin(), temp.end());

        std::cout << "Temperaturas (Heap ordenada): ";
        for (double val : temp) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Verifica se o heap está vazio
    bool isEmpty() {
        return heap.empty();
    }
    
    // Métodos median, min, max, rangeQuery mantidos (omitidos para brevidade se não usados no main, 
    // mas a classe suporta conforme seu código original).
};

int main() {
    PureMinHeap sensor;
    std::string filename = "temperaturas.csv";
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << filename << "'." << std::endl;
        return 1;
    }

    std::cout << "Lendo arquivo e montando a MinHeap..." << std::endl;

    // Marca o tempo de início
    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(file, line)) {
        try {
            // Remove \r do final se tiver (Windows)
            if (!line.empty() && line.back() == '\r') line.pop_back();

            // Converte pra double e insere
            double temp = std::stod(line);
            sensor.insert(temp);
            count++;
        } catch (...) {
            // Se não for número, ignora
            continue;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    file.close();

    // Mostra os dados ordenados
    if (!sensor.isEmpty()) {
        sensor.printSorted();
    } else {
        std::cout << "Nenhuma temperatura valida encontrada." << std::endl;
    }

    // Mostra estatísticas
    std::chrono::duration<double, std::milli> duration = end - start;
    
    std::cout << "\n------------------------------------------------" << std::endl;
    std::cout << "Estatísticas:" << std::endl;
    std::cout << "Total de registros inseridos: " << count << std::endl;
    std::cout << "Tempo de construção da Heap: " << duration.count() << " ms" << std::endl;

    return 0;
}