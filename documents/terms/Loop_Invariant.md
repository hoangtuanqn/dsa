## 3. Loop Invariant

Đây là khái niệm quan trọng nhất trong 3 — nền tảng để **chứng minh tính đúng đắn** của bất kỳ thuật toán nào.

**Định nghĩa:** Loop invariant là một **mệnh đề luôn đúng** tại một điểm cụ thể trong vòng lặp — trước mỗi iteration, sau mỗi iteration, và sau khi vòng lặp kết thúc.

Để chứng minh loop invariant, cần chứng minh 3 tính chất (giống quy nạp toán học):

```
1. Initialization  — Đúng trước iteration đầu tiên
2. Maintenance     — Nếu đúng trước iteration i, thì đúng trước iteration i+1
3. Termination     — Khi vòng lặp kết thúc, invariant cho ta kết quả đúng
```

---

**Ví dụ 1: Loop Invariant của Bubble Sort**

> **Invariant:** Sau pass thứ `i`, `i` phần tử lớn nhất đã nằm đúng vị trí tại `arr[n-i..n-1]`.

```
Initialization: i=0, chưa pass nào → 0 phần tử đúng vị trí ✓ (hiển nhiên)

Maintenance: 
  Giả sử sau pass i, arr[n-i..n-1] đã đúng.
  Pass i+1 duyệt arr[0..n-i-1], đẩy max của đoạn này
  lên arr[n-i-1] → arr[n-i-1..n-1] đúng ✓

Termination: 
  i = n-1 → arr[1..n-1] đúng → arr[0] là min còn lại → toàn mảng đúng ✓
```

---

**Ví dụ 2: Loop Invariant của Insertion Sort** (dễ hiểu hơn)

```cpp
for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
        arr[j+1] = arr[j];
        j--;
    }
    arr[j+1] = key;
}
```

> **Invariant:** Tại đầu mỗi iteration `i`, `arr[0..i-1]` đã được **sort và chứa đúng i phần tử đầu tiên** của mảng gốc.

```
i=1: arr[0..0] — 1 phần tử, hiển nhiên sorted ✓
i=2: arr[0..1] — 2 phần tử đầu đã sorted ✓
...
i=n: arr[0..n-1] — toàn mảng sorted ✓
```

---

**Tại sao Loop Invariant quan trọng trong thực tế?**

Không chỉ là lý thuyết — đây là tư duy bạn cần khi debug thuật toán sai:

```
Bug xuất hiện → hỏi: "Invariant nào đang bị vi phạm?"
              → tìm đúng iteration nó bị phá vỡ
              → fix đúng chỗ, không fix mò
```

Ví dụ thực tế: Binary Search có invariant *"phần tử cần tìm luôn nằm trong `[left, right]`"*. Mọi lỗi off-by-one của Binary Search đều là do vi phạm invariant này.

```cpp
// Sai — vi phạm invariant khi target ở vị trí right
while (left < right) {          // ← nên là left <= right
    int mid = (left + right) / 2;
    if (arr[mid] < target) left = mid + 1;
    else right = mid;           // ← hoặc right = mid - 1 tùy variant
}
```

---

## Tổng kết mối liên hệ 3 khái niệm

```
INVERSION
  → Đo "độ hỗn loạn" của mảng
  → Số swap tối thiểu (adjacent) = số inversion
  → Công cụ phân tích performance của sorting algorithm

STABILITY  
  → Đảm bảo thứ tự tương đối khi sort multi-key
  → Quyết định chọn thuật toán nào trong bài toán thực tế
  → Bubble/Merge/Insertion: stable | Quick/Heap/Selection: không

LOOP INVARIANT
  → Công cụ chứng minh thuật toán đúng
  → Công cụ debug khi thuật toán sai
  → Nền tảng tư duy của mọi thuật toán có vòng lặp
```

Ba khái niệm này không riêng của Bubble Sort — chúng là **ngôn ngữ chung** để phân tích bất kỳ thuật toán nào. Nắm vững 3 cái này là bạn đã có tư duy của người làm algorithm nghiêm túc.