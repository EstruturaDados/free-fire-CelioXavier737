/*
 COMENTÁRIO DO ALUNO: SISTEMA COMPARATIVO DE MOCHILAS
 - Implementa duas estruturas: VETOR (lista sequencial) e LISTA ENCADEADA.
 - Opera com inserção, remoção, listagem e busca (sequencial em ambas;
   binária apenas no vetor).
 - Ordenação do vetor por BUBBLE SORT.
 - Contadores de comparações/iterações são exibidos ao final do resultado da busca.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// COMENTÁRIO DO ALUNO: limite de itens na mochila quando usando vetor
#define MAX_ITENS 10

// COMENTÁRIO DO ALUNO: limites de comprimento das strings
#define MAX_NOME 30
#define MAX_TIPO 20


// COMENTÁRIO DO ALUNO: item básico usado nas duas implementações
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

// COMENTÁRIO DO ALUNO: nó da lista encadeada
typedef struct No {
    Item dados;
    struct No *proximo;
} No;


// ======================================================================
//               VARIÁVEIS GLOBAIS DE CONFIGURAÇÃO E ESTADO
// ======================================================================

// COMENTÁRIO DO ALUNO: controle global do tipo de busca (sequencial ou binária)
// 0 = Busca Sequencial (padrão)
// 1 = Busca Binária (só no vetor)
int BUSCA_USO_BINARIA = 0;

// COMENTÁRIO DO ALUNO: tipo de operação escolhido (0 = vetor, 1 = lista)
int MODO_USO = 0;

// COMENTÁRIO DO ALUNO: versão em vetor
Item mochilaVetor[MAX_ITENS];
int numItensVetor = 0;
bool vetorOrdenado = false;

// COMENTÁRIO DO ALUNO: contador usado nas buscas
long contadorComparacoes = 0;

// COMENTÁRIO DO ALUNO: ponteiro para início da lista
No *inicioLista = NULL;


// ======================================================================
//                      PROTÓTIPOS DAS FUNÇÕES
// ======================================================================

// leitura e manuseio
void limpaEntrada();
void leString(const char *prompt, char *buffer, size_t tamanho);
int leInteiro(const char *prompt);

// vetor
void inicializaVetor();
void listarVetor();
bool inserirItemVetor(Item *novo);
bool removerItemVetor(const char *nome);
void ordenarVetorBubble();
int buscarSequencialVetor(const char *nome);
int buscarBinariaVetor(const char *nome);

// lista
void inserirItemLista(Item *novo);
bool removerItemLista(const char *nome);
void listarLista();
No* buscarSequencialLista(const char *nome);
void liberarLista();

// menus
void menuVetor();
void menuLista();
void configurarTipoBusca();
void mostrarEstadoAtual();


// ======================================================================
//                      FUNÇÕES DE ENTRADA
// ======================================================================

// COMENTÁRIO DO ALUNO: limpa o stdin até newline — usado após scanf/fgets conflitantes
void limpaEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// COMENTÁRIO DO ALUNO: lê uma linha com fgets e remove o '\n' final
void leString(const char *prompt, char *buffer, size_t tamanho) {
    printf("%s", prompt);
    if (fgets(buffer, (int)tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        limpaEntrada();
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
}

// COMENTÁRIO DO ALUNO: leitura segura de inteiro com verificação
int leInteiro(const char *prompt) {
    char linha[64];
    int valor;
    while (1) {
        printf("%s", prompt);
        if (fgets(linha, sizeof(linha), stdin) == NULL) return 0;
        if (sscanf(linha, "%d", &valor) == 1) return valor;
        printf("Entrada inválida. Digite um número inteiro.\n");
    }
}


// ======================================================================
//                      FUNÇÕES DO VETOR
// ======================================================================

void inicializaVetor() {
    numItensVetor = 0;
    vetorOrdenado = false;
}

void listarVetor() {
    if (numItensVetor == 0) {
        printf("Mochila (vetor) vazia.\n");
        return;
    }
    printf("\n--- Itens no VETOR ---\n");
    for (int i = 0; i < numItensVetor; i++) {
        printf("%-20s  %-20s  Qtd: %d\n",
               mochilaVetor[i].nome,
               mochilaVetor[i].tipo,
               mochilaVetor[i].quantidade);
    }
}

bool inserirItemVetor(Item *novo) {
    if (numItensVetor >= MAX_ITENS) return false;
    mochilaVetor[numItensVetor++] = *novo;
    vetorOrdenado = false;
    return true;
}

bool removerItemVetor(const char *nome) {
    for (int i = 0; i < numItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            mochilaVetor[i] = mochilaVetor[numItensVetor - 1];
            numItensVetor--;
            vetorOrdenado = false;
            return true;
        }
    }
    return false;
}

// COMENTÁRIO DO ALUNO: Bubble Sort, pois é simples de ver funcionando
void ordenarVetorBubble() {
    if (numItensVetor < 2) {
        printf("Vetor tem menos de 2 itens. Nada a ordenar.\n");
        return;
    }

    printf("Ordenando vetor (Bubble Sort)...\n");

    for (int i = 0; i < numItensVetor - 1; i++) {
        for (int j = 0; j < numItensVetor - 1 - i; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                Item temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j + 1];
                mochilaVetor[j + 1] = temp;
            }
        }
    }
    vetorOrdenado = true;
}

int buscarSequencialVetor(const char *nome) {
    contadorComparacoes = 0;
    for (int i = 0; i < numItensVetor; i++) {
        contadorComparacoes++;
        if (strcmp(mochilaVetor[i].nome, nome) == 0) return i;
    }
    return -1;
}

int buscarBinariaVetor(const char *nome) {
    if (!vetorOrdenado) return -2;

    int ini = 0;
    int fim = numItensVetor - 1;
    contadorComparacoes = 0;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        contadorComparacoes++;

        int cmp = strcmp(nome, mochilaVetor[meio].nome);
        if (cmp == 0) return meio;
        if (cmp < 0) fim = meio - 1;
        else ini = meio + 1;
    }
    return -1;
}


// ======================================================================
//                      FUNÇÕES DA LISTA
// ======================================================================

void inserirItemLista(Item *novo) {
    No *n = malloc(sizeof(No));
    n->dados = *novo;
    n->proximo = inicioLista;
    inicioLista = n;
}

bool removerItemLista(const char *nome) {
    No *ant = NULL, *p = inicioLista;

    while (p != NULL) {
        if (strcmp(p->dados.nome, nome) == 0) {
            if (ant == NULL) inicioLista = p->proximo;
            else ant->proximo = p->proximo;
            free(p);
            return true;
        }
        ant = p;
        p = p->proximo;
    }
    return false;
}

void listarLista() {
    if (inicioLista == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    printf("\n--- Itens na LISTA ---\n");
    No *p = inicioLista;
    while (p != NULL) {
        printf("%-20s %-20s Qtd: %d\n",
               p->dados.nome,
               p->dados.tipo,
               p->dados.quantidade);
        p = p->proximo;
    }
}

No* buscarSequencialLista(const char *nome) {
    contadorComparacoes = 0;

    No *p = inicioLista;
    while (p != NULL) {
        contadorComparacoes++;
        if (strcmp(p->dados.nome, nome) == 0) return p;
        p = p->proximo;
    }
    return NULL;
}

void liberarLista() {
    No *p = inicioLista;
    while (p != NULL) {
        No *tmp = p;
        p = p->proximo;
        free(tmp);
    }
    inicioLista = NULL;
}


// ======================================================================
//                      MENUS DE OPERAÇÃO
// ======================================================================

void menuVetor() {
    while (1) {
        printf("\n===== MOCHILA (VETOR) =====\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item por nome\n");
        printf("3 - Listar itens\n");
        printf("4 - Ordenar por nome (Bubble Sort)\n");
        printf("5 - Buscar item por nome\n");
        printf("0 - Voltar\n");
        int opc = leInteiro("Escolha opção: ");

        if (opc == 0) break;

        if (opc == 1) {
            Item it;
            leString("Nome: ", it.nome, MAX_NOME);
            leString("Tipo: ", it.tipo, MAX_TIPO);
            it.quantidade = leInteiro("Quantidade: ");
            if (inserirItemVetor(&it))
                printf("Item inserido (vetor).\n");
            else
                printf("Mochila (vetor) cheia!\n");
        }
        else if (opc == 2) {
            char nome[MAX_NOME];
            leString("Nome do item a remover: ", nome, MAX_NOME);
            if (removerItemVetor(nome))
                printf("Item removido (vetor).\n");
            else
                printf("Item não encontrado (vetor).\n");
        }
        else if (opc == 3) {
            listarVetor();
        }
        else if (opc == 4) {
            ordenarVetorBubble();
        }
        else if (opc == 5) {
            char nome[MAX_NOME];
            leString("Nome do item a buscar: ", nome, MAX_NOME);

            if (BUSCA_USO_BINARIA) {

                if (!vetorOrdenado) {
                    printf("Aviso: vetor não estava ordenado. Ordenando automaticamente para usar Busca Binária.\n");
                    ordenarVetorBubble();
                }

                int pos = buscarBinariaVetor(nome);

                if (pos == -1)
                    printf("Item NÃO encontrado (Busca Binária, %ld operacoes).\n", contadorComparacoes);
                else
                    printf("Encontrado: %-*s %-*s Qtd: %d (Busca Binária, %ld operacoes)\n",
                           MAX_NOME, mochilaVetor[pos].nome,
                           MAX_TIPO, mochilaVetor[pos].tipo,
                           mochilaVetor[pos].quantidade,
                           contadorComparacoes);
            }
            else {
                int pos = buscarSequencialVetor(nome);
                if (pos == -1)
                    printf("Item NÃO encontrado (Busca Sequencial, %ld operacoes).\n", contadorComparacoes);
                else
                    printf("Encontrado: %-*s %-*s Qtd: %d (Busca Sequencial, %ld operacoes)\n",
                           MAX_NOME, mochilaVetor[pos].nome,
                           MAX_TIPO, mochilaVetor[pos].tipo,
                           mochilaVetor[pos].quantidade,
                           contadorComparacoes);
            }
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

void menuLista() {
    while (1) {
        printf("\n===== MOCHILA (LISTA ENCADEADA) =====\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item por nome\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item por nome\n");
        printf("0 - Voltar\n");
        int opc = leInteiro("Escolha opção: ");

        if (opc == 0) break;

        if (opc == 1) {
            Item it;
            leString("Nome: ", it.nome, MAX_NOME);
            leString("Tipo: ", it.tipo, MAX_TIPO);
            it.quantidade = leInteiro("Quantidade: ");
            inserirItemLista(&it);
            printf("Item inserido (lista).\n");
        }
        else if (opc == 2) {
            char nome[MAX_NOME];
            leString("Nome do item a remover: ", nome, MAX_NOME);
            if (removerItemLista(nome))
                printf("Item removido (lista).\n");
            else
                printf("Item não encontrado (lista).\n");
        }
        else if (opc == 3) {
            listarLista();
        }
        else if (opc == 4) {
            char nome[MAX_NOME];
            leString("Nome do item a buscar: ", nome, MAX_NOME);

            if (BUSCA_USO_BINARIA) {
                printf("Aviso: Busca Binária não é aplicável à lista encadeada.\n");
                printf("Executando Busca Sequencial na lista.\n");
            }

            No *res = buscarSequencialLista(nome);

            if (res == NULL)
                printf("Item NÃO encontrado (Busca Sequencial, %ld operacoes).\n", contadorComparacoes);
            else
                printf("Encontrado: %-*s %-*s Qtd: %d (Busca Sequencial, %ld operacoes)\n",
                       MAX_NOME, res->dados.nome,
                       MAX_TIPO, res->dados.tipo,
                       res->dados.quantidade,
                       contadorComparacoes);
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}


// ======================================================================
//                      CONFIGURAÇÕES E ESTADO
// ======================================================================

// COMENTÁRIO DO ALUNO: permite ao usuário escolher o tipo de busca global
void configurarTipoBusca() {
    printf("\n=== Configurar Tipo de Busca ===\n");
    printf("0 - Busca Sequencial (padrão)\n");
    printf("1 - Busca Binária (vetor)\n");

    int opc = leInteiro("Escolha: ");

    if (opc == 0) {
        BUSCA_USO_BINARIA = 0;
        printf("Configuração: Busca Sequencial selecionada.\n");
    }
    else if (opc == 1) {
        BUSCA_USO_BINARIA = 1;
        printf("Configuração: Busca Binária selecionada.\n");
    }
    else {
        printf("Opção inválida. Mantendo configuração atual.\n");
    }
}

void mostrarEstadoAtual() {
    printf("\n=== Estado Atual ===\n");
    printf("Modo em uso: %s\n", (MODO_USO == 0 ? "VETOR" : "LISTA ENCADEADA"));
    printf("Tipo de busca global: %s\n", (BUSCA_USO_BINARIA ? "Busca Binária" : "Busca Sequencial"));
    printf("Itens no vetor: %d (ordenado: %s)\n", numItensVetor, (vetorOrdenado ? "SIM" : "NAO"));
}


// ======================================================================
//                          FUNÇÃO PRINCIPAL
// ======================================================================

int main(void) {

    inicializaVetor();
    inicioLista = NULL;

    while (1) { 
        printf("\n=============================================\n");
        printf("         SISTEMA DE MOCHILAS - MENU\n");
        printf("=============================================\n");
        printf("1 - Escolher MOCHILA (Vetor)\n");
        printf("2 - Escolher MOCHILA (Lista Encadeada)\n");
        printf("3 - Configurar tipo de busca (Sequencial/Binária)\n");
        printf("4 - Mostrar estado atual\n");
        printf("0 - Sair\n");

        int opc = leInteiro("Escolha: ");

        if (opc == 0) {
            liberarLista();
            printf("Saindo... memória liberada.\n");
            break;
        }
        else if (opc == 1) {
            MODO_USO = 0;
            menuVetor();
        }
        else if (opc == 2) {
            MODO_USO = 1;
            menuLista();
        }
        else if (opc == 3) {
            configurarTipoBusca();
        }
        else if (opc == 4) {
            mostrarEstadoAtual();
        }
        else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}


/* =====================================================================
   OBSERVAÇÕES FINAIS (COMENTÁRIOS DO ALUNO)
   - O programa mantém duas implementações paralelas.
   - A escolha global determina qual método será usado.
   - Busca Binária só se aplica ao vetor e o programa organiza o vetor
     automaticamente sempre que a binária for escolhida.
   - Lista encadeada não pode usar busca binária, mas o programa avisa.
   - Cada busca exibe o total de operações realizadas para fins de estudos.
   ===================================================================== */
