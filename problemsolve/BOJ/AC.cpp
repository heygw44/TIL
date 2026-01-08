#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        string p;
        cin >> p;

        int n;
        cin >> n;

        string s;
        cin >> s; 

        deque<int> dq;

        if (n > 0) {
            string inside = s.substr(1, s.size() - 2);
            stringstream ss(inside);
            string token;
            while (getline(ss, token, ',')) {
                dq.push_back(stoi(token));
            }
        }

        bool reversed = false;
        bool error = false;

        for (char c : p) {
            if (c == 'R') {
                reversed = !reversed; 
            } else if (c == 'D') {
                if (dq.empty()) {     
                    error = true;
                    break;
                }
                if (!reversed) dq.pop_front();
                else dq.pop_back();
            }
        }

    
        if (error) {
            cout << "error\n";
            continue;
        }

        cout << '[';
        if (!dq.empty()) {
            if (!reversed) {
                for (int i = 0; i < dq.size(); i++) {
                    if (i) cout << ',';
                    cout << dq[i];
                }
            } else {
                for (int i = dq.size() - 1; i >= 0; i--) {
                    if (i != dq.size() - 1) cout << ',';
                    cout << dq[i];
                }
            }
        }
        cout << "]\n";
    }
    return 0;
}
