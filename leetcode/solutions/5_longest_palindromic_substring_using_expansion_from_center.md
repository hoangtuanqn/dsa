# Expand Around Center — Giải thích từ A-Z

## 1. Bản chất vấn đề: Palindrome có "tâm đối xứng"

Trước tiên bạn phải thấy được sự thật này: **mọi palindrome đều đối xứng qua một tâm duy nhất**. Nếu bạn đứng tại tâm đó và đi ra 2 phía trái/phải với tốc độ như nhau, bạn sẽ luôn gặp 2 ký tự giống hệt nhau — cho đến khi hết palindrome.

Ví dụ: `"racecar"`
```
r a c e c a r
    ↑
   tâm (chữ 'e')
```
Đi từ tâm `e` ra: `c-c` giống nhau → `a-a` giống nhau → `r-r` giống nhau → hết chuỗi. Đó là toàn bộ ý tưởng.

**Vấn đề đặt ra:** ta không biết trước tâm của palindromic substring dài nhất nằm ở đâu trong `s`. Giải pháp: **thử tất cả các tâm có thể**, với mỗi tâm thì mở rộng ra xem nó "sống" được bao xa.

## 2. Có bao nhiêu loại tâm?

Đây là điểm nhiều người bị vướng — palindrome có 2 dạng độ dài, và mỗi dạng có kiểu tâm khác nhau:

**Độ dài lẻ** (ví dụ `"aba"`): tâm là **1 ký tự cụ thể**.
```
a b a
  ↑
 tâm là ký tự 'b' (index 1)
```

**Độ dài chẵn** (ví dụ `"abba"`): tâm là **khe trống giữa 2 ký tự**, không có ký tự nào nằm đúng giữa cả.
```
a b b a
   ↑
 tâm nằm giữa 2 ký tự 'b' (khe giữa index 1 và 2)
```

Vì vậy với chuỗi có `n` ký tự, tổng số tâm khả dĩ là `2n - 1`:
- `n` tâm dạng lẻ (mỗi ký tự là 1 tâm)
- `n - 1` tâm dạng chẵn (mỗi khe giữa 2 ký tự liền kề là 1 tâm)

Ví dụ `s = "abba"` (n=4) → có 4 tâm lẻ + 3 tâm chẵn = 7 tâm cần thử.

## 3. Cách biểu diễn tâm bằng code: dùng cặp `(left, right)`

Đây là "mẹo" kỹ thuật quan trọng để code gọn: thay vì phân biệt tâm lẻ/chẵn bằng 2 khối logic riêng, ta biểu diễn **cả 2 loại tâm bằng một hàm duy nhất** nhận vào `left` và `right`:

- **Tâm lẻ tại vị trí `i`**: gọi hàm với `left = i, right = i` (cùng 1 điểm).
- **Tâm chẵn giữa `i` và `i+1`**: gọi hàm với `left = i, right = i + 1` (2 điểm liền kề).

```typescript
function expandFromCenter(s: string, left: number, right: number): number {
  while (left >= 0 && right < s.length && s[left] === s[right]) {
    left--;   // mở rộng sang trái
    right++;  // mở rộng sang phải
  }
  // Khi thoát while, left và right đã "đi quá" 1 bước
  // nên độ dài palindrome thực sự là:
  return right - left - 1;
}
```

**Tại sao `right - left - 1`?** Hãy trace kỹ để hiểu, đừng học thuộc công thức.

## 4. Trace từng bước ví dụ cụ thể: `s = "babad"`

### Thử tâm lẻ tại `i = 1` (ký tự `'a'`)

Gọi `expandFromCenter(s, 1, 1)`:

| Bước | left | right | s[left] | s[right] | So sánh | Hành động |
|---|---|---|---|---|---|---|
| Vào while | 1 | 1 | 'a' | 'a' | bằng | left--, right++ |
| Lần 1 | 0 | 2 | 'b' | 'b' | bằng | left--, right++ |
| Lần 2 | -1 | 3 | — | — | `left < 0` → thoát while | |

Độ dài = `right - left - 1` = `3 - (-1) - 1` = `3`.

→ Palindrome tìm được: từ index `left+1 = 0` đến `right-1 = 2`, tức là `s[0..2] = "bab"`. Đúng — `"bab"` là palindrome!

**Vì sao trừ đi 1?** Vì vòng lặp `while` luôn thực hiện thêm 1 bước "đi quá" trước khi phát hiện sai lệch hoặc chạm biên, nên `left` và `right` cuối cùng đang **nằm ngoài** phạm vi palindrome thật sự đúng 1 đơn vị mỗi bên.

### Thử tâm chẵn tại khe giữa `i=0` và `i=1` (giữa `'b'` và `'a'`)

Gọi `expandFromCenter(s, 0, 1)`:

| Bước | left | right | s[left] | s[right] | So sánh |
|---|---|---|---|---|---|
| Vào while | 0 | 1 | 'b' | 'a' | khác → thoát ngay |

Độ dài = `1 - 0 - 1 = 0`. Không có palindrome nào ở tâm chẵn này (hợp lý, vì `"ba"` không đối xứng).

## 5. Vòng lặp chính: thử TẤT CẢ các tâm

```typescript
function longestPalindrome(s: string): string {
  if (s.length < 1) return "";

  let start = 0, end = 0; // lưu vị trí palindrome dài nhất tìm được

  for (let i = 0; i < s.length; i++) {
    const len1 = expandFromCenter(s, i, i);     // tâm lẻ tại i
    const len2 = expandFromCenter(s, i, i + 1); // tâm chẵn giữa i và i+1
    const len = Math.max(len1, len2);

    if (len > end - start + 1) {
      // Tính lại vị trí start/end từ độ dài len và tâm i
      start = i - Math.floor((len - 1) / 2);
      end = i + Math.floor(len / 2);
    }
  }

  return s.substring(start, end + 1);
}
```

**Điểm cần hiểu kỹ**: công thức `start = i - Math.floor((len - 1) / 2)` dùng để suy ngược từ "tâm + độ dài" ra "vị trí bắt đầu", áp dụng đúng cho cả trường hợp `len` lẻ và chẵn (bạn nên tự thử tay với `i=1, len=3` và `i=1, len=2` để thấy công thức ra đúng cả 2 case).

## 6. Trace toàn bộ với `s = "babad"` để thấy full flow

| i | len1 (tâm lẻ) | len2 (tâm chẵn) | len lớn nhất | Cập nhật start/end? |
|---|---|---|---|---|
| 0 | 1 (`"b"`) | 0 | 1 | start=0, end=0 |
| 1 | 3 (`"bab"`) | 0 | 3 | 3 > 1 → start=0, end=2 |
| 2 | 3 (`"aba"`) | 0 | 3 | 3 = 3, không lớn hơn → giữ nguyên |
| 3 | 1 (`"a"`) | 0 | 1 | không lớn hơn |
| 4 | 1 (`"d"`) | 0 | 1 | không lớn hơn |

Kết quả: `s.substring(0, 3) = "bab"`. (`"aba"` cũng là đáp án hợp lệ — đề bài chấp nhận nhiều đáp án đúng).

## 7. Vì sao độ phức tạp là O(n²), O(1) space

- Vòng lặp ngoài: `n` lần (thử `n` vị trí tâm lẻ, `n` tâm chẵn → tổng `2n-1` lần gọi hàm).
- Mỗi lần gọi `expandFromCenter`, worst case mở rộng tối đa `n/2` bước (ví dụ chuỗi toàn ký tự giống nhau `"aaaa...a"`).
- → Tổng: O(n) tâm × O(n) mở rộng = **O(n²)**.
- Space chỉ dùng vài biến số nguyên (`left, right, start, end`) → **O(1)**, không cần bảng `dp[][]` như cách DP.

---

Bây giờ bạn đã có đủ nền tảng để tự code. Gợi ý bài tập cho bạn: hãy tự viết lại `longestPalindrome` bằng TypeScript **không nhìn code mẫu ở trên**, sau đó tự trace tay với input `"cbbd"` trước khi chạy thử — nếu bạn trace đúng tay thì code chắc chắn đúng logic. Gửi lại nếu bí chỗ nào, tôi sẽ chỉ đúng điểm sai chứ không đưa full solution.