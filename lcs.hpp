#ifndef __LCS
#define __LCS

#include <string>
#include <utility>
#include <map>

using namespace std;

#include <fstream>

static ofstream call_graph;

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
 * @param m Tamanho da primeira string
 * @param n Tamanho da segunda string
 * @param memo Tabela de memorização
 * 
 * @return string Maior subsequência comum entre as strings
 */
string lcs_memo(const string&, const string&, size_t, size_t);

#endif // __LCS