#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_INICIAL 10

// a estrutura principal será um listaDinamicaPrincipal, ou seja, um array com alocação dinâmica de memória capaz de aumentar de tamanho
// assim que seu último elemento for adicionado. Usa-se realoc para isso. Cada elemento desse listaDinamicaPrincipal contém uma struct titulos
// que por sua vez contém a sigla do título (ex: PETR4), e dois cabeçotes, um pra cada tipo de operação, ou seja, aponta para o nó inicial
// de uma lista ligada com as operações de compra/venda
// cada nó da lista ligada composta pela struct operaçao contém a próxima operação, a quantidade e o valor.
typedef struct NohOperacao{
    struct NohOperacao* nohOperacaoAnterior;
    char quantidade[10]; // tudo vai ser armazenado como string até o momento de fazer as operações matemática, a partir daí fazemos as conversões e desconversões
    char valor[10];
}NohOperacao;

typedef struct Titulo{
    char sigla[10]; // const char* é utilizado para que a string possa ser retornada de uma função
    NohOperacao ultimaCompra;
    NohOperacao ultimaVenda;
}Titulo;

//  é nossa variável global básica acessada por toda a aplicação e que é dinâmica e contém os títulos
Titulo *listaDinamicaPrincipal;
int tamanhoDaListaPrincipal = TAMANHO_INICIAL;
int espacoPrincipalUtilizado = 0;

Titulo *buscaTitulo(char *sigla, int iterador) {
    if (listaDinamicaPrincipal[iterador].sigla == sigla){
        return &listaDinamicaPrincipal[iterador];
    }
    if(iterador>=espacoPrincipalUtilizado) {
        return NULL;
    }
    return buscaTitulo(sigla, ++iterador);
}

void salvarOferta(char *sigla, const char *tipo, char *valor, char *quantidade){
    // tentativa 2:
//    Titulo *tituloExistente = buscaTitulo(sigla, 0);
//    if(tituloExistente && tipo[0]=='c'){
//        NohOperacao novaCompra;
//        novaCompra.quantidade = quantidade;
//        novaCompra.valor = valor;
//        novaCompra.nohOperacaoAnterior = &tituloExistente->ultimaCompra;
//        tituloExistente->ultimaCompra = novaCompra;
//    }
//    if(tituloExistente && tipo[0]=='v'){
//        NohOperacao novaVenda;
//        novaVenda.quantidade = quantidade;
//        novaVenda.valor = valor;
//        novaVenda.nohOperacaoAnterior = &tituloExistente->ultimaVenda;
//        tituloExistente->ultimaVenda = novaVenda;
//    }
//    if(!tituloExistente){
        strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].sigla, sigla); // os valores que estou recebendo como argumentos estão mudando, então as atribuições ficam todas erradas, é preciso usar strcpy para funcionar
        if(tipo[0]=='c'){
            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].ultimaCompra.quantidade,quantidade);
            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].ultimaCompra.valor,valor);
            listaDinamicaPrincipal[espacoPrincipalUtilizado].ultimaCompra.nohOperacaoAnterior = NULL;
        }
        if(tipo[0]=='v'){
            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].ultimaVenda.quantidade,quantidade);
            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].ultimaVenda.valor,valor);
            listaDinamicaPrincipal[espacoPrincipalUtilizado].ultimaVenda.nohOperacaoAnterior = NULL;
        }
        espacoPrincipalUtilizado = espacoPrincipalUtilizado + 1;
//    }




//    // tentativa 1:
//    NohOperacao novaOperacao;
//    int existeTitulo = 0;
////  itera sobre a estrutura principal buscando pra ver se já há título salvo com a mesma sigla
////  pois caso haja, o comportamento é armazenar a nova operação ao final da lista encadeada e bota ela para apontar para a operação anterior
//    for(int iterador = 0; iterador < espacoPrincipalUtilizado; iterador++){
//        if(listaDinamicaPrincipal[iterador].sigla == sigla){
//            existeTitulo = 1;
//           if(tolower(tipo[0])=='c'){
//               novaOperacao.valor = valor;
//               novaOperacao.quantidade = quantidade;
//               novaOperacao.nohOperacaoAnterior = &listaDinamicaPrincipal[iterador].ultimaCompra; // bota a nova operação para apontar para a última operação daquele tipo (c ou v) adicionada anteriormente
//               listaDinamicaPrincipal[iterador].ultimaCompra = novaOperacao; // atualiza a última operação daquele título para ser a nova operação
//           }
//           else{
//               novaOperacao.valor = valor;
//               novaOperacao.quantidade = quantidade;
//               novaOperacao.nohOperacaoAnterior = &listaDinamicaPrincipal[iterador].ultimaVenda;
//               listaDinamicaPrincipal[iterador].ultimaVenda = novaOperacao;
//           }
//       }
//   }
//
//    if(!existeTitulo){
////      primeiro verificamos se há espaço na estrutura principal para receber mais um novo título, caso não haja será preciso dobrar o tamanho da estrutura
////      para isso vamos atualizar a variável global que armazena o tamanho da estrutura principal e dobrar o tamanho da estrutura principal
//        if(tamanhoDaListaPrincipal==espacoPrincipalUtilizado){
//            tamanhoDaListaPrincipal = tamanhoDaListaPrincipal * 2;
//            listaDinamicaPrincipal = (Titulo *) realloc(listaDinamicaPrincipal, tamanhoDaListaPrincipal * sizeof(Titulo) );
//        }
//
////      como não existe o título, será preciso criar um apontando para NULL já que não há nenhuma operação prévia
////      a criação do título envolve além da adição da sigla, a adição da última operação para cada tipo, daí inserimos como última operação a nova operação
//        Titulo novoTitulo;
//        novoTitulo.sigla = sigla;
//        novaOperacao.nohOperacaoAnterior = NULL;
//        if(tolower(tipo[0])=='c'){
//            novaOperacao.valor = valor;
//            novaOperacao.quantidade = quantidade;
//            novoTitulo.ultimaCompra = novaOperacao;
//        }
//        else{
//            novaOperacao.valor = valor;
//            novaOperacao.quantidade = quantidade;
//            novoTitulo.ultimaVenda = novaOperacao;
//        }
//
////      ao final depois do título já montado precisamos apenas adicionar ele na nossa estrutura principal
//        listaDinamicaPrincipal[espacoPrincipalUtilizado++] = novoTitulo;
//    }
}

//char identificaTipo(){
//    char tipo;
//    printf("digite se [c]ompra ou [v]enda \n");
//    scanf("%c", &tipo);
//    return tipo;
//}

//const char *identificaSigla() {
//    return "PETR4";
//}
//
//int identificaQuantidade() {
//    return 42;
//}
//
//double identificaValor() {
//    return 51.32;
//}

void inserirOfertaDoUsuario() {
//    vai pegar os valores do usuário com uma funçao pegar valores do usuário que retorna uma array de strings
//    e depois passar esses valores para a salvarOferta


//    const char* sigla = identificaSigla();
//    char tipo = identificaTipo();
//    int quantidade = identificaQuantidade();
//    double valor = identificaValor();
//    NohOperacao operacao = {
//            .quantidade = "30",
//            .valor = "51.09",
//    };
//    Titulo titulo = {
//            .sigla= "PETR4",
//    };
//    if (tipo == 'c'){
//       titulo.ultimaCompra = operacao;
//    }
//    else{
//       titulo.ultimaVenda = operacao;
//    }
//    listaDinamicaPrincipal[0] = titulo;
//    printf(" %s %s  %s\n", listaDinamicaPrincipal[0].sigla, listaDinamicaPrincipal[0].ultimaCompra.quantidade, listaDinamicaPrincipal[0].ultimaCompra.valor);
}


//void listarOfertas(){
//    for(int iterador = 0; iterador < espacoPrincipalUtilizado; iterador++){
//        printf("Sigla: %s\n", listaDinamicaPrincipal[iterador].sigla);
//        printf("Compras:\n");
//        while(listaDinamicaPrincipal[iterador].ultimaCompra.nohOperacaoAnterior!=NULL){
//            printf(" valor: %s, quantidade: %s\n", listaDinamicaPrincipal[iterador].ultimaCompra.valor, listaDinamicaPrincipal[iterador].ultimaCompra.quantidade);
//            listaDinamicaPrincipal[iterador].ultimaCompra.nohOperacaoAnterior = listaDinamicaPrincipal[iterador].ultimaCompra.nohOperacaoAnterior->nohOperacaoAnterior;
//        }
//
//        printf("Vendas:\n");
//        while(listaDinamicaPrincipal[iterador].ultimaVenda.nohOperacaoAnterior!=NULL){
//            printf(" valor: %s, quantidade: %s\n", listaDinamicaPrincipal[iterador].ultimaVenda.valor, listaDinamicaPrincipal[iterador].ultimaVenda.quantidade);
//            listaDinamicaPrincipal[iterador].ultimaVenda.nohOperacaoAnterior = listaDinamicaPrincipal[iterador].ultimaVenda.nohOperacaoAnterior->nohOperacaoAnterior;
//        }
//    }
//}


void carregarOfertasDeArquivo(){
    //essas variáveis são temporárias e serão removidas em breve
    char *sigla;
    char *tipo;
    char *valor;
    char *quantidade;

    FILE *arquivo = fopen("../homebroker-database.csv", "r");
    if (!arquivo){
        arquivo = fopen("homebroker-database.csv", "r");
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
//        printf("%s %s %s %s \n", sigla, tipo, valor, quantidade);
        salvarOferta(sigla, tipo, valor, quantidade);
    }
    fclose(arquivo);
}

void exibirMenu() {
//    vai exibir as seguintes opções para o usuário
// 1. Inserir Oferta (vai chamar a função inserir oferta do usuário que tem vários prompts e depois chama a função salvarOferta)
// 2. Listar Ofertas
// 3. Sair
}

int main() {
    listaDinamicaPrincipal = (Titulo *) malloc(tamanhoDaListaPrincipal * sizeof(Titulo));
    carregarOfertasDeArquivo();
    printf("sigla:%s\nqtd de compra:%s\nvalor compra:%s\nvalor venda:%s\nquantidade venda:%s\n",listaDinamicaPrincipal[0].sigla, listaDinamicaPrincipal[0].ultimaCompra.quantidade, listaDinamicaPrincipal[0].ultimaCompra.valor, listaDinamicaPrincipal[0].ultimaVenda.valor, listaDinamicaPrincipal[0].ultimaVenda.quantidade);
    printf("sigla:%s\nqtd de compra:%s\nvalor compra:%s\nvalor venda:%s\nquantidade venda:%s\n",listaDinamicaPrincipal[1].sigla, listaDinamicaPrincipal[1].ultimaCompra.quantidade, listaDinamicaPrincipal[1].ultimaCompra.valor, listaDinamicaPrincipal[1].ultimaVenda.valor, listaDinamicaPrincipal[1].ultimaVenda.quantidade);
//    listarOfertas();
//    exibirMenu();
    free(listaDinamicaPrincipal);
    return 0;
}
