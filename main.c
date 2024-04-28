#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

struct Produto
{
    int cod_produto;
    char numero_de_serie[50];
    struct tm data_validade;

} Produto;

typedef struct no
{
    struct Produto prod;
    struct no *esquerda;
    struct no *direita;
} NO;

typedef struct arvore
{
    NO *raiz;
} arvore;

void inserirEsquerda(NO *no, struct Produto produto)
{
    if (no->esquerda == NULL)
    {
        NO *novo = (NO *)malloc(sizeof(NO));
        novo->prod = produto;
        novo->esquerda = NULL;
        novo->direita = NULL;
        no->esquerda = novo;
    }
    else
    {
        if (compararDatas(produto.data_validade, no->esquerda->prod.data_validade) == -1)
        {
            inserirEsquerda(no->esquerda, produto);
        }
        else if (compararDatas(produto.data_validade, no->esquerda->prod.data_validade) == 1)
        {
            inserirDireita(no->esquerda, produto);
        }
    }
}

void inserirDireita(NO *no, struct Produto produto)
{
    if (no->direita == NULL)
    {
        NO *novo = (NO *)malloc(sizeof(NO));
        novo->prod = produto;
        novo->esquerda = NULL;
        novo->direita = NULL;
        no->direita = novo;
    }
    else
    {
        if (compararDatas(produto.data_validade, no->direita->prod.data_validade) == -1)
        {
            inserirDireita(no->direita, produto);
        }
        else if (compararDatas(produto.data_validade, no->direita->prod.data_validade) == 1)
        {
            inserirEsquerda(no->direita, produto);
        }
    }
}

void inserir(arvore *arv, struct Produto novoProduto)
{
    if (arv->raiz == NULL)
    {
        NO *novo = (NO *)malloc(sizeof(NO));
        novo->prod = novoProduto;
        novo->esquerda = NULL;
        novo->direita = NULL;
        arv->raiz = novo;
    }
    else
    {
        if (compararDatas(novoProduto.data_validade, arv->raiz->prod.data_validade) == -1)
        {
            inserirEsquerda(arv->raiz, novoProduto);
        }
        else if (compararDatas(novoProduto.data_validade, arv->raiz->prod.data_validade) == 1)
        {
            inserirDireita(arv->raiz, novoProduto);
        }
    }
}

void alterar(NO *raiz, int cod_produto)
{
    printf("Testee Alterar: %d \n", cod_produto);
    if(raiz != NULL && cod_produto != raiz->prod.cod_produto) {
        alterar(raiz->esquerda, cod_produto);
        alterar(raiz->direita, cod_produto);
    } else if(cod_produto == raiz->prod.cod_produto) {
        raiz->prod.cod_produto = "Oiii";
        printf("Codigo do produto atualizado");
    } else {
        printf("Não existe esse código");
    }
}

void excluir(NO *raiz, int cod_produto)
{
    printf("Testee Excluir: %d \n", cod_produto);
}

void consultar(NO *raiz, int cod_produto)
{
    if(raiz == NULL) {
        printf("Elemento nao existe na arvore");
    } else {
        if(raiz->prod.cod_produto == cod_produto) {
            listarConsulta(raiz);
        } else if(raiz->prod.cod_produto < cod_produto) {
            consultar(raiz->esquerda, cod_produto);
        } else {
            consultar(raiz->direita, cod_produto);
        }

        listarConsulta(raiz);
    }
}

void listarConsulta(NO *noConsultado) {
    struct tm data = noConsultado->prod.data_validade;
    printf("%d \n", noConsultado->prod.cod_produto);
    printf("%s \n", noConsultado->prod.numero_de_serie);
    printf("Data e hora inseridas: %02d/%02d/%04d \n",
            data.tm_mday, data.tm_mon + 1, data.tm_year + 1900);
}

void listarArvore(NO *raiz)
{
    if (raiz != NULL)
    {
        listarArvore(raiz->esquerda);
        struct tm data = raiz->prod.data_validade;
        printf("%d \n", raiz->prod.cod_produto);
        printf("%s \n", raiz->prod.numero_de_serie);
        printf("Data e hora inseridas: %02d/%02d/%04d \n",
               data.tm_mday, data.tm_mon + 1, data.tm_year + 1900);
        listarArvore(raiz->direita);
    }
}

int compararDatas(struct tm dataNo, struct tm dataArv)
{
    if(dataNo.tm_year < dataArv.tm_year) return -1;
    if(dataNo.tm_year > dataArv.tm_year) return 1;
    if(dataNo.tm_mon < dataArv.tm_mon) return -1;
    if(dataNo.tm_mon > dataArv.tm_mon) return 1;
    if(dataNo.tm_mday < dataArv.tm_mday) return -1;
    if(dataNo.tm_mday > dataArv.tm_mday) return 1;

    return 0;
}

int compareStrings(char cod1[50], char cod2[50]) {
    int respostaComp;
    respostaComp = strcmp(cod1, cod2);

    return respostaComp;
}

void opcoes()
{
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    struct Produto produtoNovo;
    struct tm data;
    arvore arv;
    arv.raiz = NULL;

    int opcao;
    int cod_produto;
    char numero_de_serie[50];
    int dia, mes, ano;

    while (opcao != 9)
    {
        printf("Escolha uma das opçoes a seguir: \n");
        printf("1 - Incluir produto \n");
        printf("2 - Alterar produto \n");
        printf("3 - Excluir produto \n");
        printf("4 - Consultar produto \n");
        printf("5 - Listar produtos \n");
        printf("9 - Sair \n");
        scanf("%d", &opcao);
        switch (opcao)
        {
            case 1:
                printf("----------------Inserir-------------------\n");
                printf("Digite o codigo do produto: \n");
                scanf("%d", &cod_produto);
                printf("Digite o numero de serie do produto: \n");
                scanf("%s", numero_de_serie);
                printf("Digite uma data(dd/mm/yyyy): \n");
                scanf("%d/%d/%d", &dia, &mes, &ano);

                data.tm_mday = dia;
                data.tm_mon = mes - 1;
                data.tm_year = ano - 1900;

                produtoNovo.cod_produto = cod_produto;
                strcpy(produtoNovo.numero_de_serie, numero_de_serie);
                produtoNovo.data_validade = data;

                inserir(&arv, produtoNovo);
                break;

            case 2:
                printf("----------------Alterar-------------------\n");
                printf("Digite o codigo do produto: \n");
                scanf("%d", &cod_produto);
                alterar(arv.raiz, cod_produto);
                break;

            case 3:
                printf("----------------Excluir-------------------\n");
                printf("Digite o codigo do produto: \n");
                scanf("%d", &cod_produto);
                excluir(arv.raiz, cod_produto);
                break;

            case 4:
                printf("----------------Consultar-------------------\n");
                printf("Digite o codigo do produto: \n");
                scanf("%d", &cod_produto);
                consultar(arv.raiz, cod_produto);
                break;

            case 5:
                listarArvore(arv.raiz);
                break;

            default:
                printf("\nOpção invalida...\n");
        }
    }
}