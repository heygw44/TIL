#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    string s;
    cin >> s;

    int arr[26] = {0};
    int mx = -1;
    char ans;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A'; //모든 문자 대문자로 통일
        }

        int idx = s[i] - 'A';
        arr[idx]++;

        if (arr[idx] > mx) {
            mx = arr[idx];
            ans = s[i];
        }
    }

    int cnt = 0;
    for (int i = 0; i < 26; i++) {
        if (arr[i] == mx ) cnt++;
    }

    if (cnt == 1) cout << ans; 
    else cout << '?';

    

    return 0;
}