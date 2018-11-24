# LCS Demo

Demonstração de algoritmo Largest Common Subsequence (Maior subsequência comum) com e sem memorização, aplicando [`GraphViz`](http://www.graphviz.org/) e [`Matplotlib`](https://matplotlib.org/) para visualização de características internas do algoritmo.

Para executar use o arquivo `run.cmd`. Após a execução do programa serão gerados arquivos `call_graph_bad.gv`, `call_graph_memo.gv` e `memo_log`. Os dois primeiros podem ser usados para criar grafos com o `GraphViz`.

O arquivo `memo_log` é usado pelo script Python `animate_memo.py`, que usa `Matplotlib` e pode ser executado para mostrar uma animação dos estados da matriz de memorização durante a execução do algoritmo.

# Exemplos

## Árvores de chamada de função

Árvore de chamadas de função para um LCS entre as strings `asdasdasd` e `bsdbsdbsd`, sem memorização (784 chamadas a `LCS`):

![LCS sem memorização](https://i.imgur.com/vV19Byv.png)

Mesma árvore, mas para um LCS com memorização (93 chamadas a `LCS`):

![LCS com memorização](https://i.imgur.com/HNMU15t.png)

Legenda: Cinza = as sequências terminam com o mesmo caractere, Vermelho = as sequências não terminam com o mesmo caractere, Verde = salvo no memo, Branco = uma ou mais sequências vazias

## Tabela de memorização

![Tabela de memorização](https://i.imgur.com/ESngM7P.gif)

Cada linha representa um caractere da sequência `asdasdasd` e cada coluna representa um caractere da sequência `bsdbsdbsd`. Considere que as legendas das linhas e colunas são cumulativas. O valor da célula (i, j) é o LCS da substring de `asdasdasd` de 0 até i e da substring de `bsdbsdbsd` de 0 até j.

As cores na matriz representam a frequência de uso de cada célula, de forma que tons mais próximos do amarelo representam células com maior frequência. A cor branca indica células que não foram usadas para chegar à resposta (isso inclui a própria resposta).