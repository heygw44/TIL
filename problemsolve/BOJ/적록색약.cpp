#include <bits/stdc++.h>
using namespace std;

int n;
char board[102][102];
bool vis[102][102];
int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};

void bfs(int a, int b) {
    queue<pair<int,int>> q;
    q.push({a, b});
    vis[a][b] = 1;
    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        for (int dir = 0; dir < 4; dir++) {
            int nx = cur.first + dx[dir];
            int ny = cur.second + dy[dir];
            if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            if (vis[nx][ny] == 1 || board[a][b] != board[nx][ny]) continue;
            vis[nx][ny] = 1;
            q.push({nx,ny});
        }
    }
}

int area() {
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!vis[i][j]) {
                ans++;
                bfs(i,j);
            }
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    cin >> n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> board[i][j];
        }
    }

    int not_c = area(); //색약x

    for (int i = 0; i < n; i++) {
        fill(vis[i], vis[i]+n, false);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'R') {
                board[i][j] = 'G';
            }
        }
    }

    int is_c = area();

    cout << not_c << " " << is_c;

    return 0;
}