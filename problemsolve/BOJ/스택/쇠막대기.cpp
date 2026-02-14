#include <bits/stdc++.h>
using namespace std;

stack<char> st;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    long long ans = 0;
    string s;
    cin >> s;

    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') st.push(s[i]);
        else {
            if (s[i-1] == '(') {
                st.pop();
                ans+= st.size();
            }
            else {
                st.pop();
                ans++;
            }
        }
    }

    cout << ans;

    return 0;
}