## 852. Peak Index in a Mountain Array

### 1. Quan sát / Nhận diện pattern

Bài cho một mảng "mountain" — tức là tồn tại đúng một đỉnh `i` sao cho:
```
arr[0] < arr[1] < ... < arr[i] > arr[i+1] > ... > arr[n-1]
```

Yêu cầu: tìm index `i` đó.

Đây là bài **tìm kiếm trên mảng có tính đơn điệu cục bộ** → nhận ngay ra mùi của **Binary Search**.

---

### 2. Trực giác

**Brute force:** duyệt từ trái, tìm phần tử đầu tiên mà `arr[i] > arr[i+1]` → đó là đỉnh. O(n).

**Tại sao Binary Search được?**

Nhìn vào một phần tử giữa `mid`:
- Nếu `arr[mid] < arr[mid+1]` → ta đang ở **sườn tăng** → đỉnh nằm bên **phải**
- Nếu `arr[mid] > arr[mid+1]` → ta đang ở **sườn giảm** (hoặc chính là đỉnh) → đỉnh nằm ở `mid` hoặc bên **trái**

Ví dụ nhỏ:
```
index:  0  1  2  3  4
arr:    1  3  5  4  2

mid = 2 → arr[2]=5 > arr[3]=4 → sườn giảm → đỉnh ≤ 2 → right = mid = 2
mid = 1 → arr[1]=3 < arr[2]=5 → sườn tăng → đỉnh > 1 → left = mid+1 = 2
left == right == 2 → answer = 2 ✓
```

Mỗi bước ta loại nửa mảng → O(log n).

---

### 3. Phát biểu thuật toán

**Invariant:** Đỉnh luôn nằm trong đoạn `[left, right]`.

- Khởi tạo `left = 0`, `right = n - 1`
- Lặp khi `left < right`:
  - `mid = left + (right - left) / 2`
  - Nếu `arr[mid] < arr[mid + 1]`: đỉnh ở `(mid, right]` → `left = mid + 1`
  - Ngược lại: đỉnh ở `[left, mid]` → `right = mid`
- Trả về `left`

*Tại sao `right = mid` (không phải `mid - 1`)?* Vì khi `arr[mid] > arr[mid+1]`, bản thân `mid` có thể là đỉnh, không được loại nó.

---

### 4. Cài đặt C++20

```cpp
#include <vector>
#include <cassert>

class Solution {
public:
    int peakIndexInMountainArray(std::vector<int>& arr) {
        int left = 0;
        int right = static_cast<int>(arr.size()) - 1;

        // Invariant: đỉnh luôn nằm trong [left, right]
        while (left < right) {
            int mid = left + (right - left) / 2; // tránh overflow

            if (arr[mid] < arr[mid + 1]) {
                // Đang ở sườn tăng → đỉnh nằm ở nửa phải, loại mid
                left = mid + 1;
            } else {
                // Đang ở sườn giảm hoặc chính là đỉnh
                // mid vẫn là ứng viên → giữ lại
                right = mid;
            }
        }

        // left == right, hội tụ về đỉnh
        return left;
    }
};
```

---

### 5. Phân tích độ phức tạp

| | Complexity | Lý do |
|---|---|---|
| **Time** | O(log n) | Mỗi iteration loại đúng một nửa search space |
| **Space** | O(1) | Chỉ dùng con trỏ `left`, `right`, `mid` |

**So sánh 3 hướng:**

| Approach | Time | Space | Ghi chú |
|---|---|---|---|
| Linear scan | O(n) | O(1) | Đơn giản nhất |
| Binary search | O(log n) | O(1) | Tối ưu, đây là hướng đúng |
| STL `max_element` | O(n) | O(1) | Không khai thác được constraint mountain |

---

**Điểm cần nhớ:** Bài này là template cổ điển của dạng **"Binary search on answer"** — không phải tìm giá trị target cụ thể, mà tìm điểm thỏa mãn một điều kiện đơn điệu. Nhận ra `arr[mid]` vs `arr[mid+1]` là compass để biết đỉnh ở phía nào — đó là toàn bộ insight của bài.