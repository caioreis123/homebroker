# Homebroker 
É um projeto em C que simula a compra e venda de ações da bolsa de valores.

### Vídeo Explicativo no Youtube
https://youtu.be/U_0SQiBERtc

### Funcionalidades básicas:
1. leitura de dados de um arquivo csv
2. leitura de dados inseridos pelo usuário
3. execução de operações entre compras e vendas de mesmo valor e sigla
4. listagem das operações salvas pela aplicação

### Estrutura de dados utilizadas:
<b>Array com locação dinâmica</b> capaz de dobrar de tamanho sempre que atingir seu limite máximo.
Cada índice dessa array possui um título com a sigla da Ação e duas <b>listas encadeadas</b>, uma para compra e outra para venda. Cada nó dessas listas possui a quantidade e valor da operação.

### Instruções de execução:
Basta compilar o arquivo e executar o arquivo gerado.
```bash
gcc main.c && ./a.out
```