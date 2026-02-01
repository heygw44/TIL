#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;

    while (n--) {
        int arr[26] = {};
        string s1, s2;
        cin >> s1 >> s2;

        for (auto c : s1) arr[c-'a']++;
        for (auto c : s2) arr[c-'a']--;

        bool isPossible = true;

        for (int i : arr) {
            if (i != 0) isPossible = false;
        }

        if (isPossible) cout << "Possible" << "\n";
        else cout << "Impossible" << "\n";
    }

    return 0;
}