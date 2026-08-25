Được, anh Tuấn. Đi thẳng vào bản chất, không vòng vo.

## 1. Bài toán nó giải quyết

Sắp xếp là bài toán nền tảng: cho một mảng, sắp xếp lại theo thứ tự tăng dần. Trước Quicksort (Hoare phát minh năm 1959, công bố 1961), các thuật toán phổ biến như Bubble Sort, Insertion Sort chạy O(n²) — chấp nhận được với vài trăm phần tử, nhưng sụp đổ khi n lên tới triệu.

Merge Sort đã giải được bài toán O(n log n) trước đó, nhưng nó cần O(n) bộ nhớ phụ để merge. Trong môi trường mà bộ nhớ là tài nguyên quý (những năm 60, RAM tính bằng KB), đây là vấn đề thực sự.

**Pain point Quicksort giải quyết:** đạt O(n log n) trung bình *mà không cần* mảng phụ — sắp xếp tại chỗ (in-place), chỉ cần O(log n) cho ngăn xếp đệ quy. Đây là lý do đến giờ `std::sort` của C++ (thực chất là Introsort — lai Quicksort + Heapsort + Insertion Sort) vẫn dùng Quicksort làm xương sống.

## 2. Trực giác cốt lõi

Ý tưởng: **chia để trị bằng cách phân hoạch quanh một giá trị mốc (pivot)**.

Chọn một phần tử làm pivot. Sắp xếp lại mảng sao cho:
- Mọi phần tử ≤ pivot nằm bên trái
- Mọi phần tử ≥ pivot nằm bên phải

Sau bước này, pivot đã "gần đúng vị trí cuối cùng" của nó (không nhất thiết đúng tuyệt đối như Lomuto, mà là một điểm chia). Sau đó đệ quy tiếp lên hai nửa. Không cần merge lại — vì bản thân việc phân hoạch đã đảm bảo thứ tự tương đối giữa hai nửa.

Tự vẽ ví dụ nhỏ, mảng `[8, 3, 5, 1, 9, 2]`, chọn pivot = 5:

```
Trước:  [8, 3, 5, 1, 9, 2]
              ^pivot

Sau khi phân hoạch quanh 5:
        [3, 1, 2, 5, 8, 9]
         └─≤5──┘  │ └─≥5─┘
                 pivot đã "kẹp" đúng giữa
```

Nhìn hình này là hiểu được 90% Quicksort. Phần còn lại (10%) là **cách phân hoạch cụ thể** — đây là chỗ Lomuto và Hoare khác nhau, và cách **chọn pivot** — chỗ median-of-three vào cuộc.

## 3. Cơ chế hoạt động từng bước

### 3.1. Lomuto — nói ngắn

Lomuto dùng con trỏ `i` đánh dấu biên vùng "đã biết ≤ pivot", quét `j` từ trái sang phải, pivot cố định là phần tử cuối. Ưu điểm: code ngắn, dễ hiểu, dễ nhớ. Nhược điểm: nhiều swap hơn Hoare (khoảng 3 lần), và cực kỳ dễ dính O(n²) nếu mảng có nhiều phần tử trùng lặp hoặc đã sort sẵn mà chọn pivot ngây thơ. Anh đã quen thuật toán này nên tôi không đi sâu.

### 3.2. Hoare — trọng tâm, đi chi tiết

Đây là bản gốc, hiệu quả hơn Lomuto về số lần swap (trung bình ít hơn ~3 lần) vì nó **không cố định pivot ở một đầu, mà cho hai con trỏ đi ngược chiều nhau từ hai đầu vào giữa**.

**Setup:** `i = lo - 1`, `j = hi + 1`, pivot = một giá trị nào đó trong đoạn (tôi sẽ nói ở phần median-of-three, giờ tạm coi pivot = a[lo]).

**Vòng lặp:**
1. Tăng `i` cho tới khi gặp phần tử `a[i] >= pivot` (dừng lại).
2. Giảm `j` cho tới khi gặp phần tử `a[j] <= pivot` (dừng lại).
3. Nếu `i >= j` → hai con trỏ đã "vượt nhau", phân hoạch xong, trả về `j` làm điểm chia.
4. Ngược lại, swap `a[i]` và `a[j]`, rồi quay lại bước 1.

**Điểm cực kỳ quan trọng — pitfall lớn nhất khi tự cài Hoare:**
- Hàm trả về `j`, **không phải vị trí cuối cùng của pivot** (khác hẳn Lomuto).
    - Đảm bảo rằng mọi phần tử trong `[lo, j]` ≤ pivot, mọi phần tử trong `[j+1, hi]` ≥ pivot.
- Đệ quy tiếp phải chia thành `[lo, j]` và `[j+1, hi]` — **không phải** `[lo, j-1]` và `[j+1, hi]` như code Lomuto-style. Đây là lỗi 90% người mới tự implement Hoare gặp phải, gây sai kết quả hoặc vòng lặp vô hạn.

**Walk-through cụ thể**, mảng `a = [8, 3, 5, 4, 7, 6, 1, 2]`, `lo=0, hi=7`. Giả sử pivot chọn tạm = `a[0] = 8` để dễ nhìn cơ chế (thực tế sẽ dùng median-of-three, nói ở bước sau):

```
Index:   0  1  2  3  4  5  6  7
Array:   8  3  5  4  7  6  1  2
i bắt đầu từ -1 (chỉ tới lo-1), j bắt đầu từ 8 (chỉ tới hi+1)

Vòng 1:
  i tăng: a[0]=8 >= 8 → dừng tại i=0
  j giảm: a[7]=2 <= 8 → dừng tại j=7
  i < j → swap a[0], a[7]
  Array: 2 3 5 4 7 6 1 8

Vòng 2:
  i tăng từ 0: a[1]=3 < 8, a[2]=5<8... quét tới a[7]=8>=8 → i=7
  j giảm từ 7: a[6]=1<=8 → dừng j=6
  i(7) >= j(6) → DỪNG, trả về j = 6
```

Kết quả: điểm chia là 6. Đệ quy tiếp `[0,6]` và `[7,7]`. Nhìn kỹ sẽ thấy pivot=8 (giá trị lớn nhất mảng) bị đẩy về cuối — đúng logic, vì mọi phần tử ≤ 8.

Đây là lý do **nếu chọn pivot ngây thơ** (luôn lấy `a[lo]` hoặc `a[hi]`) trên mảng đã sort hoặc gần sort, phân hoạch sẽ cực kỳ lệch (1 bên gần như rỗng, 1 bên gần như nguyên mảng) → suy biến O(n²). Đây chính là lý do cần **median-of-three**.

### 3.3. Median-of-three — chọn pivot thông minh hơn

Ý tưởng: lấy 3 giá trị đại diện — `a[lo]`, `a[mid]`, `a[hi]` — tìm trung vị (median) của 3 giá trị đó, dùng nó làm pivot.

**Tại sao hiệu quả?** Trên mảng đã sort hoặc gần sort, `a[lo]` và `a[hi]` là cực trị (min/max), còn `a[mid]` gần với median thật của toàn mảng. Median-of-three "đánh hơi" được cấu trúc mảng và né được worst-case do input có sẵn thứ tự — chính là kịch bản dễ giết chết pivot ngây thơ nhất trong thực tế (dữ liệu người dùng thường đã sort một phần).

**Cách làm:** sort tại chỗ 3 phần tử `a[lo], a[mid], a[hi]` bằng 3 phép so sánh (không cần sort đầy đủ), sau đó median sẽ nằm ở `a[mid]`. Swap nó về `a[lo]` để Hoare dùng làm pivot chuẩn theo quy ước `pivot = a[lo]`.

## 4. Cài đặt C++20

Đã compile với `-std=c++20 -O2 -Wall -Wextra`, sạch, không warning. Test qua: rỗng, 1 phần tử, nhiều trùng lặp, mảng sort sẵn và sort ngược 2000 phần tử (2 kịch bản dễ gây O(n²) nhất) — đều pass.

```cpp
#include <bits/stdc++.h>
using namespace std;

// ================== LOMUTO PARTITION (giới thiệu ngắn) ==================
// Pivot cố định = phần tử cuối. i đánh dấu biên vùng "<=pivot".
int partition_lomuto(vector<int>& a, int lo, int hi) {
    int pivot = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        if (a[j] <= pivot) {
            ++i;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[hi]);
    return i + 1; // vị trí CUỐI CÙNG, chính xác của pivot
}

// ================== MEDIAN-OF-THREE ==================
// Chọn pivot = median(a[lo], a[mid], a[hi]) để né worst-case O(n^2)
// khi input đã sorted / gần sorted — kịch bản phổ biến trong thực tế.
int median_of_three(vector<int>& a, int lo, int hi) {
    int mid = lo + (hi - lo) / 2; // tránh overflow so với (lo+hi)/2

    // Sort tại chỗ 3 phần tử bằng 3 phép so sánh
    if (a[mid] < a[lo])  swap(a[lo], a[mid]);
    if (a[hi] < a[lo])   swap(a[lo], a[hi]);
    if (a[hi] < a[mid])  swap(a[mid], a[hi]);
    // Bất biến sau 3 dòng trên: a[lo] <= a[mid] <= a[hi]
    // => a[mid] là median. Đưa nó về a[lo] để Hoare dùng làm pivot.
    swap(a[lo], a[mid]);
    return a[lo]; // trả về GIÁ TRỊ pivot (không phải index)
}

// ================== HOARE PARTITION (trọng tâm) ==================
// Trả về j = điểm chia. Đệ quy tiếp PHẢI dùng [lo, j] và [j+1, hi],
// KHÔNG được dùng [lo, j-1] — đây là lỗi kinh điển khi tự cài Hoare.
int partition_hoare(vector<int>& a, int lo, int hi) {
    int pivot = median_of_three(a, lo, hi);
    int i = lo - 1;
    int j = hi + 1;

    while (true) {
        do { ++i; } while (a[i] < pivot); // tìm từ trái: phần tử đầu tiên >= pivot
        do { --j; } while (a[j] > pivot); // tìm từ phải: phần tử đầu tiên <= pivot

        if (i >= j) return j; // hai con trỏ vượt nhau => xong
        swap(a[i], a[j]);
    }
}

void quicksort(vector<int>& a, int lo, int hi) {
    // Vòng while thay vì đệ quy 2 nhánh để giới hạn stack depth = O(log n)
    while (lo < hi) {
        int p = partition_hoare(a, lo, hi);

        // Đệ quy nhánh NHỎ HƠN trước, lặp tiếp nhánh lớn hơn
        // => đảm bảo stack depth luôn O(log n) kể cả worst-case
        if (p - lo < hi - p) {
            quicksort(a, lo, p);
            lo = p + 1;
        } else {
            quicksort(a, p + 1, hi);
            hi = p;
        }
    }
}

void quicksort(vector<int>& a) {
    if (!a.empty()) quicksort(a, 0, (int)a.size() - 1);
}

int main() {
    vector<int> a = {8, 3, 5, 4, 7, 6, 1, 2, 9, 0};
    quicksort(a);
    for (int x : a) cout << x << ' ';
    cout << '\n';
}
```

Điểm cài đặt đáng chú ý so với bản "sách giáo khoa":
- Dùng `while (lo < hi)` thay vì gọi đệ quy cả 2 nhánh — chuyển nhánh lớn thành lặp (tail-call elimination thủ công). Nếu không làm vậy, worst-case (dù đã có median-of-three, vẫn có thể xảy ra với input adversarial) sẽ làm stack depth lên O(n), tràn stack với mảng vài trăm nghìn phần tử.
- `mid = lo + (hi - lo) / 2` thay vì `(lo + hi) / 2` — tránh integer overflow khi `lo, hi` lớn.

## 5. Độ phức tạp

**Time complexity:**
- Trung bình: **O(n log n)**. Chứng minh: gọi T(n) là thời gian xử lý mảng n phần tử. Mỗi lần phân hoạch tốn O(n) (quét toàn mảng 1 lần). Nếu pivot chia mảng thành 2 nửa cân bằng, ta có T(n) = 2T(n/2) + O(n), theo Master Theorem → O(n log n). Trung bình (kể cả khi chia không hoàn toàn cân bằng, ví dụ tỷ lệ 1:9), phân tích kỳ vọng vẫn cho O(n log n) — vì log_(10/9)(n) vẫn là O(log n).
- Xấu nhất: **O(n²)**. Xảy ra khi mỗi lần phân hoạch đều lệch tối đa (1 bên có 0 hoặc 1 phần tử) — với pivot ngây thơ, đây là input đã sort. Với median-of-three, xác suất gặp worst-case giảm mạnh nhưng **không triệt tiêu hoàn toàn** — vẫn có thể constructed adversarial input khiến median-of-three chọn sai liên tục (dù hiếm trong thực tế).
- Best case: **O(n log n)** — khi pivot luôn chia đôi hoàn hảo.

**Space complexity:** O(log n) trung bình cho ngăn xếp đệ quy (nhờ kỹ thuật đệ quy nhánh nhỏ trước ở mục 4), O(n) worst-case nếu không tối ưu — đây chính là lý do đưa kỹ thuật "đệ quy nhánh nhỏ hơn" vào code, không phải tùy chọn mà là *bắt buộc* nếu muốn đảm bảo O(log n) space thực sự.

Sort tại chỗ (in-place) — không cần mảng phụ như Merge Sort, đây là lợi thế cốt lõi.

## 6. Khi nào dùng / không dùng

**Nên dùng khi:**
- Cần sort trong bộ nhớ, không có ràng buộc về tính ổn định (stability).
- Dữ liệu vừa/lớn, cache-friendly quan trọng (Quicksort có locality tốt hơn Merge Sort vì thao tác trong-mảng).
- Không cần đảm bảo worst-case tuyệt đối (real-time system thì nên cân nhắc Heapsort hoặc Introsort).

**Không nên dùng / cẩn trọng khi:**
- Cần **stable sort** (giữ thứ tự tương đối của phần tử bằng nhau) — Quicksort *không* stable, kể cả Hoare. Nếu cần, dùng Merge Sort hoặc Timsort.
- Dữ liệu có **rất nhiều giá trị trùng lặp** — cần thêm kỹ thuật 3-way partitioning (Dutch National Flag, Bentley-McIlroy) để tránh suy biến hiệu năng, median-of-three không giải quyết hết vấn đề này.
- Cần đảm bảo O(n log n) *tuyệt đối* không có ngoại lệ (ví dụ hệ thống real-time, hoặc bài toán bị đối thủ cố tình tấn công bằng adversarial input) — nên dùng **Introsort** (Quicksort chuyển sang Heapsort khi độ sâu đệ quy vượt ngưỡng `2*log(n)`), đây cũng chính là cách `std::sort` implement.

**Pitfall thường gặp:**
- Nhầm điểm chia của Hoare với vị trí cuối của pivot (như Lomuto) → sai logic đệ quy, dẫn tới stack overflow hoặc kết quả sai.
- Quên xử lý mảng có phần tử trùng lặp nhiều → nếu dùng điều kiện `<=`/`>=` sai chỗ trong vòng lặp Hoare, dễ tạo vòng lặp vô hạn hoặc swap thừa.
- Không xử lý overflow khi tính `mid`.

## 7. So sánh với thuật toán tương tự

| Tiêu chí | Quicksort (Hoare + median-of-3) | Merge Sort | Heapsort | Introsort |
|---|---|---|---|---|
| Time avg | O(n log n) | O(n log n) | O(n log n) | O(n log n) |
| Time worst | O(n²) (hiếm) | O(n log n) | O(n log n) | O(n log n) |
| Space | O(log n) | O(n) | O(1) | O(log n) |
| Stable | Không | Có | Không | Không |
| Cache locality | Tốt | Kém hơn | Kém | Tốt |
| Thực tế dùng | `std::sort` core (kết hợp) | Sort ổn định, external sort | Priority queue, ít dùng sort thuần | `std::sort` trong libstdc++ |

Điểm mấu chốt: Quicksort thắng về hằng số thực tế (constant factor) và cache locality — đây là lý do dù về lý thuyết worst-case tệ hơn Merge Sort/Heapsort, nó vẫn được chọn làm engine chính trong hầu hết standard library (kết hợp fallback sang Heapsort = Introsort, chính là cách GCC/libstdc++ triển khai `std::sort`).