#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        list<char> l = {};
        string s;

        auto p = l.begin();
        cin >> s;

        for (auto c : s) {
            if (c == '-') {
                if (p != l.begin()) {
                    p--;
                    p = l.erase(p);
                }
            }
            else if (c == '<') {
                if (p != l.begin()) p--;
            }
            else if (c == '>') {
                if (p != l.end()) p++;
            }
            else {
                l.insert(p, c);
            }
        }
        for (auto c : l) cout << c;
        cout << "\n";
    }

    return 0;
}