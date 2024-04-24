#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

struct Produto{
    int cod_produto;
    char numero_de_serie[50];
    struct tm data_validade;
    
} Produto;

typedef struct no {
    struct Produto prod;
    struct no *esquerda;
    struct no *direita;
} NO;

typedef struct arvore {
    NO *raiz;
} arvore;

NO* novoProduto(int cod_produto, char numero_de_serie[50], struct tm data_validade) {
    NO* produto = (NO*)malloc(sizeof(NO));
    produto->prod.cod_produto = cod_produto;
    strcpy(produto->prod.numero_de_serie, numero_de_serie);
    produto->prod.data_validade = data_validade;
    produto->esquerda = NULL;
    produto->direita = NULL;
    return produto;
}

void inserirEsquerda(NO *no) {

}

void inserirDireita(NO *no) {

}

void inserir(arvore *arv, NO *novoProduto) {
    if(arv->raiz == NULL) {
        arv->raiz = novoProduto;
    } else {
        if(novoProduto->prod.data_validade.tm_year < arv->raiz->prod.data_validade.tm_year) {

        }
    }
}

void alterar() {

}

void excluir() {

}

void consultar() {

}

void listar() {

}

void compararDatas(struct tm dataNo, struct tm dataArv) {
    time_t timeNo = mktime(&dataNo);
    time_t timeArv = mktime(&dataArv);
    
    if(timeNo < timeArv) {
        printf("timeArv maior");
    }
}


int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    struct Produto produtoNovo;
    struct tm data = {0};
    NO *produto = NULL;
    int opcao;

    int cod_produto;
    char numero_de_serie[50];
    int dia, mes, ano;

    printf("Escolha uma das opçoes a seguir: \n");
    printf("1 - Incluir produto \n");
    printf("2 - Alterar produto \n");
    printf("3 - Excluir produto \n");
    printf("4 - Consultar produto \n");
    printf("5 - Listar produtos \n");
    printf("9 - Sair \n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("----------------Inserir-------------------\n");
            printf("Digite o codigo do produto: \n");
            scanf("%d", &cod_produto);
            printf("Digite o numero de serie do produto: \n");
            scanf("%s", &numero_de_serie);
            printf("Digite uma data(dd/mm/yyyy): \n");
            scanf("%d/%d/%d", &dia, &mes, &ano);

            data.tm_mday = dia;
            data.tm_mon = mes - 1;
            data.tm_year = ano - 1900;

            NO* novoProd = novoProduto(cod_produto, numero_de_serie, data);

            printf("Cod prod: %d", novoProd->prod.cod_produto);
            printf("Num de serie prod: %s", novoProd->prod.numero_de_serie);

            printf("Data e hora inseridas: %02d/%02d/%04d \n",
            data.tm_mday, data.tm_mon + 1, data.tm_year + 1900);

            inserir();
        break;

        case 2:
            alterar();
        break;

        case 3:
            excluir();
        break;

        case 4:
            consultar();
        break;

        case 5:
            listar();
        break;

        default:
            break;
    }

}