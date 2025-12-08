#include <vector>
#include <algorithm>
#include <iostream>

class VectorInsertionSort {
private:
    std::vector<double> data;
    bool sorted = false;

    // Algoritmo Insertion Sort Clássico: O(N^2)
    void runInsertionSort() {
        int n = data.size();
        for (int i = 1; i < n; i++) {
            double key = data[i];
            int j = i - 1;

            // Move os elementos maiores que a chave para frente
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j = j - 1;
            }
            data[j + 1] = key;
        }
        sorted = true;
    }

public:
    // Inserção O(1) - Apenas adiciona ao fim (desordenado)
    void insert(double value) {
        data.push_back(value);
        sorted = false; 
    }

    // Remoção O(N) - Busca linear e shift do vetor
    void remove(double value) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (*it == value) {
                data.erase(it);
                return;
            }
        }
    }

    // Mediana - Aciona a ordenação O(N^2) se necessário
    double median() {
        if (data.empty()) return 0.0;

        if (!sorted) {
            runInsertionSort();
        }

        int n = data.size();
        if (n % 2 != 0) return data[n / 2];
        else return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    }

    // Range Query O(N) - Varredura linear
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> result;
        for (double val : data) {
            if (val >= x && val <= y) {
                result.push_back(val);
            }
        }
        return result;
    }
};