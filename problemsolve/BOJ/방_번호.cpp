#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n, ans = 0;
    cin >> n;

    int num[10] = {};

    while(n>0) {
        num[n%10]++;
        n/=10;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 6 || i == 9) continue;
        ans = max(ans, num[i]);
    }

    ans = max(ans, (num[6] + num[9] + 1) / 2);

    cout << ans;

    return 0;
}