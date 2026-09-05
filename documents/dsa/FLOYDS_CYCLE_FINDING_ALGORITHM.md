# Floyd's Cycle-Finding Algorithm — Tortoise & Hare

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

Đây là phần toán học cần chứng minh.

**Đặt:**
- `F` = khoảng cách từ head đến cycle start
- `C` = chu vi của cycle
- `a` = khoảng cách từ cycle start đến meeting point (đo trong cycle)

**Khi slow và fast gặp nhau:**
- slow đi được: `F + a` bước
- fast đi được: `F + a + k*C` bước (fast đã đi thêm `k` vòng trong cycle)
- fast đi gấp đôi slow: `2(F + a) = F + a + k*C`

```
=> F + a = k*C
=> F = k*C - a
=> F = (k-1)*C + (C - a)
```

**Đọc vế phải:** `(C - a)` chính là khoảng cách từ meeting point đến cycle start (đi theo chiều forward trong cycle).

Tức là: **khoảng cách từ head đến cycle start = khoảng cách từ meeting point đến cycle start**.

**Hệ quả thực tế:** Reset một pointer về head, giữ pointer kia ở meeting point, cả hai đi **1 bước/lượt** — chúng sẽ gặp nhau đúng tại cycle start.

**Walk-through trên ví dụ:**

```
List: 1 → 2 → 3 → 4 → 5 → 6 → (back to 3)
Index: 0   1   2   3   4   5

F = 2  (head→node3: 0→1→2)
C = 4  (cycle: 3→4→5→6→3)
```

```
Phase 1:
Step 0: slow=0, fast=0
Step 1: slow=1, fast=2
Step 2: slow=2, fast=4
Step 3: slow=3, fast=2   (fast: 4→6→3... wait, 4+2=6, 6→3)
Step 4: slow=4, fast=4   ← MEET tại index 4 (node 5)

a = 4 - 2 = 2  (từ cycle start (idx2) đến meeting (idx4))
```

```
Phase 2:
p1 = head (idx 0), p2 = meeting (idx 4)
Step 1: p1=1, p2=5
Step 2: p1=2, p2=3 (5→6→3, tức idx 2)
  → KHÔNG khớp

Hmm, thử lại với C = 4:
F = k*C - a
2 = 1*4 - 2 = 2  ✓

p1 bắt đầu idx 0, p2 bắt đầu idx 4 (meeting)
Bước 1: p1=idx1,  p2=idx5
Bước 2: p1=idx2,  p2=idx2  ← GẶP, đây là cycle start ✓
```

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

    // ── Phase 1: Tìm meeting point ──────────────────────────────────
    // Điều kiện dừng: fast hoặc fast->next == nullptr (không có cycle)
    // hoặc slow == fast (có cycle)
    while (fast && fast->next) {
        slow = slow->next;           // rùa: 1 bước
        fast = fast->next->next;     // thỏ: 2 bước

        if (slow == fast) {
            // ── Phase 2: Tìm cycle entry point ─────────────────────
            // Reset một pointer về head, giữ pointer kia tại meeting point
            // Cả hai đi 1 bước/lượt → gặp nhau tại entry
            Node* entry = head;
            Node* from_meeting = slow;  // slow == fast == meeting point

            while (entry != from_meeting) {
                entry       = entry->next;
                from_meeting = from_meeting->next;
            }

            // entry == from_meeting == cycle start
            return {true, entry, slow};
        }
    }

    return {false, nullptr, nullptr};  // fast chạy đến nullptr → không có cycle
}

// ── Cycle length (bonus) ────────────────────────────────────────────
// Sau khi biết meeting point, đếm chu vi cycle
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

    n1->next = n2;
    n2->next = n3;
    n3->next = n4;
    n4->next = n5;
    n5->next = n6;
    n6->next = n3;  // tạo cycle về node 3

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

    // Note: không free memory vì có cycle → tránh infinite loop khi delete
    // Trong production cần break cycle trước rồi mới free
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

**Phase 1 — tại sao bounded bởi O(n)?**

Gọi `F` = đoạn trước cycle, `C` = chu vi cycle.

- Khi slow vào cycle, fast đã ở đâu đó trong cycle.
- Khoảng cách giữa fast và slow (tính trong cycle) tối đa là `C`.
- Mỗi bước, khoảng cách này giảm 1 (vì fast nhanh hơn slow đúng 1 bước trong cycle).
- Nên phase 1 mất tối đa `F + C` bước.

**Phase 2:** tối đa `F` bước (vì p1 cần đúng `F` bước để đến entry).

Tổng: `O(F + C) = O(n)`.

### Space Complexity: O(1)

Chỉ dùng 2 pointers (`slow`, `fast`) + 2 pointers trong phase 2. Constant, không phụ thuộc `n`.

**So sánh trực tiếp:**

| Approach | Time | Space |
|---|---|---|
| HashSet | O(n) | **O(n)** |
| Floyd | O(n) | **O(1)** |

---

## 6. Khi nào dùng / không dùng

### ✅ Dùng khi:
- Linked list, và bạn không được phép dùng extra memory.
- Functional sequence dạng `x → f(x) → f(f(x)) → ...` — detect rho (ρ) shape (dùng trong Pollard's rho factorization, hash collision).
- Bài toán yêu cầu tìm **entry point của cycle**, không chỉ detect.

### ❌ Không dùng khi:
- Structure **không phải** dạng single-linked — ví dụ graph tổng quát có nhiều edge vào một node. Floyd giả định mỗi node chỉ có đúng một `next`.
- Bạn cần biết **tất cả nodes trong cycle** — Floyd chỉ cho entry point và meeting point, muốn enumerate toàn bộ cycle cần thêm bước.
- List có thể bị **modify concurrent** — hai pointers cần guarantee list không thay đổi trong quá trình chạy.

### ⚠️ Pitfalls thường gặp:

**Pitfall 1 — Điều kiện loop sai:**
```cpp
// WRONG: thiếu check fast->next
while (fast != nullptr) {
    fast = fast->next->next;  // UB nếu fast->next == nullptr
}

// CORRECT:
while (fast && fast->next) { ... }
```

**Pitfall 2 — Phase 2 reset sai pointer:**
```cpp
// WRONG: reset cả hai về head
entry = head;
from_meeting = head;  // ← sai, phải giữ tại meeting point

// CORRECT:
entry        = head;
from_meeting = slow;  // giữ tại meeting point
```

**Pitfall 3 — Nhầm meeting point = cycle start:**
Meeting point và cycle start **thường khác nhau** (chỉ bằng nhau khi F là bội của C).

**Pitfall 4 — Edge case:**
```cpp
// Empty list
floyd_detect(nullptr);        // ✓ handled

// Single node, no self-loop
// 1 → nullptr              // ✓ handled (fast->next == nullptr)

// Single node, self-loop
// 1 → 1                    // ✓ slow=fast=node1 ngay bước đầu? Không —
                             //   loop chạy ít nhất 1 bước trước khi check
                             //   nên sẽ detect được
```

---

## 7. So sánh với thuật toán tương tự

### Floyd vs HashSet

| | Floyd | HashSet |
|---|---|---|
| Space | **O(1)** | O(n) |
| Time | O(n) | O(n) avg |
| Tìm entry point | Có (phase 2) | Có (node đầu tiên bị revisit) |
| Implement | Phức tạp hơn | Đơn giản hơn |
| Use case | Memory-constrained | Code clarity ưu tiên |

### Floyd vs Brent's Algorithm

Brent cũng là O(1) space, O(n) time, nhưng constant factor nhỏ hơn Floyd trong một số trường hợp.

```
Brent:  slow đứng yên, fast nhảy theo power of 2.
        Đếm bước cho đến khi fast == slow.
        Thực tế ít gặp hơn trong interview vì ít intuitive hơn.
```

Trong competitive programming, Floyd được dùng phổ biến hơn vì dễ nhớ cơ chế. Brent thường xuất hiện trong cryptography (Pollard's rho).

### Floyd vs DFS marking (graph)

Nếu bạn có **graph tổng quát** (không phải linked list), dùng DFS với 3 màu (white/gray/black) để detect back edge — đó mới là cycle detection đúng nghĩa cho graph. Floyd không áp dụng được ở đây.

---

**Tóm gọn để nhớ:**

> Hai runners trên track — nếu có cycle, thỏ sẽ bắt kịp rùa. Khoảng cách từ head đến cổng vào cycle bằng khoảng cách từ điểm gặp nhau đến cổng vào cycle. Reset một, giữ một, đi cùng tốc độ — gặp nhau đúng cổng.