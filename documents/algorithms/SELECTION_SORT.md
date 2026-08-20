## Selection Sort — Nguyên lý

Ý tưởng cốt lõi: **chia mảng thành 2 phần — đã sorted (trái) và chưa sorted (phải). Mỗi vòng lặp, tìm phần tử nhỏ nhất trong phần chưa sorted, đưa về cuối phần đã sorted.**

```
[5, 3, 8, 1, 2]
 ↑ chưa sorted

Pass 1: tìm min trong [5,3,8,1,2] = 1 tại index 3
        swap(arr[0], arr[3])
        [1 | 3, 8, 5, 2]
         ✓   └─ chưa sorted

Pass 2: tìm min trong [3,8,5,2] = 2 tại index 4
        swap(arr[1], arr[4])
        [1, 2 | 8, 5, 3]
            ✓   └─ chưa sorted

Pass 3: tìm min trong [8,5,3] = 3 tại index 4
        swap(arr[2], arr[4])
        [1, 2, 3 | 5, 8]

Pass 4: tìm min trong [5,8] = 5 — đã đúng chỗ
        [1, 2, 3, 5 | 8]

Done ✓
```

---

## Đặc điểm quan trọng

**Luôn đúng n-1 swap** — bất kể mảng đầu vào thế nào. Đây là điểm khác biệt lớn nhất so với Bubble Sort.

```
Bubble Sort: mảng đã sorted → 0 swap (early termination)
Selection Sort: mảng đã sorted → vẫn n-1 swap (luôn tìm min rồi swap)
```

---

## Độ phức tạp

| Case | Time | Space |
|---|---|---|
| Worst | O(n²) | O(1) |
| Average | O(n²) | O(1) |
| Best | O(n²) | O(1) |

**Không có early termination** — cả 3 case đều O(n²). Số phép so sánh luôn là `n(n-1)/2`, chỉ số swap tối đa là `n-1`.

---

## Stable hay Unstable?

**Unstable** — như đã phân tích hôm trước:

```
[(A,2), (B,2), (C,1)]

Pass 1: min = (C,1), swap với index 0
→ [(C,1), (B,2), (A,2)]
                  ↑ A và B bị đảo thứ tự ❌
```

---

## Code C++

```cpp
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        if (minIdx != i)          // tránh swap thừa khi đã đúng chỗ
            swap(arr[i], arr[minIdx]);
    }
}
```

---

## So sánh với Bubble Sort

| | Bubble Sort | Selection Sort |
|---|---|---|
| Số swap | O(n²) worst | O(n) — luôn ≤ n-1 |
| Số so sánh | O(n²) | O(n²) — luôn đúng n(n-1)/2 |
| Best case | O(n) | O(n²) |
| Stable | ✅ | ❌ |
| Ứng dụng | Detect sorted | Minimize swap count |

**Khi nào Selection Sort tốt hơn Bubble Sort:** khi chi phí swap rất cao (ví dụ swap object lớn trong bộ nhớ), vì Selection Sort đảm bảo tối đa `n-1` swap dù mảng thế nào.