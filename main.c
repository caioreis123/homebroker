#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_INICIAL 10

//explicar o mercado de ação compra e venda
//explicar funcionando, como se fosse um usuário
//mostrar o código funcionando

// a estrutura principal será uma listaDinamicaPrincipal, ou seja, um array com alocação dinâmica de memória capaz de aumentar de tamanho
// assim que seu último elemento for adicionado. Usa-se realoc para dobrar seu tamanho. Cada elemento desse listaDinamicaPrincipal contém uma struct titulos
// que por sua vez contém a sigla do título (ex: PETR4), e dois cabeçotes de lista ligada, um pra cada lista que armazena todas as operações daquele tipo
// cada nó dessa lista ligada é composta pela struct operaçao que contém a próxima operação, a quantidade e o valor da operação.
typedef struct operacao{
    struct operacao *proxima;
    // tudo vai ser armazenado como string até o momento de fazer as operações matemáticas
    char quantidade[10];
    char valor[10];
}Operacao;

typedef struct Titulo{
    char sigla[10];
    Operacao *primeiraCompra;
    Operacao *primeiraVenda;
}Titulo;

Titulo *listaDinamicaPrincipal; // armazena todos os títulos, aumentando de tamanho sempre que o espaço da array acaba
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
    int indiceDaListaPrincipalDaSigla = buscaIndicePorSigla(sigla);
    if (indiceDaListaPrincipalDaSigla == -1) {
        indiceDaListaPrincipalDaSigla = inserirNovoTituloAoFinalDaListaPrincipal(sigla);
    }
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
        printf("Compras:\n");
        listarOperacoes(listaDinamicaPrincipal[i].primeiraCompra);
        printf("\nVendas:\n");
        listarOperacoes(listaDinamicaPrincipal[i].primeiraVenda);
        printf("\n###########\n");
    }
}


void carregarOfertasDeArquivo(){
    char *sigla;
    char *tipo;
    char *valor;
    char *quantidade;

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

Operacao* removeOperacao(Operacao *operacaoASerRemovida, Operacao *primeiraOperacao) {
    Operacao *operacaoAtual = primeiraOperacao;
    if(primeiraOperacao->valor==operacaoASerRemovida->valor && primeiraOperacao->quantidade==operacaoASerRemovida->quantidade){
        primeiraOperacao = primeiraOperacao->proxima;
        return primeiraOperacao;
    }
    else{
        while(operacaoAtual){
            if(operacaoAtual->proxima->valor == operacaoASerRemovida->valor && operacaoAtual->proxima->quantidade == operacaoASerRemovida->quantidade){
                operacaoAtual->proxima = operacaoAtual->proxima->proxima;
            }
            operacaoAtual = operacaoAtual->proxima;
        }
    }
}

void realizarOperacao(Operacao *compra, Operacao *venda, int indice) {
    int qtdCompra = atoi(compra->quantidade);
    int qtdVenda = atoi(venda->quantidade);
    char quantidadeAtualizada[10];
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
    for(int i=0; i<espacoPrincipalUtilizado; i++){
        Operacao *compraAtual = listaDinamicaPrincipal[i].primeiraCompra;
        while(compraAtual){
            if(listaDinamicaPrincipal[i].primeiraVenda){
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
                printf("\nSaindo...");
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
