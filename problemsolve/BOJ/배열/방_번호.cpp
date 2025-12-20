#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int arr[10] = {0};
    int n, ans = 0;

    cin >> n;

    while (n) {
        arr[n%10]++;
        n/=10;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 6 || i == 9) continue;
        ans = max(ans, arr[i]);
    }
    ans = max(ans, (arr[6]+arr[9]+1)/2);

    cout << ans;

    return 0;
}