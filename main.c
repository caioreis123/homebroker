#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_INICIAL 10

// a estrutura principal será uma listaDinamicaPrincipal, ou seja, um array com alocação dinâmica de memória capaz de aumentar de tamanho
// assim que seu último elemento for adicionado. Usa-se realoc para dobrar seu tamanho.

// Cada elemento dessa listaDinamicaPrincipal contém uma struct titulos
// que por sua vez contém a sigla do título (ex: PETR4), e dois cabeçotes de lista ligada, um pra cada tipo de operação
typedef struct operacao{
    struct operacao *proxima;
    char quantidade[10];
    char valor[10];
}Operacao;

typedef struct Titulo{
    char sigla[10];
    Operacao *primeiraCompra;
    Operacao *primeiraVenda;
}Titulo;

Titulo *listaDinamicaPrincipal;
// essas 2 variáveis globais são usadas pela função de redimensionamento da array principal
int tamanhoDaListaPrincipal = TAMANHO_INICIAL;
int espacoPrincipalUtilizado = 0;


Operacao* criarOperacao(){
    Operacao* operacao = (Operacao*) malloc(sizeof(Operacao));
    return operacao;
}

Operacao* inserirOperacaoNoInicio(Operacao* primeiraOperacao, char quantidade[10], char valor[10]){
    Operacao *novaOperacao = criarOperacao();
    strcpy(novaOperacao->quantidade, quantidade);
    strcpy(novaOperacao->valor, valor);
    if(primeiraOperacao == NULL){
        primeiraOperacao=novaOperacao;
        novaOperacao->proxima=NULL;
    }
    else{
        if(atof(novaOperacao->valor) > atof(primeiraOperacao->valor)){
            novaOperacao->proxima = primeiraOperacao;
            return novaOperacao;
        }
        Operacao *auxiliar=primeiraOperacao;
        while(auxiliar->proxima!=NULL && atof(novaOperacao->valor) < atof(auxiliar->proxima->valor)){
            auxiliar=auxiliar->proxima;
        }
        novaOperacao->proxima=auxiliar->proxima;
        auxiliar->proxima=novaOperacao;
    }
    return primeiraOperacao;
}

void listarOperacoes(Operacao* primeiraOperacao){
    Operacao *auxiliar=primeiraOperacao;
    while(auxiliar!=NULL){
        printf("valor: %s\n", auxiliar->valor);
        printf("quantidade: %s\n", auxiliar->quantidade);
        auxiliar=auxiliar->proxima;
    }
}

int buscaIndicePorSigla(char *sigla) {
    for(int indice=0;indice<espacoPrincipalUtilizado;indice++){
        if(!strcmp(listaDinamicaPrincipal[indice].sigla, sigla)) return indice;
    }
    return -1;
}

int inserirNovoTituloAoFinalDaListaPrincipal(char *sigla) {
    if(tamanhoDaListaPrincipal<=espacoPrincipalUtilizado){
        tamanhoDaListaPrincipal = tamanhoDaListaPrincipal * 2;
        listaDinamicaPrincipal = (Titulo *) realloc(listaDinamicaPrincipal, tamanhoDaListaPrincipal * sizeof(Titulo) );
    }
    strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].sigla, sigla);
    listaDinamicaPrincipal[espacoPrincipalUtilizado].primeiraCompra = NULL;
    listaDinamicaPrincipal[espacoPrincipalUtilizado].primeiraVenda = NULL;
    espacoPrincipalUtilizado = espacoPrincipalUtilizado +1;
    int indiceDaNovaSigla = espacoPrincipalUtilizado -1;
    return indiceDaNovaSigla;
}

void salvarOferta(char *sigla, const char *tipo, char *valor, char *quantidade) {
//    primeiro passo: identificar qual o índice do array dinâmico principal está armazenado as operações do título a ser salvo
    int indiceDaListaPrincipalDaSigla = buscaIndicePorSigla(sigla);
//    o retorno -1 indica que essa é uma nova sigla ainda não armazenada
    if (indiceDaListaPrincipalDaSigla == -1) {
        indiceDaListaPrincipalDaSigla = inserirNovoTituloAoFinalDaListaPrincipal(sigla);
    }

//  as inserções de compra e venda retornam o cabeçote da lista correspondente, de modo que a estrutura principal possa ser atualizada com essa nova lista encadeada contendo a nova operação
    if (tolower(tipo[0]) == 'c') {
        Operacao *primeiraCompra = inserirOperacaoNoInicio(listaDinamicaPrincipal[indiceDaListaPrincipalDaSigla].primeiraCompra, quantidade, valor);
        listaDinamicaPrincipal[indiceDaListaPrincipalDaSigla].primeiraCompra = primeiraCompra;

    } else if (tolower(tipo[0]) == 'v') {
        Operacao *primeiraVenda = inserirOperacaoNoInicio(listaDinamicaPrincipal[indiceDaListaPrincipalDaSigla].primeiraVenda, quantidade, valor);
        listaDinamicaPrincipal[indiceDaListaPrincipalDaSigla].primeiraVenda = primeiraVenda;
    } else printf("%s precisa ser compra ou venda", listaDinamicaPrincipal[indiceDaListaPrincipalDaSigla].sigla);
}

void listarOfertas(){
    for(int i = 0; i < espacoPrincipalUtilizado; i++){
        printf("Sigla: %s\n", listaDinamicaPrincipal[i].sigla);
        printf("COMPRAS:\n");
        listarOperacoes(listaDinamicaPrincipal[i].primeiraCompra);
        printf("\n--------\n");
        printf("\nVENDAS:\n");
        listarOperacoes(listaDinamicaPrincipal[i].primeiraVenda);
        printf("\n###########\n");
    }
}


void carregarOfertasDeArquivo(){
    char *sigla;
    char *tipo;
    char *valor;
    char *quantidade;

    // esse duplo apontamento de caminho aqui é por conta do cmake que lê o arquivo a partir de outro diretório
    FILE *arquivo = fopen("../homebroker-database.txt", "r");
    if (!arquivo){
        arquivo = fopen("homebroker-database.txt", "r");
        if(!arquivo){
            printf("Nao foi possivel ler o arquivo\n");
            exit(1);
        }
    }
    char linhas[1024];

    // o número de linha serve apenas para que possamos pular a primeira linha contendo apenas os nomes das colunas
    int linha_atual = 0;

    // o número da coluna vai ajudar a identificar em qual variável deveremos armazenar o conteúdo em texto de uma célula
    int coluna_atual = 0;

    // iteramos pelas linhas de um arquivo
    while(fgets(linhas, 1024, arquivo)){
        coluna_atual = 0;
        linha_atual++;
        if(linha_atual == 1) continue;

        // célula é o conteúdo em texto da célula
        char *celula = strtok(linhas, ",");

        // iteramos pelas células de uma linha, atribuindo seus conteúdo às respectivas variáveis
        while(celula){
            if(coluna_atual == 0){
                sigla = celula;
            }
            if(coluna_atual == 1){
                tipo = celula;
            }
            if(coluna_atual == 2){
                // deixar pra fazer essa conversão apenas na hora do cálculo e depois converter novamente para string,
                // dessa forma vc armazena tudo sempre como string
//                valor = strtod(celula, NULL);
                valor = celula;
            }
            if(coluna_atual == 3) {
//                quantidade = strtol(celula, NULL, 10);
                quantidade = celula;
            }
          // a passagem do NULL no strtok vai avançar para a próxima célula
            celula = strtok(NULL, ",");
            coluna_atual++;
        }
        salvarOferta(sigla, tipo, valor, quantidade);
    }
    fclose(arquivo);
}

void inserirOperacaoDoUsuario() {
    char sigla[10];
    char tipo[10];
    char valor[10];
    char quantidade[10];
    printf("\nQual a sigla do Titulo?\n");
    scanf("%s", sigla);
    printf("\nQual o tipo de operacao que deseja realizar? [compra]/[venda]\n");
    scanf("%s", tipo);
    printf("\nQual o valor unitário?\n");
    scanf("%s", valor);
    printf("\nQual a quantidade que deseja operar?\n");
    scanf("%s", quantidade);
    printf("Valores lidos: %s %s %s %s", sigla, tipo, valor, quantidade);
    salvarOferta(sigla, tipo, valor, quantidade);
}

int pegaValorDaOperacao(Operacao *operacao) {
    if (!operacao) return 0;
    return atoi(operacao->valor);
}

Operacao* removeOperacao(Operacao *operacaoASerRemovida, Operacao *primeiraOperacao){
    //a operação a ser removida possui apenas valores e quantidade, por isso precisamos comparar ambos dados(valor e quantidade) de cada nó para garantir que estamos removendo o nó certo

    // a remoção da primeira operação precisa de um tratamento especial porque iremos alterar o cabeçote da lista que passará a apontar para o segundo nó e retornar essa nova lista
    if(primeiraOperacao->valor==operacaoASerRemovida->valor && primeiraOperacao->quantidade==operacaoASerRemovida->quantidade){
        primeiraOperacao = primeiraOperacao->proxima;
        return primeiraOperacao;
    }
    //para todos os outros casos em que não há remoção do cabeçote da lista vamos iterar sobre a lista e remover o nó fazendo o anterior dele apontar para o seguinte do seguinte
    else{
        Operacao *operacaoAtual = primeiraOperacao;
        while(operacaoAtual){
            if(operacaoAtual->proxima->valor == operacaoASerRemovida->valor && operacaoAtual->proxima->quantidade == operacaoASerRemovida->quantidade){
                operacaoAtual->proxima = operacaoAtual->proxima->proxima;
            }
            operacaoAtual = operacaoAtual->proxima;
        }
    }
}

void realizarOperacao(Operacao *compra, Operacao *venda, int indice) {
    // primeiro converto as quantidades para inteiro para poder realizar as comparações
    int qtdCompra = atoi(compra->quantidade);
    int qtdVenda = atoi(venda->quantidade);
    char quantidadeAtualizada[10];

    // três caminhos possíveis, um para cada resultado da comparação entre quantidades
    if(qtdCompra < qtdVenda){
        sprintf(quantidadeAtualizada, "%d", qtdVenda - qtdCompra);
        strcpy(venda->quantidade , quantidadeAtualizada);
        listaDinamicaPrincipal[indice].primeiraCompra = removeOperacao(compra, listaDinamicaPrincipal[indice].primeiraCompra);
    }
    if (qtdCompra > qtdVenda){
        sprintf(quantidadeAtualizada, "%d", qtdCompra - qtdVenda);
        strcpy(compra->quantidade , quantidadeAtualizada);
        listaDinamicaPrincipal[indice].primeiraVenda = removeOperacao(venda, listaDinamicaPrincipal[indice].primeiraVenda);
    }
    if(qtdCompra == qtdVenda){
        listaDinamicaPrincipal[indice].primeiraCompra = removeOperacao(compra, listaDinamicaPrincipal[indice].primeiraCompra);
        listaDinamicaPrincipal[indice].primeiraVenda = removeOperacao(venda, listaDinamicaPrincipal[indice].primeiraVenda);
    }
}

void comparaPrecoDaCompraComTodasAsVendas(Operacao *compra, Operacao *venda, int indice) {
    int valorDaCompra = pegaValorDaOperacao(compra);
    Operacao *vendaAtual = venda;
    while(vendaAtual){
        if(atof(vendaAtual->valor) == valorDaCompra){
            realizarOperacao(compra, venda, indice);
        }
        vendaAtual = vendaAtual->proxima;
    }
}

void realizarOperacoes() {
//  essa função possui 3 laços de repetição aninhados responsáveis por comparar todas as compras com todas as vendas de todos os títulos

//  o primeiro é esse for que itera sobre a lista principal
    for(int i=0; i<espacoPrincipalUtilizado; i++){
        Operacao *compraAtual = listaDinamicaPrincipal[i].primeiraCompra;

//      o segundo é esse while itera sobre todas as compras de cada sigla presente em cada índice da lista principal
        while(compraAtual){
            if(listaDinamicaPrincipal[i].primeiraVenda){

                //o terceiro está dentro dessa função que itera por todas as vendas de cada sigla
                comparaPrecoDaCompraComTodasAsVendas(compraAtual, listaDinamicaPrincipal[i].primeiraVenda, i);
            }
            compraAtual = compraAtual->proxima;
        }

    }
}

void exibirMenu() {
int opcao;
    while(1) {
        printf("\n**************");
        printf("\nDigite um dos numeros:\n");
        printf("[1] Listar operacoes:\n");
        printf("[2] Inserir operacoes:\n");
        printf("[0] Sair:\n");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                listarOfertas();
                break;
            case 2:
                inserirOperacaoDoUsuario();
                realizarOperacoes();
                break;
            default:
                printf("\nObrigado por usar o homebroker! Me encontre nas redes sociais buscando por caioreis123");
                free(listaDinamicaPrincipal);
                exit(0);
        }
    }
}


int main() {
    listaDinamicaPrincipal = (Titulo *) malloc(tamanhoDaListaPrincipal * sizeof(Titulo));
    carregarOfertasDeArquivo();
    exibirMenu();
    return 0;
}
