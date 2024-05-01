#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// Estrutura do produto
struct Produto {
    int cod_produto;
    char numero_de_serie[50];
    struct tm data_validade;
};

// Estrutura do heap mínimo
typedef struct {
    struct Produto *produtos;
    int tamanho;
    int capacidade;
} Heap;

// Comparador para datas
int compararDatas(struct tm data1, struct tm data2) {
    if (data1.tm_year < data2.tm_year) return -1;
    if (data1.tm_year > data2.tm_year) return 1;
    if (data1.tm_mon < data2.tm_mon) return -1;
    if (data1.tm_mon > data2.tm_mon) return 1;
    if (data1.tm_mday < data2.tm_mday) return -1;
    if (data1.tm_mday > data2.tm_mday) return 1;
    return 0;
}

// Inicialização do heap
void inicializarHeap(Heap *h, int capacidadeInicial) {
    h->produtos = (struct Produto *)malloc(capacidadeInicial * sizeof(struct Produto));
    h->capacidade = capacidadeInicial;
    h->tamanho = 0;
}

// Conceito "Heapify-up" para manter a propriedade do heap após inserção
void organizacaoCima(Heap *h, int index) {
    while (index > 0) {
        int parent = (index - 1);
        if(compararDatas(h->produtos[index].data_validade, h->produtos[parent].data_validade) < 0) {
            struct Produto temp = h->produtos[index];
            h->produtos[index] = h->produtos[parent];
            h->produtos[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

// Conceito "Heapify-down" para manter a propriedade do heap após remoção
void organizacaoBaixo(Heap *h, int index) {
    int filhoEsquerdo = 2 * index + 1;
    int filhoDireito = 2 * index + 2;
    int menorFilho = index;

    // Verificar se o filho esquerdo tem menor data de validade
    if (filhoEsquerdo < h->tamanho &&
        compararDatas(h->produtos[filhoEsquerdo].data_validade, h->produtos[menorFilho].data_validade) < 0) {
      menorFilho = filhoEsquerdo;
    }

    // Verificar se o filho direito tem menor data de validade
    if (filhoDireito < h->tamanho &&
        compararDatas(h->produtos[filhoDireito].data_validade, h->produtos[menorFilho].data_validade) < 0) {
      menorFilho = filhoDireito;
    }

    // Se o menor não for o pai, trocar os produtos e continuar a reorganização
    if (menorFilho != index) {
      struct Produto temp = h->produtos[index];
      h->produtos[index] = h->produtos[menorFilho];
      h->produtos[menorFilho] = temp;

      organizacaoBaixo(h, menorFilho); // Reorganizar a partir do filho menor
    }
}

// Inserir produto no heap
void inserir(Heap *h, struct Produto produto) {
    if (h->tamanho >= h->capacidade) {
        h->capacidade *= 2;
        h->produtos = (struct Produto *)realloc(h->produtos, h->capacidade * sizeof(struct Produto));
    }
    h->produtos[h->tamanho] = produto;
    organizacaoCima(h, h->tamanho);
    h->tamanho++;
}

// Função para alterar um produto por código
void alterar(Heap *h, int cod_produto) {
    int encontrado = 0;
    for (int i = 0; i < h->tamanho; i++) {
        if (h->produtos[i].cod_produto == cod_produto) {
            encontrado = 1;
            int dia, mes, ano;
            char numero_de_serie[50];
            
            printf("Digite o novo número de série: \n");
            scanf("%s", numero_de_serie);
            printf("Digite uma data (dd/mm/yyyy): \n");
            scanf("%d/%d/%d", &dia, &mes, &ano);

            h->produtos[i].data_validade.tm_mday = dia;
            h->produtos[i].data_validade.tm_mon = mes - 1;
            h->produtos[i].data_validade.tm_year = ano - 1900;
            strcpy(h->produtos[i].numero_de_serie, numero_de_serie);

            // Restabelecer a propriedade do heap após alteração
            organizacaoBaixo(h, i);
            organizacaoCima(h, i);
            break;
        }
    }
    if (!encontrado) {
        printf("Produto com código %d não encontrado.\n", cod_produto);
    }
}

void excluirPorCodigo(Heap *h, int cod_produto) {
  int encontrado = 0;
  int indexParaExcluir = -1;

  printf("Produtos no heap com código %d:\n", cod_produto);

  // Buscar o produto com o código especificado e a menor data de validade
  for (int i = 0; i < h->tamanho; i++) {
    if (h->produtos[i].cod_produto == cod_produto) {
      if (indexParaExcluir == -1 || 
          compararDatas(h->produtos[i].data_validade, h->produtos[indexParaExcluir].data_validade) > 0) {
        indexParaExcluir = i; // Atualiza o índice para o produto com menor data de validade
        encontrado++;
      }
    }
  }

  if (encontrado) {
    printf("\nProduto a ser excluído. Código: %d, Série: %s, Data: %02d/%02d/%04d\n",
           h->produtos[indexParaExcluir].cod_produto, 
           h->produtos[indexParaExcluir].numero_de_serie,
           h->produtos[indexParaExcluir].data_validade.tm_mday,
           h->produtos[indexParaExcluir].data_validade.tm_mon + 1,
           h->produtos[indexParaExcluir].data_validade.tm_year + 1900);

    // Substituir o produto a ser excluído pelo último elemento do heap
    h->produtos[indexParaExcluir] = h->produtos[h->tamanho - 1];
    h->tamanho--;

    // Reorganizar o heap após a exclusão para manter a propriedade do heap
    for (int i = 0; i < h->tamanho; i++) {
        if (h->produtos[i].cod_produto == cod_produto) {
            organizacaoBaixo(h, i);
            organizacaoCima(h, i);
        }
    }
  } else {
    printf("Nenhum produto com o código %d foi encontrado no heap.\n", cod_produto);
  }
}

// Listar todos os produtos no heap
void listarProdutos(Heap *h) {
    if (h->tamanho == 0) {
        printf("O heap está vazio.\n");
        return;
    }

    printf("Produtos no heap:\n");
    for (int i = 0; i < h->tamanho; i++) {
        printf("Código: %d, Série: %s, Data de Validade: %02d/%02d/%04d\n",
               h->produtos[i].cod_produto, h->produtos[i].numero_de_serie,
               h->produtos[i].data_validade.tm_mday, h->produtos[i].data_validade.tm_mon + 1,
               h->produtos[i].data_validade.tm_year + 1900);
    }
}

void listarProdutosPorCodigo(Heap *h, int codigo) {
    if (h->tamanho == 0) {
        printf("O heap está vazio.\n");
        return;
    }

    int encontrados = 0;

    printf("Produtos no heap com código %d:\n", codigo);

    // Percorrer o heap para encontrar todos os produtos com o código específico
    for (int i = 0; i < h->tamanho; i++) {
        if (h->produtos[i].cod_produto == codigo) {
            printf("Código: %d, Série: %s, Data de Validade: %02d/%02d/%04d\n",
                   h->produtos[i].cod_produto, h->produtos[i].numero_de_serie,
                   h->produtos[i].data_validade.tm_mday, h->produtos[i].data_validade.tm_mon + 1,
                   h->produtos[i].data_validade.tm_year + 1900);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum produto com o código %d foi encontrado no heap.\n", codigo);
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");

    Heap h;
    inicializarHeap(&h, 10); // Capacidade inicial do heap

    int opcao = 0;
    int cod_produto;
    char numero_de_serie[50];
    int dia, mes, ano;

    while (opcao != 9) {
        printf("\nEscolha uma das opções a seguir:\n");
        printf("1 - Incluir produto\n");
        printf("2 - Alterar produto\n");
        printf("3 - Excluir produto\n");
        printf("4 - Consultar produto\n");
        printf("5 - Listar produtos\n");
        printf("9 - Sair\n");
        printf("Digite a operação que deseja fazer de acordo com a numeração das opções: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("----------------Inserir-------------------\n");
                printf("Digite o código do produto: \n");
                scanf("%d", &cod_produto);

                printf("Digite o número de série do produto: \n");
                scanf("%s", numero_de_serie);

                printf("Digite uma data (dd/mm/yyyy): \n");
                scanf("%d/%d/%d", &dia, &mes, &ano);

                struct Produto produtoNovo;
                produtoNovo.cod_produto = cod_produto;
                strcpy(produtoNovo.numero_de_serie, numero_de_serie);
                produtoNovo.data_validade.tm_mday = dia;
                produtoNovo.data_validade.tm_mon = mes - 1;
                produtoNovo.data_validade.tm_year = ano - 1900;

                inserir(&h, produtoNovo);
                break;

            case 2:
                printf("----------------Alterar-------------------\n");
                printf("Digite o código do produto a ser alterado: \n");
                scanf("%d", &cod_produto);

                alterar(&h, cod_produto);
                break;

            case 3:
                printf("----------------Excluir-------------------\n");
                printf("Digite o código do produto a ser excluído: \n");
                scanf("%d", &cod_produto);

                excluirPorCodigo(&h, cod_produto);
                break;

            case 4:
                printf("----------------Consultar-------------------\n");
                printf("Digite o código do produto para consultar: \n");
                scanf("%d", &cod_produto);

                listarProdutosPorCodigo(&h, cod_produto); // Listar todos os produtos do heap
                break;

            case 5:
                listarProdutos(&h); // Listar todos os produtos do heap
                break;

            case 9:
                printf("Saindo do sistema\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    free(h.produtos); // Liberar a memória alocada

    return 0;
}