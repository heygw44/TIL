#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        list<char> l = {};
        auto p = l.begin();

        string s;
        cin >> s;

        for (auto c:s) {
            if (c == '<') {
                if (p != l.begin()) p--;
            } else if (c == '>') {
                if (p != l.end()) p++;
            } else if (c == '-') {
                if (p != l.begin()) {
                    p--;
                    p = l.erase(p);
                }
            } else {
                l.insert(p, c);
            }
        }

        for (auto a:l) cout << a;
        cout << "\n";
    }

    return 0;
}