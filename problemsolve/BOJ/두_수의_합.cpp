#include <bits/stdc++.h>
using namespace std;

int n, x;
int arr[100001] = {};
bool occur[200001];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int ans = 0;
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cin >> x;

    for (int i = 0; i < n; i++) {
        if (x - arr[i] > 0 && occur[x-arr[i]]) ans++;
        occur[arr[i]] = true;
    }

    cout << ans;

    return 0;
}