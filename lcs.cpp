#include "lcs.hpp"
#include <iostream>

int call_number = 0;

string lcs_bad(const string& a, const string& b, size_t m, size_t n) {

    // Se o tamanho é zero, a resposta é vazia
    if (m == 0 || n == 0) {
        call_graph.open("call_graph_bad.gv", ios::app);
        call_graph  << "n" << call_number << " [color=white]" << endl;
        call_graph.close();

        return "";

    } else {

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            call_graph.open("call_graph_bad.gv", ios::app);
            call_number++;
            call_graph  << "n" << call_number << " [label=\""
                        << "LCS(" << a.substr(0, m - 1) << ", " << b.substr(0, n - 1) << ")\"]"
                        
                        << endl

                        << "n" << (call_number - 1)
                        << " -> "
                        << "n" << call_number
                        << endl;
            call_graph.close();

            return lcs_bad(a, b, m - 1, n - 1) + a[m - 1];

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int cn = call_number;

            call_graph.open("call_graph_bad.gv", ios::app);
            call_graph  << "n" << call_number << " [color=red]" << endl;
            call_number++;
            call_graph  << "n" << call_number << " [label=\""
                        << "LCS(" << a.substr(0, m - 1) << ", " << b.substr(0, n) << ")\"]"
                        << endl

                        << "n" << cn
                        << " -> "
                        << "n" << call_number
                        << endl;
            call_graph.close();

            string sa = lcs_bad(a, b, m - 1, n);

            call_graph.open("call_graph_bad.gv", ios::app);
            call_number++;
            call_graph  << "n" << call_number << " [label=\""
                        << "LCS(" << a.substr(0, m) << ", " << b.substr(0, n - 1) << ")\"]"
                        << endl

                        << "n" << cn
                        << " -> "
                        << "n" << call_number
                        << endl;
            call_graph.close();

            string sb = lcs_bad(a, b, m, n - 1);

            return sa.length() >= sb.length() ? sa : sb;
        }
    }
}

string lcs_memo(const string& a, const string& b, size_t m, size_t n, string*** memo) {

    // Se já existe no memo, nem faz sentido calcular
    if (memo[m][n]) {
        call_graph.open("call_graph_memo.gv", ios::app);
        call_graph  << "n" << call_number << " [color=green]" << endl;
        call_graph.close();

        return *memo[m][n];
    }
    
    string result = "";
    
    // Se o tamanho é zero, a resposta é vazia
    if (m != 0 && n != 0) {

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            call_graph.open("call_graph_memo.gv", ios::app);
            call_number++;
            call_graph  << "n" << call_number << " [label=\""
                        << "LCS(" << a.substr(0, m - 1) << ", " << b.substr(0, n - 1) << ")\"]"
                        
                        << endl

                        << "n" << (call_number - 1)
                        << " -> "
                        << "n" << call_number
                        << endl;
            call_graph.close();
                        
            result = lcs_memo(a, b, m - 1, n - 1, memo) + a[m - 1];

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int cn = call_number;

            call_graph.open("call_graph_memo.gv", ios::app);
            call_graph  << "n" << call_number << " [color=red]" << endl;
            call_number++;
            call_graph  << "n" << call_number << " [label=\""
                        << "LCS(" << a.substr(0, m - 1) << ", " << b.substr(0, n) << ")\"]"
                        << endl

                        << "n" << cn
                        << " -> "
                        << "n" << call_number
                        << endl;
            call_graph.close();

            string sa = lcs_memo(a, b, m - 1, n, memo);

            call_graph.open("call_graph_memo.gv", ios::app);
            call_number++;
            call_graph  << "n" << call_number << " [label=\""
                        << "LCS(" << a.substr(0, m) << ", " << b.substr(0, n - 1) << ")\"]"
                        << endl

                        << "n" << cn
                        << " -> "
                        << "n" << call_number
                        << endl;
            call_graph.close();

            string sb = lcs_memo(a, b, m, n - 1, memo);

            result = sa.length() >= sb.length() ? sa : sb;
        }

    } else {
        call_graph.open("call_graph_memo.gv", ios::app);
        call_graph  << "n" << call_number << " [color=white]" << endl;
        call_graph.close();
    }

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