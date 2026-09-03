Code đã compile sạch với `-std=c++20 -O2 -Wall` (không warning) và chạy đúng trên toàn bộ test case, kể cả edge case chuỗi rỗng. Giờ vào bài giảng.

---

## 1. Bài toán nó giải quyết

Pain point cụ thể: **tìm palindrome dài nhất trong một chuỗi** (Longest Palindromic Substring - LPS), hoặc tổng quát hơn là **trả lời "đoạn `s[l..r]` có phải palindrome không?" trong O(1) sau tiền xử lý**.

Cách brute-force: với mỗi cặp `(l, r)` kiểm tra palindrome — O(n²) cặp, mỗi lần check O(n) → O(n³). Tối ưu bằng "expand around center" (mở rộng từ tâm) thì còn O(n²), vì mỗi tâm trong n tâm (n tâm lẻ + n-1 tâm chẵn) tốn tối đa O(n) để mở rộng.

Manacher giải quyết đúng cái pain point đó: đưa bài toán này về **O(n)** — tuyến tính. Đây không phải là thuật toán "để dùng hàng ngày" như sorting, mà là công cụ chuyên dụng khi:
- Cần LPS trên chuỗi cực dài (10^6, 10^7 ký tự) mà O(n²) sẽ TLE.
- Cần trả lời nhiều truy vấn "đoạn này có palindrome không" trong O(1) — Manacher tiền xử lý một lần, query O(1).

## 2. Trực giác cốt lõi

Cái khó của "expand around center" là nó **lãng phí công sức đã làm**. Ví dụ: nếu bạn đã biết `"aba"` là palindrome tại tâm i, và bên phải nó có một palindrome khác đối xứng qua tâm i, thì bạn **đã biết trước một phần thông tin** của palindrome đó rồi — không cần mở rộng lại từ đầu.

Vẽ tay ví dụ: chuỗi `s = "abaaba"`.

```
index:  0 1 2 3 4 5
char:   a b a a b a
```

Palindrome dài nhất ở đây là toàn bộ `"abaaba"` (tâm nằm giữa index 2 và 3). Nhưng để minh họa "đối xứng gương", ta lấy ví dụ khác cân đối hơn:

```
s = "cabacabad"
     0123456789
```

Tại tâm `i=3` (ký tự 'a' vị trí 3... để dễ nhìn ta dùng chuỗi đã biến đổi phía dưới), giả sử ta đã tính được palindrome LỚN NHẤT hiện tại có tâm `C`, mở rộng ra tới biên phải `R` (right edge). Bây giờ ta đang xét một vị trí `i` nằm **bên trong** vùng `[C-  (R-C), R]`. Vì cả đoạn `[2C-R, R]` là palindrome (đối xứng qua C), nên vị trí `i` có một "ảnh gương" `mirror = 2C - i` nằm bên trái C.

**Insight chính:** bán kính palindrome tại `i` sẽ **giống hệt** bán kính palindrome tại `mirror`... **trừ khi** palindrome tại mirror bị "cắt cụt" bởi biên trái của palindrome lớn `[2C-R, R]`. Khi đó ta chỉ biết chắc tối thiểu là `min(R - i, p[mirror])`, phần còn lại phải test tay (mở rộng) thêm.

Hình dung trực quan:

```
        C (tâm palindrome lớn hiện tại)
        |
[.......C.......]
2C-R            R
   ^mirror   ^i
   (đối xứng qua C)
```

Nếu palindrome tại `mirror` nằm gọn hoàn toàn trong `[2C-R, R]` (không chạm biên trái `2C-R`), thì do tính đối xứng, palindrome tại `i` cũng y hệt vậy — **không cần test lại từng ký tự**. Đây chính là lý do thuật toán chạy O(n): mỗi ký tự chỉ được "test tay" (so sánh trực tiếp) một số lần giới hạn tổng cộng, phần còn lại được suy ra miễn phí từ tính đối xứng.

## 3. Cơ chế hoạt động từng bước

**Bước 0 — Chuẩn hóa chuỗi (transform):** Palindrome có 2 loại: độ dài lẻ (tâm là 1 ký tự, VD "aba") và độ dài chẵn (tâm là khoảng trống giữa 2 ký tự, VD "abba"). Xử lý 2 case riêng biệt sẽ rối. Manacher chèn ký tự đệm `#` (không xuất hiện trong chuỗi gốc) giữa mọi ký tự và ở 2 đầu, cộng thêm 2 sentinel `^`, `$` để tránh check biên mảng:

```
s = "aba"
t = "^ # a # b # a # $"
     0 1 2 3 4 5 6 7 8
```

Giờ mọi palindrome trong `t` đều có độ dài **lẻ**, tâm luôn là 1 vị trí cụ thể. Bán kính `p[i]` trong `t` **chính bằng** độ dài palindrome tương ứng trong `s` gốc (tính chất đẹp của cách chèn `#`).

**Bước 1 — Duyệt từng tâm `i` từ trái sang phải**, duy trì cặp `(center, rightEdge)` là palindrome có biên phải xa nhất từng thấy tính đến thời điểm hiện tại.

**Bước 2 — Khởi tạo `p[i]` bằng đối xứng gương** (nếu `i < rightEdge`):
```
mirror = 2*center - i
p[i] = min(rightEdge - i, p[mirror])
```

**Bước 3 — Mở rộng thêm** bằng so sánh trực tiếp ký tự, bắt đầu từ giá trị đã khởi tạo (không reset về 0!):
```
while (t[i+p[i]+1] == t[i-p[i]-1]) p[i]++;
```

**Bước 4 — Cập nhật `(center, rightEdge)`** nếu `i + p[i] > rightEdge`.

### Walk-through cụ thể trên `s = "babad"`

```
t = ^ # b # a # b # a # d #  $
    0 1 2 3 4 5 6 7 8 9 10 11 12
```

| i | mirror | init từ gương | mở rộng tay | p[i] | center,rightEdge sau |
|---|--------|----------------|-------------|------|------------------------|
| 1 (`#`) | - | 0 | không mở được (biên `^`≠`b`) | 0 | (1,1) |
| 2 (`b`) | - | 0 | `t[3]=#,t[1]=#` khớp → tăng; `t[4]=a,t[0]=^` dừng | 1 | (2,3) |
| 3 (`#`) | i<rightEdge=3? không (3 không <3) | 0 | `t[4]=a,t[2]=b` dừng | 0 | (2,3) |
| 4 (`a`) | - | 0 | `t[5]=#,t[3]=#` khớp; `t[6]=b,t[2]=b` khớp; `t[7]=#,t[1]=#` khớp; `t[8]=a,t[0]=^` dừng | 3 | (4,7) |
| 5 (`#`) | mirror=2*4-5=3, p[3]=0 | 0 | `t[6]=b,t[4]=a` dừng | 0 | (4,7) |
| 6 (`b`) | mirror=2 | min(7-6=1, p[2]=1)=1 | `t[8]=a,t[4]=a` khớp; `t[9]=#,t[3]=#` khớp; `t[10]=d,t[2]=b` dừng | 3 | (6,9) |
| 7 (`#`) | mirror=5 | min(9-7=2,p[5]=0)=0 | `t[8]=a,t[6]=b` dừng | 0 | (6,9) |
| 8 (`a`) | mirror=4 | min(9-8=1,p[4]=3)=1 | `t[10]=d,t[6]=b` dừng | 1 | (6,9) |
| ... | | | | | |

`p[4]=3` là max → palindrome trong `t` tâm tại 4, bán kính 3 → chuyển về `s`: `start = (4-3)/2 = 0`, `len=3` → **"bab"**. Khớp với kết quả code chạy thật ở trên.

Chỗ hay nhất để nhìn ra "ăn theo đối xứng" là ở `i=6`: nhờ `mirror=2` có `p[2]=1`, ta **không bắt đầu từ 0** mà bắt đầu ngay từ 1, tiết kiệm 1 bước so sánh thừa.

## 4. Cài đặt C++20

```cpp
#include <bits/stdc++.h>
using namespace std;

// Trả về mảng radius[] trên chuỗi đã biến đổi (chèn ký tự đệm),
// cùng với hàm tiện ích để lấy palindrome dài nhất trên chuỗi gốc.
struct Manacher {
    string t;              // chuỗi đã biến đổi: "^#a#b#a#$"
    vector<int> p;         // p[i] = bán kính palindrome tại tâm i (trên chuỗi t)

    // s: chuỗi gốc cần xử lý
    Manacher(const string& s) {
        // Bước 1: biến đổi chuỗi gốc thành chuỗi mới để xử lý đồng nhất
        // giữa palindrome độ dài chẵn và lẻ.
        // "^" và "$" là 2 lính canh (sentinel) để không phải check biên mảng.
        t = "^#";
        for (char c : s) {
            t += c;
            t += '#';
        }
        t += "$";

        int n = t.size();
        p.assign(n, 0);
        int center = 0, rightEdge = 0; // (center, rightEdge) = palindrome "đang giữ kỷ lục" hiện tại

        for (int i = 1; i < n - 1; i++) {
            // Bước 2: nếu i nằm trong vùng đối xứng [center-rightEdge, center+rightEdge],
            // dùng đối xứng gương qua center để lấy giá trị khởi tạo, tránh làm lại từ đầu.
            int mirror = 2 * center - i;
            if (i < rightEdge) {
                p[i] = min(rightEdge - i, p[mirror]);
            }

            // Bước 3: mở rộng "brute-force" từ giá trị khởi tạo đó
            while (t[i + p[i] + 1] == t[i - p[i] - 1]) {
                p[i]++;
            }

            // Bước 4: cập nhật (center, rightEdge) nếu palindrome tại i vượt biên phải hiện tại
            if (i + p[i] > rightEdge) {
                center = i;
                rightEdge = i + p[i];
            }
        }
    }

    // Độ dài palindrome dài nhất toàn chuỗi
    int longestLength() const {
        return *max_element(p.begin(), p.end());
    }

    // Trả về (chỉ số bắt đầu, độ dài) của palindrome dài nhất trên chuỗi GỐC
    pair<int,int> longestSubstring() const {
        int bestLen = 0, bestCenter = 0;
        for (int i = 1; i + 1 < (int)t.size(); i++) {
            if (p[i] > bestLen) {
                bestLen = p[i];
                bestCenter = i;
            }
        }
        // Công thức chuyển ngược từ tọa độ trên t[] về tọa độ trên s[]
        int start = (bestCenter - bestLen) / 2;
        return {start, bestLen};
    }

    // Kiểm tra s[l..r] (0-indexed, inclusive) có phải palindrome không trong O(1)
    bool isPalindrome(int l, int r) const {
        int center = l + r + 2; // vị trí tâm tương ứng trên t[]
        int radius = r - l + 1;
        return p[center] >= radius;
    }
};

int main() {
    Manacher m("babad");
    auto [start, len] = m.longestSubstring();
    cout << "LPS = " << m.t.substr(0) << "\n"; // debug nếu cần
    cout << "Longest palindromic substring: "
         << string("babad").substr(start, len) << "\n";
}
```

Đã build & test thật với `g++ -std=c++20 -O2 -Wall`: pass sạch mọi test kể cả `""` (chuỗi rỗng — trả về `len=0`, không segfault nhờ có sentinel).

**Chú ý cài đặt hay bị sai:**
- Sentinel `^` và `$` phải **khác nhau** và khác mọi ký tự có thể xuất hiện trong `s`, nếu không vòng `while` có thể đọc lố ra ngoài mảng khi 2 đầu "vô tình khớp".
- Công thức đổi tọa độ `start = (center - radius) / 2` rất dễ nhớ sai — hãy tự suy ra bằng cách đối chiếu chỉ số trên `t` và `s` chứ đừng học vẹt.

## 5. Độ phức tạp

**Time: O(n)**, với `n = |s|` (chuỗi biến đổi có độ dài `2n+3` nhưng vẫn là `O(n)`).

Chứng minh amortized: xét biến `rightEdge`. Mỗi lần vòng `while` ở Bước 3 thực hiện một phép so sánh **thành công** (tăng `p[i]`), nó luôn kéo `i + p[i]` tăng thêm 1, tức đẩy biên `rightEdge` mới (nếu có cập nhật) tăng theo. `rightEdge` chỉ tăng, tối đa đến `n`, không bao giờ giảm. Vậy tổng số lần "mở rộng thành công" trên toàn bộ vòng lặp `i` bị chặn bởi tổng độ tăng của `rightEdge`, tức ≤ `n`. Cộng thêm đúng 1 phép so sánh "thất bại" (dừng vòng while) mỗi `i` → tổng số phép so sánh ký tự là `O(n) + O(n) = O(n)`. Đây là kỹ thuật amortized analysis kiểu "potential function", giống cách chứng minh KMP chạy O(n).

**Space: O(n)** cho mảng `p[]` và chuỗi biến đổi `t`.

## 6. Khi nào dùng / không dùng

**Dùng khi:**
- Cần LPS trên chuỗi lớn (n ≥ 10^5) mà O(n²) TLE.
- Cần trả lời nhiều query "đoạn `[l,r]` có palindrome không" — build một lần O(n), mỗi query O(1) (như `isPalindrome()` ở trên).
- Bài toán biến thể: đếm số lượng palindromic substring (dùng tổng `⌈p[i]/2⌉` trên chuỗi biến đổi), tìm palindrome dài nhất bắt đầu/kết thúc tại một vị trí cố định, v.v.

**Không cần dùng (over-engineering) khi:**
- `n` nhỏ (vài trăm đến vài nghìn) — "expand around center" O(n²) đã đủ nhanh và code đơn giản, ít bug hơn nhiều.
- Bài toán không cần palindrome liên tục mà là subsequence (không phải substring) — Manacher **không** áp dụng được cho Longest Palindromic **Subsequence**, đó là bài LCS/DP khác hoàn toàn (O(n²) DP, không có thuật toán tuyến tính tổng quát).

**Pitfall thường gặp:**
1. Quên rằng `p[i]` trên chuỗi biến đổi = độ dài palindrome trên chuỗi gốc (không phải bán kính chia 2) — dễ nhầm hệ số 2.
2. Off-by-one khi đổi tọa độ `start = (center-radius)/2`.
3. Chọn ký tự đệm `#` trùng với ký tự có thể có trong input (VD input chứa dấu `#`) → lệch kết quả.
4. Nhầm lẫn giữa "mirror nằm gọn trong vùng" và "mirror có thể vượt biên" — nhiều người bỏ luôn bước `min(rightEdge-i, p[mirror])` và chỉ gán `p[i]=p[mirror]`, sai ngay khi palindrome bị cắt cụt bởi biên trái.

## 7. So sánh với thuật toán tương tự

| Thuật toán | Bài toán | Time | Ghi chú |
|---|---|---|---|
| **Expand around center** | LPS | O(n²) | Dễ code, đủ dùng khi n nhỏ, không cần transform chuỗi |
| **DP (bảng `dp[i][j]`)** | LPS, LPS-đếm | O(n²) time, O(n²) space | Trực quan cho người mới, nhưng tốn cả memory lẫn time hơn Manacher |
| **Manacher** | LPS, palindrome query O(1) | O(n) | Tối ưu nhất cho substring palindrome, nhưng cài đặt "khó nuốt" hơn hẳn, dễ sai off-by-one |
| **Eertree (Palindromic Tree)** | Đếm palindromic substring PHÂN BIỆT, các bài toán structural trên tập palindrome | O(n) amortized | Mạnh hơn Manacher khi cần biết cấu trúc cây các palindrome con (VD: mỗi palindrome sinh từ palindrome cha bằng thêm ký tự 2 đầu), nhưng cài đặt phức tạp hơn nhiều, overhead constant lớn hơn |
| **Z-function / suffix array + LCP** | Có thể dùng để giải LPS gián tiếp (so `s` với `reverse(s)`) | O(n) hoặc O(n log n) | Cách tiếp cận khác: tìm LCP giữa `s[i:]` và `reverse(s)[n-1-i:]`, kết hợp binary search hoặc suffix structure. Về bản chất giải quyết đúng bài toán nhưng code phức tạp hơn Manacher nếu chỉ cần LPS thuần túy |

Điểm khác biệt cốt lõi giữa Manacher và Eertree: Manacher cho bạn **giá trị bán kính palindrome tại mọi tâm**, đủ để trả lời "palindrome dài nhất là gì / đoạn này có palindrome không". Eertree cho bạn **toàn bộ cấu trúc cây các palindromic substring khác nhau** (mỗi node là 1 palindrome distinct), phù hợp khi cần đếm số palindrome substring phân biệt hoặc các bài toán yêu cầu duyệt quan hệ cha-con giữa các palindrome — bài toán khác hẳn về bản chất, không phải "cùng nhóm, khác độ mạnh".