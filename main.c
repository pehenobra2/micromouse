#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

// Definição das constantes para os comandos
#define COMANDO_ESQUERDA 'l'
#define COMANDO_DIREITA 'r'
#define COMANDO_CAMINHAR 'w'
#define COMANDO_CORRIDINHA 'j'
#define COMANDO_CORRER 'R'
#define COMANDO_CORRER_MAXIMO 's'
#define COMANDO_SENSOR_PAREDES 'c'
#define COMANDO_SENSOR_OBJETIVO 'd'

// Estruturas de dados e funções

// Definindo constantes para valores de retorno
#define RESULTADO_PAREDE 0
#define RESULTADO_MOVIMENTO_SUCESSO 1
#define RESULTADO_OBJETIVO_ENCONTRADO 2

typedef struct No {
    int v;
    int valor;
    int x, y;
    struct No *prox;
} No;

typedef struct Grafo {
    int n;
    struct No **adj;
} Grafo;

typedef struct HN {
    unsigned long long key;
    int valor;
    int x, y;
    struct HN *next;
} HN;

typedef struct noPilha {
    int v;
    struct noPilha *prox;
} noPilha;

typedef struct HT {
    int tamanho;
    struct HN **tabela;
} HT;

#define TAM_MAX_GRAFO 14400
#define TAM_TABELA 120

// Função para inicializar o labirinto
void inicializarLabirinto(char labirinto[TAM_MAX_GRAFO][TAM_MAX_GRAFO], int *tamanho, int *inicioX, int *inicioY) {
    // Ler o tamanho do labirinto
    scanf("%d", tamanho);

    // Ler o labirinto
    for (int i = 0; i < *tamanho; i++) {
        for (int j = 0; j < *tamanho; j++) {
            scanf(" %c", &labirinto[i][j]);  // Ignorar espaços em branco antes do caractere
            if (labirinto[i][j] == 'S') {
                // Encontrou a posição inicial
                *inicioX = i;
                *inicioY = j;
            }
        }
    }

    // Exibir o labirinto para depuração
    exibirLabirinto(labirinto, *tamanho);
}

// Função para criar um novo nó
No *criaNo(int v, int valor, int x, int y) {
    No *novo = (No *)malloc(sizeof(No));
    novo->v = v;
    novo->valor = valor;
    novo->x = x;
    novo->y = y;
    novo->prox = NULL;
    return novo;
}

// Função para criar um novo grafo
Grafo *criaGrafo() {
    Grafo *g = (Grafo *)malloc(sizeof(Grafo));
    g->n = TAM_MAX_GRAFO;
    g->adj = (No **)malloc(TAM_MAX_GRAFO * sizeof(No *));
    return g;
}

// Função para adicionar uma aresta no grafo
void addAresta(Grafo *g, int src, int v, int valor, int x, int y) {
    No *no = criaNo(v, valor, x, y);
    no->prox = g->adj[src];
    g->adj[src] = no;
}

// Função para criar uma nova entrada na tabela hash
HN *criaHN(unsigned long long key, int valor, int x, int y) {
    HN *novo = (HN *)malloc(sizeof(HN));
    novo->key = key;
    novo->valor = valor;
    novo->x = x;
    novo->y = y;
    novo->next = NULL;
    return novo;
}

// Função para criar uma nova tabela hash
HT *criarHt() {
    HT *ht = (HT *)malloc(sizeof(HT));
    ht->tamanho = TAM_TABELA;
    ht->tabela = (HN **)malloc(TAM_TABELA * sizeof(HN *));

    for (int i = 0; i < TAM_TABELA; i++) {
        ht->tabela[i] = NULL;
    }

    return ht;
}

// Função para criar uma pilha
noPilha *criaPilha(int v) {
    noPilha *no = (noPilha *)malloc(sizeof(noPilha));
    no->v = v;
    no->prox = NULL;
    return no;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(noPilha *raiz) {
    return raiz == NULL;
}

// Função para empilhar um novo elemento na pilha
void empilhar(noPilha **raiz, int v) {
    noPilha *no = (noPilha *)malloc(sizeof(noPilha));
    no->v = v;
    no->prox = *raiz;
    *raiz = no;
}

// Função para desempilhar um elemento da pilha
int desempilhar(noPilha **raiz) {
    if (pilhaVazia(*raiz)) {
        return -1;
    }
    noPilha *aux = *raiz;
    *raiz = (*raiz)->prox;
    int v = aux->v;
    free(aux);
    return v;
}

// Função para esvaziar a pilha
void esvaziaPilha(noPilha **raiz) {
    noPilha *atual = *raiz;
    noPilha *prox;
    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }
    *raiz = NULL;
}

// Função para realizar uma ação no labirinto e obter o resultado
int fazerAcao(char acao, Grafo *grafo, noPilha **pilha, HT *tabela) {
    printf("%c\n", acao);
    fflush(stdout);

    int resultado;
    scanf("%d", &resultado);
    fflush(stdin);

    // Atualize as estruturas de dados com base na ação e no resultado obtido

    return resultado;
}

// Função para executar a busca em largura (BFS)
int bfs(Grafo *grafo, int inicio, int objetivo) {
    int *distancia = malloc(TAM_MAX_GRAFO * sizeof(int));
    bool *visitado = malloc(TAM_MAX_GRAFO * sizeof(bool));

    for (int i = 0; i < TAM_MAX_GRAFO; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = false;
    }

    noPilha *fila = NULL;
    empilhar(&fila, inicio);
    distancia[inicio] = 0;
    visitado[inicio] = true;

    while (!pilhaVazia(fila)) {
        int u = desempilhar(&fila);

        No *vizinho = grafo->adj[u];
        while (vizinho != NULL) {
            int v = vizinho->v;
            if (!visitado[v]) {
                visitado[v] = true;
                distancia[v] = distancia[u] + 1;
                empilhar(&fila, v);
            }
            vizinho = vizinho->prox;
        }
    }

    int resultado = distancia[objetivo];
    free(distancia);
    free(visitado);
    esvaziaPilha(&fila);

    return resultado;
}

void encontrarQueijo(Grafo *grafo, noPilha **pilha, HT *tabela) {
    int inicio = 0;
    int objetivoX, objetivoY;

    int *distancia = malloc(TAM_MAX_GRAFO * sizeof(int));
    int *caminho = malloc(TAM_MAX_GRAFO * sizeof(int));
    bool *visitado = malloc(TAM_MAX_GRAFO * sizeof(bool));

    for (int i = 0; i < TAM_MAX_GRAFO; i++) {
        distancia[i] = INT_MAX;
        visitado[i] = false;
        caminho[i] = -1;
    }

    noPilha *fila = NULL;
    empilhar(&fila, inicio);
    distancia[inicio] = 0;
    visitado[inicio] = true;

    int objetivo = -1;

    while (!pilhaVazia(fila)) {
        int u = desempilhar(&fila);

        No *vizinho = grafo->adj[u];
        while (vizinho != NULL) {
            int v = vizinho->v;
            if (!visitado[v]) {
                visitado[v] = true;
                distancia[v] = distancia[u] + 1;
                caminho[v] = u;
                empilhar(&fila, v);

                if (v == objetivo) {
                    objetivoX = vizinho->x;
                    objetivoY = vizinho->y;

                    esvaziaPilha(&fila);
                    break;
                }
            }
            vizinho = vizinho->prox;
        }
    }

    free(distancia);
    free(visitado);
    free(caminho);

    int atual = objetivo;
    while (caminho[atual] != -1) {
        atual = caminho[atual];
    }
}

int main() {
    HT *tabela = criarHt();
    Grafo *grafo = criaGrafo();
    noPilha *pilha = criaPilha(-1);

    char labirinto[TAM_MAX_GRAFO][TAM_MAX_GRAFO];
    int tamanhoLabirinto;
    int inicioX, inicioY;

    // Inicializar o labirinto
    inicializarLabirinto(labirinto, &tamanhoLabirinto, &inicioX, &inicioY);


    int tamanhoLabirinto = 10;

    for (int i = 0; i < tamanhoLabirinto; i++) {
        for (int j = 0; j < tamanhoLabirinto; j++) {
            addAresta(grafo, i * tamanhoLabirinto + j, i * tamanhoLabirinto + j + 1, 1, i, j);
            addAresta(grafo, i * tamanhoLabirinto + j, (i + 1) * tamanhoLabirinto + j, 1, i, j);
        }
    }

    int posicaoAtual = 0;

    encontrarQueijo(grafo, &pilha, tabela);

    int resultadoMovimento;
    while (!pilhaVazia(pilha)) {
        int proximaPosicao = desempilhar(&pilha);

        int deltaX = grafo->adj[proximaPosicao]->x - grafo->adj[posicaoAtual]->x;
        int deltaY = grafo->adj[proximaPosicao]->y - grafo->adj[posicaoAtual]->y;

        if (deltaX == 1) {
            resultadoMovimento = fazerAcao(COMANDO_ESQUERDA, grafo, &pilha, tabela);
        } else if (deltaX == -1) {
            resultadoMovimento = fazerAcao(COMANDO_DIREITA, grafo, &pilha, tabela);
        } else if (deltaY == 1) {
            resultadoMovimento = fazerAcao(COMANDO_CAMINHAR, grafo, &pilha, tabela);
        }

        if (resultadoMovimento != RESULTADO_MOVIMENTO_SUCESSO) {
            break;
        }

        posicaoAtual = proximaPosicao;
    }

    encontrarQueijo(grafo, &pilha, tabela);

    return 0;
}
