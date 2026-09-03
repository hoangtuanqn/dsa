# Quick Select — Từ Lomuto đến 3-Way

---

## 1. Bài toán nó giải quyết

**Pain point:** Tìm phần tử thứ k nhỏ nhất trong mảng chưa sort.

Cách ngây thơ nhất: sort rồi lấy `arr[k]` → **O(n log n)**. Nhưng mày không cần cả mảng được sort — mày chỉ cần **1 phần tử**. Sort toàn bộ là lãng phí.

Các bài toán thực tế:
- Tìm median của mảng (k = n/2)
- Top K phần tử lớn nhất
- Percentile trong dataset lớn

Quickselect giải quyết điều này trong **O(n) average** — không cần sort toàn bộ.

---

## 2. Trực giác cốt lõi

Sau mỗi lần partition của Quicksort, pivot đã nằm **đúng vị trí của nó** trong mảng đã sort — không bao giờ dịch chuyển nữa.

```
arr = [3, 1, 4, 1, 5, 9, 2, 6]
                 ↓ partition với pivot = 4
[3, 1, 1, 2] | 4 | [5, 9, 6]
               ↑
         pivot đang ở index 4
         → nó là phần tử nhỏ thứ 5 (0-indexed: k=4)
```

Quicksort sau đó đệ quy **cả 2 nửa**. Quickselect thông minh hơn: so sánh `p` với `k`, chỉ đệ quy vào **1 nửa chứa k**.

```
Tìm k=2 (phần tử nhỏ thứ 3):
[3, 1, 1, 2] | 4 | [5, 9, 6]
      ↑
   k=2 < p=4 → chỉ đệ quy vào nửa trái
   bỏ qua hoàn toàn nửa phải
```

---

## 3. Cơ chế hoạt động — Lomuto trước

**Ví dụ:** `arr = [7, 2, 5, 1, 4]`, tìm k=1 (phần tử nhỏ thứ 2, 0-indexed).

**Lomuto partition** — pivot luôn đặt tại `arr[r]`, dùng 1 con trỏ `k` chia mảng:

```
Bước 1: median_of_three(0, 4)
  arr[0]=7, arr[2]=5, arr[4]=4
  sort 3 phần tử: arr[0]=4, arr[2]=5, arr[4]=7
  swap median vào arr[r]: swap(arr[2], arr[4])
  → arr = [4, 2, 7, 1, 5], pivot = 5

Bước 2: Lomuto partition
  k = l-1 = -1
  i=0: arr[0]=4 <= 5 → k=0, swap(arr[0], arr[0]) → [4, 2, 7, 1, 5]
  i=1: arr[1]=2 <= 5 → k=1, swap(arr[1], arr[1]) → [4, 2, 7, 1, 5]
  i=2: arr[2]=7 > 5  → bỏ qua
  i=3: arr[3]=1 <= 5 → k=2, swap(arr[2], arr[3]) → [4, 2, 1, 7, 5]
  swap(arr[k+1], arr[r]): swap(arr[3], arr[4]) → [4, 2, 1, 5, 7]
  return p = 3

Bước 3: So sánh p=3 với k=1
  k=1 < p=3 → đệ quy trái: quickselect(arr, 0, 2, 1)
  arr = [4, 2, 1, ...]

Bước 4: partition [4, 2, 1], l=0, r=2
  median_of_three: arr[0]=4, arr[1]=2, arr[2]=1
  sort: arr[0]=1, arr[1]=2, arr[2]=4
  swap median vào arr[r]: swap(arr[1], arr[2]) → arr[2]=2, pivot=2
  arr = [1, 4, 2, 5, 7]

  Lomuto:
  k = -1
  i=0: arr[0]=1 <= 2 → k=0, swap(arr[0],arr[0])
  i=1: arr[1]=4 > 2  → bỏ qua
  swap(arr[1], arr[2]): [1, 2, 4, 5, 7]
  return p = 1

Bước 5: p=1 == k=1 → return arr[1] = 2 ✓
```

---

## 4. Implementation C++20 — Lomuto

```cpp
#include <bits/stdc++.h>
using namespace std;

// Đặt median của arr[l], arr[m], arr[r] vào arr[r]
// Lomuto cần pivot ở vị trí cố định trước khi partition
int median_of_three(vector<int>& arr, int l, int r) {
    int m = l + (r - l) / 2;

    // Sort 3 phần tử để arr[l] <= arr[m] <= arr[r]
    if (arr[l] > arr[m]) swap(arr[l], arr[m]);
    if (arr[l] > arr[r]) swap(arr[l], arr[r]);
    if (arr[m] > arr[r]) swap(arr[m], arr[r]);

    // arr[m] là median — đặt vào arr[r] để Lomuto dùng làm pivot
    swap(arr[m], arr[r]);
    return arr[r];
}

// Lomuto partition: pivot tại arr[r]
// Sau partition: [< pivot] [== pivot ở index p] [> pivot]
// Đảm bảo pivot nằm ĐÚNG VỊ TRÍ trong mảng đã sort
int lomuto_partition(vector<int>& arr, int l, int r) {
    int pivot = median_of_three(arr, l, r);
    int k = l - 1; // con trỏ vùng "< pivot"

    for (int i = l; i < r; ++i) {
        if (arr[i] <= pivot) {
            ++k;
            swap(arr[i], arr[k]);
        }
    }

    // Đặt pivot vào đúng vị trí
    swap(arr[k + 1], arr[r]);
    return k + 1; // index chính xác của pivot
}

// Quickselect: tìm phần tử thứ k nhỏ nhất (0-indexed)
int quickselect(vector<int>& arr, int l, int r, int k) {
    if (l == r) return arr[l];

    int p = lomuto_partition(arr, l, r);

    if (p == k)      return arr[p];       // pivot chính là đáp án
    else if (k < p)  return quickselect(arr, l, p - 1, k); // tìm bên trái
    else             return quickselect(arr, p + 1, r, k); // tìm bên phải
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> arr(n);
    for (auto& x : arr) cin >> x;

    // k là 1-indexed từ input → chuyển sang 0-indexed
    cout << quickselect(arr, 0, n - 1, k - 1) << "\n";
}
```

---

## Vấn đề của Lomuto với mảng trùng lặp

```
arr = [3, 3, 3, 3, 3], k=2
pivot = 3
Lomuto: tất cả <= pivot → k chạy qua toàn bộ mảng
p = 4 (cuối mảng)
→ đệ quy trái với r = p-1 = 3
→ lặp lại, p = 3, r = 2 ...
→ T(n) = T(n-1) + O(n) = O(n²)
```

**Nguyên nhân:** Lomuto không phân biệt `== pivot` với `< pivot` — tất cả dồn về 1 phía.

---

## 5. Nâng cấp lên 3-Way (Dutch National Flag)

**Ý tưởng:** Chia mảng thành 3 vùng thay vì 2:

```
[< pivot] | [== pivot] | [> pivot]
    lt-1       lt..gt      gt+1
```

Toàn bộ vùng `== pivot` được bỏ qua ngay — nếu `k` nằm trong đó, trả về luôn.

**Walk-through:** `arr = [3, 1, 3, 3, 2]`, k=2

```
pivot = arr[2] = 3
i=lt=l=0, gt=r=4

i=0: arr[0]=3 == pivot → i++
  [3, 1, 3, 3, 2], lt=0, i=1, gt=4

i=1: arr[1]=1 < pivot → swap(arr[lt], arr[i]), lt++, i++
  [1, 3, 3, 3, 2], lt=1, i=2, gt=4

i=2: arr[2]=3 == pivot → i++
  lt=1, i=3, gt=4

i=3: arr[3]=3 == pivot → i++
  lt=1, i=4, gt=4

i=4: arr[4]=2 < pivot → swap(arr[lt], arr[i]), lt++, i++
  [1, 2, 3, 3, 3], lt=2, i=5, gt=4

i=5 > gt=4 → dừng
lt=2, gt=4

k=2: lt <= k <= gt → return arr[2] = 3 ✓
Mảng đã sort đúng: [1, 2, 3, 3, 3]
```

---

## 6. Implementation C++20 — 3-Way

```cpp
#include <bits/stdc++.h>
using namespace std;

// Dutch National Flag partition
// Chia thành 3 vùng: [l..lt-1] < pivot, [lt..gt] == pivot, [gt+1..r] > pivot
// lt, gt là output params — boundary của vùng == pivot
void three_way_partition(vector<int>& arr, int l, int r, int& lt, int& gt) {
    // Chọn pivot bằng median-of-three (lấy giá trị, không cần đặt vào arr[r])
    int m = l + (r - l) / 2;
    if (arr[l] > arr[m]) swap(arr[l], arr[m]);
    if (arr[l] > arr[r]) swap(arr[l], arr[r]);
    if (arr[m] > arr[r]) swap(arr[m], arr[r]);
    int pivot = arr[m]; // arr[m] là median

    int i = l;
    lt = l;
    gt = r;

    while (i <= gt) {
        if (arr[i] < pivot) {
            // Phần tử nhỏ hơn pivot → đẩy vào vùng trái
            swap(arr[lt++], arr[i++]);
        } else if (arr[i] > pivot) {
            // Phần tử lớn hơn pivot → đẩy vào vùng phải
            // KHÔNG tăng i vì arr[gt] chưa được xét
            swap(arr[i], arr[gt--]);
        } else {
            // Phần tử bằng pivot → ở lại vùng giữa
            i++;
        }
    }
    // Sau vòng lặp: arr[lt..gt] đều == pivot
}

// Quickselect 3-way: O(n) average, O(n) worst case với mảng trùng lặp
int quickselect(vector<int>& arr, int l, int r, int k) {
    if (l == r) return arr[l];

    int lt, gt;
    three_way_partition(arr, l, r, lt, gt);

    if (k < lt) {
        // k nằm trong vùng < pivot
        return quickselect(arr, l, lt - 1, k);
    } else if (k > gt) {
        // k nằm trong vùng > pivot
        return quickselect(arr, gt + 1, r, k);
    } else {
        // k nằm trong vùng == pivot → tất cả đều là đáp án
        return arr[lt];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> arr(n);
    for (auto& x : arr) cin >> x;

    cout << quickselect(arr, 0, n - 1, k - 1) << "\n";
}
```

---

## 7. Độ phức tạp

**Time complexity:**

| Case | Lomuto | 3-Way |
|---|---|---|
| Average | O(n) | O(n) |
| Worst (sorted input) | O(n²) | O(n²) |
| Worst (all duplicates) | O(n²) | **O(n)** |

**Chứng minh O(n) average:**

Mỗi lần đệ quy chỉ vào 1 nửa. Nếu pivot chia đều (lý tưởng):
```
T(n) = T(n/2) + O(n)
     = O(n) + O(n/2) + O(n/4) + ...
     = O(n · (1 + 1/2 + 1/4 + ...))
     = O(n · 2)
     = O(n)
```

**Space complexity:** O(log n) average do call stack đệ quy — có thể tối ưu thành O(1) bằng cách chuyển sang iterative.

---

## 8. Khi nào dùng / không dùng

**Dùng khi:**
- Chỉ cần phần tử thứ k, không cần toàn bộ mảng sort
- Dataset lớn, O(n log n) quá chậm
- Mảng có nhiều phần tử trùng → dùng 3-way

**Không dùng khi:**
- Cần kết quả deterministic (Quickselect random → dùng median-of-medians nếu cần O(n) worst case guaranteed)
- Mảng nhỏ (n < 20) → insertion sort nhanh hơn do overhead thấp
- Cần tìm nhiều giá trị k khác nhau → sort 1 lần dùng mãi

**Edge cases cần xử lý:**
```cpp
// k out of bounds
assert(k >= 0 && k < n);

// n = 1
if (n == 1) return arr[0];

// Mảng toàn phần tử giống nhau → 3-way xử lý được, Lomuto thì không
```

---

## 9. So sánh với thuật toán tương tự

| | Quickselect | nth_element (STL) | Median-of-Medians | Heap (partial_sort) |
|---|---|---|---|---|
| Average | O(n) | O(n) | O(n) | O(n log k) |
| Worst case | O(n²) | O(n²)* | **O(n)** | O(n log k) |
| Mảng trùng lặp | Tệ (Lomuto) / Tốt (3-way) | Tốt | Tốt | Tốt |
| Cài đặt | Trung bình | Có sẵn | Phức tạp | Đơn giản |
| Thực tế | Nhanh | Nhanh nhất | Chậm hơn do constant lớn | Dùng khi k nhỏ |

**`std::nth_element`** về cơ bản là Quickselect với introspection — dùng nó trong production thay vì tự cài. Tự cài khi cần customization hoặc competitive programming.

**Median-of-Medians** đảm bảo O(n) worst case nhưng constant factor lớn → thực tế chậm hơn Quickselect trung bình. Dùng khi cần guarantee tuyệt đối.