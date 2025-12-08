#include <iostream>
#include <vector>
#include <algorithm>

// Estrutura do Nó
struct Node {
    double key;
    Node *left;
    Node *right;
    int height;

    Node(double k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Classe AVL Tree
class AVLTree {
private:
    Node* root;

    // Utilitários de Altura
    int height(Node* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int getBalance(Node* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    // Rotações
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;
        return y;
    }

    // Inserção Recursiva
    Node* insert(Node* node, double key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else 
            node->right = insert(node->right, key); // Duplicatas vão para a direita

        node->height = 1 + std::max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Casos de Rotação
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

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) current = current->left;
        return current;
    }

    // Remoção Recursiva
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

    // Auxiliares Estatísticos (Geram lista ordenada)
    void inorderToList(Node* root, std::vector<double>& list) {
        if (root != nullptr) {
            inorderToList(root->left, list);
            list.push_back(root->key);
            inorderToList(root->right, list);
        }
    }

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

    void printSorted() {
        std::vector<double> list;
        inorderToList(root, list);
        std::cout << "AVL Ordenada: ";
        for (double val : list) std::cout << val << " ";
        std::cout << std::endl;
    }

    double median() {
        std::vector<double> list;
        inorderToList(root, list);
        if (list.empty()) return 0.0;
        int n = list.size();
        if (n % 2 != 0) return list[n / 2];
        else return (list[n / 2 - 1] + list[n / 2]) / 2.0;
    }

    std::vector<double> min(int n) {
        std::vector<double> list;
        inorderToList(root, list);
        std::vector<double> res;
        for(int i=0; i<n && i<list.size(); i++) res.push_back(list[i]);
        return res;
    }

    std::vector<double> max(int n) {
        std::vector<double> list;
        inorderToList(root, list);
        std::vector<double> res;
        for(int i=0; i<n && i<list.size(); i++) 
            res.push_back(list[list.size() - 1 - i]);
        return res;
    }

    std::vector<double> rangeQuery(double x, double y) {
        std::vector<double> res;
        rangeQueryRec(root, x, y, res);
        return res;
    }
};

// Main para Teste
int main() {
    AVLTree avl;
    
    // Testes Básicos
    avl.insert(10.0);
    avl.insert(20.0);
    avl.insert(10.0); // Duplicata
    avl.insert(30.0);
    avl.insert(5.0);
    avl.insert(25.5);

    std::cout << "--- TESTE AVL TREE ---" << std::endl;
    avl.printSorted();
    
    std::cout << "Mediana: " << avl.median() << std::endl;
    
    std::vector<double> mins = avl.min(2);
    std::cout << "2 Menores: " << mins[0] << ", " << mins[1] << std::endl;

    std::cout << "Removendo 20.0..." << std::endl;
    avl.remove(20.0);
    avl.printSorted();

    return 0;
}