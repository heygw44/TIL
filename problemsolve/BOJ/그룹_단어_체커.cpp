#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;

    int ans = 0;

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        bool vis[26] = {0};
        bool isGroup = true;

        char prev = 0;

        for (int j = 0; j < s.length(); j++) {
            char cur = s[j];
            if (cur != prev) {
                if (vis[cur - 'a']) {
                    isGroup = false;
                    break;
                } else {
                    vis[cur - 'a'] = true;
                    prev = cur;
                }
            }
        }
        if (isGroup) ans++;
    }

    cout << ans;
    

    return 0;
}