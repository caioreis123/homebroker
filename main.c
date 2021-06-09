#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// a estrutura principal será um arrayList, ou seja, um array com alocação dinâmica de memória capaz de aumentar de tamanho
// assim que seu último elemento for adicionado. Usa-se realoc para isso. Cada elemento desse arrayList contém uma struct titulos
// que por sua vez contém a sigla do título (ex: PETR4), e dois cabeçotes, um pra cada tipo de operação, ou seja, aponta para o nó inicial
// de uma lista ligada com as operações de compra/venda

// cada nó da lista ligada composta pela struct operaçao contém a próxima operação, a quantidade e o valor.


#define TAM 10
typedef struct Operacao{
    struct Operacao* proximaOperacao;
    int quantidade;
    double valor;
}Operacao;

typedef struct Titulo{
    char sigla[10]; // const char* é utilizado para que a string possa ser retornada de uma função
    Operacao primeiraCompra;
    Operacao primeiraVenda;
}Titulo;

Titulo arrayList[TAM];
int tamanhoArrayList = TAM;
int inseridosArrayList = 0;

char identificaTipo(){
    char tipo;
    printf("digite se [c]ompra ou [v]enda \n");
    scanf("%c", &tipo);
    return tipo;
}

const char *identificaSigla() {
    return "PETR4";
}

int identificaQuantidade() {
    return 42;
}

double identificaValor() {
    return 51.32;
}

void IncluirOferta() {
//    const char* sigla = identificaSigla();
    char tipo = identificaTipo();
//    int quantidade = identificaQuantidade();
//    double valor = identificaValor();
    Operacao operacao = {
            .quantidade = 30,
            .valor = 51.09,
    };
    Titulo titulo = {
            .sigla= "PETR4",
    };
    if (tipo == 'c'){
       titulo.primeiraCompra = operacao;
    }
    else{
       titulo.primeiraVenda = operacao;
    }
    arrayList[0] = titulo;
    printf(" %s %d  %f\n", arrayList[0].sigla, arrayList[0].primeiraCompra.quantidade, arrayList[0].primeiraCompra.valor);
}

void carregarOfertasDeArquivo(){
    //essas variáveis são temporárias e serão removidas em breve
    char *sigla;
    char *tipo;
    double valor;
    long int quantidade;

    FILE *arquivo = fopen("../homebroker-database.csv", "r");
    if (!arquivo){
        arquivo = fopen("homebroker-database.csv", "r");
        if(!arquivo){
            printf("nao foi possivel ler o arquivo\n");
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
                valor = strtod(celula, NULL);
            }
            if(coluna_atual == 3) {
                quantidade = strtol(celula, NULL, 10);
            }
          // a passagem do NULL no strtok vai avançar para a próxima célula
            celula = strtok(NULL, ",");
            coluna_atual++;
        }
        printf("%s %s %f %ld \n", sigla, tipo, valor, quantidade);
//        salvarOferta(sigla, tipo, valor, quantidade);
    }
    fclose(arquivo);
}

int main() {
    carregarOfertasDeArquivo();
//    IncluirOferta();
    return 0;
}
