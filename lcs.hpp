#ifndef __LCS
#define __LCS

#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <map>

using namespace std;


/**
 * @brief Obtém a maior subsequência comum entre duas strings
 * 
 * @param a Uma string
 * @param b Outra string
 * @param m Tamanho da primeira string
 * @param n Tamanho da segunda string
 * 
 * @return string Maior subsequência comum entre as strings
 */
string lcs_bad(const string&, const string&, size_t, size_t);

/**
 * @brief Obtém a maior subsequência comum entre duas strings usando memoização
 * 
 * @param a Uma string
 * @param b Outra string
 * 
 * @return string Maior subsequência comum entre as strings
 */
string lcs_memo(const string&, const string&);

/**
 * @brief Lista de alterações feitas na string
 */
struct diff_node {
    char operation;
    string value;
    diff_node* next;
};

/**
 * @brief Obtém as diferenças entre duas strings
 * 
 * @return string Diferenças entre as strings
 */
diff_node* diff(const string&, const string&);

#endif // __LCS