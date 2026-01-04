#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;

    stack<int> st;
    string ans;

    int cur = 1;

    while (n--) {
        int t;
        cin >> t;
        while (cur <= t) {
            st.push(cur++);
            ans += "+\n";
        }
        if (st.top() != t) {
            cout << "NO";
            return 0;
        } else {
            st.pop();
            ans += "-\n";
        }
    }

    cout << ans;
}