#include <bits/stdc++.h>
using namespace std;

int a[1000001]; 
int ans[1000001]; 

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    stack<int> st;

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    for (int i = n-1; i >= 0; i--) {
        while(!st.empty() && st.top() <= a[i]) st.pop();
        if (st.empty()) ans[i] = -1;
        else ans[i] = st.top();
        st.push(a[i]);
    }

    for (int i = 0; i < n; i++) {
        cout << ans[i] << " ";
    }

    return 0;
}