#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n, v;
    cin >> n;

    int arr[201] = {};

    for (int i = 0; i < n; i++) {
        int t;
        cin >> t;
        arr[t+100]++;
    }

    cin >> v;
    cout << arr[v+100];

    return 0;
}