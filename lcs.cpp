#include "lcs.hpp"

#include <iostream>

#include <algorithm>
using namespace std;

//! Id de chamada da função, usado para plotagem dos gráficos no GraphView
int callid = 0;

//! Nome do arquivo do GraphView
string gv_file;

//! Tamanho em linhas e colunas do memo
size_t memo_r, memo_c;

/**
 * @brief Aloca uma matriz de memo e inicializa com -1
 * 
 * @param m Altura da matriz
 * @param n Largura da matriz
 * @return int** Matriz de memo
 */
int** alloc_memo(size_t m, size_t n) {
    int **memo = new int*[m + 1];

    for (int i = 0; i <= m; i++) {
        memo[i] = new int[n + 1];
        fill_n(memo[i], n + 1, -1);
    }

    return memo;
}

/**
 * @brief Libera uma matriz de memo
 * 
 * @param memo Matriz
 * @param m Altura da matriz
 */
void free_memo(int** memo, size_t m) {
    for (int i = 0; i <= m; i++)
        delete [] memo[i];
    delete [] memo;
}

/**
 * @brief Escreve um log do memo no arquivo de log
 * 
 * @param memo Matriz de memorização
 */
void log_memo(int** memo, size_t m, size_t n) {
    ofstream memo_file("memo_log", ios::app);
    memo_file << m << " " << n << endl;

    for (int i = 1; i < memo_r; i++) {
        for (int j = 1; j < memo_c; j++) {
            if (memo[i][j] >= 0)
                memo_file << memo[i][j];

            memo_file << ";";
        }
        memo_file << endl;
    }

    memo_file << endl;
    memo_file.close();
}

/**
 * @brief Define propriedades de um nó por número no gráfico
 * 
 * @param number Número do nó
 * @param attr Atributos do nó
 */
void gv_node(int number, const string& attr = "") {
    ofstream call_graph(gv_file, ios::app);

    call_graph << "node" << number;
    if (!attr.empty()) 
        call_graph << " [" << attr << "]";
    call_graph << endl;

    call_graph.close();
}

/**
 * @brief Define a label de um nó a partir dos parâmetros da função `lcs`
 * 
 * @param number Número do nó
 * @param a String A
 * @param b String B
 * @param m Tamanho da string A
 * @param n Tamanho da string B
 */
void gv_label(int number, const string& a, const string& b, size_t m, size_t n) {
    gv_node(
        number,
        "label=\"LCS(" 
        + a.substr(0, m)
        + ", "
        + b.substr(0, n)
        + ")\""
    );
}

/**
 * @brief Liga dois nós
 * 
 * @param from Nó de saída
 * @param to Nó de entrada
 */
void gv_edge(int from, int to) {
    ofstream call_graph(gv_file, ios::app);
    call_graph  << "node" << from 
                << " -> "
                << "node" << to
                << endl;
    call_graph.close();
}

// LCS ruinzão, sem memo
string lcs_bad(const string& a, const string& b, size_t m, size_t n) {

    gv_file = "call_graph_bad.gv";
    gv_label(callid, a, b, m, n);

    // Se o tamanho é zero, a resposta é vazia
    if (m == 0 || n == 0) {
        gv_node(callid, "color=white");
        return "";

    } else {

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            callid++;
            gv_edge(callid - 1, callid);

            return lcs_bad(a, b, m - 1, n - 1) + a[m - 1];

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int ci = callid;

            gv_node(callid, "color=red");

            callid++;
            gv_edge(callid - 1, callid);

            string sa = lcs_bad(a, b, m - 1, n);
            
            callid++;
            gv_edge(ci, callid);

            string sb = lcs_bad(a, b, m, n - 1);

            return sa.length() >= sb.length() ? sa : sb;
        }
    }
}

// LCS bonzão, com memo
int lcs_memo(const string& a, const string& b, size_t m, size_t n, int** memo) {

    gv_file = "call_graph_memo.gv";
    gv_label(callid, a, b, m, n);
    
    int result = 0;

    // Se o tamanho é zero, a resposta é vazia
    if (m == 0 || n == 0) {
        gv_node(callid, "color=white");
        return 0;

    } else {
        // Se já existe no memo, nem faz sentido calcular
        if (memo[m][n] != -1) {
            gv_node(callid, "color=green");
            return memo[m][n];
        }

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            callid++;
            gv_edge(callid - 1, callid);
                        
            result = lcs_memo(a, b, m - 1, n - 1, memo) + 1;

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int ci = callid;
            gv_node(ci, "color=red");

            callid++;
            gv_edge(ci, callid);
            int lcs_a = lcs_memo(a, b, m - 1, n, memo);

            callid++;
            gv_edge(ci, callid);
            int lcs_b = lcs_memo(a, b, m, n - 1, memo);

            result = max(lcs_a, lcs_b);
        }
    }
    
    memo[m][n] = result;
    log_memo(memo, m, n);

    return result;
}

// Backtracking do LCS a partir das strings e do memo
string lcs_backtrack(const string& a, const string& b, size_t m, size_t n, int** memo) {
    // Se alguma está vazia, o lcs é vazio
    if (m == 0 || n == 0)
        return "";

    // Se o último caractere das duas é igual, o lcs é o lcs do que vem antes
    // mais esse caractere no final
    if (a[m - 1] == b[n - 1])
        return lcs_backtrack(a, b, m - 1, n - 1, memo) + a[m - 1];

    // Se o LCS pra esquerda na matriz é maior ou igual, usa ele
    else if (memo[m][n - 1] >= memo[m - 1][n])
        return lcs_backtrack(a, b, m, n - 1, memo);

    // Se não, usa o LCS pra cima na matriz
    else
        return lcs_backtrack(a, b, m - 1, n, memo);
}

// LCS bonzão com memo, mas sem ele na assinatura. Serve pra chamar sem ter
// que se preocupar em alocar a matriz de memo e tal...
string lcs_memo(const string& a, const string& b) {
    ofstream memo_file("memo_log");
    memo_file   << a << endl
                << b << endl;
    memo_file.close();

    int m = a.length(), n = b.length();
    memo_r = m + 1;
    memo_c = n + 1;

    // Matriz de memo
    int** memo = alloc_memo(m, n);

    // Calcula o LCS
    lcs_memo(a, b, m, n, memo);
    string lcs = lcs_backtrack(a, b, m, n, memo);

    // Libera a matriz de memo
    free_memo(memo, m);

    return lcs;
}

// Diff recursivo com backtracking
diff_node* diff(const string& a, const string& b, int m, int n, int** memo) {
    diff_node *result, *last;

    // Se os caracteres são iguais, está no LCS e pontanto não tem diferença
    if (m > 0 && n > 0 && a[m - 1] == b[n - 1]) {
        last = diff(a, b, m - 1, n - 1, memo);

        result = new diff_node();
        result->operation = ' ';
        result->value = a[m - 1];
        
    // Se esse é um caractere que está na string B e não na string A, ele foi
    // adicionado
    } else if (n > 0 && (m == 0 || memo[m][n - 1] >= memo[m - 1][n])) {
        last = diff(a, b, m, n - 1, memo);

        result = new diff_node();
        result->operation = '+';
        result->value = b[n - 1];

    // Se esse é um caractere que está na string A e não na string B, ele foi
    // removido
    } else if (m > 0 && (n == 0 || memo[m - 1][n] > memo[m][n - 1])) {
        last = diff(a, b, m - 1, n, memo);
        
        result = new diff_node();
        result->operation = '-';
        result->value = a[m - 1];

    } else
        return nullptr;

    result->next = nullptr;
    
    // Se tinha algum nó antes, adiciona o atual ao fim da lista (as chamadas 
    // acontecem de trás pra frente)
    if (last) {
        diff_node* node = last;
        while (node->next)
            node = node->next;

        node->next = result;
        
        return last;

    // Se não retorna esse nó mesmo
    } else
        return result;
}

// Diff
diff_node* diff(const string& a, const string& b) {
    size_t m = a.length(), n = b.length();

    // Matriz de memo
    int** memo = alloc_memo(m, n);

    // Calcula o LCS
    lcs_memo(a, b, m, n, memo);

    // Calcula a lista diff das strings
    diff_node* result = diff(a, b, m, n, memo);

    // Libera a matriz de memo
    free_memo(memo, m);

    return result;
}