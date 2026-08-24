# Merge Sort — Từ Trực Giác Đến Implementation

---

## 1. Bài toán nó giải quyết

Bài toán cốt lõi: **sắp xếp một dãy n phần tử**.

Nhưng tại sao cần Merge Sort khi đã có Bubble Sort, Selection Sort?

Những thuật toán đó có chung một điểm yếu chết người: **O(n²)**. Với n = 10⁶ phần tử, O(n²) = 10¹² phép so sánh — máy tính hiện đại chạy ~10⁸–10⁹ phép/giây, tức là mất **hàng nghìn giây**. Không dùng được trong thực tế.

**Pain point cụ thể:** Làm sao sắp xếp hiệu quả khi dữ liệu lớn, và quan trọng hơn — làm sao **ổn định** (stable sort, giữ nguyên thứ tự tương đối của phần tử bằng nhau)?

Merge Sort ra đời năm 1945 bởi **John von Neumann**, giải quyết đúng cả hai: **O(n log n)** và **stable**.

---

## 2. Trực giác cốt lõi

**Divide & Conquer** — chia để trị.

Trực giác đến từ một quan sát đơn giản:

> Nếu tôi có **hai dãy đã được sắp xếp**, tôi có thể **ghép chúng lại thành một dãy sắp xếp trong O(n)** — chỉ cần duyệt từ đầu mỗi dãy, lấy phần tử nhỏ hơn.

Ví dụ:

```
Dãy A (đã sort): [1, 4, 7]
Dãy B (đã sort): [2, 5, 6]

Bước 1: So 1 vs 2 → lấy 1.  Result: [1]
Bước 2: So 4 vs 2 → lấy 2.  Result: [1, 2]
Bước 3: So 4 vs 5 → lấy 4.  Result: [1, 2, 4]
Bước 4: So 7 vs 5 → lấy 5.  Result: [1, 2, 4, 5]
Bước 5: So 7 vs 6 → lấy 6.  Result: [1, 2, 4, 5, 6]
Bước 6: Còn [7] → lấy hết.  Result: [1, 2, 4, 5, 6, 7]
```

Chỉ **6 bước** cho 6 phần tử — **O(n)**.

Vậy ý tưởng của Merge Sort là:

```
1. Chia đôi mảng
2. Đệ quy sort từng nửa
3. Merge hai nửa đã sort lại
```

Base case: mảng 1 phần tử → đã sort sẵn.

---

## 3. Cơ chế hoạt động từng bước

Lấy mảng: `[5, 2, 8, 1, 9, 3]`

**Giai đoạn Divide** — chia đôi đệ quy cho đến khi còn 1 phần tử:

```
                [5, 2, 8, 1, 9, 3]
                /                \
          [5, 2, 8]           [1, 9, 3]
          /       \           /       \
       [5, 2]    [8]       [1, 9]    [3]
       /    \              /    \
     [5]   [2]           [1]   [9]
```

**Giai đoạn Merge** — ghép ngược từ dưới lên:

```
[5] + [2]  → merge → [2, 5]
[2, 5] + [8] → merge → [2, 5, 8]

[1] + [9]  → merge → [1, 9]
[1, 9] + [3] → merge → [1, 3, 9]

[2, 5, 8] + [1, 3, 9] → merge:
  So 2 vs 1 → lấy 1.   [1]
  So 2 vs 3 → lấy 2.   [1, 2]
  So 5 vs 3 → lấy 3.   [1, 2, 3]
  So 5 vs 9 → lấy 5.   [1, 2, 3, 5]
  So 8 vs 9 → lấy 8.   [1, 2, 3, 5, 8]
  Còn [9]   → lấy hết. [1, 2, 3, 5, 8, 9]
```

Kết quả: `[1, 2, 3, 5, 8, 9]` ✓

---

## 4. Cài đặt C++20

```cpp
#include <iostream>
#include <vector>
#include <ranges>   // C++20
#include <span>     // C++20 — zero-copy subarray view

// Merge hai nửa [left, mid) và [mid, right) của arr
// Dùng buffer tạm để tránh in-place shift tốn kém
void merge(std::vector<int>& arr, int left, int mid, int right) {
    // Tạo buffer tạm chứa vùng [left, right)
    std::vector<int> tmp(arr.begin() + left, arr.begin() + right);

    int i = 0;                  // con trỏ nửa trái trong tmp
    int j = mid - left;         // con trỏ nửa phải trong tmp
    int k = left;               // con trỏ ghi vào arr gốc

    int left_end  = mid - left; // độ dài nửa trái
    int right_end = right - left; // độ dài toàn vùng

    // So sánh và ghi phần tử nhỏ hơn trước
    while (i < left_end && j < right_end) {
        // Dùng <= thay vì < để đảm bảo STABLE SORT
        // (phần tử bằng nhau: ưu tiên nửa trái → giữ thứ tự gốc)
        if (tmp[i] <= tmp[j]) {
            arr[k++] = tmp[i++];
        } else {
            arr[k++] = tmp[j++];
        }
    }

    // Đổ phần còn dư của nửa nào chưa hết
    while (i < left_end)  arr[k++] = tmp[i++];
    while (j < right_end) arr[k++] = tmp[j++];
    // Lưu ý: chỉ một trong hai while trên thực sự chạy
}

// Đệ quy sort vùng [left, right) — half-open interval
void mergeSort(std::vector<int>& arr, int left, int right) {
    // Base case: 0 hoặc 1 phần tử → đã sort
    if (right - left <= 1) return;

    int mid = left + (right - left) / 2; // tránh overflow so với (left+right)/2

    mergeSort(arr, left, mid);   // sort nửa trái
    mergeSort(arr, mid, right);  // sort nửa phải
    merge(arr, left, mid, right); // ghép lại
}

int main() {
    std::vector<int> arr = {5, 2, 8, 1, 9, 3};

    std::cout << "Before: ";
    for (int x : arr) std::cout << x << ' ';
    std::cout << '\n';

    mergeSort(arr, 0, static_cast<int>(arr.size()));

    std::cout << "After:  ";
    for (int x : arr) std::cout << x << ' ';
    std::cout << '\n';

    return 0;
}
```

**Compile:**
```bash
g++ -std=c++20 -O2 -Wall -Wextra -o merge_sort merge_sort.cpp && ./merge_sort
```

**Output:**
```
Before: 5 2 8 1 9 3
After:  1 2 3 5 8 9
```

---

## 5. Độ phức tạp

### Time Complexity: O(n log n)

**Tại sao log n tầng?**

Mỗi lần chia đôi, kích thước bài toán giảm một nửa. Từ n phần tử:

```
Tầng 0: 1 mảng kích thước n
Tầng 1: 2 mảng kích thước n/2
Tầng 2: 4 mảng kích thước n/4
...
Tầng k: 2^k mảng kích thước n/2^k
```

Dừng khi `n/2^k = 1` → `k = log₂(n)`. Vậy có **log₂(n) tầng**.

**Tại sao mỗi tầng là O(n)?**

Ở mỗi tầng, tổng công việc merge = tổng kích thước tất cả các mảng ở tầng đó:

```
Tầng 1: 2 lần merge, mỗi lần n/2 phần tử → 2 × (n/2) = n
Tầng 2: 4 lần merge, mỗi lần n/4 phần tử → 4 × (n/4) = n
...
Tầng k: 2^k lần merge, mỗi lần n/2^k phần tử → n
```

**Mỗi tầng luôn tốn đúng O(n)**. Có log n tầng → **Tổng: O(n log n)**.

Đây là lower bound lý thuyết cho comparison-based sorting — không thể tốt hơn O(n log n) trong trường hợp chung.

### Space Complexity: O(n)

Buffer tạm trong `merge()` tốn O(n) total (tại bất kỳ thời điểm nào, tổng buffer đang active = O(n)).

Call stack đệ quy sâu O(log n) → **bộ nhớ phụ trợ: O(n + log n) = O(n)**.

---

## 6. Khi nào dùng / không dùng

### Dùng khi:

- Cần **stable sort** — ví dụ sort danh sách user theo điểm, giữ nguyên thứ tự alphabet khi bằng điểm.
- Sort **Linked List** — Merge Sort không cần random access, trong khi Quick Sort rất khó implement trên linked list.
- Dữ liệu nằm trên **external storage** (file, disk) — có thể merge từng chunk, không cần load toàn bộ vào RAM. Đây là lý do lịch sử Merge Sort ra đời.
- Cần **worst-case guarantee O(n log n)** — Quick Sort worst-case O(n²).

### Không dùng khi:

- **Bộ nhớ bị giới hạn chặt** — Merge Sort cần O(n) extra space, in-place version rất phức tạp và ít dùng thực tế.
- **Mảng nhỏ** (n < ~20) — overhead đệ quy không đáng, Insertion Sort nhanh hơn trên thực tế. Đây là lý do `std::sort` trong STL dùng Introsort (kết hợp Quick Sort + Heap Sort + Insertion Sort).
- **Cache performance quan trọng** — Quick Sort in-place có cache locality tốt hơn vì không cần buffer tạm.

### Pitfall thường gặp:

```cpp
// BUG: tràn int khi n lớn
int mid = (left + right) / 2;  // ❌

// ĐÚNG:
int mid = left + (right - left) / 2;  // ✓
```

```cpp
// BUG: không stable — dùng < thay vì <=
if (tmp[i] < tmp[j]) { ... }  // ❌ — khi bằng nhau lấy nửa phải trước

// ĐÚNG:
if (tmp[i] <= tmp[j]) { ... }  // ✓ — khi bằng nhau ưu tiên nửa trái
```

---

## 7. So sánh với thuật toán tương tự

| Tiêu chí | Merge Sort | Quick Sort | Heap Sort | Tim Sort |
|---|---|---|---|---|
| **Time (avg)** | O(n log n) | O(n log n) | O(n log n) | O(n log n) |
| **Time (worst)** | O(n log n) | **O(n²)** | O(n log n) | O(n log n) |
| **Space** | **O(n)** | O(log n) | **O(1)** | O(n) |
| **Stable** | **✓** | ✗ | ✗ | **✓** |
| **Cache** | Kém | **Tốt** | Kém | Tốt |
| **Linked List** | **Tốt** | Kém | Không phù hợp | — |
| **Dùng trong STL** | Không | Một phần | Fallback | Python/Java |

**Quick Sort** nhanh hơn trong thực tế vì cache locality tốt, nhưng không stable và có worst-case O(n²) (có thể mitigate bằng random pivot).

**Heap Sort** dùng O(1) extra space nhưng cache performance kém — trên CPU hiện đại cache miss rất tốn, nên Heap Sort thường chậm hơn Merge Sort mặc dù lý thuyết tốt hơn về bộ nhớ.

**Tim Sort** (Python `sorted()`, Java `Arrays.sort()` cho objects) thực chất là Merge Sort tối ưu — phát hiện và tận dụng các "run" (đoạn đã sort sẵn) trong dữ liệu thực tế, cực kỳ hiệu quả.

---

**Tóm lại:** Merge Sort là nền tảng của mọi stable sort production-grade. Nếu bạn hiểu Merge Sort thật sự, Tim Sort chỉ là một bước ngắn — và đó là thuật toán đang chạy trong Python, Java, Swift mỗi ngày.