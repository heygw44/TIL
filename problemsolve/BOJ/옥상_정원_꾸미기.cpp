#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    long long h, ans = 0;
    stack<int> st;

    cin >> n;

    while (n--) {
        cin >> h;
        while(!st.empty() && st.top() <= h) {
            st.pop();
        }
        ans += st.size();
        st.push(h);
    }

    cout << ans;


    return 0;
}