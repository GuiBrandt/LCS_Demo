#include "lcs.hpp"

#include <algorithm>
using namespace std;

// LCS ruinzão, sem memo
string lcs_bad(const string& a, const string& b, size_t m, size_t n) {
    // Se o tamanho é zero, a resposta é vazia
    if (m == 0 || n == 0) {
        return "";

    } else {

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {
            return lcs_bad(a, b, m - 1, n - 1) + a[m - 1];

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            string  sa = lcs_bad(a, b, m - 1, n),
                    sb = lcs_bad(a, b, m, n - 1);

            return sa.length() >= sb.length() ? sa : sb;
        }
    }
}

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

// LCS bonzão, com memo
int lcs_memo(const string& a, const string& b, size_t m, size_t n, int** memo) {
    int result = 0;

    // Se o tamanho é zero, a resposta é vazia
    if (m == 0 || n == 0) {
        return 0;

    } else {
        // Se já existe no memo, nem faz sentido calcular
        if (memo[m][n] != -1) {
            return memo[m][n];
        }

        // Se o último caractere das duas strings é igual, tira ele e põe no LCS
        if (a[m - 1] == b[n - 1]) {                        
            result = lcs_memo(a, b, m - 1, n - 1, memo) + 1;

        // Se não, pega o maior LCS: 
        //      - da string A sem o último caractere e B
        //      - ou da string B sem o último caractere e A
        } else {
            int lcs_a = lcs_memo(a, b, m - 1, n, memo),
                lcs_b = lcs_memo(a, b, m, n - 1, memo);

            result = max(lcs_a, lcs_b);
        }
    }
    
    memo[m][n] = result;

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
    int m = a.length(), n = b.length();

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