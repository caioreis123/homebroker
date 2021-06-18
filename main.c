#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_INICIAL 10

//explicar o mercado de ação compra e venda
//explicar funcionando, como se fosse um usuário
//mostrar o código funcionando

// a estrutura principal será um listaDinamicaPrincipal, ou seja, um array com alocação dinâmica de memória capaz de aumentar de tamanho
// assim que seu último elemento for adicionado. Usa-se realoc para isso. Cada elemento desse listaDinamicaPrincipal contém uma struct titulos
// que por sua vez contém a sigla do título (ex: PETR4), e dois cabeçotes, um pra cada tipo de operação, ou seja, aponta para o nó inicial
// de uma lista ligada com as operações de compra/venda
// cada nó da lista ligada composta pela struct operaçao contém a próxima operação, a quantidade e o valor.
typedef struct NohOperacao{
    struct NohOperacao *nohOperacaoSeguinte;
    char quantidade[10]; // tudo vai ser armazenado como string até o momento de fazer as operações matemática, a partir daí fazemos as conversões e desconversões
    char valor[10];
}NohOperacao;

typedef struct Titulo{
    char sigla[10]; // const char* é utilizado para que a string possa ser retornada de uma função
    NohOperacao *listaDeCompras;
    NohOperacao *primeiraVenda;
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
    int tituloExistente = 0;
    // tentativa 2:
//    Titulo *tituloExistente = buscaTitulo(sigla, 0);
    for(int i = 0; i<espacoPrincipalUtilizado;i++){
        if(!strcmp(sigla, listaDinamicaPrincipal[i].sigla)){ //se a sigla já existe
            tituloExistente = 1;
            if(tipo[0]=='c'){
                NohOperacao *novaCompra;
                novaCompra = malloc(sizeof (NohOperacao));
                strcpy(novaCompra->quantidade, quantidade);
                strcpy(novaCompra->valor, valor);
                novaCompra->nohOperacaoSeguinte = listaDinamicaPrincipal[i].listaDeCompras;
                listaDinamicaPrincipal[i].listaDeCompras = novaCompra;
//                NohOperacao novaCompra;
//                novaCompra->nohOperacaoSeguinte = NULL;
//                novaCompra->nohOperacaoSeguinte = &listaDinamicaPrincipal[i].listaDeCompras;
            }
        }
    }

//    if(tituloExistente && tipo[0]=='c'){
//        NohOperacao novaCompra;
//        strcpy(novaCompra.quantidade, quantidade);
//        strcpy(novaCompra.valor, valor);
//        novaCompra.nohOperacaoSeguinte = &tituloExistente->primeiraCompra;
//        tituloExistente->primeiraCompra = novaCompra;
//    }
//    if(tituloExistente && tipo[0]=='v'){
//        NohOperacao novaVenda;
//        novaVenda.quantidade = quantidade;
//        novaVenda.valor = valor;
//        novaVenda.nohOperacaoSeguinte = &tituloExistente->primeiraVenda;
//        tituloExistente->primeiraVenda = novaVenda;
//    }
    if(!tituloExistente){
        strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].sigla, sigla); // os valores que estou recebendo como argumentos estão mudando, então as atribuições ficam todas erradas, é preciso usar strcpy para funcionar
        if(tipo[0]=='c'){
            NohOperacao *novaListaDeCompras = NULL;
            NohOperacao  *novaCompra = (NohOperacao*) malloc(sizeof(NohOperacao));
            strcpy(novaCompra->quantidade, quantidade);
//            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].listaDeCompras.quantidade, quantidade);
//            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].listaDeCompras.valor, valor);
            strcpy(novaCompra->valor, valor);
            novaListaDeCompras=novaCompra;
            novaCompra->nohOperacaoSeguinte = NULL;
//            listaDinamicaPrincipal[espacoPrincipalUtilizado].listaDeCompras.nohOperacaoSeguinte = NULL;
        }
        if(tipo[0]=='v'){
//            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].primeiraVenda.quantidade, quantidade);
//            strcpy(listaDinamicaPrincipal[espacoPrincipalUtilizado].primeiraVenda.valor, valor);
//            listaDinamicaPrincipal[espacoPrincipalUtilizado].primeiraVenda.nohOperacaoSeguinte = NULL;
        }
        espacoPrincipalUtilizado = espacoPrincipalUtilizado + 1;
    }




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
//               novaOperacao.nohOperacaoSeguinte = &listaDinamicaPrincipal[iterador].primeiraCompra; // bota a nova operação para apontar para a última operação daquele tipo (c ou v) adicionada anteriormente
//               listaDinamicaPrincipal[iterador].primeiraCompra = novaOperacao; // atualiza a última operação daquele título para ser a nova operação
//           }
//           else{
//               novaOperacao.valor = valor;
//               novaOperacao.quantidade = quantidade;
//               novaOperacao.nohOperacaoSeguinte = &listaDinamicaPrincipal[iterador].primeiraVenda;
//               listaDinamicaPrincipal[iterador].primeiraVenda = novaOperacao;
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
//        novaOperacao.nohOperacaoSeguinte = NULL;
//        if(tolower(tipo[0])=='c'){
//            novaOperacao.valor = valor;
//            novaOperacao.quantidade = quantidade;
//            novoTitulo.primeiraCompra = novaOperacao;
//        }
//        else{
//            novaOperacao.valor = valor;
//            novaOperacao.quantidade = quantidade;
//            novoTitulo.primeiraVenda = novaOperacao;
//        }
//
////      ao final depois do título já montado precisamos apenas adicionar ele na nossa estrutura principal
//        listaDinamicaPrincipal[espacoPrincipalUtilizado++] = novoTitulo;
//    }
}

void listarOfertas(){
    for(int i = 0; i < espacoPrincipalUtilizado; i++){
        printf("Sigla: %s\n", listaDinamicaPrincipal[i].sigla);
        printf("Compras:\n");
        while(listaDinamicaPrincipal[i].listaDeCompras->nohOperacaoSeguinte != NULL){
//            printf(" valor: %s, quantidade: %s\n", listaDinamicaPrincipal[i].listaDeCompras.valor, listaDinamicaPrincipal[i].listaDeCompras.quantidade);
//            listaDinamicaPrincipal[i].listaDeCompras->nohOperacaoSeguinte = listaDinamicaPrincipal[i].listaDeCompras.nohOperacaoSeguinte->nohOperacaoSeguinte;
            printf("teste");
        }

//        printf("Vendas:\n");
//        while(listaDinamicaPrincipal[i].primeiraVenda.nohOperacaoSeguinte!=NULL){
//            printf(" valor: %s, quantidade: %s\n", listaDinamicaPrincipal[i].primeiraVenda.valor, listaDinamicaPrincipal[i].primeiraVenda.quantidade);
//            listaDinamicaPrincipal[i].primeiraVenda.nohOperacaoSeguinte = listaDinamicaPrincipal[i].primeiraVenda.nohOperacaoSeguinte->nohOperacaoSeguinte;
//        }
    }
}


void carregarOfertasDeArquivo(){
    //essas variáveis são temporárias e serão removidas em breve
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

typedef struct operacao{
    char quantidade[10];
    char valor[10];
    struct operacao *proxima;
}Operacao;

Operacao* criarOperacao(){
    Operacao* operacao = (Operacao*) malloc(sizeof(Operacao));
    return operacao;
}

Operacao* inserirOperacaoNoInicio(Operacao* Lista, char quantidade[10], char valor[10]){
    Operacao *novaOperacao = criarOperacao();
    strcpy(novaOperacao->quantidade, quantidade);
    strcpy(novaOperacao->valor, valor);
    if(Lista == NULL){
        Lista=novaOperacao;
        novaOperacao->proxima=NULL;
    }
    else{
        novaOperacao->proxima=Lista;
        Lista=novaOperacao;
    }
    return Lista;
}

void listarOperacoes(Operacao* Lista){
    Operacao *auxiliar=Lista;
    while(auxiliar!=NULL){
        printf("quantidade: %s\n", auxiliar->quantidade);
        printf("valor: %s\n", auxiliar->valor);
        auxiliar=auxiliar->proxima;
    }
}

int main() {
    Operacao* Lista=NULL;
    Lista=inserirOperacaoNoInicio(Lista, "29", "12");
    Lista=inserirOperacaoNoInicio(Lista, "17", "09");
    Lista=inserirOperacaoNoInicio(Lista, "09", "05");
    listarOperacoes(Lista);


//    listaDinamicaPrincipal = (Titulo *) malloc(tamanhoDaListaPrincipal * sizeof(Titulo));
//    carregarOfertasDeArquivo();
//    listarOfertas();
//    exibirMenu();
//    free(listaDinamicaPrincipal);
    return 0;
}
