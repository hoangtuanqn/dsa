## 1. Inversion (Nghịch thế)

**Định nghĩa:** Một cặp chỉ số `(i, j)` được gọi là inversion nếu `i < j` nhưng `arr[i] > arr[j]` — tức là phần tử đứng trước lại **lớn hơn** phần tử đứng sau.

```
arr = [3, 1, 4, 2]

Các cặp inversion:
  (0,1): arr[0]=3 > arr[1]=1 ✓
  (0,3): arr[0]=3 > arr[3]=2 ✓
  (2,3): arr[2]=4 > arr[3]=2 ✓

→ Tổng: 3 inversions
```

**Ý nghĩa thực sự:** Số inversion = **mức độ "hỗn loạn"** của mảng.

```
Mảng đã sorted:   [1,2,3,4] → 0 inversions   (hoàn hảo)
Mảng ngẫu nhiên:  [3,1,4,2] → 3 inversions   (lộn xộn vừa)
Mảng sorted ngược:[4,3,2,1] → 6 inversions   (tệ nhất, n*(n-1)/2)
```

**Liên hệ trực tiếp với Bubble Sort:**

> Mỗi lần Bubble Sort thực hiện 1 adjacent swap, nó loại bỏ **đúng 1 inversion**. Vậy tổng số swap = tổng số inversion ban đầu.

```cpp
// Đếm inversion bằng Merge Sort — O(n log n)
long long mergeCount(vector<int>& arr, int l, int r) {
    if (l >= r) return 0;
    int mid = (l + r) / 2;
    long long cnt = 0;
    cnt += mergeCount(arr, l, mid);
    cnt += mergeCount(arr, mid + 1, r);

    vector<int> tmp;
    int i = l, j = mid + 1;
    while (i <= mid && j <= r) {
        if (arr[i] <= arr[j]) {
            tmp.push_back(arr[i++]);
        } else {
            cnt += (mid - i + 1); // tất cả phần tử từ i→mid đều > arr[j]
            tmp.push_back(arr[j++]);
        }
    }
    while (i <= mid) tmp.push_back(arr[i++]);
    while (j <= r)   tmp.push_back(arr[j++]);
    for (int k = l; k <= r; k++) arr[k] = tmp[k - l];
    return cnt;
}
```

**Bài toán thực tế từ inversion:**
- *"Cần tối thiểu bao nhiêu lần đổi chỗ 2 phần tử liền kề để sort mảng?"* → đáp án là số inversion.
- Đo độ tương đồng giữa 2 ranking (ví dụ: so sánh ranking người dùng vs hệ thống gợi ý).
