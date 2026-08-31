## 49. Group Anagrams

### 1. Quan sát / Nhận diện pattern

Hai string là anagram của nhau khi và chỉ khi chúng có **cùng bộ ký tự với cùng tần suất**.

Bài yêu cầu gom nhóm → cần một **key duy nhất** đại diện cho mỗi nhóm anagram → đây là bài **HashMap grouping**.

---

### 2. Trực giác — từ brute force đến tối ưu

**Brute force O(n² · k):** So từng cặp string xem có phải anagram không. Chậm, không dùng.

**Hướng tốt hơn:** Nếu ta biến mỗi string thành một **canonical form** — dạng chuẩn mà mọi anagram của nó đều cho ra giống nhau — thì chỉ cần dùng HashMap với key là canonical form đó.

Câu hỏi then chốt: **Canonical form là gì?**

**Cách 1 — Sort string: O(n · k log k)**
```
"eat" → sort → "aet"
"tea" → sort → "aet"   ← cùng key
"tan" → sort → "ant"
"ate" → sort → "aet"   ← cùng key với "eat"
```
Đơn giản, dễ cài. Nhưng sort mỗi string tốn O(k log k).

**Cách 2 — Frequency array: O(n · k)**

Thay vì sort, encode tần suất 26 ký tự thành một string key:
```
"eat" → [1,0,0,0,1,0,...,1,0,...] → "1#0#0#0#1#0#...#1#..."
         a  b  c  d  e            t
```
Mọi anagram → cùng frequency array → cùng key. Và O(k) để tạo key thay vì O(k log k).

Với constraint `k ≤ 100, n ≤ 10⁴` thì cả hai đều pass, nhưng **cách 2 là tối ưu về mặt lý thuyết**.

---

### 3. Phát biểu thuật toán (Cách 2)

1. Khởi tạo `HashMap<string, vector<string>> groups`
2. Với mỗi string `s`:
   - Đếm tần suất 26 ký tự → `freq[26]`
   - Encode thành key: `"f0#f1#f2#...#f25"` (dùng `#` làm separator để tránh collision)
   - `groups[key].push_back(s)`
3. Collect tất cả values của HashMap → return

**Tại sao cần separator?** Không có separator, `[1,12]` và `[11,2]` đều encode thành `"112"` → collision. Với `#`: `"1#12"` vs `"11#2"` → distinct.

---

### 4. Cài đặt C++20

```cpp
#include <vector>
#include <string>
#include <unordered_map>
#include <array>
#include <ranges>

class Solution {
public:
    std::vector<std::vector<std::string>> groupAnagrams(
        std::vector<std::string>& strs
    ) {
        std::unordered_map<std::string, std::vector<std::string>> groups;
        groups.reserve(strs.size()); // tránh rehash

        for (const auto& s : strs) {
            groups[encodeFrequency(s)].push_back(s);
        }

        // Extract values từ map
        std::vector<std::vector<std::string>> result;
        result.reserve(groups.size());

        for (auto& [key, group] : groups) {
            result.push_back(std::move(group)); // move, không copy
        }

        return result;
    }

private:
    // Encode tần suất 26 ký tự thành string key
    // "eat" → "1#0#0#0#1#0#0#0#0#0#0#0#0#0#0#0#0#0#0#1#0#0#0#0#0#0"
    std::string encodeFrequency(const std::string& s) {
        std::array<int, 26> freq{};  // zero-initialized

        for (char c : s) {
            freq[c - 'a']++;
        }

        std::string key;
        key.reserve(26 * 2); // ước tính để tránh realloc

        for (int count : freq) {
            key += std::to_string(count);
            key += '#'; // separator ngăn collision
        }

        return key;
    }
};
```

---

### 5. Phân tích độ phức tạp

| | Complexity | Lý do |
|---|---|---|
| **Time** | O(n · k) | n strings, mỗi string xử lý O(k) để đếm freq + build key |
| **Space** | O(n · k) | Lưu toàn bộ strings trong HashMap |

So sánh hai cách:

| Approach | Time | Space | Ghi chú |
|---|---|---|---|
| Sort làm key | O(n · k log k) | O(n · k) | Code ngắn hơn |
| Frequency encode | O(n · k) | O(n · k) | Tối ưu hơn, key cố định 26 slots |

---

**Điểm cần nhớ:** Bài này dạy một pattern rất hay — **transform-then-group**: bất cứ khi nào bài yêu cầu "gom nhóm theo tính chất X", hãy nghĩ ngay đến việc biến tính chất X thành một **hashable key**. Pattern này tái xuất hiện ở rất nhiều bài HashMap khác như 242, 438, 266...