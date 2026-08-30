# DNF Algorithm (Dutch National Flag) — Phân tích chuyên sâu

## 1. Bài toán nó giải quyết

Pain point cụ thể: **sắp xếp một mảng chỉ có 3 loại giá trị phân biệt (hoặc phân thành 3 nhóm theo 1 pivot) trong đúng 1 lượt duyệt, không dùng thêm bộ nhớ phụ.**

Bối cảnh lịch sử: Edsger Dijkstra đặt ra bài toán này (tên gọi lấy cảm hứng từ lá cờ Hà Lan có 3 dải màu đỏ-trắng-xanh xếp liền nhau). Bài toán gốc: cho một mảng các viên bi màu đỏ/trắng/xanh xáo trộn ngẫu nhiên, sắp xếp lại sao cho cùng màu đứng cạnh nhau, theo đúng thứ tự đỏ-trắng-xanh.

Nếu dùng counting sort thông thường (đếm số lượng 0/1/2 rồi ghi đè lại mảng), bạn tốn 2 lượt duyệt (1 lượt đếm, 1 lượt ghi). DNF giải quyết trong **1 lượt duy nhất**, và quan trọng hơn — nó là nền tảng cho **3-way partition trong Quicksort**, giải quyết pain point thực sự nghiêm trọng của Quicksort thường: khi mảng có nhiều phần tử trùng lặp (duplicate keys), 2-way partition (Lomuto/Hoare) suy biến độ phức tạp về O(n²) trong trường hợp mảng gồm toàn phần tử giống nhau. DNF-based 3-way partition xử lý case này về O(n log n) ổn định.

Ứng dụng thực tế bạn sẽ gặp: LeetCode 75 "Sort Colors", 3-way Quicksort (dual-pivot quicksort mà Java dùng cho primitive array cũng dựa trên ý tưởng họ hàng với cái này), và bất kỳ bài toán nào cần partition mảng thành 3 vùng theo 1 điều kiện so sánh.

## 2. Trực giác cốt lõi

Ý tưởng: dùng **3 con trỏ** chia mảng thành 4 vùng, trong đó có 1 vùng "chưa biết" đang co dần lại cho đến khi biến mất.

Vẽ tay ra cho dễ hình dung, giả sử mảng có pivot = 1 (bài toán 0/1/2):

```
Trạng thái bất kỳ trong quá trình chạy:

  [ 0 0 0 | 1 1 | ? ? ? ? | 2 2 2 ]
   0    low-1  mid-1  mid       high  high+1   n-1
        ^low          ^mid              ^high
   
  Vùng "< pivot"   Vùng "= pivot"   Vùng "chưa xét"   Vùng "> pivot"
  [0, low-1]       [low, mid-1]     [mid, high]        [high+1, n-1]
```

Trực giác then chốt: **`mid` là con trỏ "đang khảo sát"**. Nó chạy dọc mảng, và tại mỗi vị trí, nó quyết định phần tử đó thuộc 1 trong 3 vùng rồi "đẩy" phần tử về đúng vùng bằng swap. Vùng chưa biết `[mid, high]` co lại dần từ 2 phía: `low`/`mid` đẩy biên trái vào, `high` đẩy biên phải vào. Khi `mid > high` nghĩa là vùng chưa biết rỗng → xong.

Điểm khó hiểu nhất (mình từng thấy rất nhiều bạn sai chỗ này): **tại sao gặp phần tử < pivot thì tăng mid, nhưng gặp phần tử > pivot thì KHÔNG tăng mid?**

Trả lời: vì khi `a[mid] < pivot`, ta swap `a[low]` và `a[mid]`. Trước thời điểm đó, bất biến đảm bảo `a[low]` chỉ có thể là pivot hoặc đã được xử lý — cụ thể là `a[low]` luôn là phần tử "an toàn để nhận" (nó luôn là `==pivot` khi `low < mid`, hoặc chính là `a[mid]` khi `low == mid`). Nên sau swap, phần tử mới nằm ở vị trí `mid` chắc chắn đã "biết trước" là hợp lệ (== pivot hoặc đã xử lý), an toàn để `++mid`.

Ngược lại khi `a[mid] > pivot`, ta swap với `a[high]` — nhưng `a[high]` là phần tử **chưa từng được xem xét** (nó thuộc vùng chưa biết). Sau swap, phần tử vừa đưa về vị trí `mid` là một ẩn số hoàn toàn mới — có thể lại `< pivot`, `> pivot`, hoặc `== pivot`. Vì vậy **không được** tăng `mid`, phải xét lại nó ở vòng lặp kế tiếp.

## 3. Cơ chế hoạt động từng bước

Trace tay trên mảng `[2, 0, 2, 1, 1, 0]`, pivot = 1, n = 6.

Khởi tạo: `low=0, mid=0, high=5`

| Bước | mid | a[mid] | So sánh | Hành động | Mảng sau | low | mid | high |
|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 2 | >pivot | swap(a[0],a[5]) | `[0,0,2,1,1,2]` | 0 | 0 | 4 |
| 2 | 0 | 0 | <pivot | swap(a[0],a[0]); low++; mid++ | `[0,0,2,1,1,2]` | 1 | 1 | 4 |
| 3 | 1 | 0 | <pivot | swap(a[1],a[1]); low++; mid++ | `[0,0,2,1,1,2]` | 2 | 2 | 4 |
| 4 | 2 | 2 | >pivot | swap(a[2],a[4]) | `[0,0,1,1,2,2]` | 2 | 2 | 3 |
| 5 | 2 | 1 | ==pivot | mid++ | `[0,0,1,1,2,2]` | 2 | 3 | 3 |
| 6 | 3 | 1 | ==pivot | mid++ | `[0,0,1,1,2,2]` | 2 | 4 | 3 |
| — | 4 > 3 | — | dừng vòng lặp | | `[0,0,1,1,2,2]` | | | |

Kết quả: `[0,0,1,1,2,2]` — chính xác, chỉ 1 lượt duyệt, tổng cộng 4 phép swap thực (2 swap ở bước 1,4 và 2 swap "tự nhân" vô hại ở bước 2,3 mà compiler/runtime có thể tối ưu nếu bạn thêm check `low != mid`).

## 4. Cài đặt C++20

Đã compile thật với `g++ -std=c++20 -O2 -Wall`, **0 warning**, chạy đúng cả 2 test case:

```cpp
#include <bits/stdc++.h>
using namespace std;

// Dutch National Flag partition: sắp xếp mảng chỉ gồm 3 giá trị {0, 1, 2}
// (hoặc tổng quát hơn: partition mảng thành 3 vùng <pivot, ==pivot, >pivot)
// trong 1 lượt duyệt, O(n) thời gian, O(1) bộ nhớ phụ.
void dutchNationalFlag(vector<int>& a, int pivot) {
    int low = 0;                  // biên phải của vùng "< pivot", mọi phần tử a[0..low-1] đều < pivot
    int mid = 0;                  // con trỏ duyệt hiện tại, a[low..mid-1] đều == pivot
    int high = (int)a.size() - 1; // biên trái của vùng "> pivot", a[high+1..n-1] đều > pivot

    // Bất biến vòng lặp (invariant) cần giữ đúng SAU MỖI bước:
    //   a[0 .. low-1]      : tất cả < pivot
    //   a[low .. mid-1]    : tất cả == pivot
    //   a[mid .. high]     : chưa biết (vùng đang xử lý)
    //   a[high+1 .. n-1]   : tất cả > pivot
    while (mid <= high) {
        if (a[mid] < pivot) {
            // Phần tử thuộc vùng "<pivot" -> đổi chỗ về đầu vùng mid,
            // đồng thời mở rộng vùng low. a[low] trước đó luôn "an toàn để nhận"
            // (đã biết là ==pivot hoặc chính a[mid]), nên mid an toàn để tăng.
            swap(a[low], a[mid]);
            ++low;
            ++mid;
        } else if (a[mid] > pivot) {
            // Phần tử thuộc vùng ">pivot" -> đẩy ra cuối bằng cách swap với a[high],
            // thu hẹp biên high lại. KHÔNG tăng mid vì phần tử mới nhận từ a[high]
            // là ẩn số, phải xét lại ở vòng lặp kế tiếp.
            swap(a[mid], a[high]);
            --high;
        } else {
            // a[mid] == pivot -> đã đúng vị trí logic, chỉ cần đi tiếp.
            ++mid;
        }
    }
}

int main() {
    // Test 1: bài toán kinh điển Sort Colors (LeetCode 75) với {0,1,2}
    vector<int> colors = {2, 0, 2, 1, 1, 0, 0, 2, 1, 0};
    dutchNationalFlag(colors, 1);
    cout << "Sau DNF (pivot=1): ";
    for (int x : colors) cout << x << ' ';
    cout << '\n';

    // Test 2: dùng như 3-way partition cho Quicksort với mảng có duplicate nhiều
    vector<int> arr = {5, 3, 8, 3, 9, 3, 1, 8, 3, 5};
    dutchNationalFlag(arr, 3);
    cout << "Sau 3-way partition (pivot=3): ";
    for (int x : arr) cout << x << ' ';
    cout << '\n';

    return 0;
}
```

Output thực tế:
```
Sau DNF (pivot=1): 0 0 0 0 1 1 1 2 2 2 
Sau 3-way partition (pivot=3): 1 3 3 3 3 9 8 8 5 5 
```

Note: kết quả test 2 không sort toàn phần (đó không phải mục đích) — nó chỉ đảm bảo `<3` nằm bên trái, `==3` ở giữa, `>3` bên phải (thứ tự nội bộ mỗi vùng không xác định, do swap). Đây chính xác là input bạn cần cho bước tiếp theo của 3-way Quicksort: recurse vào 2 vùng biên, bỏ qua vùng giữa.

## 5. Độ phức tạp

**Time: O(n)** — chứng minh bằng argument về "tiến độ đơn điệu" (monovariant):
- Mỗi vòng lặp, hoặc `mid` tăng (case `<pivot` và case `==pivot`), hoặc `high` giảm (case `>pivot`).
- Không có case nào giữ nguyên cả `mid` và `high`.
- Khoảng cách `high - mid` là hữu hạn (tối đa n), và giảm chặt (strictly decrease) mỗi vòng lặp.
- Vòng lặp dừng khi `mid > high` → tổng số vòng lặp ≤ n → O(n) so sánh, O(n) swap trong trường hợp xấu nhất.

Đây chính là lý do DNF nhanh hơn "đếm rồi ghi lại" (2 lượt) — vì DNF chỉ cần 1 lượt.

**Space: O(1)** — chỉ dùng 3 biến con trỏ, swap in-place, không cấp phát mảng phụ. Đây là điểm khác biệt lớn so với counting sort (cần mảng đếm phụ, dù nhỏ).

## 6. Khi nào dùng / không dùng

**Dùng khi:**
- Mảng chỉ có (hoặc bạn chỉ quan tâm) 3 nhóm giá trị phân biệt theo 1 tiêu chí so sánh (Sort Colors, phân loại theo pivot).
- Làm 3-way partition bên trong Quicksort để chống suy biến O(n²) khi dữ liệu có nhiều key trùng lặp — đây là ứng dụng quan trọng nhất trong thực chiến competitive programming.
- Cần in-place, O(1) extra space, và chỉ 1 lượt duyệt là yêu cầu bắt buộc (interview constraint).

**Không nên dùng khi:**
- Mảng có nhiều hơn 3 nhóm giá trị phân biệt cần sort hoàn chỉnh — DNF không tổng quát hóa tốt cho k > 3 nhóm (dùng counting sort hoặc radix sort thay thế).
- Cần thuật toán **stable** (giữ thứ tự tương đối ban đầu của các phần tử bằng nhau) — DNF **không stable**, vì swap phá vỡ thứ tự gốc. Nếu bài toán yêu cầu stable partition, phải dùng `std::stable_partition` hoặc cách tiếp cận khác (tốn thêm O(n) space thường).

**Pitfall thường gặp** (mình chấm bài sinh viên F-Code rất hay thấy):
1. Tăng `mid` cả trong nhánh `>pivot` — dẫn đến bỏ sót phần tử chưa xét, kết quả sai nhưng không crash, rất khó nhận ra khi test case nhỏ.
2. Điều kiện dừng viết `mid < high` thay vì `mid <= high` — bỏ sót đúng 1 phần tử cuối cùng khi `mid == high`.
3. Dùng `high = a.size()` thay vì `a.size() - 1` — off-by-one, truy cập ngoài mảng hoặc bỏ sót phần tử cuối.
4. Không unsigned/signed mismatch: nếu bạn dùng `size_t` cho `high` và mảng rỗng, `a.size() - 1` sẽ underflow thành số cực lớn → UB ngay lập tức. Đó là lý do trong code trên mình ép kiểu `(int)a.size() - 1` — luôn kiểm tra `a.empty()` trước nếu dùng kiểu unsigned.

## 7. So sánh với thuật toán tương tự

| Tiêu chí | DNF (3-way partition) | 2-way partition (Lomuto/Hoare) | Counting Sort |
|---|---|---|---|
| Số lượt duyệt | 1 | 1 | 2 (đếm + ghi) |
| Extra space | O(1) | O(1) | O(k) với k = số giá trị phân biệt |
| Xử lý duplicate | Tối ưu — gom hết `==pivot` vào giữa, loại khỏi recursion | Kém — duplicate làm quicksort suy biến O(n²) | Tối ưu tự nhiên vì đếm theo giá trị |
| Stable | Không | Không | Có (nếu cài đặt đúng) |
| Tổng quát cho k nhóm | Chỉ 3 nhóm | 2 nhóm | Bất kỳ k nhóm |
| Use case chính | Quicksort với nhiều duplicate, Sort Colors | Quicksort thông thường | Sort số nguyên trong range nhỏ |

Điểm mấu chốt phân biệt DNF với 2-way partition thường: 2-way chỉ trả về 1 điểm chia (`<=pivot` | `>pivot`), còn DNF trả về **2 điểm chia**, tạo ra vùng giữa `==pivot` bị loại hoàn toàn khỏi recursion tiếp theo. Đây chính là "3-way quicksort" mà Sedgewick đề xuất, và là lý do Java's `Arrays.sort()` cho kiểu nguyên thủy dùng **dual-pivot quicksort** (họ hàng gần với ý tưởng DNF, dùng 2 pivot thay vì 1) — về bản chất cùng một tư tưởng: chia nhiều hơn 2 vùng trong 1 lượt duyệt để giảm số lần recursion khi dữ liệu có cấu trúc lặp lại.

Nếu bạn muốn, bước tiếp theo tự nhiên là mình viết luôn **3-way Quicksort hoàn chỉnh** dùng DNF làm partition subroutine — đó mới là chỗ thuật toán này phát huy giá trị thực sự trong competitive programming, chứ không chỉ dừng ở bài "Sort Colors" cơ bản.