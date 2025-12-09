// Bibliotecas que vou usar
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>  // pra ler arquivo
#include <sstream>
#include <chrono>   // pra medir tempo

// Estrutura do nó da árvore
struct Node {
    double key;
    Node *left;
    Node *right;
    int height;
    Node(double k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Classe da AVL
class AVLTree {
private:
    Node* root;

    // Função pra pegar altura
    int height(Node* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    // Calcula o balanceamento
    int getBalance(Node* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    // Rotação à direita
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    // Rotação à esquerda
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    // Inserção recursiva
    Node* insert(Node* node, double key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else 
            node->right = insert(node->right, key); // se for igual, vai pra direita

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Casos de rotação
        if (balance > 1 && key < node->left->key) return rightRotate(node);
        if (balance < -1 && key >= node->right->key) return leftRotate(node);
        if (balance > 1 && key >= node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    // Pega o nó com menor valor
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) current = current->left;
        return current;
    }

    // Remove um nó
    Node* remove(Node* root, double key) {
        if (root == nullptr) return root;

        if (key < root->key) root->left = remove(root->left, key);
        else if (key > root->key) root->right = remove(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = remove(root->right, temp->key);
            }
        }

        if (root == nullptr) return root;

        root->height = 1 + std::max(height(root->left), height(root->right));
        int balance = getBalance(root);

        // Balanceia se precisar
        if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    // Preenche um vetor com os valores em ordem
    void inorderToList(Node* root, std::vector<double>& list) {
        if (root != nullptr) {
            inorderToList(root->left, list);
            list.push_back(root->key);
            inorderToList(root->right, list);
        }
    }

    // Busca todos os valores no intervalo [x, y]
    void rangeQueryRec(Node* node, double x, double y, std::vector<double>& res) {
        if (node == nullptr) return;
        if (x < node->key) rangeQueryRec(node->left, x, y, res);
        if (node->key >= x && node->key <= y) res.push_back(node->key);
        if (y > node->key) rangeQueryRec(node->right, x, y, res);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(double value) { root = insert(root, value); }
    void remove(double value) { root = remove(root, value); }

    // Imprime os valores em ordem
    void printSorted() {
        std::vector<double> list;
        inorderToList(root, list);
        std::cout << "Temperaturas Ordenadas (AVL): ";
        for (double val : list) std::cout << val << " ";
        std::cout << std::endl;
    }

    // Calcula a mediana
    double median() {
        std::vector<double> list;
        inorderToList(root, list);
        if (list.empty()) return 0.0;
        int n = list.size();
        if (n % 2 != 0) return list[n / 2];
        else return (list[n / 2 - 1] + list[n / 2]) / 2.0;
    }

    // Verifica se a árvore está vazia
    bool isEmpty() {
        return root == nullptr;
    }
};

int main() {
    AVLTree avl;
    std::string filename = "temperaturas.csv";
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo '" << filename << "'." << std::endl;
        std::cerr << "Certifique-se que ele está na mesma pasta do executável." << std::endl;
        return 1;
    }

    std::cout << "Lendo arquivo e montando a AVL..." << std::endl;

    // Marca o tempo de início
    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(file, line)) {
        try {
            // Remove \r do final se tiver (Windows)
            if (!line.empty() && line.back() == '\r') line.pop_back();

            // Tenta converter pra double
            double temp = std::stod(line);
            avl.insert(temp);
            count++;
        } catch (...) {
            // Se não for número, ignora
            continue;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Fecha o arquivo
    file.close();

    // Mostra a árvore ordenada
    if (!avl.isEmpty()) {
        avl.printSorted();
    } else {
        std::cout << "Nenhuma temperatura válida encontrada no arquivo." << std::endl;
    }

    // Mostra estatísticas
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "\n------------------------------------------------" << std::endl;
    std::cout << "Estatísticas:" << std::endl;
    std::cout << "Total de registros inseridos: " << count << std::endl;
    std::cout << "Tempo de construção da Árvore: " << duration.count() << " ms" << std::endl;

    return 0;
}