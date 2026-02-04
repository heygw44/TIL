#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    stack<int> st;
    int n;
    cin >> n;

    while (n--) {
        string op;
        cin >> op;

        if (op == "push") {
            int t;
            cin >> t;
            st.push(t);
        } 
        else if (op == "pop") {
            if (st.empty()) cout << -1 << "\n";
            else {
                cout << st.top() << "\n";
                st.pop();
            }
        } 
        else if (op == "size") {
            cout << st.size() << "\n";
        } 
        else if (op == "empty") {
            cout << (st.empty() ? 1 : 0) << "\n";
        } 
        else if (op == "top") {
            cout << (st.empty() ? -1 : st.top()) << "\n";
        }
    }

    return 0;
}
