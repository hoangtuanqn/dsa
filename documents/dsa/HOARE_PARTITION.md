# Thuật toán Hoare Partition — Giải thích từ gốc rễ

---

## 1. Bài toán nó giải quyết

Năm 1959, **Sir Charles Antony Richard Hoare** — lúc đó 25 tuổi, đang làm luận văn tại Moscow State University — cần sort một danh sách từ tiếng Nga để tra từ điển. Ông nghĩ ra QuickSort, và Hoare Partition là trái tim của nó.

**Pain point cụ thể ông giải quyết:**

Các thuật toán sort trước đó (Bubble, Insertion, Selection) đều có một điểm chung: chúng so sánh và di chuyển từng phần tử **một bước một**, không tận dụng được cấu trúc của mảng. Worst case luôn là O(n²).

Hoare đặt câu hỏi khác hẳn:

> *"Thay vì tìm vị trí đúng cho từng phần tử một, tại sao không chia mảng thành hai nửa — một nửa chắc chắn nhỏ hơn, một nửa chắc chắn lớn hơn — rồi đệ quy?"*

Đây là tư duy **Divide & Conquer** được áp dụng triệt để. Và Hoare Partition là cơ chế thực hiện việc chia đó.

**Điểm khác biệt cốt lõi so với Lomuto** (ra đời sau):

Hoare không quan tâm đến việc pivot nằm đúng chỗ sau partition. Ông chỉ cần đảm bảo một bất biến đơn giản hơn nhiều:

> *"Tất cả phần tử bên trái ≤ pivot, tất cả phần tử bên phải ≥ pivot."*

Pivot có thể nằm bất kỳ đâu trong hai vùng đó — không cần biết. Và chính sự "lỏng lẻo" này làm cho Hoare ít swap hơn Lomuto khoảng 3 lần.

---

## 2. Trực giác cốt lõi

Quên code đi. Tưởng tượng bạn đứng trước một hàng người xếp thành một dãy, mỗi người cầm một tờ số:

```
[ 5  3  8  1  9  2  7  4  6 ]
  ^                         ^
  Người từ trái             Người từ phải
```

Bạn chọn pivot = `5` (phần tử đầu). Nhiệm vụ: **gom người cầm số nhỏ về bên trái, số lớn về bên phải**.

**Cách Hoare làm:** Cử hai "trọng tài" — một đứng từ đầu trái đi vào, một đứng từ đầu phải đi vào.

- Trọng tài trái: tiến vào cho đến khi gặp người cầm số **≥ pivot** — người này đứng sai chỗ (đáng lẽ phải ở bên phải)
- Trọng tài phải: tiến vào cho đến khi gặp người cầm số **≤ pivot** — người này cũng đứng sai chỗ (đáng lẽ phải ở bên trái)

Hai người sai chỗ này → **đổi chỗ cho nhau**. Cả hai đều về đúng vùng. Lặp lại đến khi hai trọng tài gặp nhau.

**Vẽ tay ví dụ nhỏ:**

```
arr = [4, 7, 2, 9, 1], pivot = arr[0] = 4
       0  1  2  3  4

Trọng tài trái  (i) bắt đầu từ -1, tăng trước rồi kiểm tra
Trọng tài phải (j) bắt đầu từ  5, giảm trước rồi kiểm tra

Vòng 1:
  i tiến: arr[0]=4 ≥ 4 → dừng, i=0
  j lùi:  arr[4]=1 ≤ 4 → dừng, j=4
  i=0 < j=4 → swap(arr[0], arr[4])
  [ 1  7  2  9  4 ]

Vòng 2:
  i tiến: arr[1]=7 ≥ 4 → dừng, i=1
  j lùi:  arr[3]=9 > 4 → tiếp, arr[2]=2 ≤ 4 → dừng, j=2
  i=1 < j=2 → swap(arr[1], arr[2])
  [ 1  2  7  9  4 ]

Vòng 3:
  i tiến: arr[2]=7 ≥ 4 → dừng, i=2
  j lùi:  arr[1]=2 ≤ 4 → dừng, j=1
  i=2 > j=1 → DỪNG, return j=1

Kết quả: [ 1  2 | 7  9  4 ]
                ^j=1 là điểm chia
```

Chú ý: `4` (pivot) không nằm đúng vị trí cuối cùng — nó đang ở index 4, trong khi sorted array là `[1,2,4,7,9]` nên pivot phải ở index 2. Nhưng bất biến vẫn đúng: `arr[0..1]` = `{1,2}` đều ≤ 4, `arr[2..4]` = `{7,9,4}` đều ≥ 4. Đệ quy sẽ tự lo phần còn lại.

---

## 3. Cơ chế hoạt động từng bước

**Ví dụ đầy đủ:**

```
arr = [6, 3, 8, 2, 7, 1, 5]
       0  1  2  3  4  5  6
pivot = arr[0] = 6
i = -1 (sẽ tăng trước khi dùng)
j =  7 (sẽ giảm trước khi dùng)
```

---

**Vòng lặp 1:**

```
Bước i: do { i++ } while arr[i] < 6
  arr[0]=6, 6 < 6? Không → dừng, i=0

Bước j: do { j-- } while arr[j] > 6
  arr[6]=5, 5 > 6? Không → dừng, j=6

i=0 < j=6 → swap(arr[0], arr[6])
[ 5  3  8  2  7  1  6 ]
```

---

**Vòng lặp 2:**

```
Bước i: do { i++ } while arr[i] < 6
  arr[1]=3 < 6? Có → i=1
  arr[2]=8 < 6? Không → dừng, i=2   ← 8 đứng sai chỗ

Bước j: do { j-- } while arr[j] > 6
  arr[5]=1, 1 > 6? Không → dừng, j=5   ← 1 đứng sai chỗ

i=2 < j=5 → swap(arr[2], arr[5])
[ 5  3  1  2  7  8  6 ]
```

---

**Vòng lặp 3:**

```
Bước i: do { i++ } while arr[i] < 6
  arr[3]=2 < 6? Có → i=3
  arr[4]=7 < 6? Không → dừng, i=4   ← 7 đứng sai chỗ

Bước j: do { j-- } while arr[j] > 6
  arr[4]=7, 7 > 6? Có → j=4... wait
  
  Đúng hơn — j đang ở 5, tiếp tục giảm:
  arr[4]=7 > 6? Có → j=3
  arr[3]=2 > 6? Không → dừng, j=3   ← 2 không sai chỗ

i=4 > j=3 → DỪNG
return j = 3
```

---

**Kết quả partition:**

```
[ 5  3  1  2 | 7  8  6 ]
              ^j=3
arr[0..3] = {5,3,1,2} đều ≤ 6 ✓
arr[4..6] = {7,8,6} đều ≥ 6 ✓
```

**Đệ quy tiếp:**
- `quicksort(arr, 0, 3)` → sort `[5,3,1,2]`
- `quicksort(arr, 4, 6)` → sort `[7,8,6]`

Lưu ý quan trọng: đệ quy là `[low, j]` và `[j+1, high]` — **không exclude j** vì j không phải là vị trí chính xác của pivot.

---

## 4. Cài đặt C++20

```cpp
#include <algorithm>   // std::swap
#include <concepts>    // std::totally_ordered
#include <vector>
#include <iostream>
#include <span>
#include <stdexcept>

template<std::totally_ordered T>
int hoare_partition(std::span<T> arr, int low, int high) {
    // Chọn pivot là phần tử đầu tiên
    // Lựa chọn này ảnh hưởng trực tiếp đến worst case
    // — sẽ bàn kỹ ở phần complexity
    T pivot = arr[low];

    // i và j được khởi tạo "lệch ra ngoài" một bước
    // vì chúng ta dùng do-while: tăng/giảm TRƯỚC rồi mới kiểm tra
    // Nếu khởi tạo i=low, j=high và dùng while thông thường
    // sẽ có nguy cơ infinite loop khi arr[i] == arr[j] == pivot
    int i = low - 1;
    int j = high + 1;

    while (true) {
        // Tiến i sang phải cho đến khi gặp phần tử >= pivot
        // Phần tử này "đứng sai chỗ" — nó thuộc vùng phải
        // do-while đảm bảo i tăng ít nhất 1 lần mỗi vòng
        // → tránh infinite loop khi arr[i] == pivot
        do { ++i; } while (arr[i] < pivot);

        // Tiến j sang trái cho đến khi gặp phần tử <= pivot
        // Phần tử này "đứng sai chỗ" — nó thuộc vùng trái
        do { --j; } while (arr[j] > pivot);

        // Điều kiện kết thúc: hai con trỏ gặp nhau hoặc vượt qua nhau
        // j là điểm chia — arr[low..j] ≤ pivot, arr[j+1..high] ≥ pivot
        // KHÔNG return i vì i có thể ra ngoài bounds
        if (i >= j) return j;

        // Cả hai đang chỉ vào phần tử sai vùng — đổi chỗ
        // Sau swap: arr[i] về đúng vùng trái, arr[j] về đúng vùng phải
        std::swap(arr[i], arr[j]);

        // Không cần ++i hay --j ở đây
        // do-while đầu vòng sau sẽ tự tiến một bước
        // trước khi kiểm tra điều kiện
    }
}

template<std::totally_ordered T>
void quicksort_hoare(std::span<T> arr, int low, int high) {
    // Base case: 0 hoặc 1 phần tử — không cần làm gì
    if (low >= high) return;

    // p là điểm chia, KHÔNG phải vị trí của pivot
    // arr[low..p]   → tất cả ≤ pivot (pivot có thể ở đây)
    // arr[p+1..high] → tất cả ≥ pivot (pivot có thể ở đây)
    int p = hoare_partition(arr, low, high);

    // Đệ quy INCLUDE p — khác hoàn toàn với Lomuto
    // Nếu bạn dùng [low, p-1] và [p, high] như Lomuto
    // → infinite loop khi p == low
    quicksort_hoare(arr, low, p);
    quicksort_hoare(arr, p + 1, high);
}

// Bonus: random pivot để tránh worst case
template<std::totally_ordered T>
int hoare_partition_random(std::span<T> arr, int low, int high) {
    // Chọn pivot ngẫu nhiên, swap về arr[low] trước khi partition
    // Phá vỡ pattern của sorted/reverse-sorted input
    int rand_idx = low + rand() % (high - low + 1);
    std::swap(arr[low], arr[rand_idx]);
    return hoare_partition(arr, low, high);
}

int main() {
    std::vector<int> arr = {6, 3, 8, 2, 7, 1, 5};

    quicksort_hoare<int>(arr, 0, static_cast<int>(arr.size()) - 1);

    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
    // Output: 1 2 3 5 6 7 8
}
```

Compile:
```bash
g++ -std=c++20 -O2 -Wall -o hoare main.cpp && ./hoare
```

---

## 5. Độ phức tạp

### Time Complexity

**Average case: O(n log n)**

Mỗi lần partition, `i` và `j` di chuyển ngược chiều nhau, tổng cộng quét qua đúng n phần tử — O(n) per partition.

```
T(n) = 2T(n/2) + O(n)   ← khi pivot chia đôi hoàn hảo
     = O(n log n)        ← Master Theorem case 2
```

**Tại sao Hoare ít swap hơn Lomuto ~3 lần?**

Lomuto swap mỗi khi gặp phần tử nhỏ hơn pivot — trung bình n/2 lần.

Hoare chỉ swap khi **cả hai** con trỏ đồng thời tìm thấy phần tử sai chỗ. Xác suất để một phần tử cụ thể bị swap là thấp hơn nhiều — trung bình n/6 lần trên random input.

**Worst case: O(n²)**

Xảy ra khi pivot luôn là min hoặc max — chia mảng thành `[0]` và `[n-1]`. Với pivot = `arr[low]`:
- Mảng đã sort tăng dần → pivot luôn là min → worst case
- Mảng đã sort giảm dần → pivot luôn là max → worst case

```
T(n) = T(n-1) + T(0) + O(n)
     = T(n-1) + O(n)
     = O(n²)
```

**Worst case với random pivot:** Xác suất để luôn chọn min/max là `(2/n)^n → 0` rất nhanh. Trong thực tế coi như O(n log n).

### Space Complexity

**O(log n) average, O(n) worst case** — hoàn toàn do call stack.

Không có heap allocation. Mỗi frame chỉ lưu `low`, `high`, `i`, `j`, `pivot` — O(1) per frame. Depth của đệ quy quyết định tổng space.

**Tail recursion optimization:** Nếu luôn đệ quy nhánh nhỏ hơn trước, depth tối đa là O(log n) ngay cả khi mảng lệch.

```cpp
// Kỹ thuật: luôn đệ quy nhánh nhỏ trước
// → đảm bảo stack depth O(log n) trong mọi trường hợp
while (low < high) {
    int p = hoare_partition(arr, low, high);
    if (p - low < high - p) {
        quicksort_hoare(arr, low, p);   // nhánh nhỏ hơn → đệ quy
        low = p + 1;                    // nhánh lớn hơn → loop
    } else {
        quicksort_hoare(arr, p + 1, high);
        high = p;
    }
}
```

---

## 6. Khi nào dùng / Không dùng

### Nên dùng khi:

**Performance-critical sort trên large dataset với random data:**

Hoare là nền tảng của hầu hết production sort. C++ STL `std::sort` dùng **introsort** — Hoare-based quicksort + heapsort fallback + insertion sort cho mảng nhỏ.

**Mảng có nhiều phần tử trùng nhau** (so với Lomuto):

Hoare xử lý duplicate tốt hơn tự nhiên. Khi `arr[i] == arr[j] == pivot`, cả hai dừng và swap — phân phối đều hơn. Lomuto với `<=` sẽ dồn toàn bộ duplicate vào một bên.

**Khi bạn không cần biết vị trí chính xác của pivot** sau partition.

### Không nên dùng khi:

**QuickSelect** — Lomuto tự nhiên hơn nhiều vì pivot ở đúng vị trí.

**Mảng nhỏ (n < 10~16)** — Insertion Sort nhanh hơn vì overhead thấp, cache-friendly tuyệt đối.

**Cần stable sort** — Hoare không stable. `[3a, 3b, 1]` có thể thành `[1, 3b, 3a]`.

### Pitfalls cực kỳ quan trọng:

**Pitfall 1 — Dùng while thay do-while:**

```cpp
// SAI — infinite loop khi arr[i] == arr[j] == pivot
while (arr[i] < pivot) i++;
while (arr[j] > pivot) j--;
// Nếu arr[i] == arr[j] == pivot: cả hai dừng ngay, swap, rồi lặp lại mãi mãi

// ĐÚNG — mỗi vòng i và j tiến ít nhất 1 bước
do { ++i; } while (arr[i] < pivot);
do { --j; } while (arr[j] > pivot);
```

**Pitfall 2 — Đệ quy sai bounds:**

```cpp
// SAI — infinite loop khi p == low
quicksort_hoare(arr, low, p - 1);  // Lomuto style
quicksort_hoare(arr, p, high);

// ĐÚNG với Hoare
quicksort_hoare(arr, low, p);      // include p
quicksort_hoare(arr, p + 1, high);
```

**Pitfall 3 — pivot = arr[high] với Hoare:**

```cpp
// NGUY HIỂM — j có thể xuống dưới low nếu pivot là max của mảng
// Cần guard thêm: if (j < low) return low;
// Dùng pivot = arr[low] hoặc arr[mid] an toàn hơn
```

**Pitfall 4 — Integer overflow khi tính mid:**

```cpp
int mid = (low + high) / 2;      // SAI nếu low + high > INT_MAX
int mid = low + (high - low) / 2; // ĐÚNG
```

---

## 7. So sánh với thuật toán tương tự

| Tiêu chí | Hoare | Lomuto | 3-way DNF | Introsort |
|---|---|---|---|---|
| Pivot sau partition | Không xác định | Đúng vị trí | Toàn dải `[lt,gt]` | Không xác định |
| Số swap (avg) | ~n/6 | ~n/2 | Phụ thuộc | ~n/6 |
| Duplicate elements | Tốt | Kém | Tốt nhất O(n) | Tốt |
| QuickSelect | Phức tạp hơn | Tự nhiên | Overkill | N/A |
| Off-by-one risk | Cao | Thấp | Trung bình | Cao |
| Worst case | O(n²) | O(n²) | O(n²) | O(n log n) |
| Production use | Nền tảng | Hiếm | Đặc thù | ✓ Thực tế |

**Hoare vs Merge Sort:**

Hoare in-place O(log n) stack, unstable, worst O(n²). Merge Sort cần O(n) extra heap, stable, guaranteed O(n log n). Trong competitive programming, khi cần stable sort thì dùng Merge Sort hoặc `std::stable_sort`.

**Hoare vs Heap Sort:**

Cả hai in-place, worst case Heap Sort là O(n log n) guaranteed — tốt hơn Hoare. Nhưng Heap Sort cache miss rất nhiều do access pattern nhảy lung tung trong heap. Trên hardware thực tế, Hoare với random pivot thường nhanh hơn Heap Sort 2-3x.

**Hoare vs pdqsort (pattern-defeating quicksort):**

pdqsort là Hoare + một loạt heuristic: median-of-3 pivot, insertion sort cho small array, heapsort fallback khi detect worst case, block partition để tránh branch misprediction. Đây là `std::sort` trong GCC libstdc++ và Rust's standard library. Hoare thuần túy là nền tảng lý thuyết; pdqsort là engineering thực tế trên đó.

---

### Tóm lại một câu

Hoare Partition đánh đổi sự tường minh về vị trí pivot để đổi lấy **số lần swap ít hơn ~3x và khả năng xử lý duplicate tốt hơn** — và chính những đặc tính đó làm nó trở thành nền tảng của mọi production sort engine hiện đại, từ GCC STL đến Rust standard library.