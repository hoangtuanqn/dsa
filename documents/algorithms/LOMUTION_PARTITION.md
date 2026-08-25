# Thuật toán Lomuto Partition — Giải thích từ gốc rễ

---

## 1. Bài toán nó giải quyết

Trước khi có Lomuto, bài toán đặt ra là:

> **Cho một mảng, hãy sắp xếp lại sao cho tất cả phần tử nhỏ hơn một giá trị X nằm bên trái X, tất cả phần tử lớn hơn nằm bên phải X — và X nằm đúng vị trí cuối cùng của nó.**

Đây là bước **partition** — trái tim của QuickSort.

**Pain point cụ thể:** Hoare partition (ra đời trước) hoạt động tốt nhưng có một vấn đề lớn về mặt sư phạm và correctness: sau khi partition xong, pivot **không nằm đúng vị trí cuối cùng**. Bạn chỉ biết "bên trái nhỏ hơn hoặc bằng, bên phải lớn hơn hoặc bằng" — nhưng pivot ở đâu trong đó thì mù. Điều này làm cho việc hiểu, debug, và extend (như QuickSelect) trở nên khó hơn.

**Nico Lomuto** — kỹ sư tại Bell Labs — muốn một variant mà sau partition, pivot luôn ngồi đúng chỗ của nó, index trả về chính là index của pivot. Đó là toàn bộ lý do Lomuto tồn tại.

---

## 2. Trực giác cốt lõi

Hãy quên code đi. Tưởng tượng bạn có một dãy thẻ bài úp mặt xuống bàn:

```
[ 5 | 3 | 8 | 1 | 9 | 2 | 7 ]
```

Bạn chọn thẻ cuối cùng làm **pivot** — đó là `7`. Bây giờ nhiệm vụ: **gom tất cả thẻ nhỏ hơn 7 về phía trái, lớn hơn về phía phải, và đặt 7 vào giữa đúng chỗ.**

**Trực giác Lomuto:** Dùng một cái "rào" — một biên giới `i` — ngăn cách vùng "đã xét và nhỏ hơn pivot" với vùng "chưa xét". Ban đầu rào này ở vị trí -1 (chưa có gì trong vùng nhỏ).

```
Vùng nhỏ hơn pivot    |    Vùng chưa xét    | pivot
[                     |  5  3  8  1  9  2   |  7  ]
 ^                                              
 i = -1 (rào)
```

Bạn lần lượt nhìn từng thẻ trong "vùng chưa xét":
- Nếu thẻ đó **nhỏ hơn hoặc bằng pivot** → kéo rào sang phải một bước, đổi chỗ thẻ đó vào vùng nhỏ
- Nếu thẻ đó **lớn hơn pivot** → bỏ qua, nó tự nhiên thuộc vùng lớn hơn

Cuối cùng, kéo rào sang phải một bước rồi đặt pivot vào đó — pivot ngồi đúng vị trí cuối cùng.

**Ví dụ nhỏ tự vẽ:**

```
arr = [3, 1, 4, 2], pivot = arr[3] = 2
       0  1  2  3

Trạng thái ban đầu:
Vùng nhỏ  | Vùng chưa xét | pivot
[]         [3, 1, 4]        [2]
i = -1,  j = 0

j=0: arr[0]=3 > 2 → skip
j=1: arr[1]=1 ≤ 2 → i=0, swap(arr[0], arr[1])
     [1, 3, 4, 2]
      ^i
j=2: arr[2]=4 > 2 → skip

Đặt pivot vào: swap(arr[i+1], arr[3]) = swap(arr[1], arr[3])
     [1, 2, 4, 3]
         ^pivot ngồi đây, index = 1
```

Pivot `2` hiện tại ở index 1 — đúng vị trí của nó trong mảng đã sort `[1, 2, 3, 4]`. Đây là điều Lomuto đảm bảo.

---

## 3. Cơ chế hoạt động từng bước

**Ví dụ đầy đủ:**
```
arr = [6, 3, 8, 2, 7, 1, 5]
       0  1  2  3  4  5  6
```

Pivot = `arr[6]` = `5`, low = 0, high = 6, `i = low - 1 = -1`

---

**Bước 1:** j = 0, `arr[0]` = 6

```
6 > 5 → bỏ qua
[ 6 | 3  8  2  7  1 | 5 ]
  ^j
i = -1 (rào chưa nhúc nhích)
```

---

**Bước 2:** j = 1, `arr[1]` = 3

```
3 ≤ 5 → i++ = 0, swap(arr[0], arr[1])
[ 3 | 6  8  2  7  1 | 5 ]
  ^i  ^j
Vùng nhỏ hơn: [3]
```

---

**Bước 3:** j = 2, `arr[2]` = 8

```
8 > 5 → bỏ qua
[ 3 | 6  8  2  7  1 | 5 ]
  ^i     ^j
```

---

**Bước 4:** j = 3, `arr[3]` = 2

```
2 ≤ 5 → i++ = 1, swap(arr[1], arr[3])
[ 3  2 | 8  6  7  1 | 5 ]
     ^i        ^j
Vùng nhỏ hơn: [3, 2]
```

---

**Bước 5:** j = 4, `arr[4]` = 7

```
7 > 5 → bỏ qua
[ 3  2 | 8  6  7  1 | 5 ]
     ^i           ^j
```

---

**Bước 6:** j = 5, `arr[5]` = 1

```
1 ≤ 5 → i++ = 2, swap(arr[2], arr[5])
[ 3  2  1 | 6  7  8 | 5 ]
        ^i           ^j
Vùng nhỏ hơn: [3, 2, 1]
```

---

**Bước cuối — đặt pivot vào đúng chỗ:**

```
swap(arr[i+1], arr[high]) = swap(arr[3], arr[6])
[ 3  2  1  5  7  8  6 ]
            ^
        pivot ở index 3
return 3
```

Kiểm tra: `[3, 2, 1]` < 5 < `[7, 8, 6]` ✓
Vị trí index 3 trong sorted array `[1, 2, 3, 5, 6, 7, 8]` là `5` ✓

---

## 4. Cài đặt C++20

```cpp
#include <algorithm>  // std::swap
#include <concepts>   // std::totally_ordered
#include <vector>
#include <iostream>
#include <span>

// Concept: chỉ chấp nhận kiểu có thể so sánh thứ tự toàn phần
// Ví dụ: int, double, string — không phải struct tùy tiện
template<std::totally_ordered T>
int lomuto_partition(std::span<T> arr, int low, int high) {
    // Chọn phần tử cuối làm pivot
    // Đây là lựa chọn đơn giản nhất — trade-off sẽ bàn ở phần complexity
    T pivot = arr[high];

    // i là "rào ngăn cách":
    // arr[low..i]   → vùng đã xét, tất cả ≤ pivot
    // arr[i+1..j-1] → vùng đã xét, tất cả > pivot
    // arr[j..high-1] → vùng chưa xét
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            // Phần tử này thuộc về vùng nhỏ hơn
            // Mở rộng rào sang phải, đưa arr[j] vào vùng nhỏ
            ++i;
            std::swap(arr[i], arr[j]);
        }
        // Nếu arr[j] > pivot: không làm gì
        // j tự tăng cuối vòng lặp, arr[j] "tự nhiên" nằm vào vùng lớn hơn
    }

    // Đặt pivot vào đúng vị trí của nó
    // Mọi phần tử bên trái (arr[low..i]) đều ≤ pivot
    // Mọi phần tử bên phải (arr[i+2..high]) đều > pivot
    std::swap(arr[i + 1], arr[high]);

    // Trả về index chính xác của pivot trong mảng đã partition
    // Đây là điểm khác biệt then chốt so với Hoare
    return i + 1;
}

template<std::totally_ordered T>
void quicksort(std::span<T> arr, int low, int high) {
    if (low >= high) return;  // Base case: 0 hoặc 1 phần tử

    // p là vị trí CHÍNH XÁC của pivot sau partition
    // arr[p] đã ở đúng chỗ vĩnh viễn — không cần đụng nữa
    int p = lomuto_partition(arr, low, high);

    // Đệ quy KHÔNG include p — vì p đã đúng chỗ rồi
    // Đây là lý do tại sao cần pivot ở đúng vị trí sau partition
    quicksort(arr, low, p - 1);
    quicksort(arr, p + 1, high);
}

int main() {
    std::vector<int> arr = {6, 3, 8, 2, 7, 1, 5};

    quicksort<int>(arr, 0, static_cast<int>(arr.size()) - 1);

    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";
    // Output: 1 2 3 5 6 7 8
}
```

Compile và chạy:
```bash
g++ -std=c++20 -O2 -Wall -o lomuto main.cpp && ./lomuto
```

---

## 5. Độ phức tạp

### Time Complexity

**Average case: O(n log n)**

Mỗi lần partition, pivot chia mảng thành hai nửa. Nếu pivot luôn chia đôi hoàn hảo:

```
T(n) = 2T(n/2) + O(n)
     = O(n log n)   ← theo Master Theorem, case 2
```

Với pivot ngẫu nhiên, kỳ vọng số phép so sánh là `2n ln n ≈ 1.386 n log₂n`.

**Worst case: O(n²)**

Xảy ra khi pivot luôn là phần tử nhỏ nhất hoặc lớn nhất — tức là mảng đã sort hoặc sort ngược, với pivot = last element:

```
arr = [1, 2, 3, 4, 5], pivot luôn là max
T(n) = T(0) + T(n-1) + O(n)
     = T(n-1) + O(n)
     = O(n²)
```

Recurrence tree sẽ thẳng một nhánh, không có nhánh còn lại.

**Cách phòng tránh worst case:**

```cpp
// Median-of-three: lấy median của arr[low], arr[mid], arr[high] làm pivot
// rồi swap nó về arr[high] trước khi gọi partition
int mid = low + (high - low) / 2;
// Sắp xếp 3 phần tử, lấy median → swap vào arr[high]
```

Hoặc dùng random pivot — giảm xác suất worst case về O(1/n!).

### Space Complexity

**O(log n) average, O(n) worst case** — hoàn toàn do call stack đệ quy.

Không cần mảng phụ. Mỗi frame stack chỉ lưu `low`, `high`, `i`, `j`, `pivot` — O(1) per frame. Nhưng depth của đệ quy là O(log n) average và O(n) worst case.

**Lomuto là in-place** theo nghĩa không allocate heap memory — nhưng không phải O(1) space hoàn toàn vì stack.

---

## 6. Khi nào dùng / Không dùng

### Nên dùng khi:

**QuickSelect — tìm kth phần tử nhỏ nhất:**

```cpp
// Lomuto lý tưởng cho bài này vì pivot luôn ở đúng vị trí
int quickselect(std::span<int> arr, int low, int high, int k) {
    if (low == high) return arr[low];
    int p = lomuto_partition(arr, low, high);
    if (p == k) return arr[p];       // pivot chính là đáp án
    if (k < p)  return quickselect(arr, low, p - 1, k);
    else        return quickselect(arr, p + 1, high, k);
}
```

Hoare không làm được gọn như vậy vì pivot không ở đúng chỗ.

**Cần code dễ đọc, dễ maintain, dễ debug** — Lomuto ít off-by-one hơn Hoare.

**Teaching/Interview** — logic tường minh, dễ giải thích.

### Không nên dùng khi:

**Performance critical với large dataset** — Hoare ít swap hơn ~3x, cache-friendly hơn.

**Mảng có nhiều phần tử trùng nhau** — Lomuto với điều kiện `<=` sẽ phân phối không đều. Dùng **3-way partition (Dutch National Flag)** thay thế:

```
arr = [3, 3, 3, 3, 3]
Lomuto: pivot=3, toàn bộ vào vùng "≤ pivot" → phân hoạch lệch hoàn toàn
```

**Production sort** — không ai dùng Lomuto thuần túy. Thực tế dùng **pdqsort** (C++ STL), **Timsort** (Python/Java), hoặc **introsort**.

### Edge cases cần nhớ:

```cpp
// 1. Mảng 1 phần tử — base case phải catch được
if (low >= high) return;  // Không phải low == high vì low > high xảy ra khi p=0

// 2. Tất cả phần tử bằng nhau — chạy được nhưng O(n²)
// 3. Mảng đã sort — worst case với pivot=last
// 4. Integer overflow khi tính mid — dùng low + (high-low)/2, không dùng (low+high)/2
```

---

## 7. So sánh với thuật toán tương tự

| Tiêu chí | Lomuto | Hoare | 3-way (DNF) |
|---|---|---|---|
| Pivot sau partition | Đúng vị trí, index chính xác | Không xác định | Toàn dải `[lt, gt]` |
| Số swap (random) | ~n/2 | ~n/6 | Phụ thuộc duplicate |
| Duplicate elements | Kém | Tốt hơn | Tốt nhất |
| QuickSelect | Tự nhiên, gọn | Phải xử lý thêm | Overkill |
| Dễ implement | ★★★★★ | ★★★☆☆ | ★★★☆☆ |
| Off-by-one risk | Thấp | Cao | Trung bình |
| Đệ quy exclude pivot | ✓ Có thể | ✗ Không | ✓ Exclude cả dải |

**So với Merge Sort:** Lomuto/QuickSort in-place O(1) heap memory, nhưng không stable và worst case O(n²). Merge Sort stable, worst O(n log n), nhưng cần O(n) extra space.

**So với Heap Sort:** Heap Sort worst case O(n log n) guaranteed, in-place — nhưng cache miss nhiều vì access pattern không locality. Trong thực tế Heap Sort chậm hơn QuickSort trên hardware hiện đại dù complexity tương đương.

---

### Tóm lại một câu

Lomuto là Partition scheme đánh đổi số lần swap (nhiều hơn Hoare) để đổi lấy một bất biến quý giá: **pivot luôn ngồi đúng chỗ sau partition** — và chính bất biến đó làm cho QuickSelect trở nên trivial, code trở nên tường minh, và reasoning về correctness trở nên dễ hơn nhiều.