#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    int c;
    cin >> c;

    int arr[1001] = {0};

    while (c--) {
        int n = 0, avg = 0;
        double ans = 0;

        cin >> n;

        for (int i = 0; i < n; i++) {
            cin >> arr[i];
            avg += arr[i];
        }
        avg /= n;

        for (int i = 0; i < n; i++) {
            if (arr[i] > avg) ans++;
        }

        cout << fixed;
        cout.precision(3);
        cout << ans/n * 100 << "%" << "\n";
    }

    return 0;
}