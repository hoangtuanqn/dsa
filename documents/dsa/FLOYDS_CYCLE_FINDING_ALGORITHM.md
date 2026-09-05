# Floyd's Cycle-Finding Algorithm — Tortoise & Hare

> Tên đầy đủ: **Floyd's Cycle-Finding Algorithm**, còn gọi là **Hare-Tortoise Algorithm** (Thỏ và Rùa). Hai tên này chỉ cùng một thuật toán.

---

## 1. Bài toán nó giải quyết

Cho một **linked list** (hoặc bất kỳ cấu trúc dạng con trỏ `f(x) → next`), trả lời hai câu hỏi:

- **Có cycle không?**
- **Nếu có, cycle bắt đầu từ node nào?**

**Pain point thực tế:** Trước Floyd, cách ngây thơ nhất là dùng `HashSet` — duyệt từng node, lưu địa chỉ đã thăm, nếu gặp lại thì có cycle.

```
HashSet approach: O(n) time, O(n) space
Floyd:            O(n) time, O(1) space  ← đây là lý do tồn tại
```

Trong embedded systems, interview problems, hay memory-constrained environments — `O(1) space` là non-negotiable. Floyd sinh ra để giải quyết đúng constraint đó.

---

## 2. Trực giác cốt lõi

Hình dung một **con rùa** và một **con thỏ** cùng xuất phát từ đầu danh sách:

- Rùa đi **1 bước/lượt**
- Thỏ đi **2 bước/lượt**

```
Nếu KHÔNG có cycle:
  Thỏ chạy đến NULL trước → done, không có cycle.

Nếu CÓ cycle:
  Thỏ chạy nhanh hơn, nhưng cả hai bị kẹt trong vòng lặp.
  Thỏ "đuổi kịp" rùa từ phía sau, giống như hai runner
  trên một track hình tròn — người nhanh hơn sẽ lap người chậm hơn.
```

**Ví dụ nhỏ — vẽ tay:**

```
List:  1 → 2 → 3 → 4 → 5
                    ↑         |
                    └─── 6 ←──┘

Node: 1  2  3  4  5  6
             cycle start = node 3
```

```
Bước 0:  Rùa=1,  Thỏ=1
Bước 1:  Rùa=2,  Thỏ=3
Bước 2:  Rùa=3,  Thỏ=5
Bước 3:  Rùa=4,  Thỏ=4   ← GẶP NHAU (meeting point)
```

Gặp nhau → có cycle. Nhưng meeting point **không phải** là cycle start. Đây là điểm nhiều người nhầm.

---

## 3. Cơ chế hoạt động từng bước

### Phase 1 — Detect cycle

```
slow đi 1 bước, fast đi 2 bước.
Nếu fast == NULL hoặc fast->next == NULL → không có cycle.
Nếu slow == fast → có cycle, ghi nhận meeting point.
```

### Phase 2 — Tìm cycle start (Entry point)

> Đây là phần hay nhất, và cũng là phần nhiều người nhầm nhất.

**Lưu ý quan trọng trước:** Meeting point ≠ Cycle start. Chỉ bằng nhau trong trường hợp đặc biệt.

#### Đặt tên các đoạn đường

```
head                Entry Point          Meeting Point
 |                       |                    |
 1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 9 → 10
                    ↑                         |
                    └─────────────────────────┘

 ←────── F ────────→←──────── a ─────────────→
                    ←──────────────── C ───────→
```

- **F** = khoảng cách từ `head` đến Entry Point
- **a** = khoảng cách từ Entry Point đến Meeting Point (đo trong cycle)
- **C** = chu vi toàn bộ cycle
- **C - a** = khoảng cách còn lại từ Meeting Point ngược về Entry Point (đi tiếp theo chiều forward)

#### Lập phương trình

Khi slow và fast gặp nhau tại Meeting Point:

```
slow đi được:  F + a
fast đi được:  F + a + k×C   (fast chạy thêm k vòng trong cycle)
fast = 2 × slow nên:

  2(F + a) = F + a + k×C
  F + a = k×C
  F = k×C - a
  F = (k-1)×C + (C - a)
```

Với k = 1 (trường hợp đơn giản nhất):

```
F = C - a
```

#### Đọc ý nghĩa công thức

| Vế | Ý nghĩa |
|---|---|
| **F** | Bước đi từ `head` → Entry Point |
| **C - a** | Bước đi từ Meeting Point → Entry Point (đi tiếp theo chiều forward) |

**Hai con đường dài bằng nhau** → cho hai pointer đi cùng tốc độ 1 bước, chúng tự hội tụ tại Entry Point.

#### Walk-through số cụ thể

```
List: 1 → 2 → 3 → 4 → 5 → 6 → (quay về 3)
idx:  0   1   2   3   4   5

F = 2  (head → node3: đi qua 0, 1)
C = 4  (cycle: 3 → 4 → 5 → 6 → 3)
```

Phase 1 — tìm Meeting Point:

| Bước | slow | fast |
|------|------|------|
| 0 | idx 0 | idx 0 |
| 1 | idx 1 | idx 2 |
| 2 | idx 2 | idx 4 |
| 3 | idx 3 | idx 2 |
| 4 | idx 4 | idx 4 ← GẶP NHAU |

Meeting Point = idx 4 (node 5). a = 4 - 2 = 2.

Kiểm tra: `F = C - a = 4 - 2 = 2` ✓

Phase 2 — tìm Entry Point:

```
p1 = head (idx 0)
p2 = Meeting Point (idx 4)
```

| Bước | p1 | p2 |
|------|----|----|
| 1 | idx 1 | idx 5 |
| 2 | idx 2 | idx 2 ← GẶP NHAU = Entry Point ✓ |

#### Code

```javascript
function findCycleEntry(head) {
  let slow = head;
  let fast = head;

  // Phase 1: tìm Meeting Point
  while (fast && fast.next) {
    slow = slow.next;
    fast = fast.next.next;

    if (slow === fast) {
      // Phase 2: tìm Entry Point
      let p1 = head;
      let p2 = slow; // giữ tại Meeting Point

      while (p1 !== p2) {
        p1 = p1.next; // đi từ head
        p2 = p2.next; // đi từ Meeting Point
      }

      return p1; // Entry Point
    }
  }

  return null; // không có cycle
}
```

#### Pitfall hay gặp nhất

```javascript
// ❌ WRONG — reset cả hai về head
p1 = head;
p2 = head; // sai, phải giữ tại Meeting Point

// ✅ CORRECT
p1 = head;
p2 = slow; // giữ nguyên tại Meeting Point
```

#### Tóm lại 1 câu

> Công thức `F = C - a` chứng minh khoảng cách từ `head` đến Entry Point **bằng đúng** khoảng cách từ Meeting Point đến Entry Point — nên reset một pointer về head, giữ một tại Meeting Point, đi cùng tốc độ, chúng tự gặp nhau đúng tại cổng vào cycle.

---

## 3.5. Quick Reference — Code theo từng mục tiêu

### Chỉ detect có cycle hay không

```javascript
function hasCycle(head) {
  let slow = head;
  let fast = head;

  while (fast && fast.next) {
    slow = slow.next;
    fast = fast.next.next;

    if (slow === fast) return true; // có cycle, dừng luôn
  }

  return false; // fast chạy đến null → không có cycle
}
```

Dừng ngay khi gặp nhau, không cần Phase 2. Đây là version nhẹ nhất.

### Detect + tìm Entry Point

```javascript
function detectCycleStart(head) {
  let slow = head;
  let fast = head;

  // Phase 1: tìm meeting point
  while (fast && fast.next) {
    slow = slow.next;
    fast = fast.next.next;

    if (slow === fast) {
      // Phase 2: tìm entry point
      let p1 = head;
      let p2 = slow; // giữ tại meeting point, KHÔNG reset về head

      while (p1 !== p2) {
        p1 = p1.next;
        p2 = p2.next;
      }

      return p1; // entry point
    }
  }

  return null; // không có cycle
}
```

### So sánh nhanh

| Mục tiêu | Phase cần chạy | Return |
|---|---|---|
| Chỉ detect loop | Phase 1 only | `true / false` |
| Tìm entry point | Phase 1 + Phase 2 | `node / null` |
| Tính chu vi cycle | Phase 1 + đếm vòng từ meeting | `number` |

---

## 4. Implementation C++20

```cpp
#include <iostream>
#include <optional>

// ── Node definition ────────────────────────────────────────────────
struct Node {
    int val;
    Node* next;
    explicit Node(int v) : val(v), next(nullptr) {}
};

// ── Floyd's Cycle Detection ─────────────────────────────────────────
struct CycleResult {
    bool has_cycle;
    Node* entry;      // nullptr nếu không có cycle
    Node* meeting;    // meeting point trong phase 1, nullptr nếu không có cycle
};

CycleResult floyd_detect(Node* head) {
    if (!head || !head->next)
        return {false, nullptr, nullptr};

    Node* slow = head;
    Node* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            Node* entry = head;
            Node* from_meeting = slow;

            while (entry != from_meeting) {
                entry        = entry->next;
                from_meeting = from_meeting->next;
            }

            return {true, entry, slow};
        }
    }

    return {false, nullptr, nullptr};
}

// ── Cycle length (bonus) ────────────────────────────────────────────
int cycle_length(Node* meeting_point) {
    if (!meeting_point) return 0;

    int length = 1;
    Node* cur = meeting_point->next;
    while (cur != meeting_point) {
        ++length;
        cur = cur->next;
    }
    return length;
}

// ── Helper: build list với cycle ────────────────────────────────────
Node* build_test_list() {
    // 1 → 2 → 3 → 4 → 5 → 6 → (back to 3)
    Node* n1 = new Node(1);
    Node* n2 = new Node(2);
    Node* n3 = new Node(3);
    Node* n4 = new Node(4);
    Node* n5 = new Node(5);
    Node* n6 = new Node(6);

    n1->next = n2; n2->next = n3; n3->next = n4;
    n4->next = n5; n5->next = n6; n6->next = n3;

    return n1;
}

int main() {
    Node* head = build_test_list();
    auto [has_cycle, entry, meeting] = floyd_detect(head);

    if (has_cycle) {
        std::cout << "Cycle detected!\n";
        std::cout << "Entry node value : " << entry->val   << "\n";
        std::cout << "Meeting pt value : " << meeting->val << "\n";
        std::cout << "Cycle length     : " << cycle_length(meeting) << "\n";
    } else {
        std::cout << "No cycle.\n";
    }

    return 0;
}
```

**Compile:**
```bash
g++ -std=c++20 -O2 -Wall -Wextra -o floyd main.cpp && ./floyd
```

**Output:**
```
Cycle detected!
Entry node value : 3
Meeting pt value : 5
Cycle length     : 4
```

---

## 5. Độ phức tạp

### Time Complexity: O(n)

- Khi slow vào cycle, fast đã ở đâu đó trong cycle.
- Khoảng cách giữa fast và slow tối đa là `C`, mỗi bước giảm 1.
- Phase 1 mất tối đa `F + C` bước. Phase 2 mất tối đa `F` bước.
- Tổng: `O(F + C) = O(n)`.

### Space Complexity: O(1)

Chỉ dùng 2 pointers. Constant, không phụ thuộc `n`.

| Approach | Time | Space |
|---|---|---|
| HashSet | O(n) | **O(n)** |
| Floyd | O(n) | **O(1)** |

---

## 6. Khi nào dùng / không dùng

**✅ Dùng khi:**
- Linked list, không được phép dùng extra memory.
- Functional sequence dạng `x → f(x) → f(f(x)) → ...` (Pollard's rho factorization, hash collision).
- Cần tìm **entry point của cycle**, không chỉ detect.

**❌ Không dùng khi:**
- Structure không phải dạng single-linked (graph tổng quát).
- Cần biết **tất cả nodes trong cycle**.
- List bị **modify concurrent**.

---

## 7. So sánh với thuật toán tương tự

### Floyd vs HashSet

| | Floyd | HashSet |
|---|---|---|
| Space | **O(1)** | O(n) |
| Time | O(n) | O(n) avg |
| Tìm entry point | Có (phase 2) | Có |
| Implement | Phức tạp hơn | Đơn giản hơn |
| Use case | Memory-constrained | Code clarity ưu tiên |

### Floyd vs Brent's Algorithm

Brent cũng O(1) space, O(n) time, constant factor nhỏ hơn một số trường hợp. Ít gặp trong interview vì ít intuitive. Thường xuất hiện trong cryptography (Pollard's rho).

### Floyd vs DFS marking (graph)

Graph tổng quát → dùng DFS với 3 màu (white/gray/black) để detect back edge. Floyd không áp dụng được ở đây.

**Tóm gọn để nhớ:**

> Hai runners trên track — nếu có cycle, thỏ sẽ bắt kịp rùa. Khoảng cách từ head đến cổng vào cycle bằng khoảng cách từ điểm gặp nhau đến cổng vào cycle. Reset một, giữ một, đi cùng tốc độ — gặp nhau đúng cổng.
