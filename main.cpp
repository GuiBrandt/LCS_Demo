#include "lcs.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char** argv) {
    string a, b;

    cout << "String A: ";
    cin >> a;

    cout << "String B: ";
    cin >> b;

    cout << "Diff: ";

    #ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi );
    #endif

    char _last_op = 0;
    for (diff_node* _diff = diff(a, b); _diff != nullptr; _diff = _diff->next) {
        switch (_diff->operation) {
            case '-':
                #ifdef _WIN32
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
                #else
                cout << "\x1B[30;41m" << endl;
                #endif
                break;
            case '+':
                #ifdef _WIN32
                SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
                #else
                cout << "\x1B[30;42m" << endl;
                #endif
                break;
            default:
                #ifdef _WIN32
                SetConsoleTextAttribute(hConsole, csbi.wAttributes);
                #else
                cout << "\x1B[0m" << endl;
                #endif
                break;
        }

        cout << _diff->value;
    }
    
    #ifdef _WIN32
    SetConsoleTextAttribute(hConsole, csbi.wAttributes);
    #else
    cout << "\x1B[0m" << endl;
    #endif

    cout << endl;
    cout << endl;

    cout << "Calculating LCS using memoization...";

    ofstream call_graph("call_graph_memo.gv");
    call_graph << "strict digraph {" << endl;
    call_graph << "node [shape=rect style=filled];" << endl;
    call_graph.close();

    cout << lcs_memo(a, b) << endl;

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