## Đệ quy (Recursion) là gì?

Đệ quy là kỹ thuật một hàm **tự gọi lại chính nó** để giải quyết bài toán bằng cách chia nhỏ nó thành các bài toán con cùng dạng, nhỏ hơn.

Mọi hàm đệ quy đúng nghĩa phải có 2 phần:

- **Base case** — điều kiện dừng, không gọi lại nữa
- **Recursive case** — thu nhỏ bài toán và gọi lại chính mình

```js
function factorial(n) {
  if (n <= 1) return 1;        // base case
  return n * factorial(n - 1); // recursive case
}
```

---

## Cách xác định bài toán có thể dùng đệ quy

Có **3 dấu hiệu chính**:

### 1. Bài toán có cấu trúc tự tương đồng (self-similar)
Bài toán lớn có thể định nghĩa lại bằng chính bài toán đó ở quy mô nhỏ hơn.

> "Tính `f(n)` = tính `f(n-1)` rồi làm thêm gì đó"

Ví dụ điển hình: Fibonacci, factorial, binary search, tree traversal.

### 2. Dữ liệu đầu vào có cấu trúc phân cấp (hierarchical / nested)
Cây (Tree), đồ thị (Graph), JSON lồng nhau, nested components — những thứ mà bạn không biết trước độ sâu.

```php
// Duyệt cây danh mục sản phẩm lồng nhau
function buildCategoryTree($categories, $parentId = null) {
    return array_filter($categories, fn($c) => $c['parent_id'] === $parentId)
        |> array_map(fn($c) => [
            ...$c,
            'children' => buildCategoryTree($categories, $c['id'])
        ]);
}
```

Dùng loop ở đây rất khó, đệ quy là tự nhiên hơn nhiều.

### 3. Có thể chia bài toán thành subproblem độc lập (Divide & Conquer)
MergeSort, QuickSort, tìm đường trong maze — chia đôi, giải từng nửa, gộp lại.

---

## Dấu hiệu KHÔNG nên dùng đệ quy

| Tình huống | Lý do |
|---|---|
| Input size lớn, depth sâu | Stack overflow — PHP/JS/Java đều có giới hạn call stack |
| Không có base case rõ ràng | Vòng lặp vô tận |
| Subproblem bị tính lại nhiều lần | Cần memoization hoặc dùng DP thay thế |
| Tail call không được optimize | Ngôn ngữ không hỗ trợ TCO (PHP, Java không có) |

---

## Framework tư duy để phân tích

Khi gặp bài toán, tự hỏi 3 câu:

1. **"Nếu tôi đã có kết quả của bài toán nhỏ hơn, tôi có thể xây kết quả bài toán này không?"** → Có = đệ quy được
2. **"Dữ liệu có dạng nested/tree không?"** → Có = đệ quy là lựa chọn tự nhiên
3. **"Base case của tôi là gì?"** → Không xác định được = chưa đủ điều kiện dùng đệ quy