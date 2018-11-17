#include "lcs.hpp"

#include <iostream>

int main(int argc, char** argv) {
    string a, b;

    cout << "String A: ";
    cin >> a;

    cout << "String B: ";
    cin >> b;

    cout << "Calculating LCS using memoization...";

    ofstream call_graph("call_graph_memo.gv");
    call_graph << "strict digraph {" << endl;
    call_graph << "node [shape=rect style=filled];" << endl;
    call_graph.close();

    cout << lcs_memo(a, b, a.size(), b.size()) << endl;

    call_graph.open("call_graph_memo.gv", ios::app);
    call_graph << "}";
    call_graph.close();

    cout << "Calculating LCS without memoization...";
    
    call_graph.open("call_graph_bad.gv");
    call_graph << "strict digraph {" << endl;
    call_graph << "node [shape=rect style=filled];" << endl;
    call_graph.close();

    cout << lcs_bad(a, b, a.size(), b.size()) << endl;

    call_graph.open("call_graph_bad.gv", ios::app);
    call_graph << "}";
    call_graph.close();

    return 0;
}