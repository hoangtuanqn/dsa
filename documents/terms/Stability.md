
## 2. Stability (Tính ổn định)

**Định nghĩa:** Một thuật toán sort được gọi là **stable** nếu các phần tử có **giá trị bằng nhau** giữ nguyên **thứ tự tương đối** so với mảng ban đầu sau khi sort.

```
Input: [(A,3), (B,1), (C,3), (D,1)]
         ↑key là số, chữ cái là định danh

Stable sort theo key:
  → [(B,1), (D,1), (A,3), (C,3)]
       B trước D ✓ (giữ thứ tự gốc)
       A trước C ✓ (giữ thứ tự gốc)

Unstable sort có thể ra:
  → [(D,1), (B,1), (C,3), (A,3)]  ← sai thứ tự tương đối
```

**Tại sao quan trọng trong thực tế?**

Tình huống điển hình: sort danh sách đơn hàng — **trước theo ngày, sau theo khách hàng**.

```
Bước 1: sort theo ngày
  → [đơn 1/8, đơn 2/8, đơn 2/8, đơn 3/8]

Bước 2: sort theo tên khách hàng (stable)
  → Các đơn cùng khách hàng vẫn giữ thứ tự ngày tăng dần ✓

Nếu unstable → thứ tự ngày bị phá vỡ ❌
```

**Phân loại các thuật toán:**

| Thuật toán | Stable? | Ghi chú |
|---|---|---|
| Bubble Sort | ✅ | Không swap khi bằng nhau |
| Merge Sort | ✅ | Merge ưu tiên phần tử bên trái |
| Insertion Sort | ✅ | Chỉ dịch chuyển khi thực sự nhỏ hơn |
| Selection Sort | ❌ | Swap xa phá vỡ thứ tự |
| Quick Sort | ❌ | Partition không đảm bảo |
| Heap Sort | ❌ | Cấu trúc heap phá vỡ thứ tự |
| `std::sort` C++ | ❌ | Introsort — unstable |
| `std::stable_sort` C++ | ✅ | Merge Sort biến thể |

**Tại sao Selection Sort unstable?**

```
arr = [(A,2), (B,2), (C,1)]

Pass 1: tìm min = (C,1) ở index 2, swap với index 0
  → [(C,1), (B,2), (A,2)]
              ↑ A và B bị đảo thứ tự — unstable ❌
```