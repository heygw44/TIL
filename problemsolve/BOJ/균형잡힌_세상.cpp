#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    while(true) {
        string s;
        getline(cin, s);
        if (s == ".") break;

        stack<char> st;
        bool isValid = true;

        for (char c:s) {
            if (c == '(' || c == '[') st.push(c);
            else if (c == ')') { 
                if (st.empty() || st.top() != '(') {
                    isValid = false;
                    break;
                }
                st.pop();
            } else if (c == ']') {
                if (st.empty() || st.top() != '[') {
                    isValid = false;
                    break;
                }
                st.pop();
            }
        }
        if (!st.empty()) isValid = false;
        if (isValid == false) cout << "no\n";
        else cout << "yes\n";
    }

    return 0;
}