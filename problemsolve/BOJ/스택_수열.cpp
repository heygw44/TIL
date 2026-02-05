#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;
    stack<int> st;

    string ans;
    int cnt = 1;

    while(n--) {
        int t;
        cin >> t;

        while(cnt <= t) {
            st.push(cnt++);
            ans += "+\n";
        }
        if (st.top() != t) {
            cout << "NO";
            return 0;
        }
        st.pop();
        ans += "-\n";
    }

    cout << ans;

}