#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int arr[2][7] = {0}; 
    int n, k, s, y, ans = 0;

    cin >> n >> k;

    for (int i = 0; i < n; i++) {
        cin >> s >> y;
        arr[s][y]++;
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 1; j <= 6; j++) {
            ans += arr[i][j] / k;
            if (arr[i][j] % k) ans++;
        }
    }

    cout << ans;
    return 0;
}
