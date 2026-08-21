## Đọc đề

```
height = [1,8,6,2,5,4,8,3,7]
```

Mỗi phần tử là một cột có chiều cao `height[i]`. Chọn 2 cột bất kỳ, nước chứa được tính:

```
water = (j - i) × min(height[i], height[j])
         khoảng cách    chiều cao thấp hơn
```

Tìm lượng nước **lớn nhất**.

---

## Trực giác

```
[1, 8, 6, 2, 5, 4, 8, 3, 7]
 0  1  2  3  4  5  6  7  8
```

Thử chọn cột 1 (h=8) và cột 6 (h=8):
```
water = (6-1) × min(8,8) = 5 × 8 = 40  ← đây là max
```

Thử chọn cột 0 (h=1) và cột 8 (h=7):
```
water = (8-0) × min(1,7) = 8 × 1 = 8
```

Khoảng cách rộng nhưng cột thấp quá → nước ít.

---

## Tại sao Two Pointers?

Đặt `L=0`, `R=n-1` (khoảng cách lớn nhất có thể):

```
[1, 8, 6, 2, 5, 4, 8, 3, 7]
 L                          R
```

Mỗi bước, di chuyển pointer **thấp hơn** vào trong. Tại sao?

```
water = (R - L) × min(height[L], height[R])
```

Khi di chuyển vào trong, `(R-L)` chắc chắn **giảm**. Muốn `water` tăng, chỉ có thể tăng `min(height[L], height[R])`. Muốn tăng `min`, phải bỏ cái **thấp hơn** đi tìm cái cao hơn.

```
height[L]=1, height[R]=7
→ min = 1, bị giới hạn bởi L
→ dù R có cao bao nhiêu cũng vô nghĩa
→ L++ để tìm cột cao hơn
```

---

## Phát biểu thuật toán

1. `L=0`, `R=n-1`
2. Tính `water = (R-L) × min(height[L], height[R])`
3. Cập nhật `maxWater`
4. Di chuyển pointer **thấp hơn** vào trong
5. Lặp đến khi `L >= R`

---

## Code

```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0;
        int right = height.size() - 1;
        int maxWater = 0;

        while (left < right) {
            // Tính lượng nước hiện tại
            int water = (right - left) * min(height[left], height[right]);
            maxWater = max(maxWater, water);

            // Di chuyển pointer thấp hơn
            if (height[left] < height[right])
                left++;
            else
                right--;
        }

        return maxWater;
    }
};
```

---

## Trace ví dụ

```
height = [1, 8, 6, 2, 5, 4, 8, 3, 7]
          0  1  2  3  4  5  6  7  8
```

| L | R | h[L] | h[R] | water | max | di chuyển |
|---|---|------|------|-------|-----|-----------|
| 0 | 8 | 1 | 7 | 8×1=8 | 8 | L++ (1<7) |
| 1 | 8 | 8 | 7 | 7×7=49 | 49 | R-- (8>7) |
| 1 | 7 | 8 | 3 | 6×3=18 | 49 | R-- (8>3) |
| 1 | 6 | 8 | 8 | 5×8=**40** | 49 | R-- (bằng nhau) |
| 1 | 5 | 8 | 4 | 4×4=16 | 49 | R-- |
| 1 | 4 | 8 | 5 | 3×5=15 | 49 | R-- |
| 1 | 3 | 8 | 2 | 2×2=4 | 49 | R-- |
| 1 | 2 | 8 | 6 | 1×6=6 | 49 | R-- |
| 1 | 1 | - | - | - | - | dừng |

Kết quả: **49** ✅

---

## Độ phức tạp

- **Time:** O(n) — mỗi bước L hoặc R di chuyển 1 bước, tối đa n bước
- **Space:** O(1) — không dùng thêm bộ nhớ