# Set, Map 그리고 Unordered_ 

## 왜 Set과 Map이 필요할까?

배열은 인덱스(0, 1, 2, ...)로 값을 빠르게 찾을 수 있다. 하지만 배열만으로는 풀기 어려운 상황들이 있다.

- "이 숫자가 지금까지 나왔던 적이 있나?" 를 빠르게 확인하고 싶을 때
- "학생 이름을 key로, 점수를 value로" 처럼 정수가 아닌 것을 인덱스 삼아 데이터를 찾고 싶을 때
- 데이터의 범위가 너무 커서 배열로 만들 수 없을 때 (예: 입력값이 $10^{18}$ 까지)

이럴 때 쓰는 자료구조가 `set`과 `map`이다. 한마디로 정리하면 **배열의 인덱스를 정수가 아닌 일반적인 값으로 확장한 자료구조**다.

---

## Set의 정의와 성질
- 정의: 중복을 허용하지 않는 원소들의 집합. 항상 정렬된 상태가 유지된다.
- 헤더: `<set>`
- 내부 구조: Red-Black Tree (균형 이진 탐색 트리)
- 핵심 성질
  - 모든 원소가 유일하다 (중복 불가).
  - 항상 오름차순으로 정렬되어 있다.
  - 삽입, 삭제, 검색이 모두 $O(\log N)$이다.

### 시간 복잡도
- 삽입(`insert`): $O(\log N)$
- 삭제(`erase`): $O(\log N)$
- 검색(`find`, `count`): $O(\log N)$
- 크기 확인(`size`), 비었는지 확인(`empty`): $O(1)$

### 기본 사용법
```cpp
#include <set>
using namespace std;

set<int> s;
s.insert(3);
s.insert(1);
s.insert(4);
s.insert(1); // 중복은 무시됨, 아무 일도 안 일어남

s.size();    // 3
s.count(3);  // 있으면 1, 없으면 0
s.find(3);   // iterator 반환, 없으면 s.end()
s.erase(3);  // 원소 3 삭제

// 순회하면 자동으로 오름차순 (1, 4)
for(int x : s) cout << x << ' ';
```

### lower_bound, upper_bound
정렬되어 있기 때문에 이분 탐색이 가능하다.
- `s.lower_bound(x)`: x **이상**인 원소 중 가장 작은 것의 iterator
- `s.upper_bound(x)`: x **초과**인 원소 중 가장 작은 것의 iterator
- 둘 다 $O(\log N)$

```cpp
set<int> s = {1, 3, 5, 7, 9};
auto it1 = s.lower_bound(4); // 5를 가리킴
auto it2 = s.upper_bound(5); // 7을 가리킴
```

> 주의: `set`에서는 반드시 멤버 함수 형태인 `s.lower_bound(x)`를 써야 한다. `<algorithm>` 헤더의 `lower_bound(s.begin(), s.end(), x)`를 쓰면 $O(N)$이 되어버린다. set의 iterator가 random access가 아니라 bidirectional이기 때문이다. 이거 모르고 쓰다가 시간초과 나는 경우가 정말 많다.

---

## Map의 정의와 성질
- 정의: `key`와 `value` 쌍을 저장하는 자료구조. key는 중복되지 않는다.
- 헤더: `<map>`
- 내부 구조: Red-Black Tree
- 핵심 성질
  - key를 기준으로 자동 정렬된다.
  - key는 유일하고, value는 중복돼도 상관없다.
  - 삽입, 삭제, 검색 모두 $O(\log N)$.

배열의 일반화 버전이라고 생각하면 된다. 배열은 인덱스로 음이 아닌 정수만 쓸 수 있지만, map은 어떤 비교 가능한 타입이든 쓸 수 있다.

### 기본 사용법
```cpp
#include <map>
using namespace std;

map<string, int> score;
score["Alice"] = 90;
score["Bob"] = 85;
score.insert({"Charlie", 70});

cout << score["Alice"]; // 90

// 존재 여부 확인
if(score.count("Alice")) { ... }
if(score.find("Bob") != score.end()) { ... }

// 순회하면 key 오름차순
for(auto& [k, v] : score) {
    cout << k << ": " << v << '\n';
}
```

### `[]` 연산자의 함정
`map[key]`로 접근할 때, 해당 key가 존재하지 않으면 **자동으로 기본값을 가진 원소가 새로 만들어진다**. (int면 0, string이면 "")

```cpp
map<string, int> m;
if(m["없는키"] == 0) { ... }  // 이 줄 실행하는 순간 "없는키"가 추가됨
cout << m.size();              // 1 (의도와 다름!)
```

값을 읽기만 하려고 했을 뿐인데 원소가 늘어나는 부작용이 있다. **존재 여부만 확인하고 싶다면 반드시 `count`나 `find`를 쓰자.**

---

## Unordered_set과 Unordered_map
- 헤더: `<unordered_set>`, `<unordered_map>`
- 내부 구조: 해시 테이블 (Hash Table)
- set/map과의 핵심 차이
  - **정렬되지 않는다** (순회 순서가 무작위처럼 보임).
  - **평균적으로 $O(1)$**, 하지만 **최악의 경우 $O(N)$**.
  - `lower_bound` / `upper_bound`가 없다.

사용법은 set/map과 거의 똑같다.

```cpp
#include <unordered_set>
#include <unordered_map>

unordered_set<int> us;
unordered_map<string, int> um;

us.insert(5);
um["hello"] = 3;
```

### Unordered_의 가장 큰 함정 - 해시 충돌 공격
C++ 표준의 `unordered_map`은 기본 해시 함수가 단순해서, 일부러 해시값이 같아지는 값들을 입력으로 넣으면 모든 원소가 같은 버킷에 들어가 $O(N)$짜리 연산이 된다.

백준(BOJ)이나 Codeforces 같은 사이트에서는 이걸 노린 데이터를 만들어 둔 문제들이 있어서, 평소에 잘 돌던 코드가 갑자기 시간 초과가 나는 일이 자주 생긴다. **`unordered_map`을 썼는데 시간 초과가 나면 90%는 이게 원인이다.**

해결법: 커스텀 해시 함수를 쓴다. 아래는 자주 쓰이는 템플릿.

```cpp
#include <chrono>
struct custom_hash {
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = 
            chrono::steady_clock::now().time_since_epoch().count();
        x ^= FIXED_RANDOM;
        x ^= x >> 33;
        x *= 0xff51afd7ed558ccdULL;
        x ^= x >> 33;
        x *= 0xc4ceb9fe1a85ec53ULL;
        x ^= x >> 33;
        return x;
    }
};

unordered_map<int, int, custom_hash> safe_map;
```

---

## Multiset과 Multimap
- **중복을 허용하는 버전**이다.
- `multiset<int>`에는 같은 숫자를 여러 번 넣을 수 있다.
- `count(x)`는 x가 몇 개 들어 있는지 정확히 반환한다.
- `erase(x)`를 값으로 호출하면 **그 값을 가진 원소가 전부 다 지워진다**. 하나만 지우려면 반드시 iterator를 넘겨야 한다.

```cpp
multiset<int> ms;
ms.insert(3);
ms.insert(3);
ms.insert(3);

ms.erase(3);            // 3이 3개 다 사라짐 (의도하지 않은 결과일 수 있음)
ms.erase(ms.find(3));   // 3을 하나만 지움 (이게 보통 원하는 동작)
```

이거 모르고 그냥 `erase`했다가 데이터가 다 날아가서 디버깅 못 하는 경우가 정말 많다. 꼭 기억하자.

---

## 시간 복잡도 비교 정리

| 연산 | set / map | unordered_set / unordered_map |
|------|-----------|-------------------------------|
| 삽입 | $O(\log N)$ | 평균 $O(1)$, 최악 $O(N)$ |
| 삭제 | $O(\log N)$ | 평균 $O(1)$, 최악 $O(N)$ |
| 검색 | $O(\log N)$ | 평균 $O(1)$, 최악 $O(N)$ |
| 정렬된 순회 | 가능 ($O(N)$) | 불가능 |
| `lower_bound` | 가능 ($O(\log N)$) | 없음 |

---

## 언제 무엇을 쓸까? (PS 관점)

| 상황 | 추천 자료구조 |
|------|---------------|
| 값의 범위가 작은 정수 ($\le 10^7$ 정도) | 그냥 배열이 제일 빠름 |
| 큰 정수, 문자열의 **등장 여부**만 체크 | `unordered_set` |
| **정렬된 순서**로 순회 / 최솟값 / `lower_bound` 필요 | `set` |
| key-value 매핑, 정렬은 필요 없음 | `unordered_map` |
| key-value인데 정렬이나 `lower_bound`가 필요 | `map` |
| 같은 값을 여러 개 저장 + 정렬 필요 | `multiset` |

핵심 판단 기준 세 가지만 외우면 된다.
1. 정렬이 필요하면 → `set` / `map` (트리 기반, $O(\log N)$)
2. 정렬이 필요 없으면 → `unordered_set` / `unordered_map` (해시 기반, 평균 $O(1)$)
3. 인덱스로 표현 가능한 작은 범위라면 → 그냥 배열 (가장 빠르고 캐시 친화적)

---

## PS에서 자주 쓰는 패턴

### 패턴 1: 등장 여부 체크 (합이 K인 쌍 찾기)
```cpp
unordered_set<int> seen;
for(int x : arr) {
    if(seen.count(K - x)) {
        // 합이 K인 쌍을 찾음!
    }
    seen.insert(x);
}
```

### 패턴 2: 빈도수 세기
```cpp
unordered_map<string, int> cnt;
for(string& w : words) cnt[w]++;
```

### 패턴 3: 가장 가까운 값 찾기 (이진 탐색)
```cpp
set<int> s;
// ... 값들을 넣고 ...
auto it = s.lower_bound(target); 
// target 이상인 원소 중 최솟값
// it이 s.begin()이 아니면 prev(it)이 target 미만 중 최댓값
```

### 패턴 4: 슬라이딩 윈도우의 최댓값 / 최솟값
`multiset`을 쓰면 윈도우에 들어오고 나가는 값을 관리하면서 매번 최댓값/최솟값을 $O(\log N)$에 구할 수 있다.

```cpp
multiset<int> window;
for(int i=0; i<n; i++) {
    window.insert(arr[i]);
    if(i >= k) window.erase(window.find(arr[i-k])); // 반드시 find 사용!
    if(i >= k-1) {
        cout << *window.begin() << ' ';   // 최솟값
        cout << *window.rbegin() << '\n'; // 최댓값
    }
}
```

### 패턴 5: 좌표 압축
값의 범위는 매우 큰데($10^9$) 서로 다른 값의 개수는 적을 때, 큰 값을 작은 인덱스로 변환하기 위해 `map`을 쓴다.

```cpp
vector<int> v = {100, 1000000000, 5, 100};
map<int, int> compress;
for(int x : v) compress[x] = 0;

int idx = 0;
for(auto& [k, v] : compress) v = idx++;
// 이제 compress[100] = 1, compress[5] = 0, compress[1000000000] = 2
```

---

## 자주 빠지는 함정 모음

1. **`map[key]`로 존재 확인 금지.** 없는 키를 만들어버린다. `count`나 `find` 써라.
2. **`<algorithm>`의 `lower_bound`를 set/map에 쓰지 말 것.** $O(N)$이 된다. 멤버 함수 `s.lower_bound()`를 써라.
3. **`multiset::erase(value)`는 다 지운다.** 하나만 지우려면 `erase(find(x))`.
4. **`unordered_map`이 시간 초과 나면 커스텀 해시 함수 써라.** 기본 해시는 공격당할 수 있다.
5. **사용자 정의 구조체를 key로 쓸 때**
   - `map`이면 `<` 연산자(또는 비교 함수)가 필요하다.
   - `unordered_map`이면 hash 함수와 `==` 연산자가 필요하다. 그래서 보통 `map`이 더 쓰기 쉽다.
6. **작은 범위 정수면 그냥 배열이 항상 빠르다.** `unordered_map<int, int>`이 평균 $O(1)$이라고 해도 상수가 크고 캐시 적중률이 낮아서, 배열보다 수십 배 느릴 수 있다.

---

## 마지막 한 줄 요약

> **정렬이 필요하면 `set`/`map` ($O(\log N)$), 필요 없으면 `unordered_set`/`unordered_map` (평균 $O(1)$), 작은 정수 범위면 그냥 배열.**
>
> 그리고 `unordered_map`은 시간 초과 나면 커스텀 해시. 항상.
