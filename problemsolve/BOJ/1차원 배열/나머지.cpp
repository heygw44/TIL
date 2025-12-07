#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    int arr[42] = {0,};

    for (int i = 0; i < 10; i++) {
        cin >> n;
        arr[n%42]++;
    }

    int ans = 0;

    for (int a : arr) {
        if (a > 0) {
            ans++;
        }
    }

    cout << ans;

    return 0;
}