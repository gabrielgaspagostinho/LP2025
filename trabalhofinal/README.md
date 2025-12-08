# Análise Comparativa: AVL, Heap e Vector (Insertion Sort)

Este repositório contém o trabalho final da disciplina, focado na análise de desempenho de estruturas de dados aplicadas ao processamento de sensores de temperatura. O projeto compara uma **Árvore AVL**, uma **Min-Heap** e um **Vetor com Ordenação Tardia** (Lazy Insertion Sort).

## 1\. Resumo

O projeto implementa três estruturas de dados fundamentais em C++ sem o uso de bibliotecas de contêineres complexas (como `std::set` ou `std::priority_queue`). O objetivo é processar um fluxo de dados de temperatura e medir a eficiência de cada estrutura em quatro operações críticas: ingestão de dados, cálculos estatísticos (mediana), filtragem por intervalo e limpeza de dados.

## 2\. Descrição do Problema e da Escolha da Solução

**O Problema:** Em sistemas embarcados e IoT, a memória e o processamento são limitados. É crucial escolher a estrutura de dados correta dependendo do perfil de uso da aplicação (ex: um sistema que apenas grava dados versus um sistema que realiza muitas consultas estatísticas).

**Soluções Escolhidas:**

1.  **Min-Heap (Pura):** Escolhida pela sua teórica eficiência na inserção e acesso imediato ao valor mínimo.
2.  **Árvore AVL (Padrão):** Escolhida para garantir buscas e remoções eficientes ($O(\log n)$) através do auto-balanceamento, evitando a degradação de performance.
3.  **Vector (Insertion Sort):** Adicionado como "Grupo de Controle". Ele utiliza uma estratégia *Lazy*: insere instantaneamente no final do vetor ($O(1)$), mas paga um alto custo de ordenação ($O(N^2)$) apenas quando a estatística é solicitada.

## 3\. Descrição Técnica das Estruturas de Dados

  * **Min-Heap Pura (`heaptree.cpp`):** Implementada sobre um vetor dinâmico. Mantém a propriedade de que o nó pai é sempre menor que os filhos. Otimizada para inserção ($O(\log n)$ via *sift-up*), mas lenta para buscas arbitrárias ($O(n)$).
  * **Árvore AVL Padrão (`AVLtree.cpp`):** Árvore Binária de Busca onde a diferença de altura das subárvores é no máximo 1. Garante ordenação total dos dados. Utiliza rotações simples e duplas para manter o balanceamento após inserções e remoções.
  * **Vector Sorter (`insetionsort.cpp`):** Encapsula um `std::vector` simples.
      * *Inserção:* $O(1)$ amortizado (apenas `push_back`, sem ordenação no momento da escrita).
      * *Mediana:* Utiliza o algoritmo **Insertion Sort** ($O(N^2)$) para ordenar o vetor apenas no momento em que a mediana é requisitada.
      * *Range Query:* Realiza varredura linear ($O(N)$).

## 4\. Metodologia de Comparação

  * **Ambiente:** Compilador C++ (G++), medição de tempo via biblioteca `<chrono>`.
  * **Dados:** 1.000 registros de temperatura (float) gerados aleatoriamente via *Mersenne Twister* (`gerardados.cpp`).
  * **Métrica:** Tempo absoluto de execução em Microssegundos ($\mu s$).
  * **Cenários Testados:**
    1.  **Insert:** Inserção de todos os 1.000 elementos.
    2.  **Median Calc:** Cálculo da mediana (exige dados ordenados).
    3.  **Range Query:** Busca de valores entre 20.0 e 30.0.
    4.  **Remove:** Remoção de 100 elementos específicos do conjunto.

## 5\. Resultados Experimentais

Os dados abaixo foram coletados da execução real do `benchmark.cpp`.

### Tabela de Resultados

| Operação | Tempo HEAP ($\mu s$) | Tempo AVL ($\mu s$) | Tempo VEC (Ins) ($\mu s$) | Vencedor |
| :--- | :---: | :---: | :---: | :---: |
| **Insert (1000x)** | 170 | 1481 | **87** | **VEC** |
| **Median Calc** | 102 | **46** | 2801 | **AVL** |
| **Range Query** | 29 | 24 | **15** | **VEC** |
| **Remove (100x)** | 208 | **41** | 124 | **AVL** |

### Gráfico Visual

```text
Escala (us): 0      200      400      600      800      1000+
             |--------|--------|--------|--------|--------|

Inserção:
  VEC  (87)   : [==>         ]
  HEAP (170)  : [===>        ]
  AVL  (1481) : [========================================>]

Mediana:
  AVL  (46)   : [=>          ]
  HEAP (102)  : [==>         ]
  VEC  (2801) : [========================================>]

Range Query:
  VEC  (15)   : [>           ]
  AVL  (24)   : [>           ]
  HEAP (29)   : [>           ]

Remoção:
  AVL  (41)   : [=>          ]
  VEC  (124)  : [==>         ]
  HEAP (208)  : [====>       ]
```

### Análise dos Dados

1.  **Inserção:** O **Vector** venceu (87us) pois realiza apenas uma operação de adição ao final da memória ($O(1)$), superando a Heap (170us) que faz trocas logarítmicas e a AVL (1481us) que tem alto custo de alocação dinâmica de nós e rotações de balanceamento.
2.  **Mediana:** O **Vector** teve o pior desempenho disparado (2801us), validando a ineficiência do *Insertion Sort* ($O(N^2)$) para ordenar grandes volumes de dados de uma só vez. A **AVL** venceu (46us) pois os dados já estão estruturados, exigindo apenas um percurso linear.
3.  **Range Query:** O **Vector** venceu (15us), superando levemente a AVL (24us).
      * *Explicação Técnica:* Embora a AVL tenha melhor complexidade assintótica (ignora ramos da árvore), para um $N=1000$, a varredura linear de um vetor é extremamente rápida devido à **localidade de cache** (dados contíguos na RAM). A AVL sofre com *cache misses* ao pular entre ponteiros de nós dispersos na memória.
4.  **Remoção:** A **AVL** venceu (41us) mostrando sua robustez. Sua capacidade de busca binária ($O(\log n)$) superou a busca linear necessária tanto na Heap quanto no Vector para encontrar o elemento a ser removido.

## 6\. Organização do Código e dos Arquivos

Como o projeto não utiliza arquivos de cabeçalho (`.h`), a estrutura é composta pelos códigos fonte (`.cpp`) contendo as implementações das classes.

  * **`trabalhofinal/`**
      * **`benchmark.cpp`**: Programa principal. Contém a `main`, lê o CSV, e orquestra os testes comparativos.
      * **`AVLtree.cpp`**: Implementação da classe `AVLTree` e lógica de balanceamento.
      * **`heaptree.cpp`**: Implementação da classe `PureMinHeap`.
      * **`insetionsort.cpp`**: Implementação da classe `VectorInsertionSort` (Vector + Insertion Sort).
      * **`gerardados.cpp`**: Utilitário para gerar o arquivo CSV com números aleatórios (`std::mt19937`).
      * **`temperaturas.csv`**: Arquivo de dados gerado.

**Instruções de Compilação e Execução:**

Como não há arquivos `.h`, todos os arquivos `.cpp` das estruturas devem ser compilados junto com o benchmark:

```bash
# 1. Gerar dados (se necessário)
g++ gerardados.cpp -o gerardados
./gerardados

# 2. Compilar o Benchmark (Juntando todos os cpp)
g++ benchmark.cpp AVLtree.cpp heaptree.cpp insetionsort.cpp -o benchmark

# 3. Rodar
./benchmark
```

*(Nota: No Windows, o executável será `benchmark.exe`)*

## 7\. Uso de Ferramentas de IA Generativa

Ferramentas de IA (ChatGPT/Gemini) foram utilizadas para:

Ferramentas de IA generativa foram utilizadas para auxiliar na estruturação deste documento `README.md`, garantindo que ele atendesse a todos os requisitos de documentação mínima exigidos no enunciado do trabalho (estrutura de tópicos) e para formatar os dados do benchmark real em tabelas e gráficos textuais markdown.

## 8\. Referências

  * 
  * Documentação C++ Reference (std::vector, std::chrono).
  * 
