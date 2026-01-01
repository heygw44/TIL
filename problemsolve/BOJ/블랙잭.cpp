#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n, m;
    cin >> n >> m;

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int card = 0;
    int ans = 0;

    for (int i = 0; i < n-2; i++) {
        for (int j = i+1; j < n-1; j++) {
            for (int k = j+1; k < n; k++) {
                card = arr[i] + arr[j] + arr[k];
                if (card <= m && card > ans) ans = card;
            }
        }
    }

    cout << ans;

    return 0;
}