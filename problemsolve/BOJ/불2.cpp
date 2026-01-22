
#include <bits/stdc++.h>
using namespace std;

#define X first
#define Y second

int TC;
int w, h;                 
int board[1002][1002];    
int visF[1002][1002];     
int visS[1002][1002];     

int dx[4] = {0, 0, 1, -1}; 
int dy[4] = {1, -1, 0, 0}; 

int main(void) {
  ios::sync_with_stdio(0);
  cin.tie(0);

  cin >> TC;
  while (TC--) {
    bool escape = false;
    queue<pair<int,int>> Qf, Qs;

    cin >> w >> h; 

 
    for (int x = 0; x < h; x++) {
      fill(board[x], board[x] + w, 0);
      fill(visF[x],  visF[x]  + w, 0);
      fill(visS[x],  visS[x]  + w, 0);
    }


    for (int x = 0; x < h; x++) {
      for (int y = 0; y < w; y++) {
        char c;
        cin >> c;
        if (c == '#') {
          board[x][y] = -1;
        } else {
          board[x][y] = 0;
          if (c == '*') {
            Qf.push({x, y});
            visF[x][y] = 1; 
          } else if (c == '@') {
            Qs.push({x, y});
            visS[x][y] = 1; 
          }
        }
      }
    }


    while (!Qf.empty()) {
      auto v = Qf.front(); Qf.pop();
      for (int dir = 0; dir < 4; dir++) {
        int nx = v.X + dx[dir];
        int ny = v.Y + dy[dir];
        if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
        if (board[nx][ny] == -1) continue;
        if (visF[nx][ny] != 0) continue;
        visF[nx][ny] = visF[v.X][v.Y] + 1;
        Qf.push({nx, ny});
      }
    }

   
    while (!Qs.empty() && !escape) {
      auto v = Qs.front(); Qs.pop();
      for (int dir = 0; dir < 4; dir++) {
        int nx = v.X + dx[dir];
        int ny = v.Y + dy[dir];

   
        if (nx < 0 || nx >= h || ny < 0 || ny >= w) {
          cout << visS[v.X][v.Y] << '\n';
          escape = true;
          break;
        }

        if (board[nx][ny] == -1) continue;
        if (visS[nx][ny] != 0) continue;

        int nt = visS[v.X][v.Y] + 1;

        
        if (visF[nx][ny] != 0 && visF[nx][ny] <= nt) continue;

        visS[nx][ny] = nt;
        Qs.push({nx, ny});
      }
    }

    if (!escape) cout << "IMPOSSIBLE\n";
  }

  return 0;
}
