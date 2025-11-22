#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


// COMENTÁRIO DO ALUNO: MEU CODIGO COMEÇA AQUI

#define MAX_NOME 30  // maximo de caracteres no nome do item
#define MAX_TIPO 20  // maximo de caracteres no nome do tipo
#define MAX_ITENS 10 // maximo de itens na mochila

int CONTADOR_ITENS = 0; // variavel global para manter o informe de qtos itens eu tenho na mochila

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} item_mochila;

/* ======================================================================
   PROTÓTIPOS DAS FUNÇÕES
   (declaração das funções para que o compilador as reconheça antes do main)
   ====================================================================== */

void OrganizarMochilha(item_mochila Mochila[]);
void Exibe_Mochila(item_mochila Mochila[]);
int  LocalizaItem(item_mochila Mochila[], char texto[]);
void ApagaItem(item_mochila Mochila[], int Posicao);
void InserirItem(item_mochila Mochila[]);
void Menu(item_mochila Mochila[]);


// COMENTARIO DO ALUNO: DEIXO TANTO NO INICIO QTO AQUI, INTACTAS 
// AS OBSERVAÇÕES QUE ORIGINALMENTE SE ENCONTRAVAM NO ARQUIVO DE INICIO

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

int main() {
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.


    // COMENTÁRIO DO ALUNO: MEU CODIGO NO MAIN COMEÇA AQUI

    item_mochila Mochila[MAX_ITENS];

    // inicializa a mochila vazia
    for (int i = 0; i < MAX_ITENS; i++) {
        Mochila[i].nome[0] = '\0';
        Mochila[i].tipo[0] = '\0';
        Mochila[i].quantidade = 0;
    }

    Menu(Mochila);

    return 0;    
}

// COMENTARIO DO ALUNO: A IMPLEMENTAÇÃO DAS MINHAS FUNÇÕES COMEA AQUI
// esse função tem como objetivo manter organizado o vetor de itens, fazendo com que os itens do vetor que forem removidos
// sejam movidos para o slot a frente. dessa forma sempre as primeiras posições do vetor de itens estarão ocupados pelos 
// itens que estão presentes na mochila e os ultimos vazios para receber novos itens. 
// também serve para atualizar o contador global de itens da mochila.

void OrganizarMochilha(item_mochila Mochila[]) {

    // criei o vetor temp já zerado usando memset, garantindo que não haverá lixo de memória
    // dica do uso de menset obtive na internet 

    item_mochila temp[MAX_ITENS];

    memset(temp, 0, sizeof(temp));

    CONTADOR_ITENS = 0; // zero o contador de itens pois vou atualizar ele

    for (int i = 0; i < MAX_ITENS; i++) {

        if (strlen(Mochila[i].nome) == 0) {    // não há item nessa posição
            continue; // não faz nada e volta para o for
        }

        // copia item para o vetor organizado
        strcpy(temp[CONTADOR_ITENS].nome, Mochila[i].nome); // transcrevo a posição ocupada para o vetor organizado na posição indicada pelo CONTADOR
        strcpy(temp[CONTADOR_ITENS].tipo, Mochila[i].tipo);
        temp[CONTADOR_ITENS].quantidade = Mochila[i].quantidade;

        CONTADOR_ITENS++; // incremento o contator de itens;
    }

    // uma vez preenchido o vetor temp e atualizado o numero de itens substituo o vetor desorganizado pelo organizado  

    // zera o vetor original
    for (int i = 0; i < MAX_ITENS; i++) {
        Mochila[i].nome[0] = '\0';     // string vazia
        Mochila[i].tipo[0] = '\0';
        Mochila[i].quantidade = 0;
    }

    // copia o vetor organizado para o original
    memcpy(Mochila, temp, sizeof(temp));
}


// essa função exibe o conteudo da mochila

void Exibe_Mochila(item_mochila Mochila[]) {


    printf("******************************************************\n");
    printf("            ITEM                 TIPO          QTD\n");
    printf("******************************************************\n");

    for (int i = 0; i < MAX_ITENS; i++) {

        if (strlen(Mochila[i].nome) == 0)
            continue;

        // imprime alinhado conforme largura definida nos #defines
        printf("%-*s %-*s %d\n",
               MAX_NOME, Mochila[i].nome,
               MAX_TIPO, Mochila[i].tipo,
               Mochila[i].quantidade);
    }
}



// função que procura pelo nome do item

int LocalizaItem(item_mochila Mochila[], char texto[]) {

    for (int i = 0; i < MAX_ITENS; i++) {

        // procura o item cujo nome é igual ao texto
        // usa strlen para determinar se a string é maior que zero, ou seja que não esta vazia
        if (strlen(Mochila[i].nome) > 0 && strcmp(Mochila[i].nome, texto) == 0) {
            return i;  // se achar retorna o valor de i que será o indice do item   
        }
    }

    // se chegou aqui é porque não achou o item. Então retorna -1 para indicar erro
    return -1;
}



// função para apagar item da mochila

void ApagaItem(item_mochila Mochila[], int Posicao) {

    strcpy(Mochila[Posicao].nome, "");  // move brancos para a posição a ser apagada
    strcpy(Mochila[Posicao].tipo, "");
    Mochila[Posicao].quantidade = 0;    // seta o valor em zero limpando a informação anterior

    // feito a remoção do item precisamos reaorganizar o vetor;
    OrganizarMochilha(Mochila);
}



void InserirItem(item_mochila Mochila[]) {

    // verifica se ainda existe espaço na mochila
    if (CONTADOR_ITENS >= MAX_ITENS) {
        printf("\nA mochila está cheia! Não é possível inserir mais itens.\n");
        return;  // interrompe a função
    }

    // posição onde o novo item será inserido
    int pos = CONTADOR_ITENS;

    printf("\n=== Inserção de Novo Item ===\n");

    // Lê o nome do item
    printf("Digite o nome do item: ");
    fgets(Mochila[pos].nome, MAX_NOME, stdin);
    Mochila[pos].nome[strcspn(Mochila[pos].nome, "\n")] = '\0'; // remove o '\n'

    // Lê o tipo do item
    printf("Digite o tipo do item: ");
    fgets(Mochila[pos].tipo, MAX_TIPO, stdin);
    Mochila[pos].tipo[strcspn(Mochila[pos].tipo, "\n")] = '\0';

    // Lê a quantidade (número inteiro)
    printf("Digite a quantidade do item: ");
    scanf("%d", &Mochila[pos].quantidade);

    getchar(); // limpa o '\n' que sobra do scanf

    CONTADOR_ITENS++; // incrementa o contador

    // *** AJUSTE FEITO ***
    // removi a chamada duplicada de OrganizarMochilha(), deixando apenas 1 chamada
    OrganizarMochilha(Mochila);

    printf("\nItem inserido com sucesso!\n");
}



void Menu(item_mochila Mochila[]) {

    int opcao;
    char nomeBusca[MAX_NOME];
    int pos;

    do {
        printf("\n===========================================\n");
        printf("               MENU PRINCIPAL\n");
        printf("===========================================\n");
        printf("1 - Inserir Ítem\n");
        printf("2 - Remover Ítem\n");
        printf("3 - Exibir Mochila\n");
        printf("4 - Sair\n");
        printf("===========================================\n");
        printf("Escolha uma opção: ");

        scanf("%d", &opcao);
        getchar(); // limpa o \n deixado pelo scanf

        switch(opcao) {

            case 1:
                InserirItem(Mochila);
                break;

            case 2:
                printf("\nDigite o nome do ítem que deseja remover: ");
                fgets(nomeBusca, MAX_NOME, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';  // remove \n

                pos = LocalizaItem(Mochila, nomeBusca);

                if (pos == -1) {
                    printf("Ítem não encontrado!\n");
                } else {
                    ApagaItem(Mochila, pos);
                    printf("Ítem removido com sucesso!\n");
                }
                break;

            case 3:
                Exibe_Mochila(Mochila);
                break;

            case 4:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }

    } while (opcao != 4);
}


// COMENTARIO DO ALUNO: DEIXO TANTO NO INICIO QTO AQUI, INTACTAS 
// AS OBSERVAÇÕES QUE ORIGINALMENTE SE ENCONTRAVAM NO ARQUIVO DE INICIO

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
