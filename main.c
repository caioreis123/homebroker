#include <stdio.h>
// a estrutura principal será um arrayList, ou seja, um array com alocação dinâmica de memória capaz de aumentar de tamanho
// assim que seu último elemento for adicionado. Usa-se realoc para isso. Cada elemento desse arrayList contém uma struct titulos
// que por sua vez contém a sigla do título (ex: PETR4), e dois cabeçotes, um pra cada tipo de operação, ou seja, aponta para o nó inicial
// de uma lista ligada com as operações de compra/venda

// cada nó da lista ligada composta pela struct operaçao contém a próxima operação, a quantidade e o valor.


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

Titulo arrayList[3];
int tamanhoArrayList = 3;
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

int main() {
    IncluirOferta();
    return 0;
}
