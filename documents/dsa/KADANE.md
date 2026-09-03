# Thuật toán Kadane — Giải thích từ gốc rễ

---

## 1. Bài toán nó giải quyết

**Maximum Subarray Problem** — tìm subarray liên tiếp có tổng lớn nhất trong một mảng số nguyên.

```
Input:  [-2, 1, -3, 4, -1, 2, 1, -5, 4]
Output: 6  (subarray [4, -1, 2, 1])
```

**Pain point thực sự là gì?**

Brute force là O(n²) hoặc O(n³). Với n = 10⁶, brute force chết ngay. Kadane giải quyết bài này trong O(n) — một pass duy nhất, không cần nhìn lại.

Bài toán tưởng đơn giản nhưng có cái bẫy: mảng có thể chứa số âm, nên không phải lúc nào "lấy nhiều phần tử hơn" cũng cho kết quả tốt hơn.

---

## 2. Trực giác cốt lõi

**Câu hỏi Kadane đặt ra tại mỗi vị trí i:**

> "Subarray kết thúc tại đây nên bắt đầu từ đâu — từ trước đó kéo dài tới, hay bắt đầu lại từ chính tôi?"

Đó là toàn bộ ý tưởng. Không hơn không kém.

**Cụ thể hóa:**

Gọi `local` = tổng subarray tốt nhất **bắt buộc kết thúc tại i**.

Khi đứng ở vị trí `i`, chỉ có 2 lựa chọn:

```
1. Nối tiếp: local_prev + arr[i]   → kéo dài subarray trước đó
2. Bắt đầu mới: arr[i]             → vứt hết, bắt đầu từ đây
```

Chọn cái nào lớn hơn. Xong.

**Tại sao điều này đúng?**

Vì nếu `local_prev` âm, kéo nó vào chỉ làm tổng nhỏ lại — cắt đi và bắt đầu mới luôn tốt hơn. Nếu `local_prev` dương, giữ nó lại để "boost" cho phần tử hiện tại.

**Ví dụ tay — mảng nhỏ 5 phần tử:**

```
arr = [−2, 3, −1, 4, −3]
         ↑
         i=0: local = max(−2, −2) = −2   → bắt đầu mới (không có gì trước)
              global = −2

              i=1: local_prev = −2 (âm!)
                   → lấy max(−2+3, 3) = max(1, 3) = 3   → bắt đầu mới
                   global = max(−2, 3) = 3

              i=2: local_prev = 3 (dương!)
                   → lấy max(3+(−1), −1) = max(2, −1) = 2   → kéo dài
                   global = max(3, 2) = 3

              i=3: local_prev = 2 (dương!)
                   → lấy max(2+4, 4) = max(6, 4) = 6   → kéo dài
                   global = max(3, 6) = 6  ✓

              i=4: local_prev = 6 (dương!)
                   → lấy max(6+(−3), −3) = max(3, −3) = 3   → kéo dài
                   global = max(6, 3) = 6
```

Kết quả: `6` — subarray `[3, −1, 4]`. Đúng.

---

## 3. Cơ chế hoạt động từng bước

Dùng mảng chuẩn từ đầu bài:

```
arr = [−2, 1, −3, 4, −1, 2, 1, −5, 4]
idx =   0  1   2  3   4  5  6   7  8
```

Bảng trạng thái — theo dõi `local` và `global`:

```
i │ arr[i] │ local_prev │ local = max(local_prev+arr[i], arr[i]) │ global
──┼────────┼────────────┼─────────────────────────────────────────┼───────
0 │  −2    │   −∞/none  │  max(−2, −2) = −2                       │  −2
1 │   1    │   −2       │  max(−2+1, 1) = max(−1, 1) = 1          │   1
2 │  −3    │    1       │  max(1+(−3), −3) = max(−2, −3) = −2     │   1
3 │   4    │   −2       │  max(−2+4, 4) = max(2, 4) = 4           │   4
4 │  −1    │    4       │  max(4+(−1), −1) = max(3, −1) = 3       │   4
5 │   2    │    3       │  max(3+2, 2) = max(5, 2) = 5            │   5
6 │   1    │    5       │  max(5+1, 1) = max(6, 1) = 6            │   6  ✓
7 │  −5    │    6       │  max(6+(−5), −5) = max(1, −5) = 1       │   6
8 │   4    │    1       │  max(1+4, 4) = max(5, 4) = 5            │   6
```

**Đáp án: 6**, subarray `[4, −1, 2, 1]` (index 3→6).

Chú ý i=3: `local_prev = −2` (âm) → thuật toán **cắt đứt** và bắt đầu lại từ 4. Đây là quyết định then chốt.

---

## 4. Cài đặt C++20

```cpp
#include <algorithm>
#include <iostream>
#include <limits>
#include <span>
#include <vector>

// Struct chứa kết quả đầy đủ — không chỉ trả về giá trị
struct KadaneResult {
    long long  max_sum;
    std::size_t left;   // index bắt đầu subarray tối ưu
    std::size_t right;  // index kết thúc (inclusive)
};

// Xử lý cả mảng toàn âm — không ép kết quả >= 0
// std::span<const int>: read-only view, không copy, C++20
[[nodiscard]]
KadaneResult kadane(std::span<const int> arr) {
    // Guard: mảng rỗng
    if (arr.empty()) {
        return { std::numeric_limits<long long>::min(), 0, 0 };
    }

    // Khởi tạo với phần tử đầu tiên — tránh bẫy "khởi tạo global = 0"
    // (nếu toàn âm thì 0 sẽ là sai vì subarray phải có ít nhất 1 phần tử)
    long long local  = arr[0];
    long long global = arr[0];

    // Tracking index để trả về vị trí subarray
    std::size_t local_start = 0;           // subarray hiện tại bắt đầu từ đâu
    std::size_t best_left   = 0;
    std::size_t best_right  = 0;

    for (std::size_t i = 1; i < arr.size(); ++i) {
        // Quyết định cốt lõi: kéo dài hay bắt đầu mới?
        if (local + arr[i] >= arr[i]) {
            // Kéo dài — local_start giữ nguyên
            local = local + arr[i];
        } else {
            // Bắt đầu mới từ i
            local       = arr[i];
            local_start = i;
        }

        // Cập nhật global nếu tìm được kết quả tốt hơn
        if (local > global) {
            global     = local;
            best_left  = local_start;
            best_right = i;
        }
    }

    return { global, best_left, best_right };
}

int main() {
    const std::vector<int> arr = { -2, 1, -3, 4, -1, 2, 1, -5, 4 };

    auto [max_sum, left, right] = kadane(arr);

    std::cout << "Max sum   : " << max_sum << '\n';
    std::cout << "Subarray  : [";
    for (std::size_t i = left; i <= right; ++i) {
        std::cout << arr[i];
        if (i < right) std::cout << ", ";
    }
    std::cout << "]\n";
    std::cout << "Index     : [" << left << ", " << right << "]\n";

    // Test case: toàn âm — phải trả về phần tử lớn nhất, không phải 0
    const std::vector<int> all_neg = { -5, -1, -3, -2 };
    auto [s2, l2, r2] = kadane(all_neg);
    std::cout << "\nAll-negative test: " << s2
              << " at index [" << l2 << ", " << r2 << "]\n";
    // Expected: −1 at [1, 1]

    return 0;
}
```

Compile và chạy:

```bash
g++ -std=c++20 -O2 -Wall -Wextra -o kadane kadane.cpp && ./kadane
```

Output mong đợi:

```
Max sum   : 6
Subarray  : [4, -1, 2, 1]
Index     : [3, 6]

All-negative test: -1 at index [1, 1]
```

---

## 5. Độ phức tạp

**Time complexity: O(n)**

Chứng minh đơn giản: mỗi phần tử được xét đúng một lần, tại bước `i`. Không có vòng lặp lồng, không look-back, không recursion. Đây là lower bound của bài toán — bạn *phải* đọc qua toàn bộ mảng ít nhất một lần để biết đáp án, nên O(n) là tối ưu về mặt lý thuyết.

**Space complexity: O(1)**

Chỉ dùng một số biến scalar (`local`, `global`, vài index). Không allocate thêm bộ nhớ theo n. Đây là lý do Kadane đặc biệt hấp dẫn so với các DP approach dùng mảng phụ.

---

## 6. Khi nào dùng / không dùng

**Dùng khi:**

- Bài toán yêu cầu subarray **liên tiếp** có tổng lớn nhất/nhỏ nhất
- Mảng 1D, số nguyên hoặc số thực
- Cần O(n) — không thể chấp nhận O(n²)
- Biến thể: maximum subarray product (Kadane biến đổi), circular subarray (chạy Kadane 2 lần)

**Không dùng / cẩn thận khi:**

| Tình huống | Vấn đề |
|---|---|
| Subsequence (không cần liên tiếp) | Kadane sai — cần DP khác |
| Subarray 2D | Cần reduce về 1D trước, O(n³) tổng thể |
| Cần đếm số subarray đạt max | Kadane không thiết kế cho việc đó |
| Mảng rỗng | Phải handle riêng — implementation trên đã xử lý |
| Overflow | Với `int`, tổng có thể overflow — dùng `long long` như trên |

**Pitfall phổ biến nhất — khởi tạo `global = 0`:**

```cpp
// SAI nếu mảng toàn âm
long long global = 0;  // ← Bẫy này giết ~40% solution trên contest

// ĐÚNG
long long global = arr[0];
```

Nếu khởi tạo `global = 0`, bài toán ngầm định bạn *được phép chọn subarray rỗng*. Đọc đề kỹ — một số bài cho phép điều này, nhưng định nghĩa chuẩn yêu cầu ít nhất 1 phần tử.

---

## 7. So sánh với thuật toán tương tự

**Kadane vs. Prefix Sum:**

Prefix sum cũng giải được bài này trong O(n) nhưng cần O(n) space và logic phức tạp hơn (tìm min prefix sum trước mỗi vị trí). Kadane sạch hơn và O(1) space.

**Kadane vs. Divide & Conquer:**

D&C cũng cho O(n log n) — chậm hơn Kadane. D&C hữu ích khi bạn cần parallel computation trên distributed system, nhưng cho single-machine thì Kadane thắng tuyệt đối.

**Kadane vs. Segment Tree / Sparse Table:**

Segment Tree có thể trả lời max subarray sum trên range [l, r] bất kỳ trong O(log n) sau O(n log n) build time. Nếu bài toán có **nhiều query** trên các range khác nhau, Segment Tree là lựa chọn đúng. Nếu chỉ hỏi một lần trên toàn mảng, Kadane đơn giản hơn nhiều.

```
                 Time    Space   Range Query   Code Complexity
Brute Force      O(n²)   O(1)    Có            Thấp
Prefix Sum       O(n)    O(n)    Biến thể      Trung bình
Kadane           O(n)    O(1)    Không         Thấp ✓
D&C              O(n lgn) O(lgn) Có            Cao
Segment Tree     O(lgn)* O(n)   Có            Cao
                 (*per query, O(n) build)
```

---

**Tóm lại trong một câu:** Kadane là DP một chiều được tối giản đến mức không còn gì để bỏ đi — mỗi bước chỉ giữ đúng một quyết định: *kéo dài hay bắt đầu lại*, và đó là tất cả những gì cần thiết.