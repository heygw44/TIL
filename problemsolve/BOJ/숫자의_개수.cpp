#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int a,b,c;
    cin >> a >> b >> c;
    int temp = a*b*c;

    int d[10] = {};

    while (temp > 0) {
        d[temp%10]++;
        temp/=10;
    }

    for (int i = 0; i < 10; i++) {
        cout << d[i] << "\n";
    }

    return 0;
}