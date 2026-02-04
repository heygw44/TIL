#include <bits/stdc++.h>
using namespace std;

int k, ans = 0;
stack<int> st;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    cin >> k;

    for (int i = 0; i < k; i++) {
        int n;
        cin >> n;
        if (n == 0) st.pop();
        else st.push(n);
    }

    while(!st.empty()) {
        ans += st.top();
        st.pop();
    }

    cout << ans;

    return 0;
}