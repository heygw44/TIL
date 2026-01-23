
#include <bits/stdc++.h>
using namespace std;

#define X first
#define Y second

int M, N, K;
int board[102][102]; // 0: empty, 1: filled
bool vis[102][102];

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

int bfs(int sy, int sx) {
    queue<pair<int,int>> q;
    q.push({sy, sx});
    vis[sy][sx] = true;
    int area = 1;

    while(!q.empty()) {
        auto [y, x] = q.front(); q.pop();
        for(int dir = 0; dir < 4; dir++) {
            int ny = y + dy[dir];
            int nx = x + dx[dir];
            if(ny < 0 || ny >= M || nx < 0 || nx >= N) continue;
            if(vis[ny][nx]) continue;
            if(board[ny][nx] == 1) continue; // filled
            vis[ny][nx] = true;
            q.push({ny, nx});
            area++;
        }
    }
    return area;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> M >> N >> K;

    // 직사각형 채우기
    for(int i = 0; i < K; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        // y: 행(0~M-1), x: 열(0~N-1)
        for(int y = y1; y < y2; y++) {
            for(int x = x1; x < x2; x++) {
                board[y][x] = 1;
            }
        }
    }

    vector<int> areas;
    for(int y = 0; y < M; y++) {
        for(int x = 0; x < N; x++) {
            if(board[y][x] == 0 && !vis[y][x]) {
                areas.push_back(bfs(y, x));
            }
        }
    }

    sort(areas.begin(), areas.end());

    cout << areas.size() << "\n";
    for(int i = 0; i < (int)areas.size(); i++) {
        cout << areas[i] << (i + 1 == (int)areas.size() ? '\n' : ' ');
    }
    return 0;
}

