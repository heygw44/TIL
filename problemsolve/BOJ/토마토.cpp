#include <bits/stdc++.h>
using namespace std;

int board[1002][1002];
int dist[1002][1002];
int n, m;
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    queue<pair<int,int>> q;

    cin >> m >> n; 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> board[i][j];
            if (board[i][j] == 1) q.push({i,j});
            if (board[i][j] == 0) dist[i][j] = -1;
        }
    }

    while(!q.empty()) {
        auto cur = q.front(); q.pop();
        for (int dir = 0; dir < 4; dir++) { 
            int nx = cur.first + dx[dir];
            int ny = cur.second + dy[dir];

            if (nx >= n || nx < 0 || ny >= m || ny < 0) continue;
            if (dist[nx][ny] >= 0) continue; 
            dist[nx][ny] = dist[cur.first][cur.second] + 1;
            q.push({nx, ny});
        }
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (dist[i][j] == -1) { 
                cout << -1;
                return 0;
            }
            ans = max(ans, dist[i][j]);
        }
    }
    cout << ans;
}
