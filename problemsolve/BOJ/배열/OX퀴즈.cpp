#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int t;
    cin >> t;
    string s;


    for (int i = 0; i < t; i++){
        cin >> s;
        int cnt = 0, ans = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == 'O') cnt++;
            else cnt = 0;
            ans+=cnt;
        }
        cout << ans << "\n";
    }

    return 0;
}