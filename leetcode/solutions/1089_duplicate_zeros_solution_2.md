## Duplicate Zeros

### 1. Quan sát / Nhận diện pattern

Bài này yêu cầu **in-place**, không dùng mảng phụ. Đây là dấu hiệu của bài **two-pointer** hoặc **simulation ngược**.

Cái khó: nếu duyệt từ trái sang phải và duplicate zero tại chỗ, ta sẽ ghi đè lên dữ liệu chưa xử lý.

→ Gợi ý ngay: **duyệt từ phải sang trái**.

---

### 2. Trực giác

**Brute force trước** để hiểu bài:

```
arr = [1, 0, 2, 3, 0, 4, 5, 0]
         ↑
      Gặp 0 → duplicate, shift phần còn lại sang phải, bỏ phần tử cuối
→    [1, 0, 0, 2, 3, 0, 4, 5]  (bỏ số 0 cuối)
              ↑
           Tiếp tục duyệt từ index 2...
```

Brute force này O(n²) vì mỗi lần gặp 0 phải shift toàn bộ mảng.

---

**Hướng tối ưu — Two Pass:**

**Pass 1:** Đếm xem sau khi duplicate, có bao nhiêu phần tử của mảng gốc "sống sót" trong n ô.

```
arr = [1, 0, 2, 3, 0, 4, 5, 0]  n = 8

Duyệt từ trái, đếm số ô sẽ chiếm:
  1 → chiếm 1 ô  → tổng = 1
  0 → chiếm 2 ô  → tổng = 3
  2 → chiếm 1 ô  → tổng = 4
  3 → chiếm 1 ô  → tổng = 5
  0 → chiếm 2 ô  → tổng = 7
  4 → chiếm 1 ô  → tổng = 8  ← đủ n=8, dừng tại index i=5
                                 nhưng chỉ còn 1 ô trống cho số 4
                                 (không còn chỗ cho bản sao thứ 2 của 4)
```

Sau pass 1, ta biết: **chỉ các phần tử từ index 0..i của mảng gốc mới được ghi vào mảng kết quả**.

**Pass 2:** Duyệt ngược từ i về 0, ghi từ cuối mảng (`write = n-1`) ngược lên đầu. Vì ghi ngược nên không bao giờ ghi đè dữ liệu chưa đọc.

```
i=5 (arr[5]=4), write=7 → arr[7] = 4,  write=6
i=4 (arr[4]=0), write=6 → arr[6] = 0,  write=5
                           arr[5] = 0,  write=4   ← duplicate
i=3 (arr[3]=3), write=4 → arr[4] = 3,  write=3
i=2 (arr[2]=2), write=3 → arr[3] = 2,  write=2
i=1 (arr[1]=0), write=2 → arr[2] = 0,  write=1
                           arr[1] = 0,  write=0   ← duplicate
i=0 (arr[0]=1), write=0 → arr[0] = 1,  write=-1
```

Kết quả: `[1, 0, 0, 2, 3, 0, 0, 4]` ✓

---

### 3. Phát biểu thuật toán

**Pass 1 — Tìm "boundary":**
- Dùng con trỏ `left` duyệt từ 0, biến `write_pos` đếm số ô sẽ dùng.
- Khi `write_pos >= n`: dừng. `left - 1` là index cuối cùng của mảng gốc được ghi.
- Edge case: nếu phần tử cuối cùng là `0` nhưng chỉ còn đúng 1 ô (không đủ 2 ô để duplicate) → đánh dấu `last_zero_truncated = true`.

**Pass 2 — Ghi ngược:**
- `right = n - 1`, `i = left` (boundary vừa tìm).
- Nếu `last_zero_truncated`: ghi 1 lần `0` (không duplicate), `right--`, `i--`.
- Duyệt `i` từ boundary-1 về 0:
  - Nếu `arr[i] != 0`: `arr[right--] = arr[i]`
  - Nếu `arr[i] == 0`: `arr[right--] = 0; arr[right--] = 0`

---

### 4. Cài đặt C++20

```cpp
#include <vector>
#include <span>

class Solution {
public:
    void duplicateZeros(std::vector<int>& arr) {
        const int n = static_cast<int>(arr.size());

        // ── Pass 1: tìm phần tử cuối cùng của arr gốc còn nằm trong kết quả ──
        // left  : index trong arr gốc đang xét
        // write : số ô đã "đặt chỗ" trong mảng kết quả n ô
        int left = 0, write = 0;
        while (left < n) {
            write += (arr[left] == 0) ? 2 : 1;  // 0 chiếm 2 ô, còn lại 1 ô
            if (write >= n) break;
            ++left;
        }

        // Edge case: arr[left] == 0 nhưng chỉ còn đúng 1 ô cuối mảng
        // → chỉ ghi 1 lần 0 (bị cắt cụt), không duplicate được
        bool last_zero_truncated = (arr[left] == 0 && write == n + 1);

        // ── Pass 2: ghi ngược từ phải sang trái ──
        // right: con trỏ ghi vào mảng kết quả (bắt đầu từ cuối)
        int right = n - 1;

        if (last_zero_truncated) {
            arr[right--] = 0;   // chỉ ghi 1 lần, không duplicate
            --left;             // phần tử này đã xử lý, lùi left
        }

        // Duyệt ngược, ghi vào vị trí right
        for (int i = left; i >= 0; --i) {
            if (arr[i] != 0) {
                arr[right--] = arr[i];
            } else {
                // Duplicate: ghi 0 hai lần
                arr[right--] = 0;
                arr[right--] = 0;
            }
        }
    }
};
```

---

### 5. Phân tích độ phức tạp

| | Độ phức tạp | Lý do |
|---|---|---|
| **Time** | O(n) | Pass 1 duyệt tối đa n phần tử, Pass 2 duyệt tối đa n phần tử → 2n |
| **Space** | O(1) | Không dùng mảng phụ, chỉ có vài biến đếm |

**Tại sao Two-Pass tối ưu hơn Brute Force?**

Brute force shift từng phần tử khi gặp 0 → mỗi shift là O(n), tổng O(n²).  
Two-pass biết trước "điểm kết thúc" nên chỉ cần 1 lần ghi ngược, mỗi phần tử chỉ được chạm đúng 1 lần.

---

**Điểm dễ sai nhất của bài này** là edge case `last_zero_truncated`: khi `0` cuối cùng rơi đúng vào ô chót của mảng, nó chỉ được ghi 1 lần thay vì 2 lần. Nếu bỏ qua case này sẽ sai test `[1,0,2,3,0,4,5,0]` → `right` bị âm và ghi ra ngoài mảng.