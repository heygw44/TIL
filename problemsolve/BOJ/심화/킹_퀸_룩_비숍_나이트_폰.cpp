#include <bits/stdc++.h>
using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int arr[6];
    int count[6] = {1, 1, 2, 2, 2, 8};

    for (int i = 0; i < 6; i++) {
        cin >> arr[i];
        cout << count[i] - arr[i] << " ";
    }

    return 0;
}