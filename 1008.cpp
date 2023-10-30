#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;



bool detectCycle(char c, const vector<vector<bool>>& graph, vector<bool>& visited) {
    if (visited[c - 'A']) return true;
    
    visited[c - 'A'] = true;
    for (int i = 0; i < 26; i++) {
        if (graph[c - 'A'][i]) {
            if (detectCycle((char)('A' + i), graph, visited)) {
                return true;
            }
        }
    }
    return false;
}

string getSexpression(char root, const vector<vector<bool>>& graph) {
    string left = "";
    string right = "";
    for (int i = 0; i < 26; i++) {
        if (graph[root - 'A'][i]) {
            left = getSexpression((char)('A' + i), graph);
            for (int j = i + 1; j < 26; j++) {
                if (graph[root - 'A'][j]) {
                    right = getSexpression((char)('A' + j), graph);
                    break;
                }
            }
            break;
        }
    };
    string tempo{root};
    return "(" +tempo + left + right + ")";
}

string sExpression(string nodes) {
    vector<vector<bool>> graph(26, vector<bool>(26, false));
    set<char> unique_nodes;
    bool E2 = false;

    for (size_t i = 1; i < nodes.length(); i += 6) {
        int x = nodes[i] - 'A';
        int y = nodes[i + 2] - 'A';
        if (graph[x][y]) {
            E2 = true;
        }
        
            graph[x][y] = true;
        unique_nodes.insert(nodes[i]);
        unique_nodes.insert(nodes[i + 2]);
    }

    for (int i = 0; i < 26; i++) {
        int count = 0;
        for (int j = 0; j < 26; j++) {
            if (graph[i][j]) {
                count++;
            }
        }
        if (count > 2) {
            return "E1";
        }
    }
    if (E2) return "E2";

    int numOfRoots = 0;
    char root = ' ';
    set<char>::iterator c;
   
    // Displaying set elements
    for (c = unique_nodes.begin(); 
        c != unique_nodes.end(); c++) 
{
        bool isRoot = true;
        for (int i = 0; i < 26; i++) {
            if (graph[i][*c - 'A']) {
                isRoot = false;
                break;
            }
        }
        if (isRoot) {
            numOfRoots++;
            root = *c;
            vector<bool> visited(26, false);
            if (detectCycle(*c, graph, visited)) {
                return "E3";
            }
        }
    }
    if (numOfRoots == 0) return "E3";
    if (numOfRoots > 1) return "E4";

    return getSexpression(root, graph);
}

int main() {
    string nodes = "(B,D) (D,E) (A,B) (F,C) (E,G) (A,C)";
    cout << sExpression(nodes) << endl;
    return 0;
}