# Knuth-Morris-Pratt (KMP) — Giải thích từ gốc rễ

---

## 1. Bài toán nó giải quyết

**Pattern matching**: Cho text `T` độ dài `n` và pattern `P` độ dài `m`, tìm tất cả vị trí mà `P` xuất hiện trong `T`.

Naive approach: với mỗi vị trí `i` trong `T`, thử khớp `P` từ đầu → O(n·m). Worst case kinh điển:

```
T = "AAAAAAAAAB"  (n=10)
P = "AAAAB"       (m=5)
```

Naive sẽ thử 6 vị trí × 5 ký tự = 30 so sánh. Với n=10⁶, m=10³ → 10⁹ phép tính. **Chết.**

**Pain point cốt lõi**: Khi mismatch xảy ra ở vị trí `j` trong pattern, naive bỏ đi toàn bộ thông tin về các ký tự đã so sánh thành công trước đó và reset về đầu. KMP khai thác thông tin đó.

---

## 2. Trực giác cốt lõi

**Câu hỏi then chốt**: Khi đang khớp và gặp mismatch tại `P[j]`, ta có thể nhảy đến đâu trong pattern mà không cần quay lui `i` trong text?

**Ví dụ nhỏ — tự vẽ:**

```
P = "ABCABD"
       0123456
```

Giả sử đã khớp được `"ABCAB"` rồi gặp mismatch tại `D`:

```
T: ...A B C A B X...
P:    A B C A B D
                ^mismatch tại j=5
```

Naive: dịch P đi 1, so sánh lại từ `j=0`.

KMP nhận ra: trong phần đã khớp `"ABCAB"`, **prefix `"AB"` đồng thời là suffix `"AB"`**. Nghĩa là ta đã biết chắc text đang chứa `"AB"` ngay trước vị trí mismatch. Vậy ta có thể đặt pattern sao cho prefix `"AB"` của P trùng với suffix `"AB"` vừa khớp — tức nhảy về `j=2` mà không cần lùi `i`.

```
T: ...A B C A B X...
P:          A B C A B D
                ^ tiếp tục so sánh từ j=2
```

**Đây chính là failure function (hay prefix function)**: với mỗi vị trí `j` trong pattern, tính độ dài của **longest proper prefix của `P[0..j]` đồng thời là suffix**.

```
P:    A  B  C  A  B  D
j:    0  1  2  3  4  5
lps:  0  0  0  1  2  0
```

- `j=3` (`"ABCA"`): prefix `"A"` = suffix `"A"` → lps=1
- `j=4` (`"ABCAB"`): prefix `"AB"` = suffix `"AB"` → lps=2
- `j=5` (`"ABCABD"`): không có → lps=0

Khi mismatch tại `j`, nhảy về `j = lps[j-1]`. Không lùi `i`.

---

## 3. Cơ chế hoạt động từng bước

**Input:**
```
T = "AABCAABXAABCAAB"
P = "AABCAAB"
```

**Bước 3.1 — Xây lps table:**

```
P:    A  A  B  C  A  A  B
j:    0  1  2  3  4  5  6
lps:  0  1  0  0  1  2  3
```

Tại sao `j=5` (`"AABCAA"`) → lps=2?  
Prefix `"AA"` = suffix `"AA"` → đúng.

Tại sao `j=6` (`"AABCAAB"`) → lps=3?  
Prefix `"AAB"` = suffix `"AAB"` → đúng.

**Bước 3.2 — Matching:**

```
i=0, j=0: T[0]='A' == P[0]='A' → i=1, j=1
i=1, j=1: T[1]='A' == P[1]='A' → i=2, j=2
i=2, j=2: T[2]='B' == P[2]='B' → i=3, j=3
i=3, j=3: T[3]='C' == P[3]='C' → i=4, j=4
i=4, j=4: T[4]='A' == P[4]='A' → i=5, j=5
i=5, j=5: T[5]='A' == P[5]='A' → i=6, j=6
i=6, j=6: T[6]='B' == P[6]='B' → j==m, FOUND tại i-m = 6-7 = ... i=7, j=lps[6-1]=lps[5]=2

i=7, j=2: T[7]='X' != P[2]='B' → mismatch
  j = lps[1] = 1
i=7, j=1: T[7]='X' != P[1]='A' → mismatch
  j = lps[0] = 0
i=7, j=0: T[7]='X' != P[0]='A' → mismatch, j==0 nên i++
i=8, j=0: T[8]='A' == P[0]='A' → i=9, j=1
... (tiếp tục đến khi tìm ra match thứ 2 tại vị trí 8)
```

**Key insight**: `i` **không bao giờ giảm**. Đây là lý do O(n+m).

---

## 4. Cài đặt C++20

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <span>

// Xây dựng failure function (lps array)
// lps[i] = độ dài longest proper prefix của P[0..i]
//           đồng thời là suffix của P[0..i]
auto build_lps(std::string_view pattern) -> std::vector<int> {
    const int m = static_cast<int>(pattern.size());
    std::vector<int> lps(m, 0);

    // len: độ dài prefix-suffix đang xét
    // i  : vị trí đang tính lps
    int len = 0;
    int i   = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            // Extend prefix-suffix hiện tại
            ++len;
            lps[i] = len;
            ++i;
        } else {
            if (len != 0) {
                // Không extend được → fall back qua lps
                // KHÔNG tăng i ở đây — quan trọng!
                len = lps[len - 1];
            } else {
                // len == 0, không thể fall back nữa
                lps[i] = 0;
                ++i;
            }
        }
    }

    return lps;
}

// KMP search: trả về danh sách vị trí bắt đầu (0-indexed) của pattern trong text
auto kmp_search(std::string_view text, std::string_view pattern)
    -> std::vector<int>
{
    std::vector<int> result;

    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());

    if (m == 0 || m > n) return result;

    const auto lps = build_lps(pattern);

    int i = 0; // con trỏ text    — KHÔNG bao giờ giảm
    int j = 0; // con trỏ pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;
        }

        if (j == m) {
            // Full match tại i - m
            result.push_back(i - m);
            // Tiếp tục tìm match tiếp: fall back qua lps
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            // Mismatch
            if (j != 0) {
                j = lps[j - 1]; // fall back, KHÔNG lùi i
            } else {
                ++i; // j==0, mismatch ngay từ đầu → tiến i
            }
        }
    }

    return result;
}

int main() {
    const std::string text    = "AABCAABXAABCAAB";
    const std::string pattern = "AABCAAB";

    const auto matches = kmp_search(text, pattern);

    std::cout << "Pattern: \"" << pattern << "\"\n";
    std::cout << "Text:    \"" << text    << "\"\n";
    std::cout << "Found at indices: ";
    for (int pos : matches) {
        std::cout << pos << " ";
    }
    std::cout << "\n";

    return 0;
}
```

```bash
g++ -std=c++20 -O2 -Wall -Wextra -o kmp kmp.cpp && ./kmp
# Output:
# Pattern: "AABCAAB"
# Text:    "AABCAABXAABCAAB"
# Found at indices: 0 8
```

---

## 5. Độ phức tạp

### Time: O(n + m)

**Chứng minh bằng amortized analysis:**

`build_lps`: biến `len` chỉ tăng tối đa `m-1` lần (mỗi lần `i` tăng, `len` tăng tối đa 1). Tổng lần giảm `len` không thể vượt tổng lần tăng → O(m).

`kmp_search`: biến `i` chỉ tăng, không giảm, chạy từ 0 đến n-1 → tối đa `n` lần tăng. Biến `j` mỗi khi tăng thì `i` cũng tăng → `j` tăng tối đa `n` lần. `j` giảm qua `lps` nhưng tổng lần giảm ≤ tổng lần tăng → O(n).

Tổng: **O(n + m)**. Với naive là O(n·m), đây là cải thiện cực kỳ đáng kể khi m lớn.

### Space: O(m)

Chỉ lưu `lps` array độ dài `m`. Text và pattern không cần copy.

---

## 6. Khi nào dùng / không dùng

### Dùng khi:
- Tìm kiếm exact substring, single pattern
- Alphabet nhỏ hoặc có nhiều ký tự lặp (worst case naive bộc lộ rõ)
- Cần **tìm tất cả** occurrences, không chỉ cái đầu tiên
- Streaming input (i không giảm → có thể xử lý online từng ký tự)
- Bài toán liên quan đến **chu kỳ chuỗi**: `period = m - lps[m-1]`

### Không dùng khi:
- **Nhiều patterns** → dùng Aho-Corasick (KMP multi-pattern)
- **Approximate matching** (cho phép sai số) → dynamic programming / bitap
- **Regex** → NFA/DFA
- Chuỗi cực ngắn (m < 10): naive đủ dùng, overhead build_lps không đáng

### Edge cases & pitfalls:

```cpp
// Pitfall 1: pattern rỗng
kmp_search("abc", "");  // Phải handle riêng

// Pitfall 2: pattern dài hơn text
kmp_search("ab", "abc");  // Trả về rỗng, không crash

// Pitfall 3: overlapping matches
// T = "AAAA", P = "AA"
// KMP cho ra: 0, 1, 2 — ĐÚNG vì dùng lps để continue sau match

// Pitfall 4: lps của single char luôn là 0
// Đừng nhầm lps[0] = 0 với "chưa tính"
```

---

## 7. So sánh với thuật toán tương tự

| | Naive | KMP | Rabin-Karp | Boyer-Moore | Aho-Corasick |
|---|---|---|---|---|---|
| **Time (worst)** | O(nm) | O(n+m) | O(nm)* | O(nm)* | O(n+m+k) |
| **Time (avg)** | O(nm) | O(n+m) | O(n+m) | O(n/m) | O(n+m+k) |
| **Space** | O(1) | O(m) | O(1) | O(m+σ) | O(m·σ) |
| **Multi-pattern** | ❌ | ❌ | ✅ (hash) | ❌ | ✅ |
| **Preprocessing** | None | lps O(m) | hash O(m) | bad/good suffix | trie+fail |

*với hash collision tệ

**KMP vs Rabin-Karp**: RK dùng hash → dễ extend sang multi-pattern (tìm một trong k patterns). KMP guarantee O(n+m) worst case tuyệt đối, RK có thể degenerate nếu hash collision. Với bài single pattern và cần guarantee → KMP.

**KMP vs Boyer-Moore**: BM skip nhiều ký tự hơn nhờ nhìn từ phải sang trái → thực tế nhanh hơn KMP đáng kể trên alphabet lớn (text tiếng Anh). Nhưng BM phức tạp hơn nhiều để implement đúng (bad character + good suffix rule). KMP đơn giản hơn, predictable hơn, dễ adapt sang các bài toán biến thể.

**KMP vs Aho-Corasick**: AC về bản chất là KMP nhưng trie thay vì single pattern. Nếu đã hiểu KMP thì học AC rất tự nhiên — failure link trong AC chính là lps trong KMP.

---

### Tóm lại một câu

> KMP khai thác cấu trúc nội tại của pattern để không bao giờ so sánh lại ký tự đã biết — `i` chỉ tiến, không bao giờ lùi. Toàn bộ "trí tuệ" nằm ở `lps` array, xây trong O(m), dùng trong O(n).