#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    while(true) {
        string s;
        getline(cin, s);

        bool isValid = true;
        stack<char> st;

        if (s == ".") break;
        
        for (auto c : s) {
            if (c == '[' || c == '(') st.push(c);
            else if (c == ')') {
                if (st.empty() || st.top() != '(') {
                    isValid = false;
                    break;
                }
                st.pop();
            }
            else if (c == ']') {
                if (st.empty() || st.top() != '[') {
                    isValid = false;
                    break;
                }
                st.pop();
            }
        }
        if (!st.empty()) isValid = false;
        if (isValid) cout << "yes\n";
        else cout << "no\n"; 
    }
}