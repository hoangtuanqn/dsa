# Bubble Sort — Phân tích toàn diện

## 1. Nguyên lý sâu hơn

Bubble Sort dựa trên **loop invariant**: sau pass thứ `i`, `i` phần tử lớn nhất đã nằm đúng vị trí cuối mảng — và điều này được **chứng minh bằng quy nạp**.

```
Mảng: [5, 3, 8, 1, 2]

Pass 1: so sánh từng cặp liền kề
  [5,3] → swap → [3,5,8,1,2]
  [5,8] → OK
  [8,1] → swap → [3,5,1,8,2]
  [8,2] → swap → [3,5,1,2,8]  ← 8 "nổi" lên cuối ✓

Pass 2: bỏ phần tử cuối, lặp lại
  [3,5] → OK
  [5,1] → swap → [3,1,5,2,8]
  [5,2] → swap → [3,1,2,5,8]  ← 5 về đúng chỗ ✓

Pass 3:
  [3,1] → swap → [1,3,2,5,8]
  [3,2] → swap → [1,2,3,5,8]  ✓

Pass 4: không có swap → early termination
```

**Tính chất quan trọng: Stable Sort**
Hai phần tử bằng nhau **không bao giờ bị swap** vì điều kiện là `arr[j] > arr[j+1]` (không có dấu `=`). Thứ tự tương đối được bảo toàn — đây là lý do Bubble Sort được gọi là **stable**.

---

## 2. Độ phức tạp — phân tích từng case

### Worst Case — O(n²)
Mảng **sorted ngược** hoàn toàn: `[5,4,3,2,1]`

Số phép so sánh:
```
Pass 1: n-1 lần
Pass 2: n-2 lần
...
Pass n-1: 1 lần

Tổng = (n-1) + (n-2) + ... + 1 = n(n-1)/2 ≈ O(n²)
```

### Best Case — O(n)
Mảng **đã sorted**: `[1,2,3,4,5]`

Pass 1 duyệt `n-1` lần, không có swap nào → flag `swapped = false` → **dừng ngay**. Chỉ tốn O(n).

> Lưu ý: Best case O(n) chỉ đạt được khi có early termination. Không có flag `swapped` thì vẫn là O(n²) dù mảng đã sorted.

### Average Case — O(n²)
Trung bình mảng ngẫu nhiên vẫn cần ~n²/4 phép so sánh — cùng bậc với worst case.

### Space — O(1)
In-place hoàn toàn. Biến `temp` hoặc `swap()` là hằng số, không phụ thuộc n.

### So sánh thực tế với n = 100.000

| Thuật toán | Số phép tính ước tính |
|---|---|
| Bubble Sort | ~5.000.000.000 |
| Merge Sort | ~1.700.000 |
| Timsort | ~100.000–300.000 |

Bubble Sort **thua tuyệt đối** ở scale thực tế.

---

## 3. Tính thực tế

**Thẳng thắn mà nói: gần như không có giá trị production.**

| Tiêu chí | Đánh giá |
|---|---|
| Performance | ❌ O(n²) — chậm nhất trong các sort phổ biến |
| Memory | ✅ O(1) in-place |
| Stable | ✅ Bảo toàn thứ tự phần tử bằng nhau |
| Implement | ✅ Đơn giản nhất |
| Cache-friendly | ✅ Truy cập tuần tự, ít cache miss |

Trường hợp thực tế duy nhất Bubble Sort có lợi thế nhỏ:

- **Mảng gần như đã sorted** (nearly sorted), n nhỏ, cần detect "đã sorted chưa" với chi phí thấp nhất — nhưng Insertion Sort vẫn làm tốt hơn trong case này.
- **Hệ thống embedded cực kỳ giới hạn bộ nhớ**, không thể dùng recursion (loại Merge Sort, Quick Sort).

---

## 4. Bài toán có thể giải quyết

### 4.1 Bài toán thuần túy (học thuật / competitive)

**Đếm số lần swap của Bubble Sort**
Số swap của Bubble Sort trên một mảng = **số cặp nghịch thế (inversion)** trong mảng đó.

```
Inversion: cặp (i,j) mà i < j nhưng arr[i] > arr[j]

[3,1,2] có 2 inversions: (3,1) và (3,2)
→ Bubble Sort cần đúng 2 swap
```

Đây là bài toán xuất hiện nhiều trong competitive programming: *"Tìm số swap tối thiểu để sort mảng bằng adjacent swap"* — đáp án chính là số inversion, tính bằng **Merge Sort hoặc BIT trong O(n log n)**.

**Ví dụ bài điển hình:**
- VNOI: bài đếm inversion
- Codeforces: các bài liên quan adjacent swap

---

### 4.2 Ứng dụng tính chất Stable Sort

Khi cần sort theo nhiều tiêu chí, stable sort giữ thứ tự pass trước:

```cpp
// Sort sinh viên: ưu tiên GPA giảm dần, cùng GPA thì giữ thứ tự nhập ban đầu
// Dùng stable sort để đảm bảo tính chất này
```

Tuy nhiên `std::stable_sort` trong C++ STL (Merge Sort biến thể) làm tốt hơn nhiều.

---

### 4.3 Phát hiện mảng đã sorted — O(n)

```cpp
bool isSorted(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        if (arr[i] > arr[i + 1]) return false;
    return true;
}
```

Về bản chất đây là 1 pass của Bubble Sort với early termination — O(n).

---

### 4.4 Cocktail Shaker Sort — biến thể 2 chiều

Bubble Sort chuẩn chỉ đẩy phần tử lớn về cuối. Cocktail sort chạy **cả 2 chiều** mỗi pass:

```cpp
void cocktailSort(vector<int>& arr) {
    int left = 0, right = arr.size() - 1;
    while (left < right) {
        for (int i = left; i < right; i++)      // đẩy max về cuối
            if (arr[i] > arr[i+1]) swap(arr[i], arr[i+1]);
        right--;
        for (int i = right; i > left; i--)      // đẩy min về đầu
            if (arr[i] < arr[i-1]) swap(arr[i], arr[i-1]);
        left++;
    }
}
```

Giảm số pass xuống còn ~n/2 nhưng vẫn O(n²) — cải thiện hằng số, không cải thiện bậc.

---

## 5. Kết luận thực dụng

```
Dùng Bubble Sort khi:
  ✅ Học thuật, dạy khái niệm sorting
  ✅ Bài toán đếm inversion / số adjacent swap
  ✅ n < 50, code nhanh không cần tối ưu

Không dùng Bubble Sort khi:
  ❌ n > 1000 trong bất kỳ hoàn cảnh production nào
  ❌ Cần performance thực tế → std::sort (Introsort, O(n log n))
  ❌ Cần stable sort thực tế → std::stable_sort
```

Bubble Sort quan trọng không phải vì nó nhanh — mà vì nó là nền để hiểu **inversion, stability, loop invariant** — những khái niệm nền tảng của mọi thuật toán sort phức tạp hơn.