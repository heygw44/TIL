#include <bits/stdc++.h>
using namespace std;


int m, n, h;                  // m: cols(열), n: rows(행), h: height(층)
int board[102][102][102];     // board[z][x][y]

int dz[6] = {1, -1, 0, 0, 0, 0};
int dx[6] = {0, 0, 1, -1, 0, 0};
int dy[6] = {0, 0, 0, 0, 1, -1};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> m >> n >> h;

  queue<tuple<int,int,int>> q; // (z, x, y)

  for (int z = 0; z < h; z++) {
    for (int x = 0; x < n; x++) {
      for (int y = 0; y < m; y++) {
        cin >> board[z][x][y];
        if (board[z][x][y] == 1) q.push({z, x, y});
      }
    }
  }

  while (!q.empty()) {
    auto [z, x, y] = q.front();
    q.pop();

    for (int dir = 0; dir < 6; dir++) {
      int nz = z + dz[dir];
      int nx = x + dx[dir];
      int ny = y + dy[dir];

      if (nz < 0 || nz >= h || nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
      if (board[nz][nx][ny] != 0) continue; 

      board[nz][nx][ny] = board[z][x][y] + 1;
      q.push({nz, nx, ny});
    }
  }

  int mx = 1;
  for (int z = 0; z < h; z++) {
    for (int x = 0; x < n; x++) {
      for (int y = 0; y < m; y++) {
        if (board[z][x][y] == 0) {
          cout << -1 << "\n";
          return 0;
        }
        mx = max(mx, board[z][x][y]);
      }
    }
  }

  cout << mx - 1 << "\n";
  return 0;
}
