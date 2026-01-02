#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    stack<int> st;
    int n;
    cin >> n;

    while (n--) {
        string op;
        cin >> op;

        if (op == "push") {
            int x;
            cin >> x;
            st.push(x);
        } else if (op == "pop") {
            if (st.empty()) {
                cout << -1 << "\n";
            } else {
                cout << st.top() << "\n";
                st.pop();
            }
        } else if (op == "size") {
            cout << st.size() << "\n";
        } else if (op == "empty") {
            if (st.empty()) {
                cout << 1 << "\n";
            } else {
                cout << 0 << "\n";
            }
        }else if (op == "top") {
            if (st.empty()) {
                cout << -1 << "\n";
            } else {
                cout << st.top() << "\n";
            }
        }
    }

    return 0;
}