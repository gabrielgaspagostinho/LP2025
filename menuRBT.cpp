#include <iostream>
#include <set>



void setup() {

  RBTMenuItem* atual

  //para colorir a arvore
  enum Color { RED, BLACK };

  //Objeto que vai representar cada nó
  struct RBTMenuItem {
    const char* nome;
    Color color;
    RBTMenuItem* pai;
    RBTMenuItem* left;
    RBTMenuItem* right;
    void (*acao)();
  };

  //criando na mão cada nó (item do menu)
  RBTMenuItem ligarLED1;
  RBTMenuItem ligarLED2;
  RBTMenuItem piscarled1;
  RBTMenuItem controleLEDs;
  RBTMenuItem config;
  RBTMenuItem menuPrincipal;

  //Menu principal é a raiz que tem que ser negra
  menuPrincipal.nome = "Menu Principal";
  menuPrincipal.color = BLACK;
  menuPrincipal.pai = NULL;
  menuPrincipal.left = &controleLEDs;
  menuPrincipal.right = &config;
  menuPrincipal.acao = NULL;

  //Filho da esquerda do menu, preto tambem
  controleLEDs.nome = "Controle de LEDs";
  controleLEDs.color = BLACK;
  controleLEDs.pai = &menuPrincipal;
  controleLEDs.left = &ligarLED1;
  controleLEDs.right = &ligarLED2;
  controleLEDs.acao = NULL;

  //Filho da esquerda do controle, vermelho por ser a ponta
  ligarLED1.nome = "Ligar LED1";
  ligarLED1.color = RED;
  ligarLED1.pai = &controleLEDs;
  ligarLED1.left = NULL;
  ligarLED1.right = NULL;
  ligarLED1.acao = &acaoLigarLED1;

  //Filho da direita do controle, vermelho por ser a ponta
  ligarLED2.nome = "Ligar LED2";
  ligarLED2.color = RED;
  ligarLED2.pai = &controleLEDs;
  ligarLED2.left = NULL;
  ligarLED2.right = NULL;
  ligarLED2.acao = &acaoLigarLED2;

  //Filho do menu principal, preto
  config.nome = "Configuracoes";
  config.color = BLACK;
  config.pai = &menuPrincipal;
  config.left = &piscarled1;
  config.right = NULL; // Não tem mais filhos
  config.acao = NULL;

  //Filho de configurações, vermelho por ser antes da folha
  piscarled1.nome = "Piscar LED1";
  piscarled1.color = RED;
  piscarled1.pai = &config;
  piscarled1.left = NULL;
  piscarled1.right = NULL;
  piscarled1.acao = &acaoPiscarLED1;

  atual = &menuPrincipal;

}

void loop() {

  //Botoes a serem clicados
  bool btnVoltar = NULL
  bool btnSel = NULL
  bool btnCima = NULL

  if(btnVoltar && atual->pai =! NULL && atual->pai->pai =! NULL){
    atual = atual->pai
  }

  if(btnSel){
    if(atual->acao =! NULL){
      atual.acao()
    }
    else if(atual->acao == NULL && atual->left =! NULL){
      atual = atual->left
    }
  }

  if(btnCima){
    if(atual->right =! NULL){
      atual = atual->right
    }
    else if(atual->left){
      atual = atual->left
    }
  }

}
