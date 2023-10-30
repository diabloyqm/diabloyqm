#include <vector>
#include <stdio.h>
#include <iostream>
#include <queue>

using namespace std;

const int MAX_COINS = 10;

int minMoves(vector<vector<int>> maze, int x, int y) {
    int n = maze.size();
    int m = maze[0].size();
    
    // Collect coins information
    vector<pair<int, int>> coins;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (maze[i][j] == 2) {
                coins.emplace_back(i, j);
            }
        }
    }
    
    int totalCoins = coins.size();
    int targetState = (1 << totalCoins) - 1;

    vector<vector<vector<bool>>> visited(n, vector<vector<bool>>(m, vector<bool>(1 << MAX_COINS)));
    queue<pair<pair<int, int>, pair<int, int>>> bfsQueue; // ((row, col), (moves, state))

    bfsQueue.push({{0, 0}, {0, 0}});

    array<int, 4> dr = {-1, 1, 0, 0};
    array<int, 4> dc = {0, 0, -1, 1};

    while (!bfsQueue.empty()) {
        auto [position, status] = bfsQueue.front();
        bfsQueue.pop();
        auto [r, c] = position;
        auto [moves, state] = status;

        if (r == x && c == y && state == targetState) {
            return moves;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            
            if (nr < 0 || nr >= n || nc < 0 || nc >= m || maze[nr][nc] == 1 || visited[nr][nc][state]) {
                continue;
            }

            int newState = state;
            if (maze[nr][nc] == 2) {
                for (int j = 0; j < totalCoins; ++j) {
                    if (coins[j] == make_pair(nr, nc)) {
                        newState |= (1 << j);
                        break;
                    }
                }
            }

            visited[nr][nc][newState] = true;
            bfsQueue.push({{nr, nc}, {moves + 1, newState}});
        }
    }

    return -1;
}
