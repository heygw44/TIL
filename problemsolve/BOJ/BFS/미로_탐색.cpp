#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<string> board(n);
    for (int i = 0; i < n; i++) cin >> board[i];

    vector<vector<int>> dist(n, vector<int>(m,0));
    queue<pair<int,int>> q;

    dist[0][0] = 1;
    q.push({0,0});

    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};

    while(!q.empty()) {
        auto [x,y] = q.front();
        q.pop();

        for (int dir = 0; dir < 4; dir ++) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            if (board[nx][ny] == '0' || dist[nx][ny] != 0) continue;

            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx,ny});
        }
    }
    cout << dist[n-1][m-1];
    return 0;
}