## Cinema Seat Allocation

### 1. Quan sát / Nhận diện pattern

Nhìn vào constraints ngay:
- `n` lên đến **10⁹** rows → không thể duyệt từng row
- `reservedSeats.length` chỉ tối đa **10⁴** → số row **bị ảnh hưởng** rất ít

→ Phần lớn các row **không có ghế reserved** → mỗi row trống hoàn toàn cho được **2 nhóm** (block [2-5] và [6-9]).

→ Chỉ cần xử lý **những row có reserved seats**, còn lại tính thẳng.

Ba block hợp lệ duy nhất: `[2,3,4,5]`, `[4,5,6,7]`, `[6,7,8,9]` — chỉ có **3 lựa chọn**, và ghế 1, 10 không bao giờ thuộc block nào → bỏ qua hoàn toàn.

→ Đây là bài **Greedy + Bitmask** trên tập nhỏ (chỉ quan tâm seat 2–9, tức 8 bit).

---

### 2. Trực giác

Mỗi row chỉ có 3 block khả thi. Với mỗi row bị ảnh hưởng, ta cần biết **ghế nào trong 2–9 đã bị reserved**, rồi thử 3 tổ hợp:

```
Block A: seats 2,3,4,5  → bit mask: positions 2,3,4,5
Block B: seats 4,5,6,7  → bit mask: positions 4,5,6,7
Block C: seats 6,7,8,9  → bit mask: positions 6,7,8,9
```

Chiến lược greedy tối ưu:
- Thử A trước → nếu được, thử C (chúng không overlap)
- Nếu không được A → thử B
- Nếu không được A và B → thử C

Tại sao A+C tốt hơn A+B hay B+C? Vì A và C **không overlap** nhau, nên có thể lấy cả 2. B overlap cả A lẫn C nên chỉ dùng khi A và C đều không khả dụng.

```
Seats:  1  2  3  4  5  6  7  8  9  10
Block A:   [2  3  4  5]
Block B:         [4  5  6  7]
Block C:               [6  7  8  9]
```

---

### 3. Phát biểu thuật toán

1. Group `reservedSeats` theo row → với mỗi row bị ảnh hưởng, tạo bitmask 10-bit (bit `i` = seat `i` đã reserved).
2. Đếm số row bị ảnh hưởng = `affected`. Số row **sạch** = `n - affected` → đóng góp `2 * (n - affected)` nhóm.
3. Với mỗi row bị ảnh hưởng, kiểm tra 3 block bằng bitmask AND:
   - Nếu block A trống (`mask & A == 0`): lấy A, thử thêm C
   - Else nếu block B trống: lấy B
   - Else nếu block C trống: lấy C
4. Cộng tổng lại.

---

### 4. Cài đặt C++20

```cpp
#include <vector>
#include <unordered_map>

class Solution {
public:
    int maxNumberOfFamilies(int n, std::vector<std::vector<int>>& reservedSeats) {

        // Bitmask cho 3 block hợp lệ (dùng seat number làm bit index)
        // Seat 2,3,4,5 → bit 2,3,4,5
        constexpr int BLOCK_A = 0b0000111100;  // seats 2,3,4,5
        constexpr int BLOCK_B = 0b0011110000;  // seats 4,5,6,7
        constexpr int BLOCK_C = 0b1111000000;  // seats 6,7,8,9

        // Gom reserved seats theo row → bitmask các ghế đã bị chiếm
        std::unordered_map<int, int> reserved_mask;
        for (auto& [row, seat] : reservedSeats) {
            reserved_mask[row] |= (1 << seat);  // đánh dấu bit tại vị trí seat
        }

        // Row sạch hoàn toàn → luôn xếp được 2 nhóm
        int result = 2 * (n - static_cast<int>(reserved_mask.size()));

        // Xử lý từng row bị ảnh hưởng
        for (auto& [row, mask] : reserved_mask) {
            bool can_A = (mask & BLOCK_A) == 0;
            bool can_C = (mask & BLOCK_C) == 0;
            bool can_B = (mask & BLOCK_B) == 0;

            if (can_A && can_C) {
                result += 2;        // lấy cả 2 block không overlap
            } else if (can_A || can_B || can_C) {
                result += 1;        // chỉ lấy được 1 block
            }
            // else: không block nào khả dụng → +0
        }

        return result;
    }
};
```

---

### 5. Phân tích độ phức tạp

| | Độ phức tạp | Lý do |
|---|---|---|
| **Time** | O(R) | R = số reserved seats ≤ 10⁴; build map O(R), duyệt map O(affected rows) ≤ O(R) |
| **Space** | O(R) | HashMap lưu tối đa R entries |

`n` lên đến 10⁹ nhưng **không xuất hiện trong vòng lặp** — đó là lý do bài này phải nhận ra trick "chỉ xử lý row bị ảnh hưởng" trước khi code bất cứ thứ gì.

---

**Điểm dễ sai:**
- Quên rằng ghế 1 và 10 không thuộc block nào → không cần đưa vào mask cũng được, nhưng nếu dùng bit index = seat number thì chúng tự nhiên nằm ngoài 3 block mask → an toàn.
- Nhầm thứ tự ưu tiên: thử B trước thay vì A → bỏ mất trường hợp A+C = 2 nhóm.