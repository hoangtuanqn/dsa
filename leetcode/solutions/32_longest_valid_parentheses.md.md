# 32. Longest Valid Parentheses

---

## 1. Quan sát / nhận diện pattern

Bài yêu cầu tìm **substring dài nhất** thỏa điều kiện → nghĩ ngay đến:
- DP trên substring
- Stack theo dõi trạng thái
- Two-pass greedy

Dấu hiệu nhận ra **stack**: bài liên quan đến matching cặp `()` — đây là DNA của stack-based parentheses problems.

---

## 2. Trực giác — từ ví dụ nhỏ

Lấy `)()())`:

```
Vấn đề: khi gặp ')' hợp lệ, độ dài valid = i - "điểm neo cuối cùng"
Điểm neo = index của '(' chưa match gần nhất, hoặc index của ')' thừa gần nhất
```

```
) ( ) ( ) )
0 1 2 3 4 5

i=0: ')' thừa → neo mới tại 0
i=2: ')' match '(' tại 1 → dài = 2 - neo(0) = 2
i=4: ')' match '(' tại 3 → dài = 4 - neo(0) = 4  ← gộp được vì cùng "vùng"
i=5: ')' thừa → neo mới tại 5
```

**Key insight**: Stack không lưu char mà lưu **index**, để tính khoảng cách. Đáy stack luôn giữ "điểm neo" — ranh giới của vùng invalid gần nhất.

---

## 3. Phát biểu thuật toán

```
1. Push -1 vào stack làm sentinel (điểm neo ban đầu)
2. Duyệt từng index i:
   - Nếu s[i] == '(' → push i
   - Nếu s[i] == ')':
       + Pop top
       + Nếu stack rỗng → push i làm neo mới (')' thừa)
       + Nếu stack không rỗng → ans = max(ans, i - stack.top())
3. Return ans
```

Tại sao đúng: `i - stack.top()` luôn cho độ dài của đoạn valid **liên tục** tính từ sau điểm neo — vì mọi `(` đã matched đều bị pop ra khỏi stack.

---

## 4. Cài đặt C++20

```cpp
#include <stack>
#include <string>
#include <algorithm>

class Solution {
public:
    int longestValidParentheses(const std::string& s) {
        // Đáy stack là "điểm neo" — index của boundary invalid gần nhất
        std::stack<int> indexStack;
        indexStack.push(-1); // sentinel: neo ban đầu trước cả string

        int maxLength = 0;

        for (int i = 0; i < static_cast<int>(s.size()); ++i) {
            if (s[i] == '(') {
                // Lưu index để sau này tính khoảng cách
                indexStack.push(i);
            } else {
                // ')': thử match với '(' gần nhất
                indexStack.pop();

                if (indexStack.empty()) {
                    // ')' thừa, không match được → trở thành neo mới
                    indexStack.push(i);
                } else {
                    // Tính độ dài valid từ sau neo đến i
                    int validLength = i - indexStack.top();
                    maxLength = std::max(maxLength, validLength);
                }
            }
        }

        return maxLength;
    }
};
```

---

## 5. Phân tích độ phức tạp

| | Complexity | Lý do |
|---|---|---|
| **Time** | O(n) | Mỗi index push/pop đúng 1 lần |
| **Space** | O(n) | Stack worst case chứa toàn bộ `(((((` |

---

## Bonus: Two-pass O(1) space

Nếu phỏng vấn hỏi "không dùng stack được không?":

```cpp
int longestValidParentheses(const std::string& s) {
    int maxLength = 0;

    // Pass 1: trái → phải
    auto pass = [&](auto begin, auto end, char openChar) {
        int open = 0, close = 0;
        for (auto it = begin; it != end; ++it) {
            (*it == openChar) ? ++open : ++close;
            if (open == close)
                maxLength = std::max(maxLength, 2 * close);
            else if (close > open)
                open = close = 0; // reset khi invalid
        }
    };

    pass(s.begin(), s.end(), '(');        // trái → phải
    pass(s.rbegin(), s.rend(), ')');      // phải → trái

    return maxLength;
}
```

**Tại sao cần 2 pass?** Pass trái→phải bỏ sót case `(()`  (open > close mãi không reset). Pass phải→trái bắt được case đó. Hai pass bù nhau → cover toàn bộ.

| | Complexity |
|---|---|
| **Time** | O(n) |
| **Space** | O(1) |