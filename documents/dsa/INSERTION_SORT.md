# INSERTION SORT — Giải thích đầy đủ

## 1. Nguyên lý (nói theo cách dễ hình dung nhất)

Tưởng tượng mảng của bạn được **chia làm 2 vùng**:

```
[ ĐÃ SẮP XẾP ] [ CHƯA SẮP XẾP ]
```

- Ban đầu: vùng "đã sắp xếp" chỉ có **1 phần tử duy nhất** — `arr[0]`. Một phần tử thì luôn tự động coi là "đã sorted" (không có gì để so sánh).
- Vùng "chưa sắp xếp" là toàn bộ phần còn lại: `arr[1..n-1]`.

**Mỗi vòng lặp làm đúng 3 bước:**

1. **Lấy** phần tử đầu tiên của vùng "chưa sắp xếp" ra (gọi là `value`).
2. **Dò ngược** từ cuối vùng "đã sắp xếp" về đầu, so sánh `value` với từng phần tử. Hễ phần tử nào **lớn hơn** `value` thì đẩy nó lùi sang phải 1 ô (shift) để nhường chỗ.
3. Khi gặp phần tử **nhỏ hơn hoặc bằng** `value` (hoặc dò tới đầu mảng), dừng lại — đó chính là chỗ trống đúng để **chèn** `value` vào.

Sau bước này, vùng "đã sắp xếp" **tăng thêm 1 phần tử**, vùng "chưa sắp xếp" **giảm đi 1 phần tử**. Lặp lại đến khi vùng "chưa sắp xếp" rỗng → toàn mảng đã sorted.

### Minh họa từng bước với mảng `[5, 2, 4, 6, 1, 3]`

```
Bước 0: [5] | 2 4 6 1 3        ← "5" mặc định đã sorted
Bước 1: [2 5] | 4 6 1 3        ← lấy 2, đẩy 5 sang phải, chèn 2 vào đầu
Bước 2: [2 4 5] | 6 1 3        ← lấy 4, đẩy 5 sang phải, chèn 4 vào giữa
Bước 3: [2 4 5 6] | 1 3        ← lấy 6, không ai lớn hơn nên đứng yên cuối
Bước 4: [1 2 4 5 6] | 3        ← lấy 1, đẩy cả 4 phần tử (2,4,5,6) sang phải
Bước 5: [1 2 3 4 5 6]          ← lấy 3, đẩy (4,5,6) sang phải, chèn vào giữa
```

Nhìn kỹ Bước 4: đây chính là **worst-case cục bộ** — `value` quá nhỏ so với cả vùng đã sorted nên phải shift rất nhiều lần. Đây là lý do worst-case toàn cục là mảng sorted **ngược hoàn toàn**.

## 2. Code minh họa — bám sát đúng 3 bước ở trên

```cpp
#include <vector>
using namespace std;

void insertionSort(vector<int> &arr)
{
    int n = arr.size();

    // Vòng lặp i chạy trên vùng "chưa sắp xếp", bắt đầu từ index 1
    // vì arr[0] mặc định đã là vùng "đã sắp xếp" (1 phần tử)
    for (int i = 1; i < n; i++)
    {
        // BƯỚC 1: Lấy phần tử đầu tiên của vùng chưa sắp xếp
        int value = arr[i];

        // j chạy dò ngược từ cuối vùng đã sắp xếp
        int j = i - 1;

        // BƯỚC 2: Dò ngược + shift các phần tử lớn hơn value sang phải
        while (j >= 0 && arr[j] > value)
        {
            arr[j + 1] = arr[j]; // đẩy phần tử lớn hơn sang phải 1 ô
            j--;
        }

        // BƯỚC 3: Chèn value vào đúng chỗ trống (ngay sau vị trí j)
        arr[j + 1] = value;

        // Debug: in trạng thái mảng sau mỗi vòng để thấy "vùng sorted" mở rộng dần
        // for (int x : arr) cout << x << " "; cout << "\n";
    }
}
```

**Đối chiếu với code gốc bạn viết:** `FOR(i, 1, n)` chính là `for(i=1; i<n; i++)` — hoàn toàn tương đương, không có gì khác biệt về logic.

## 3. Bảng độ phức tạp

| Trường hợp | Số lần compare | Số lần shift | Time | Khi nào xảy ra |
|---|---|---|---|---|
| Best | O(n) | 0 | **O(n)** | Mảng đã sorted sẵn |
| Average | O(n²) | O(n²) | **O(n²)** | Mảng random |
| Worst | O(n²) | O(n²) | **O(n²)** | Mảng sorted ngược |
| Space | — | — | **O(1)** | In-place, không cần mảng phụ |

**Insight quan trọng:** Tổng số lần shift trong toàn bộ quá trình = **số cặp nghịch thế (inversions)** của mảng gốc. Mảng càng gần sorted → càng ít inversions → chạy càng nhanh, gần tiệm cận O(n) thực tế. Đây gọi là tính chất **adaptive**.

## 4. Ứng dụng thực tế — không chỉ là bài học lý thuyết

- **Timsort** (Python `sort()`, Java `Collections.sort()`) dùng Insertion Sort cho các đoạn (run) nhỏ, thường ngưỡng ~32-64 phần tử, vì constant factor thấp thắng overhead recursive của Merge Sort ở kích thước nhỏ.
- **Introsort** (C++ `std::sort`) cũng chuyển sang Insertion Sort khi partition còn lại đủ nhỏ.
- **Online/streaming sort**: khi dữ liệu đến từng phần tử một, không biết trước toàn bộ mảng — Insertion Sort là lựa chọn tự nhiên vì mỗi phần tử mới chỉ cần chèn vào đúng chỗ trong tập đã sorted.
- **Sort Linked List**: không cần shift vật lý, chỉ relink pointer → giữ O(1) extra space, lợi thế hơn Quick Sort (vốn cần random access).

## 5. Các dạng bài toán hay gặp

**a) Đếm số cặp nghịch thế (Inversion Count)**
Vì shift count = inversion count, bài "tìm số swap tối thiểu để sort mảng" thực chất là bài đếm inversions. Solution O(n²) chính là mô phỏng Insertion Sort trực tiếp; solution tối ưu O(n log n) dùng Merge Sort hoặc Fenwick Tree (BIT).

```cpp
// Đếm inversions bằng chính cơ chế shift của Insertion Sort — O(n^2)
long long countInversions(vector<int> arr)
{
    int n = arr.size();
    long long count = 0;
    for (int i = 1; i < n; i++)
    {
        int value = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > value)
        {
            arr[j + 1] = arr[j];
            j--;
            count++; // mỗi lần shift = 1 inversion
        }
        arr[j + 1] = value;
    }
    return count;
}
```

**b) Hybrid sort tự viết** — nếu bạn tự implement Quick Sort/Merge Sort, kỹ thuật chuẩn production là chuyển sang Insertion Sort khi subarray < 10-20 phần tử để giảm constant factor.

**c) Nearly-sorted array** — các bài kiểu "mảng bị xáo trộn nhẹ (mỗi phần tử lệch tối đa k vị trí so với vị trí đúng), sort tối ưu" — Insertion Sort chạy gần O(n·k) trong trường hợp này, rất hiệu quả.

---

Gợi ý cho buổi dạy F-Code: cho sinh viên chạy thử code debug (dòng comment ở trên) để **in mảng sau mỗi vòng lặp `i`** — nhìn trực quan vùng sorted "phình to dần" sẽ giúp các bạn nhớ nguyên lý lâu hơn là chỉ đọc code suông.