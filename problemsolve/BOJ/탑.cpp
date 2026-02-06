#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    stack<pair<int,int>> tower; // 높이, 위치
    int n;
    cin >> n;

    tower.push({100000001, 0});

    for (int i = 1; i <= n; i++) {
        int h;
        cin >> h;
        while(tower.top().first < h) {
            tower.pop();
        }
        cout << tower.top().second << " ";
        tower.push({h,i});
    }

    return 0;
}