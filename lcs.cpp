#include "lcs.hpp"
#include <iostream>

int callid = 0;
string gv_file;

void gv_node(int number, const string& attr = "") {
    call_graph.open(gv_file, ios::app);

    call_graph << "node" << number;
    if (!attr.empty()) 
        call_graph << " [" << attr << "]";
    call_graph << endl;

    call_graph.close();
}

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

void gv_edge(int from, int to) {
    call_graph.open(gv_file, ios::app);
    call_graph  << "node" << from 
                << " -> "
                << "node" << to
                << endl;
    call_graph.close();
}

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

string lcs_memo(const string& a, const string& b, size_t m, size_t n, string*** memo) {

    gv_file = "call_graph_memo.gv";
    gv_label(callid, a, b, m, n);

    // Se já existe no memo, nem faz sentido calcular
    if (memo[m][n]) {
        gv_node(callid, "color=green");

        return *memo[m][n];
    }
    
    string result = "";
    
    // Se o tamanho é zero, a resposta é vazia
    if (m != 0 && n != 0) {

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            callid++;
            gv_edge(callid - 1, callid);
                        
            result = lcs_memo(a, b, m - 1, n - 1, memo) + a[m - 1];

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int ci = callid;
            gv_node(ci, "color=red");

            callid++;
            gv_edge(ci, callid);
            string sa = lcs_memo(a, b, m - 1, n, memo);
            
            callid++;
            gv_edge(ci, callid);
            string sb = lcs_memo(a, b, m, n - 1, memo);

            result = sa.length() >= sb.length() ? sa : sb;
        }

    } else
        gv_node(callid, "color=white");

    memo[m][n] = new string(result);
    return result;
}

string lcs_memo(const string& a, const string& b, size_t m, size_t n) {

    // Matriz de memo
    string*** memo = (string***)calloc(m + 1, sizeof(string**));
    for (int i = 0; i <= m; i++)
        memo[i] = (string**)calloc(n + 1, sizeof(string*));
    
    // Calcula o LCS
    string lcs = lcs_memo(a, b, m, n, memo);

    // Libera a matriz de memo
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++)
            if (memo[i][j])
                free(memo[i][j]);

        free(memo[i]);
    }
    free(memo);

    return lcs;
}