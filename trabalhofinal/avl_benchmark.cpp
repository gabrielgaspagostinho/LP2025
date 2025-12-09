// Incluindo as libs que vou usar
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>   // pra medir tempo
#include <iomanip>  // pra formatar saída

// Estrutura do nó da árvore AVL
struct Node {
    double key;
    Node *left;
    Node *right;
    int height;
    Node(double k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Classe da árvore AVL
class AVLTree {
private:
    Node* root;

    // Função pra pegar altura do nó
    int height(Node* N) { return (N == nullptr) ? 0 : N->height; }
    // Balanceamento
    int getBalance(Node* N) { return (N == nullptr) ? 0 : height(N->left) - height(N->right); }

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
        if (key < node->key) node->left = insert(node->left, key);
        else node->right = insert(node->right, key);

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Verifica os casos de rotação
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
                if (temp == nullptr) { temp = root; root = nullptr; }
                else *root = *temp;
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

    // Gera lista ordenada (pra não gastar tempo imprimindo)
    void getSortedList(std::vector<double>& list) {
        inorderToList(root, list);
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

    // Retorna os n menores
    std::vector<double> min(int n) {
        std::vector<double> list;
        inorderToList(root, list); // não é o mais eficiente, mas funciona
        std::vector<double> res;
        for(int i=0; i<n && i < (int)list.size(); i++) res.push_back(list[i]);
        return res;
    }

    // Retorna os n maiores
    std::vector<double> max(int n) {
        std::vector<double> list;
        inorderToList(root, list);
        std::vector<double> res;
        for(int i=0; i<n && i < (int)list.size(); i++) 
            res.push_back(list[list.size() - 1 - i]);
        return res;
    }

    // Busca todos os valores no intervalo
    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> res;
        rangeQueryRec(root, x, y, res);
        return res;
    }

    // Verifica se a árvore está vazia
    bool isEmpty() { return root == nullptr; }
};

// Main com os testes/benchmarks
int main() {
    AVLTree avl;
    std::string filename = "temperaturas.csv";
    std::ifstream file(filename);
    std::string line;
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir " << filename << std::endl;
        return 1;
    }

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "==== AVL TREE BENCHMARK ====" << std::endl;

    // 1. Inserção dos dados do arquivo
    auto start = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        try {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            avl.insert(std::stod(line));
            count++;
        } catch (...) {}
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_insert = end - start;
    std::cout << "[INSERT] " << count << " registros | Tempo: " << t_insert.count() << " ms" << std::endl;
    file.close();

    if (avl.isEmpty()) return 0;

    // 2. Mediana
    start = std::chrono::high_resolution_clock::now();
    double med = avl.median();
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_median = end - start;
    std::cout << "[MEDIAN] Resultado: " << med << " | Tempo: " << t_median.count() << " ms" << std::endl;

    // 3. Menores valores (3 menores)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> mins = avl.min(3);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_min = end - start;
    std::cout << "[MIN(3)] "; 
    for(auto v : mins) std::cout << v << " ";
    std::cout << "| Tempo: " << t_min.count() << " ms" << std::endl;

    // 4. Maiores valores (3 maiores)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> maxs = avl.max(3);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_max = end - start;
    std::cout << "[MAX(3)] "; 
    for(auto v : maxs) std::cout << v << " ";
    std::cout << "| Tempo: " << t_max.count() << " ms" << std::endl;

    // 5. Consulta por intervalo (exemplo: 20.0 até 25.0)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> range = avl.rangeQuery(20.0, 25.0);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_range = end - start;
    std::cout << "[RANGE(20, 25)] Encontrados: " << range.size() << " itens | Tempo: " << t_range.count() << " ms" << std::endl;

    // 6. Remover valor (exemplo: 22.5)
    start = std::chrono::high_resolution_clock::now();
    avl.remove(22.5); // só remove se existir
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_remove = end - start;
    std::cout << "[REMOVE(22.5)] Tempo: " << t_remove.count() << " ms" << std::endl;

    // 7. Gera lista ordenada (simula print)
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> sortedList;
    avl.getSortedList(sortedList);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_sort = end - start;
    std::cout << "[PRINTSORTED] Gerada lista de " << sortedList.size() << " itens | Tempo: " << t_sort.count() << " ms" << std::endl;

    return 0;
}