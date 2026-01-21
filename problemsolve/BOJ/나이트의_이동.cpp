#include <bits/stdc++.h>
using namespace std;

int t, n;
int x, y, xx, yy;
int dist[302][302];

int dx[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[8] = {-1,  1, -2,  2,-2, 2,-1, 1};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> t;
    while (t--) {
        cin >> n;

        for (int i = 0; i < n; i++) {
            fill(dist[i], dist[i] + n, -1);
        }

        cin >> x >> y;
        cin >> xx >> yy;

        queue<pair<int,int>> q;
        dist[x][y] = 0;
        q.push({x, y});

        while (!q.empty()) {
            auto [cx, cy] = q.front(); q.pop();

            if (cx == xx && cy == yy) break;

            for (int dir = 0; dir < 8; dir++) {
                int nx = cx + dx[dir];
                int ny = cy + dy[dir];

                if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
                if (dist[nx][ny] != -1) continue;

                dist[nx][ny] = dist[cx][cy] + 1;
                q.push({nx, ny});
            }
        }

        cout << dist[xx][yy] << "\n";
    }

    return 0;
}
