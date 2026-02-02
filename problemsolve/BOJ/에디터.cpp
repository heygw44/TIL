#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);
    
    string s;
    cin >> s;

    list<char> l;
    for (auto c : s) l.push_back(c);

    auto cursor = l.end();

    int m;
    cin >> m;

    while(m--) {
        char op;
        cin >> op;
        if (op == 'L') {
            if (cursor != l.begin()) cursor--;
        }
        else if (op == 'D') {
            if (cursor != l.end()) cursor++;
        }
        else if (op == 'B') {
            if (cursor != l.begin()) {
                cursor--;
                cursor = l.erase(cursor);
            }
        }
        else {
            char add;
            cin >> add;
            l.insert(cursor, add);
        }
    }

    for (auto c : l) cout << c;
    
    return 0;
}