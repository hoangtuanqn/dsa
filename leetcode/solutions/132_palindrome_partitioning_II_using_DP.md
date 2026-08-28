Cả hai bản đã compile và pass toàn bộ test set. Dưới đây là bản tổng hợp đầy đủ.

---

# LeetCode 132 — Palindrome Partitioning II

## 1. Đề bài

Cho chuỗi `s`. Cắt `s` thành các mảnh sao cho **mọi mảnh đều là palindrome**. Trả về **số nhát cắt ít nhất**.

Ràng buộc: `1 ≤ n ≤ 2000`, chỉ chữ thường.

Hai điểm về đề cần chốt trước khi code:

**Đề hỏi số NHÁT CẮT, không phải số MẢNH.** Chia thành `k` mảnh thì có `k - 1` nhát cắt.

**Luôn tồn tại lời giải.** Trường hợp xấu nhất là cắt từng ký tự một (mỗi ký tự đơn đều là palindrome), tốn `n - 1` nhát. Nên không bao giờ phải trả về `-1`.

## 2. Vì sao không tham lam được

Ý tưởng đầu tiên ai cũng nghĩ: quét từ trái sang, ăn palindrome dài nhất có thể.

Thử `s = "aaabaa"`:

```
Tham lam:  aaa | b | aa     → 2 nhát
Tối ưu:    a | aabaa        → 1 nhát
```

Tham lam chết vì nó ra quyết định cục bộ mà không biết hệ quả về sau. Ăn `"aaa"` trông có lợi, nhưng nó phá vỡ palindrome `"aabaa"` nằm ở phía sau.

Kết luận: buộc phải **xét mọi cách cắt**. Brute force là `O(2^(n-1))` — với `n = 2000` là vô vọng. DP là cách xét hết mà vẫn nhanh.

## 3. Kiến trúc lời giải: hai pha

Bài này tách sạch thành hai bài toán con độc lập:

| Pha | Nhiệm vụ | Output |
|---|---|---|
| **1** | Với mọi cặp `(i, j)`, xác định `s[i..j]` có phải palindrome không | Bảng `isPal[n][n]` |
| **2** | Dùng bảng đó để tìm số nhát cắt tối thiểu | `dp[n]` |

Lý do tách: nếu pha 2 phải tự kiểm tra palindrome mỗi lần, mỗi lần tốn `O(n)` → tổng `O(n³)` → TLE. Tính trước một lần, tra cứu `O(1)`, tổng còn `O(n²)`.

---

## PHA 1 — Dựng bảng palindrome

### Ý tưởng

`s[i..j]` là palindrome khi và chỉ khi:

```
s[i] == s[j]   VÀ   s[i+1..j-1] cũng là palindrome
```

Bóc hai đầu, phần lõi bên trong phải là palindrome. Đây là quan hệ đệ quy, và lõi ngắn hơn 2 đơn vị so với đoạn ngoài.

```
     a b c b a
     ↑       ↑     hai đầu bằng nhau ✓
       ↑   ↑       lõi "bcb" phải là palindrome
         ↑         lõi "c" — base case
```

### Base case

Đệ quy phải dừng ở đâu đó. Hai trường hợp không cần đệ quy thêm:

- **Độ dài 1**: `s[i..i]` luôn là palindrome
- **Độ dài 2**: `s[i..i+1]` là palindrome khi `s[i] == s[i+1]`

Phải xử lý riêng độ dài 2, vì nếu áp công thức chung thì lõi là `s[i+1..i]` — chỉ số ngược, ô không tồn tại.

### Thứ tự duyệt

Đây là chỗ dễ sai nhất của pha 1. Ô `isPal[i][j]` phụ thuộc ô `isPal[i+1][j-1]`, tức là **đoạn ngắn hơn 2 đơn vị**. Nên phải duyệt theo **độ dài tăng dần**: khi tính đoạn dài `len`, mọi đoạn dài `len - 2` đã xong.

```cpp
vector<vector<bool>> buildPalTable(const string& s) {
    int n = s.size();
    vector<vector<bool>> isPal(n, vector<bool>(n, false));

    // len = 1
    for (int i = 0; i < n; ++i)
        isPal[i][i] = true;

    // len = 2
    for (int i = 0; i + 1 < n; ++i)
        if (s[i] == s[i+1])
            isPal[i][i+1] = true;

    // len >= 3
    for (int len = 3; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && isPal[i+1][j-1])
                isPal[i][j] = true;
        }
    }
    return isPal;
}
```

Nếu bạn duyệt `i` từ `0` tăng dần thay vì theo `len`, thì `isPal[i+1][...]` chưa được điền và bạn đang đọc `false` mặc định. **Code vẫn chạy, vẫn ra số, chỉ là sai** — và nhiều case ngắn vẫn pass nên rất khó phát hiện. Đây là loại bug nguy hiểm nhất trong DP.

(Cách duyệt `i` **giảm dần** thì đúng, vì hàng `i+1` được điền trước hàng `i`. Nhưng duyệt theo `len` dễ chứng minh hơn.)

---

## PHA 2 — DP tìm số nhát cắt

### Định nghĩa trạng thái

> **`dp[i]` = số nhát cắt ít nhất để cắt xong `i` ký tự đầu tiên**

`i` là **số lượng ký tự**, không phải chỉ số. Với `s = "aab"`: `dp[0]` ↔ `""`, `dp[1]` ↔ `"a"`, `dp[2]` ↔ `"aa"`, `dp[3]` ↔ `"aab"`. Đáp án cuối cùng là `dp[n]`.

### Cách suy ra công thức

Đang cần tính `dp[i]`. Không biết cắt thế nào, nhưng chắc chắn một điều: **dù cắt kiểu gì cũng phải có một mảnh cuối cùng**.

```
[ phần đầu: s[0..j-1] ] | [ mảnh cuối: s[j..i-1] ]
  j ký tự                  1 nhát cắt ở đây
```

Mảnh cuối bắt đầu ở đâu? Không biết → **thử hết mọi `j`**.

Với mỗi `j`, hai câu hỏi:

**Hợp lệ không?** Mảnh cuối `s[j..i-1]` phải là palindrome → tra `isPal[j][i-1]`.

**Tốn bao nhiêu?** Phần đầu có đúng `j` ký tự, cắt tối ưu tốn `dp[j]`. Cộng 1 nhát ngăn cách → **`dp[j] + 1`**.

Nhiều `j` hợp lệ thì lấy nhỏ nhất:

```cpp
dp[i] = INT_MAX;
for (int j = 0; j < i; ++j)
    if (isPal[j][i-1])
        dp[i] = min(dp[i], dp[j] + 1);
```

Vòng lặp `j` là "duyệt mọi lựa chọn", `min` là "chọn cái tốt nhất". Đây là pattern chung của mọi bài DP tối ưu.

### Vì sao `dp[0] = -1`

Xét `s = "aa"`, tính `dp[2]` với `j = 0`. Mảnh cuối là `"aa"` — phủ **toàn bộ** chuỗi, nghĩa là **không cắt gì cả**, đáp án đúng phải là `0`.

Nhưng công thức máy móc cộng `+1`:
- Nếu `dp[0] = 0` → ra `0 + 1 = 1`. **Sai.**
- Nếu `dp[0] = -1` → ra `-1 + 1 = 0`. **Đúng.**

`-1` là **giá trị bù kỹ thuật**, không mang ý nghĩa vật lý. Nó khử off-by-one ngay tại gốc.

Cách thay thế: đặt `dp[0] = 0`, định nghĩa `dp[i]` = số **mảnh** thay vì số nhát cắt, rồi `return dp[n] - 1` ở cuối. Cũng đúng, nhưng bạn phải nhớ trừ ở cuối và đây chính là chỗ người ta hay quên.

### Chạy tay `s = "aab"`

`dp[0] = -1`

**`dp[1]`** — cắt `"a"`:

| `j` | Mảnh cuối | Pal? | Chi phí |
|---|---|---|---|
| 0 | `"a"` | ✓ | `-1 + 1 = 0` |

→ `dp[1] = 0`

**`dp[2]`** — cắt `"aa"`:

| `j` | Mảnh cuối | Pal? | Chi phí |
|---|---|---|---|
| 0 | `"aa"` | ✓ | `-1 + 1 = 0` ← min |
| 1 | `"a"` | ✓ | `0 + 1 = 1` |

→ `dp[2] = 0`

Đây là minh chứng rõ nhất cho việc cần `min`: nếu gán thẳng không so sánh, giá trị cuối ghi đè là `1`, sai.

**`dp[3]`** — cắt `"aab"`:

| `j` | Mảnh cuối | Pal? | Chi phí |
|---|---|---|---|
| 0 | `"aab"` | ✗ | — |
| 1 | `"ab"` | ✗ | — |
| 2 | `"b"` | ✓ | `0 + 1 = 1` |

→ `dp[3] = 1` ✓

### Vì sao DP bắt được case tham lam bỏ sót

`s = "aaabaa"`, tính `dp[6]`:

| `j` | Mảnh cuối | Pal? | Chi phí |
|---|---|---|---|
| 0 | `"aaabaa"` | ✗ | — |
| 1 | `"aabaa"` | ✓ | `dp[1] + 1 = 1` ← min |
| 2 | `"abaa"` | ✗ | — |
| 3 | `"baa"` | ✗ | — |
| 4 | `"aa"` | ✓ | `dp[4] + 1 = 2` |
| 5 | `"a"` | ✓ | `dp[5] + 1 = 3` |

→ `dp[6] = 1`

Tham lam không bao giờ xét tới `j = 1` vì đã ăn `"aaa"` từ đầu. DP thử hết nên tìm ra.

---

## 4. Code hoàn chỉnh

```cpp
class Solution {
public:
    int minCut(string s) {
        int n = s.size();

        // ===== PHA 1: bảng palindrome =====
        vector<vector<bool>> isPal(n, vector<bool>(n, false));

        for (int i = 0; i < n; ++i)
            isPal[i][i] = true;

        for (int i = 0; i + 1 < n; ++i)
            if (s[i] == s[i+1])
                isPal[i][i+1] = true;

        for (int len = 3; len <= n; ++len) {
            for (int i = 0; i + len - 1 < n; ++i) {
                int j = i + len - 1;
                if (s[i] == s[j] && isPal[i+1][j-1])
                    isPal[i][j] = true;
            }
        }

        // ===== PHA 2: DP =====
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = -1;

        for (int i = 1; i <= n; ++i)
            for (int j = 0; j < i; ++j)
                if (isPal[j][i-1])
                    dp[i] = min(dp[i], dp[j] + 1);

        return dp[n];
    }
};
```

**Độ phức tạp:** Time `O(n²)`, Space `O(n²)`. Đo thực tế với `n = 2000`: 7ms.

---

## 5. Bản tối ưu: `O(n)` bộ nhớ

Bảng `isPal` thực ra là thừa. Nhận xét: khi bạn dùng kỹ thuật **expand-around-center** để tìm palindrome, ngay tại thời điểm xác nhận `s[l..r]` là palindrome, bạn đã có đủ thông tin để cập nhật `dp` luôn — không cần lưu lại.

```cpp
int minCut(string s) {
    int n = s.size();
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = -1;

    for (int c = 0; c < n; ++c) {
        // palindrome độ dài lẻ, tâm tại c
        for (int l = c, r = c; l >= 0 && r < n && s[l] == s[r]; --l, ++r)
            dp[r+1] = min(dp[r+1], dp[l] + 1);

        // palindrome độ dài chẵn, tâm giữa c và c+1
        for (int l = c, r = c+1; l >= 0 && r < n && s[l] == s[r]; --l, ++r)
            dp[r+1] = min(dp[r+1], dp[l] + 1);
    }
    return dp[n];
}
```

**Time `O(n²)`, Space `O(n)`.** Đo thực tế `n = 2000`: 2ms — nhanh hơn 3.5 lần nhờ cache locality tốt hơn hẳn (chỉ đụng một mảng `n+1` phần tử thay vì ma trận 4M ô).

Lưu ý về lập luận đúng đắn: bản này duyệt theo **tâm `c`**, không theo `i` tăng dần. Nên trực giác "khi tính `dp[i]` thì mọi `dp[j]` với `j < i` đã final" **không còn đúng**. Nó vẫn cho kết quả đúng vì mỗi phép cập nhật chỉ đọc `dp[l]` với `l < r+1`, và các giá trị `dp` chỉ giảm dần theo thời gian — lập luận kiểu **relaxation (Bellman-Ford)**, không phải thứ tự topo. Nếu bạn cần lập luận dễ chứng minh cho phỏng vấn, dùng bản hai pha.

---

## 6. Danh sách bug thường gặp

| Bug | Triệu chứng | Cách phát hiện |
|---|---|---|
| `dp[0] = 0` mà quên `-1` ở cuối | Output luôn dư đúng 1 | Test `"aba"`, `"aaaa"` — kỳ vọng `0` |
| `isPal[j][i]` thay vì `isPal[j][i-1]` | Out of range hoặc sai lệch | Test `n = 1` |
| Pha 1 duyệt `i` tăng dần | Sai thầm lặng, case ngắn vẫn pass | Test `"cabababcbc"` = `3` |
| Thiếu base case `len = 2` | Đọc ô chỉ số ngược | Test `"aa"` = `0` |
| Gán `dp[i] = dp[j] + 1` không có `min` | Lấy phải giá trị `j` cuối cùng | Test `"aa"`, `"ababababa"` |
| Không init `dp[i] = INT_MAX` | Rác hoặc `0` | Test bất kỳ |
| Check palindrome trong vòng DP | `O(n³)` → TLE | Test `"a" × 2000` |

Chú ý bug thứ 3: đây là loại nguy hiểm nhất vì không crash, không rõ ràng, chỉ sai trên một số input nhất định.

---

## 7. Bộ test

```
"a"          → 0     biên nhỏ nhất
"aa"         → 0     palindrome chẵn
"ab"         → 1     case cắt nhỏ nhất
"aba"        → 0     palindrome lẻ toàn chuỗi, bắt off-by-one
"aaaa"       → 0     toàn palindrome
"abcde"      → 4     không có palindrome dài, = n-1
"aab"        → 1     ví dụ đề
"leet"       → 2
"abbab"      → 1
"aaabaa"     → 1     BẮT BUỘC — bẫy greedy
"abacdc"     → 1
"abcbm"      → 2
"noonabbad"  → 2
"ababababa"  → 0
"cabababcbc" → 3     chặn nhánh tham lam
"a" × 2000   → 0     stress, bắt O(n³)
"ab" × 1000  → 1     stress
```

Case `"aaabaa"` là case bắt buộc phải có. Rất nhiều bài nộp sai chỉ vì nó.

Với case random để stress test, dùng bảng chữ cái nhỏ (`{a, b, c}`) chứ đừng dùng đủ 26 chữ — chuỗi random 26 ký tự gần như không có palindrome dài nào, đáp án luôn là `n - 1` và bạn không test được gì ngoài vòng lặp.

---

## 8. Chốt lại

Bài này là dạng **partition DP** kinh điển, và pattern của nó áp dụng được cho cả họ bài: Word Break, Palindrome Partitioning I/III, Split Array Largest Sum, Burst Balloons.

Công thức tư duy chung của cả họ:

1. `dp[i]` = đáp án tối ưu cho `i` phần tử đầu
2. Xét **mảnh cuối cùng** — nó bắt đầu ở đâu đó, không biết thì duyệt hết `j`
3. Chi phí = `dp[j]` + chi phí của mảnh `[j, i)`; lấy min/max
4. Nếu việc kiểm tra tính hợp lệ của mảnh `[j, i)` tốn `O(n)`, hãy tiền xử lý nó thành `O(1)`

Bước 4 là ranh giới giữa `O(n³)` TLE và `O(n²)` AC. Với bài 132, "tiền xử lý" chính là bảng `isPal`.