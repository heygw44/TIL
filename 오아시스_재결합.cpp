#include <bits/stdc++.h>
using namespace std;

#define x first
#define y second
#define ll long long

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int n;
    cin >> n;

    stack<pair<int,int>> st; //키, 볼 수 있는 쌍(묶음)

    ll ans = 0;

    while (n--) {
        int h;
        cin >> h;
        int cnt = 1;
        while(!st.empty() && st.top().x <= h) {
            ans += st.top().y;
            if (st.top().x == h) cnt += st.top().y;
            st.pop();
        }
        if (!st.empty()) ans++;
        st.push({h, cnt});
    }

    cout << ans;



    return 0;
}