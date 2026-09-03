# Singly Linked List — Giảng viên giải thích

---

## 1. Định nghĩa & cấu trúc bộ nhớ

**Linked list là gì về bản chất?**

Array lưu dữ liệu **liên tục** trong RAM — phần tử thứ `i` luôn ở địa chỉ `base + i * sizeof(T)`. Truy cập O(1) nhưng insert/delete giữa chừng tốn O(n) vì phải shift.

Linked list từ bỏ tính liên tục đó. Mỗi phần tử tự lưu **địa chỉ của phần tử kế tiếp**. Đổi lại: insert/delete O(1) tại chỗ đã biết, nhưng truy cập index O(n).

**Node trông như thế nào trong RAM?**

```cpp
struct Node {
    int val;    // 4 bytes
    Node* next; // 8 bytes (64-bit system)
};
// sizeof(Node) = 12 bytes, nhưng compiler có thể pad → 16 bytes
```

Trong RAM, các node **không liền nhau** — nằm rải rác tùy heap allocator:

```
Địa chỉ:  0x1000        0x2F40        0x0B80
          ┌────┬──────┐  ┌────┬──────┐  ┌────┬──────┐
          │ 10 │0x2F40│→ │ 20 │0x0B80│→ │ 30 │ NULL │
          └────┴──────┘  └────┴──────┘  └────┴──────┘
           head                          tail
```

`head` là pointer trỏ vào node đầu. `tail` (nếu lưu) trỏ vào node cuối. Mất `head` = mất toàn bộ list, không có cách nào recover.

**Hệ quả về cache:**

Array: khi CPU load phần tử, nó prefetch cả cache line 64 bytes xung quanh → các phần tử liền kề được load "miễn phí".

Linked list: mỗi node ở địa chỉ khác nhau → mỗi bước traverse có thể là **cache miss**. Thực tế với list 10k phần tử, linked list traverse chậm hơn array **5–10x** dù cùng O(n) trên giấy.

---

## 2. Các operation cơ bản

### Traverse

Không có index, chỉ có thể đi từ `head` xuôi chiều:

```
cur = head
while cur != nullptr:
    xử lý cur->val
    cur = cur->next
```

Không bao giờ đi ngược được — đây là giới hạn cốt lõi của *singly* linked list.

---

### Insert

**3 trường hợp khác nhau hoàn toàn:**

**Insert đầu — O(1):**
```
Trước:  head → [10] → [20] → [30] → NULL

Bước 1: tạo node mới [5]
Bước 2: new->next = head      →  [5] → [10] → [20] → [30] → NULL
Bước 3: head = new            →  head trỏ vào [5]

Sau:    head → [5] → [10] → [20] → [30] → NULL
```

**Insert cuối — O(1) nếu có `tail`, O(n) nếu không:**
```
Bước 1: tạo node mới [40]
Bước 2: tail->next = new      →  [30] → [40]
Bước 3: tail = new            →  tail trỏ vào [40]
```

**Insert giữa tại index `i` — O(n):**
```
Muốn insert [99] vào index 2:
head → [10] → [20] → [30] → [40] → NULL
                ↑
              prev (index 1)

Bước 1: traverse đến index i-1, gọi là prev
Bước 2: new->next = prev->next    →  [99] → [30]
Bước 3: prev->next = new          →  [20] → [99]

Sau: head → [10] → [20] → [99] → [30] → [40] → NULL
```

**Thứ tự bước 2 và 3 không thể đảo.** Nếu làm `prev->next = new` trước, bạn mất reference tới `[30]` — không còn cách nào gán `new->next` đúng nữa, `[30]` và mọi thứ sau đó bị leak.

---

### Delete

**Delete đầu — O(1):**
```
Trước: head → [10] → [20] → [30] → NULL

Bước 1: tmp = head
Bước 2: head = head->next
Bước 3: delete tmp

Sau: head → [20] → [30] → NULL
```

Edge case: nếu sau bước 2, `head == nullptr` → list vừa rỗng → phải set `tail = nullptr` luôn.

**Delete cuối — O(n):**

Đây là điểm đau của singly linked list. Muốn xóa `tail`, cần biết node **trước** `tail` để set `next = nullptr`. Nhưng không có pointer ngược → phải traverse từ đầu:

```
Trước: head → [10] → [20] → [30] → NULL
                             tail

Traverse đến node mà cur->next == tail, tức [20]
Bước 1: delete tail
Bước 2: [20]->next = nullptr
Bước 3: tail = [20]
```

**Delete node có giá trị `v` — O(n):**

Cần pattern "two-pointer" `prev` + `cur`:

```
Tìm [20], xóa nó:

prev=NULL, cur=[10]  →  cur->val != 20, tiến
prev=[10], cur=[20]  →  tìm thấy

Bước 1: prev->next = cur->next    →  [10] → [30]
Bước 2: delete cur

Nếu cur là head: head = cur->next  (prev == nullptr lúc này)
Nếu cur là tail: tail = prev
```

---

### Search — O(n)

Traverse từ đầu, so sánh từng `val`. Không có shortcut. Nếu list chưa sorted thì phải đi hết.

---

## 3. Cài đặt C++20

```cpp
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node* next;
    Node(int v) : val{v}, next{nullptr} {}
};

class SinglyLinkedList {
   private:
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz = 0;

    void destroy() noexcept {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        sz = 0;
    }

   public:
        // ── Constructors ──────────────────────────────────────────────────────

    SinglyLinkedList() = default;

    // Copy constructor — deep copy, tạo node mới hoàn toàn
    SinglyLinkedList(const SinglyLinkedList& other) {
        Node* cur = other.head;
        while (cur) {
            push_back(cur->val);
            cur = cur->next;
        }
    }

    // Move constructor — lấy ownership, list gốc thành rỗng
    SinglyLinkedList(SinglyLinkedList&& other) noexcept
        : head{other.head}, tail{other.tail}, sz{other.sz} {
        other.head = other.tail = nullptr;
        other.sz = 0;
    }

    // Copy assignment
    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this == &other) return *this;  // self-assignment guard
        clear();
        Node* cur = other.head;
        while (cur) {
            push_back(cur->val);
            cur = cur->next;
        }
        return *this;
    }

    // Move assignment
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this == &other) return *this;
        clear();
        head = other.head;
        tail = other.tail;
        sz   = other.sz;
        other.head = other.tail = nullptr;
        other.sz = 0;
        return *this;
    }

    ~SinglyLinkedList() { destroy(); }

    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }

    void push_front(int v) {
        Node* n = new Node(v);
        if (!head) {
            head = tail = n;
        } else {
            n->next = head;
            head = n;
        }
        sz++;
    }

    void push_back(int v) {
        Node* n = new Node(v);
        if (!tail) {
            head = tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        sz++;
    }

    // insert tại index idx (0-based)
    bool insert(int idx, int v) {
        if (idx < 0 || idx > sz) return false;
        if (idx == 0) { push_front(v); return true; }
        if (idx == sz) { push_back(v); return true; }

        Node* cur = head;
        for (int i = 0; i < idx - 1; i++) cur = cur->next;

        Node* n = new Node(v);
        n->next = cur->next;  // bước 1
        cur->next = n;        // bước 2 — không đảo được
        sz++;
        return true;
    }

    bool pop_front() {
        if (!head) return false;
        Node* tmp = head;
        head = head->next;
        if (!head) tail = nullptr;  // list vừa rỗng
        delete tmp;
        sz--;
        return true;
    }

    bool pop_back() {
        if (!tail) return false;
        if (head == tail) {  // chỉ còn 1 node
            delete head;
            head = tail = nullptr;
            sz--;
            return true;
        }
        Node* cur = head;
        while (cur->next != tail) cur = cur->next;
        delete tail;
        cur->next = nullptr;
        tail = cur;
        sz--;
        return true;
    }

    // xóa node đầu tiên có val == v
    bool remove(int v) {
        Node* prev = nullptr;
        Node* cur = head;
        while (cur && cur->val != v) {
            prev = cur;
            cur = cur->next;
        }
        if (!cur) return false;

        if (cur == tail) tail = prev;
        if (prev) prev->next = cur->next;
        else head = cur->next;
        delete cur;
        sz--;
        return true;
    }

    // tìm index đầu tiên có val == v, trả về -1 nếu không có
    int find(int v) const {
        Node* cur = head;
        int idx = 0;
        while (cur) {
            if (cur->val == v) return idx;
            cur = cur->next;
            idx++;
        }
        return -1;
    }

    void reverse() {
        if (sz <= 1) return;
        Node* prev = nullptr;
        Node* cur = head;
        tail = head;
        while (cur) {
            Node* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }
        head = prev;
    }

    void clear() { destroy(); }

    void print() const {
        for (Node* cur = head; cur; cur = cur->next)
            cout << cur->val << " -> ";
        cout << "nullptr\n";
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    SinglyLinkedList list;

    for (int i = 1; i <= 5; i++) list.push_back(i);
    list.print();  // 1 -> 2 -> 3 -> 4 -> 5 -> nullptr

    list.insert(2, 99);
    list.print();  // 1 -> 2 -> 99 -> 3 -> 4 -> 5 -> nullptr

    list.remove(99);
    list.print();  // 1 -> 2 -> 3 -> 4 -> 5 -> nullptr

    list.reverse();
    list.print();  // 5 -> 4 -> 3 -> 2 -> 1 -> nullptr

    cout << "find(3) = index " << list.find(3) << "\n";  // index 2
    cout << "size = " << list.size() << "\n";

    return 0;
}
```

**Copy constructor/assignment** — traverse từng node của `other`, `push_back` từng cái. Hai list hoàn toàn độc lập sau đó, sửa 1 bên không ảnh hưởng bên kia.

**Move constructor/assignment** — không copy gì cả, chỉ "chuyển" 3 pointer (`head`, `tail`, `sz`) sang object mới, rồi set list gốc về rỗng. O(1) thay vì O(n). Dùng khi list gốc không cần nữa, ví dụ: `SinglyLinkedList b = std::move(a)`.

**Tại sao cần `self-assignment guard`** (`if (this == &other) return *this`) — nếu ai đó viết `list = list`, không có guard thì `clear()` xóa hết data trước khi copy → list rỗng.

Compile:
```bash
g++ -std=c++20 -O2 -Wall -Wextra -o sll main.cpp
```

---

## 4. Độ phức tạp

| Operation | Time | Space | Lý do |
|---|---|---|---|
| `push_front` | O(1) | O(1) | Chỉ thao tác với `head` |
| `push_back` | O(1) | O(1) | Có `tail` pointer → không traverse |
| `insert(idx)` | O(n) | O(1) | Traverse đến `idx-1` |
| `pop_front` | O(1) | O(1) | Chỉ thao tác với `head` |
| `pop_back` | **O(n)** | O(1) | Không có pointer ngược → traverse tìm node trước `tail` |
| `remove(v)` | O(n) | O(1) | Search theo value |
| `find` | O(n) | O(1) | Không có index, không có shortcut |
| `reverse` | O(n) | O(1) | Một lần traverse duy nhất |
| `get(idx)` | O(n) | O(1) | Traverse từ head |

**Space tổng:** O(n) — n node, mỗi node 1 pointer thêm so với array thuần.

**Tại sao `pop_back` không thể O(1) với singly LL?**

Để xóa `tail`, bạn cần set `node_trước_tail->next = nullptr`. Nhưng node đó không tự biết mình là node trước `tail` — phải traverse từ `head` để tìm. Không có cách nào tắt đường này trong singly linked list. Đây là lý do doubly linked list tồn tại.

---

## 5. Khi nào dùng / không dùng

**Dùng khi:**

- Cần insert/delete **đầu list** liên tục — O(1), tốt hơn array rõ rệt. Ví dụ: undo stack, message queue đơn giản.
- Size thay đổi liên tục, không biết trước — không cần realloc như `vector`.
- Implement stack (push/pop front) hoặc queue một chiều (push back, pop front) không cần random access.
- Bộ nhớ phân mảnh — các node có thể nằm rải rác, không cần vùng liên tục.

**Không dùng khi:**

- Cần random access thường xuyên — `list[i]` là O(n), dùng `vector` hoặc `deque`.
- Dataset lớn và cần traverse nhiều — cache miss sẽ giết performance. `vector` với O(n) insert/delete đôi khi vẫn nhanh hơn trong thực tế do cache locality.
- Cần `pop_back` thường xuyên — O(n), không acceptable. Dùng doubly linked list.
- Memory footprint là vấn đề — mỗi node tốn thêm 8 bytes pointer, với list hàng triệu phần tử là đáng kể.

**Edge case nguy hiểm:**

```
Empty list:   head = tail = nullptr — mọi operation phải check trước
1 node:       head == tail — xóa node này phải set cả 2 về nullptr
Insert/delete head hoặc tail — luôn là trường hợp đặc biệt, không thể dùng logic giữa list
```

**Pitfall phổ biến:**

```cpp
// Sai — đảo thứ tự khi insert giữa
cur->next = n;    // mất reference tới node sau → leak
n->next = ...;    // cur->next đã là n rồi, vô nghĩa

// Sai — không update tail khi pop_back xóa node cuối cùng
// Sai — không update tail khi remove() xóa tail
// Sai — copy constructor mặc định: shallow copy pointer → double-free
```

---

## 6. Biến thể liên quan

**Doubly Linked List** — mỗi node có thêm pointer `prev`, cho phép traverse 2 chiều và `pop_back` O(1).

**Circular Linked List** — `tail->next` trỏ về `head` thay vì `nullptr`, dùng cho các cấu trúc vòng như round-robin scheduler.

**Skip List** — nhiều tầng "express lane" trên linked list, cho phép search O(log n) thay vì O(n), dùng trong Redis sorted set.

**XOR Linked List** — lưu `prev XOR next` trong một pointer duy nhất, tiết kiệm memory nhưng không dùng được với GC hay smart pointer.

**Unrolled Linked List** — mỗi node chứa một mảng nhỏ thay vì 1 phần tử, cải thiện cache locality đáng kể so với linked list thông thường.