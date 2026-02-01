#include <bits/stdc++.h>
using namespace std;

int arr[2][26] = {}, ans;
string s1, s2;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    cin >> s1 >> s2;
    for (auto c : s1) arr[0][c-'a']++;
    for (auto c : s2) arr[1][c-'a']++;

    for (int i = 0; i < 26; i++) {
        ans += abs(arr[0][i] - arr[1][i]);
    }

    cout << ans;

    return 0;
}