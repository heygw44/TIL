#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;
    int ans = 0;
    while (n--) {
        string s;
        cin >> s;
        stack<char> st;


        for (auto c:s) {
            if(!st.empty() && st.top() == c) st.pop();
            else st.push(c);
        }
        if (st.empty()) ans++;
    }

    cout << ans;

    return 0;
}