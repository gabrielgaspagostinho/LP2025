# Análise Comparativa de Estruturas de Dados: AVL Tree vs. Min-Heap

Este repositório contém o trabalho final da disciplina, focando na implementação e análise comparativa de desempenho entre duas estruturas de dados fundamentais — Árvore AVL (Padrão) e Min-Heap (Pura) — aplicadas a um cenário de processamento de dados de sensores de temperatura.

## 1\. Resumo

O objetivo deste projeto é comparar a eficiência de uma Árvore Binária de Busca Balanceada (AVL) e uma Fila de Prioridade (Min-Heap) em operações críticas para sistemas de monitoramento. Foram implementadas versões "puras" (sem metadados estatísticos adicionais) de ambas as estruturas em C++. Um *benchmark* foi desenvolvido para ingerir um dataset de temperaturas gerado sinteticamente e cronometrar as operações de inserção em massa, cálculo da mediana, consulta por intervalo e remoção de elementos.

## 2\. Descrição do Problema e da Escolha da Solução

**O Problema:** Sistemas que lidam com dados de sensores (IoT) precisam de alta performance para ingerir grandes volumes de dados rapidamente. No entanto, também é necessário realizar consultas analíticas (como encontrar a mediana ou filtrar por faixas de temperatura) e remover dados antigos para liberar memória.

**A Solução e as Estruturas:**
A escolha das estruturas visa contrastar duas abordagens distintas:

  * **Min-Heap (Pura):** Estrutura otimizada para acesso rápido ao elemento mínimo e inserções eficientes, teoricamente ideal para a fase de ingestão de dados.
  * **Árvore AVL (Padrão):** Estrutura que mantém os dados totalmente ordenados e balanceados, garantindo que operações de busca e remoção arbitrária não se degradem, teoricamente melhor para consultas e manutenção.

## 3\. Descrição Técnica das Estruturas e Ferramentas

### Min-Heap Pura (`heaptree.cpp`)

Implementada utilizando um vetor dinâmico (`std::vector`) como estrutura subjacente. Mantém a propriedade de heap onde cada nó pai é menor ou igual aos seus filhos.

  * **Características:** É uma estrutura parcialmente ordenada. O acesso ao menor elemento é $O(1)$, mas não oferece mecanismos eficientes para busca de elementos arbitrários ou ordenação total necessária para estatísticas como a mediana sem processamento adicional.

### Árvore AVL Padrão (`AVLtree.cpp`)

Uma árvore binária de busca (BST) auto-balanceada. Cada nó armazena sua altura, e a diferença de altura entre as subárvores esquerda e direita nunca excede 1.

  * **Características:** Mantém os dados em ordem total. Utiliza rotações (simples e duplas) após inserções e remoções para garantir que a altura da árvore permaneça logarítmica $O(\log N)$, facilitando buscas e consultas por intervalo.

### Gerador de Dados Sintéticos (`gerardados.cpp`)

Um utilitário desenvolvido especificamente para este projeto para garantir a reprodutibilidade dos testes.

  * **Técnica:** Ao invés de usar o `rand()` padrão do C (que possui baixa qualidade estatística), este script utiliza a biblioteca moderna `<random>` do C++11 com o motor **Mersenne Twister (`std::mt19937`)**.
  * **Função:** Gera uma distribuição uniforme de números de ponto flutuante (representando temperaturas) dentro do intervalo realista de -10.0°C a 45.0°C, formatados com precisão de duas casas decimais.

## 4\. Metodologia de Comparação

Os tempos foram medidos utilizando a biblioteca `<chrono>` do C++ em um ambiente controlado.

  * **Dados de Entrada:** Arquivo `temperaturas.csv` gerado pelo script `gerardados.cpp`, contendo 1.000 registros.
  * **Unidade de Medida:** Microssegundos ($\mu s$).
  * **Cenários de Teste (conforme log de execução):**
    1.  **Insert (1000x):** Tempo total para inserir todos os 1.000 elementos do dataset.
    2.  **Median Calc:** Tempo para calcular a mediana do conjunto de dados atual.
    3.  **Range Query:** Tempo para encontrar todos os elementos dentro de uma faixa específica (ex: 20.0 a 30.0).
    4.  **Remove (100x):** Tempo total para remover 100 elementos específicos da estrutura.

## 5\. Resultados Experimentais

Os resultados abaixo foram obtidos diretamente da execução do *benchmark*.

### Tabela de Resultados

| Operação | Tempo HEAP (Pura) ($\mu s$) | Tempo AVL (Padrão) ($\mu s$) | Vencedor |
| :--- | :---: | :---: | :---: |
| **Insert (1000x)** | **109** | 1013 | **HEAP** |
| **Median Calc** | 142 | **68** | **AVL** |
| **Range Query** | 40 | **18** | **AVL** |
| **Remove (100x)** | 239 | **42** | **AVL** |

### Gráfico Comparativo (Visualização Textual)

Abaixo está uma representação visual dos tempos de execução (quanto menor a barra, melhor o desempenho).

```text
Escala (us): 0      200      400      600      800      1000+
             |--------|--------|--------|--------|--------|

Inserção:
  HEAP (109) : [====>       ]
  AVL (1013) : [==================================================>]

Mediana:
  HEAP (142) : [=====>      ]
  AVL (68)   : [==>         ]

Range Query:
  HEAP (40)  : [=>          ]
  AVL (18)   : [>           ]

Remoção:
  HEAP (239) : [=========>  ]
  AVL (42)   : [=>          ]
```

### Análise dos Resultados

Com base nos dados coletados e no conhecimento teórico das estruturas:

1.  **Inserção (Vencedor: Heap):** A Heap foi aproximadamente 9x mais rápida. Isso ocorre porque a inserção na Heap envolve apenas operações de *sift-up* (trocas simples no vetor) com complexidade $O(\log N)$ simples. A AVL, além da inserção, precisa verificar fatores de balanceamento e realizar rotações complexas para manter sua estrutura.
2.  **Remoção e Range Query (Vencedor: AVL):** A AVL teve um desempenho muito superior.
      * Para **Range Query** e **Remoção**, a AVL utiliza sua propriedade de ordenação e balanceamento para encontrar nós em $O(\log N)$ e ignorar subárvores inteiras que estão fora do intervalo de busca.
      * A Heap pura, por não ser totalmente ordenada, obriga uma varredura linear $O(N)$ no vetor para encontrar elementos específicos para remoção ou para verificar quais se encaixam em um intervalo.
3.  **Cálculo da Mediana (Vencedor: AVL):** Embora ambas as implementações "puras" não sejam ideais para isso, a AVL foi mais rápida.
      * Na **AVL**, foi realizado um percurso *in-order* para linearizar os dados em $O(N)$ e encontrar o centro.
      * Na **Heap**, foi necessário copiar todo o vetor e realizar uma ordenação completa ($O(N \log N)$) para encontrar a mediana, tornando a operação mais custosa.

## 6\. Organização do Código e dos Arquivos

  * **`trabalhofinal/`**: Diretório raiz do projeto.
      * **`AVLtree.cpp` / `AVLtree.h`**: Definição da estrutura do nó, classes e implementação dos métodos da Árvore AVL (rotações, inserção, percursos).
      * **`heaptree.cpp` / `heaptree.h`**: Definição e implementação da classe `PureMinHeap` e seus métodos de manipulação de vetor (*sift-up/down*).
      * **`benchmark.cpp`**: Arquivo principal contendo a função `main`. Responsável por ler o CSV, executar os testes cronometrados nas estruturas e exibir os resultados.
      * **`gerardados.cpp`**: Código fonte responsável pela geração estocástica dos dados de teste utilizando `std::mt19937`.
      * **`temperaturas.csv`**: Dataset gerado contendo as 1.000 amostras.
      * **`README.md`**: Este arquivo de documentação.

**Instruções de Execução:**

1.  Gerar os dados (opcional, caso o CSV não exista):
    ```bash
    g++ gerardados.cpp -o gerardados
    ./gerardados
    ```
2.  Compilar e executar o Benchmark:
    ```bash
    g++ benchmark.cpp AVLtree.cpp heaptree.cpp -o benchmark_final
    ./benchmark_final
    ```

## 7\. Uso de Ferramentas de IA Generativa

Ferramentas de IA generativa foram utilizadas para auxiliar na estruturação deste documento `README.md`, garantindo que ele atendesse a todos os requisitos de documentação mínima exigidos no enunciado do trabalho (estrutura de tópicos) e para formatar os dados do benchmark real em tabelas e gráficos textuais markdown.

## 8\. Referências

  * Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. *Introduction to Algorithms*. 3rd ed. MIT Press. (Seções sobre Heaps e Árvores Binárias de Busca Balanceadas).
  * Material de aula da disciplina de Estrutura de Dados.
  * Documentação da referência C++ para a biblioteca `<chrono>`, `<vector>` e `<random>`.
