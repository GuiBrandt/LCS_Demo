#include "lcs.hpp"

#include <iostream>

//! Id de chamada da função, usado para plotagem dos gráficos no GraphView
int callid = 0;

//! Nome do arquivo do GraphView
string gv_file;

//! Tamanho em linhas e colunas do memo
size_t memo_r, memo_c;

/**
 * @brief Escreve um log do memo no arquivo de log
 * 
 * @param memo Matriz de memorização
 */
void log_memo(string*** memo, size_t m, size_t n) {
    ofstream memo_file("memo_log", ios::app);
    memo_file << m << " " << n << endl;

    for (int i = 0; i < memo_r; i++) {
        for (int j = 0; j < memo_c; j++) {
            if (memo[i][j])
                memo_file << *memo[i][j];

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
string lcs_memo(const string& a, const string& b, size_t m, size_t n, string*** memo) {

    gv_file = "call_graph_memo.gv";
    //gv_label(callid, a, b, m, n);
    
    // Se o tamanho é zero, a resposta é vazia
    if (m == 0 || n == 0) {
        //gv_node(callid, "color=white");
        return "";

    } else {
        string result = "";

        // Se já existe no memo, nem faz sentido calcular
        if (memo[m - 1][n - 1]) {
            //gv_node(callid, "color=green");
            return *memo[m - 1][n - 1];
        }

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            callid++;
            //gv_edge(callid - 1, callid);
                        
            result = lcs_memo(a, b, m - 1, n - 1, memo) + a[m - 1];

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int ci = callid;
            //gv_node(ci, "color=red");

            callid++;
            //gv_edge(ci, callid);

            string sa = lcs_memo(a, b, m - 1, n, memo);
            
            callid++;
            //gv_edge(ci, callid);

            string sb = lcs_memo(a, b, m, n - 1, memo);

            result = sa.length() >= sb.length() ? sa : sb;
        }

        memo[m - 1][n - 1] = new string(result);
        //log_memo(memo, m, n);

        return result;
    }
}

// LCS bonzão com memo, mas sem ele na assinatura. Serve pra chamar sem ter
// que se preocupar em alocar a matriz de memo e tal...
string lcs_memo(const string& a, const string& b) {
    ofstream memo_file("memo_log");
    memo_file   << a << endl
                << b << endl;
    memo_file.close();

    int m = a.length(), n = b.length();

    // Matriz de memo
    string*** memo = new string**[m];
    for (int i = 0; i < m; i++)
        memo[i] = (string**)calloc(n, sizeof(string*));
    
    memo_r = m;
    memo_c = n;

    // Calcula o LCS
    string lcs = lcs_memo(a, b, m, n, memo);

    // Libera a matriz de memo
    for (int i = 0; i < m; i++)
        delete [] memo[i];
    delete [] memo;

    return lcs;
}

/**
 * @brief Gera uma lista de diferenças entre duas strings
 * 
 * @param a String A
 * @param b String B
 * @param m Tamanho da string A - 1
 * @param n Tamanho da string B - 1
 * @param memo Matriz de memorização gerada pela função lcs_memo
 * @return diff_node* Lista ligada de diferenças entre as strings
 */
diff_node* diff(const string& a, const string& b, int m, int n, string*** memo) {
    diff_node *result, *last;

    if (m >= 0 && n >= 0 && a[m] == b[n]) {
        last = diff(a, b, m - 1, n - 1, memo);

        result = new diff_node();
        result->operation = ' ';
        result->value = a[m];
        
    } else if (
        (n == 0 && (m <= 0 || !memo[m - 1][n])) ||
        (n > 0 && (m <= 0 || !memo[m - 1][n] || (memo[m][n - 1] ? memo[m][n - 1]->length() : 0) >= memo[m - 1][n]->length()))
    ) {
        last = diff(a, b, m, n - 1, memo);

        result = new diff_node();
        result->operation = '+';
        result->value = b[n];

    } else if (
        (m == 0 && (n <= 0 || !memo[m][n - 1])) ||
        (m > 0 && (n <= 0 || !memo[m][n - 1] || (memo[m - 1][n] ? memo[m - 1][n]->length() : 0) >= memo[m][n - 1]->length()))
    ) {
        last = diff(a, b, m - 1, n, memo);

        result = new diff_node();
        result->operation = '-';
        result->value = a[m];

    } else
        return nullptr;

    result->next = nullptr;
    
    if (last) {
        diff_node* node = last;
        while (node->next)
            node = node->next;

        node->next = result;
        
        return last;

    } else
        return result;
}

diff_node* diff(const string& a, const string& b) {
    int m = a.length(), n = b.length();

    string*** memo = new string**[m];
    for (int i = 0; i < m; i++)
        memo[i] = (string**)calloc(n, sizeof(string*));

    // Calcula o LCS
    string lcs = lcs_memo(a, b, m, n, memo);

    diff_node* result = diff(a, b, m - 1, n - 1, memo);

    // Libera a matriz de memo
    for (int i = 0; i < m; i++)
        delete [] memo[i];
    delete [] memo;

    return result;
}