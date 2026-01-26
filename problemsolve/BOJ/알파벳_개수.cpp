#include <bits/stdc++.h>
using namespace std;

int alpa[26];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    string s;
    cin >> s;
    
    for (auto c : s) {
        alpa[c-'a']++;
    }
    for (int i = 0; i < 26; i++) {
        cout << alpa[i] << " "; 
    }

    return 0;
}