/*
 COMENTÁRIO DO ALUNO: SISTEMA DE COMPONENTES DA TORRE DE FUGA - NIVEL MESTRE
 - Estrutura principal: Componente (nome, tipo, prioridade)
 - Permite cadastro de até 20 componentes
 - Ordenação: Bubble Sort por nome, Insertion Sort por tipo, Selection Sort por prioridade
 - Busca binária por nome (após ordenação por nome)
 - Contagem de comparações e tempo de execução
 - Menu interativo atualizado conforme instruções do desafio final
 Obs: reduzi o tamanho de alguns comentários pois o arquivo ficando com muitas linhas
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   //COMENTARIO DO ALUNO: para manipulação das strings
#include <stdbool.h>  //COMENTARIO DO ALUNO: para uso de TRUE e FALSE
#include <time.h>     //COMENTARIO DO ALUNO: para o uso do sistema de medição de desempenho

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20

//COMENTARIO DO ALUNO: antes a estrutura chamava Item agora muda de nome conforme pedido no exercício
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade;
} Componente;

Componente componentes[MAX_COMPONENTES];
int numComponentes = 0;
bool nomeOrdenado = false; // COMENTÁRIO DO ALUNO: status da ordenação por nome

// ======================================================================
//                          FUNÇÕES DE ENTRADA
// ======================================================================

void leString(const char *prompt, char *buffer, size_t tamanho) {
    printf("%s", prompt);
    if (fgets(buffer, (int)tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
}

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
//                       FUNÇÕES DE ORDENAÇÃO
// ======================================================================

void bubbleSortNome(Componente arr[], int n, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j + 1].nome) > 0) {
                Componente temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    nomeOrdenado = true;
}

void insertionSortTipo(Componente arr[], int n, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, key.tipo) > 0) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

void selectionSortPrioridade(Componente arr[], int n, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[minIdx].prioridade) minIdx = j;
        }
        if (minIdx != i) {
            Componente temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
        }
    }
}

// ======================================================================
//                       FUNÇÃO DE BUSCA BINÁRIA
// ======================================================================

int buscaBinariaPorNome(Componente arr[], int n, char chave[], int *comparacoes) {
    int ini = 0, fim = n - 1;
    *comparacoes = 0;
    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, arr[meio].nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) fim = meio - 1;
        else ini = meio + 1;
    }
    return -1;
}

// ======================================================================
//                       FUNÇÃO DE EXIBIÇÃO
// ======================================================================

void mostrarComponentes(Componente arr[], int n) {
    printf("\n--- Lista de Componentes ---\n");
    printf("%-30s %-20s %-10s\n", "Nome", "Tipo", "Prioridade");
    for (int i = 0; i < n; i++) {
        printf("%-30s %-20s %d\n", arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

// ======================================================================
//                       MEDIÇÃO DE TEMPO
// ======================================================================

double medirTempo(void (*algoritmo)(Componente[], int, int *), Componente arr[], int n, int *comparacoes) {
    clock_t inicio = clock();
    algoritmo(arr, n, comparacoes);
    clock_t fim = clock();
    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

// ======================================================================
//                       FUNÇÃO PRINCIPAL / MENU NIVEL MESTRE
// ======================================================================

int main(void) {
    while (1) {
        printf("\n==================================================================\n");
        printf("           PLANO DE FUGA - CODIGO DA ILHA NIVEL MESTRE\n");
        printf("==================================================================\n");
        printf("Itens na Mochila: %d/%d\n", numComponentes, MAX_COMPONENTES);
        printf("Status da Ordenacao por Nome: %s\n", nomeOrdenado ? "ORDENADO" : "NAO ORDENADO");

        printf("\n1 - Adicionar Componente\n");
        printf("2 - Descartar Componente\n");
        printf("3 - Listar Componente (Inventario)\n");
        printf("4 - Organizar Mochila (Ordenar Componentes)\n");
        printf("5 - Busca Binaria por Componente-Chave (por nome)\n");
        printf("0 - ATIVAR TORRE DE FUGA (Sair)\n");

        int opc = leInteiro("Escolha uma opcao: ");

        if (opc == 0) break;

        if (opc == 1) { // Adicionar
            if (numComponentes >= MAX_COMPONENTES) {
                printf("Limite de componentes atingido!\n");
                continue;
            }
            Componente c;
            leString("Nome do componente: ", c.nome, MAX_NOME);
            leString("Tipo do componente: ", c.tipo, MAX_TIPO);
            c.prioridade = leInteiro("Prioridade (1-10): ");
            if (c.prioridade < 1) c.prioridade = 1;
            if (c.prioridade > 10) c.prioridade = 10;
            componentes[numComponentes++] = c;
            nomeOrdenado = false;
        }
        else if (opc == 2) { // Descartar
            if (numComponentes == 0) {
                printf("Nao ha componentes para descartar!\n");
                continue;
            }
            char chave[MAX_NOME];
            leString("Nome do componente a descartar: ", chave, MAX_NOME);
            bool removido = false;
            for (int i = 0; i < numComponentes; i++) {
                if (strcmp(componentes[i].nome, chave) == 0) {
                    componentes[i] = componentes[numComponentes - 1];
                    numComponentes--;
                    removido = true;
                    nomeOrdenado = false;
                    break;
                }
            }
            if (removido) printf("Componente descartado.\n");
            else printf("Componente nao encontrado.\n");
        }
        else if (opc == 3) { // Listar
            if (numComponentes == 0) printf("Mochila vazia.\n");
            else mostrarComponentes(componentes, numComponentes);
        }
        else if (opc == 4) { // Organizar
            if (numComponentes == 0) {
                printf("Nao ha componentes para ordenar.\n");
                continue;
            }

            // COMENTÁRIO DO ALUNO: inseri aqui um submenu para deixar a lista do menu mais curta
            printf("Escolha criterio de ordenacao:\n");
            printf("1 - Bubble Sort por Nome\n");
            printf("2 - Insertion Sort por Tipo\n");
            printf("3 - Selection Sort por Prioridade\n");
            int crit = leInteiro("Opcao: ");
            int comps;
            double tempo;
            if (crit == 1) {
                tempo = medirTempo(bubbleSortNome, componentes, numComponentes, &comps);
                printf("Bubble Sort por Nome concluido: %d comparacoes, tempo %.6f s\n", comps, tempo);
            }
            else if (crit == 2) {
                tempo = medirTempo(insertionSortTipo, componentes, numComponentes, &comps);
                printf("Insertion Sort por Tipo concluido: %d comparacoes, tempo %.6f s\n", comps, tempo);
            }
            else if (crit == 3) {
                tempo = medirTempo(selectionSortPrioridade, componentes, numComponentes, &comps);
                printf("Selection Sort por Prioridade concluido: %d comparacoes, tempo %.6f s\n", comps, tempo);
            }
            else {
                printf("Opcao invalida.\n");
            }
        }
        else if (opc == 5) { // Busca Binaria
            if (!nomeOrdenado) {
                printf("Aviso: A busca binaria requer ordenacao por Nome. Ordenando automaticamente...\n");
                int comps;
                medirTempo(bubbleSortNome, componentes, numComponentes, &comps);
            }
            char chave[MAX_NOME];
            leString("Nome do componente-chave: ", chave, MAX_NOME);
            int comps;
            int pos = buscaBinariaPorNome(componentes, numComponentes, chave, &comps);
            if (pos == -1) printf("Componente nao encontrado (comparacoes: %d)\n", comps);
            else {
                printf("Componente encontrado: %-30s %-20s %d (comparacoes: %d)\n",
                       componentes[pos].nome,
                       componentes[pos].tipo,
                       componentes[pos].prioridade,
                       comps);
            }
        }
        else {
            printf("Opcao invalida.\n");
        }
    }

    printf("TORRE DE FUGA ATIVADA! Saindo do sistema...\n");
    return 0;
}

/* =====================================================================
   COMENTÁRIOS FINAIS DO ALUNO
   - Menu atualizado conforme plano de fuga do nivel mestre.
   - Itens na mochila e status de ordenacao exibidos dinamicamente.
   - Funcionalidades completas: adicionar, descartar, listar, ordenar, buscar e sair.
   ===================================================================== */
