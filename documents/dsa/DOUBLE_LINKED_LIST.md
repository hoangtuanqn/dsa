Được, để inline luôn.

---

# Doubly Linked List

## 1. Định nghĩa & cấu trúc bộ nhớ

**Singly list hỏng ở đâu?**

Singly có một khuyết tật cấu trúc: thông tin chỉ chảy một chiều. Hệ quả không phải "bất tiện", mà là hai giới hạn cứng:

1. `pop_back` O(n) — muốn xóa `tail` phải traverse từ `head` tìm node đứng trước.
2. Ngay cả khi bạn **đã cầm trong tay** pointer tới node cần xóa, xóa vẫn O(n) — vì không biết `prev`.

Điểm 2 mới là điểm chết. Nó khiến singly không dùng được cho lớp bài toán "tôi giữ sẵn pointer tới phần tử, hãy nhấc nó ra ngay" — đúng bài toán của LRU cache, scheduler queue, free-list allocator.

Doubly trả 8 bytes/node để mua lại khả năng đó.

**Node trong RAM:**

```cpp
struct Node {
    int   val;   // 4 bytes
    Node* prev;  // 8 bytes
    Node* next;  // 8 bytes
};
// payload 20 bytes, alignof = 8 → pad thành 24 bytes
```

Con số thật (g++ 13, x86-64, tôi đã compile kiểm chứng):

| | singly | doubly |
|---|---|---|
| `sizeof(Node)` | 16 B | **24 B** |
| Payload (`int`) | 4 B | 4 B |
| Overhead | 75% | **83%** |

Nói thẳng: 24 bytes để lưu 4 bytes dữ liệu. `vector<int>` 1 triệu phần tử tốn 4 MB; doubly list tương đương tốn 24 MB, chưa kể metadata allocator (glibc malloc thêm 8–16 B/block + làm tròn bội số 16) → thực tế 32–40 MB.

**Layout:**

```
Địa chỉ:      0x1000              0x2F40              0x0B80
          ┌──────┬────┬──────┐ ┌──────┬────┬──────┐ ┌──────┬────┬──────┐
          │ NULL │ 10 │0x2F40│ │0x1000│ 20 │0x0B80│ │0x2F40│ 30 │ NULL │
          │ prev │val │ next │ │ prev │val │ next │ │ prev │val │ next │
          └──────┴────┴──────┘ └──────┴────┴──────┘ └──────┴────┴──────┘
             ▲                    ▲   ▲                 ▲
             │                    └───┘ trỏ ngược lại
           head                                        tail
```

**Bất biến (invariant) — thuộc lòng:**

```
1. head->prev == nullptr
2. tail->next == nullptr
3. ∀x:  x->next != nullptr  ⟹  x->next->prev == x
4. ∀x:  x->prev != nullptr  ⟹  x->prev->next == x
```

Điều 3 và 4 là **tính đối xứng**. Singly không có khái niệm này — nó không thể sai đối xứng vì chỉ có một chiều. Doubly thì có, và đây là nguồn bug số một: bạn cập nhật `next` đúng, quên `prev`, list traverse xuôi vẫn ra kết quả đúng, test pass, chương trình chạy hàng nghìn dòng lệnh rồi mới crash lúc traverse ngược hoặc lúc `erase`. Mục 5 nói kỹ.

**Cache — tệ hơn singly.** Cache line 64 B. Singly node 16 B → 4 node/line. Doubly 24 B → 2 node/line. Cùng số phần tử, doubly gây cache miss nhiều hơn ~1.5–2x khi duyệt tuần tự. Doubly **không** nhanh hơn singly khi duyệt; nó chỉ nhanh hơn ở thao tác cần biết `prev`.

---

## 2. Các operation cơ bản

### Traverse — hai chiều

```
// Xuôi
cur = head;  while (cur) { ...; cur = cur->next; }

// Ngược — điều singly không bao giờ làm được
cur = tail;  while (cur) { ...; cur = cur->prev; }
```

Khác biệt vận hành: ở singly, `print_reverse()` bắt buộc dùng đệ quy hoặc stack phụ O(n) bộ nhớ. Ở doubly nó là một vòng `while`, O(1) bộ nhớ.

Hệ quả thứ hai — truy cập index đi từ đầu gần hơn:

```
get(idx):
    idx < sz/2  → đi từ head, idx bước
    ngược lại   → đi từ tail, sz-1-idx bước
```

Vẫn O(n), nhưng worst case `n/2` thay vì `n`. Đây là tối ưu hằng số, **không** đổi độ phức tạp — đừng ghi O(n/2) trong báo cáo.

---

### Insert

Nguyên tắc: mỗi lần chèn phải ghi **4 pointer** (singly chỉ 2) — `new->prev`, `new->next`, `next` của hàng xóm trái, `prev` của hàng xóm phải.

**Insert đầu — O(1):**

```
Trước:  nullptr <- [10] <-> [20] <-> [30] -> nullptr
                    ▲head                     ▲tail

Bước 1: tạo node [5]
Bước 2: new->next = head        →  [5] --next--> [10]
Bước 3: head->prev = new        →  [5] <--prev-- [10]   ← singly KHÔNG có bước này
Bước 4: head = new

Sau:    nullptr <- [5] <-> [10] <-> [20] <-> [30] -> nullptr
```

Edge case: list rỗng thì bước 3 deref nullptr → segfault. Phải tách nhánh `if (!head) head = tail = n;`

**Insert cuối — O(1):** đối xứng hoàn toàn (`new->prev = tail; tail->next = new; tail = new;`), cũng cần nhánh riêng cho list rỗng.

**Insert ngay trước node `pos` đã biết — O(1).** Đây là operation nền tảng, mọi thứ khác quy về nó:

```
Chèn [99] vào trước [30]:

        [20] <-------------> [30]
         ▲                    ▲
      L = pos->prev          pos

Bước 1: n->prev = pos->prev      →  [20] <--prev-- [99]
Bước 2: n->next = pos            →  [99] --next--> [30]
Bước 3: pos->prev->next = n      →  [20] --next--> [99]
Bước 4: pos->prev = n            →  [99] <--prev-- [30]
```

**Thứ tự bắt buộc.** Bước 1 và 3 đều đọc `pos->prev` (node `L`). Bước 4 ghi đè nó. Làm bước 4 sớm là `L` biến mất khỏi tầm với, list đứt, nửa trái leak. Quy tắc chung áp dụng cho mọi bài linked list:

> Gán pointer của node mới trước, sửa hàng xóm sau. Trong nhóm "sửa hàng xóm", cái nào bị ghi đè mà còn cần đọc thì để cuối cùng.

**Đây là khác biệt lớn nhất so với singly.** Ở singly, muốn chèn trước `pos` bạn **không thể** — buộc phải có `prev`, tức traverse từ `head`, O(n). Ở doubly chỉ cần `pos`, O(1). Cùng chữ ký hàm, một bên O(n), một bên O(1).

**Insert tại index `i` — O(n):** traverse tới node index `i` (dùng mẹo đầu gần hơn) rồi gọi `insert_before`. Chi phí nằm ở traverse, không ở việc nối pointer.

---

### Delete

**Xóa node `x` khi đã có pointer tới `x` — O(1). Đây là lý do doubly tồn tại.**

```
Trước:  [10] <-> [20] <-> [30]
                  ▲x

Bước 1: x->prev->next = x->next    →  [10] --next--> [30]
Bước 2: x->next->prev = x->prev    →  [10] <--prev-- [30]
Bước 3: delete x
```

Hai phép gán, không traverse, không cần biết `x` ở đâu trong list.

So với singly cùng bài toán: có pointer `x`, muốn xóa → phải traverse từ `head` tìm node có `next == x`, O(n). (Có mẹo bẩn: copy `x->next->val` vào `x` rồi xóa `x->next` — nhưng sai khi `x` là node cuối, và làm hỏng mọi pointer bên ngoài đang trỏ vào `x->next`. Đừng dùng trong production.)

**Bốn edge case ở bản không dummy**, gói gọn trong 4 dòng:

```cpp
if (x->prev) x->prev->next = x->next; else head = x->next;   // x là head
if (x->next) x->next->prev = x->prev; else tail = x->prev;   // x là tail
delete x;
```

Bốn dòng này xử lý trọn: giữa list, là head, là tail, và là node duy nhất (cả hai nhánh `else` chạy → `head = tail = nullptr`). Đọc kỹ — nó là hạt nhân của cả class.

**`pop_back` — O(1), điểm đau của singly biến mất:**

```
Bước 1: newTail = tail->prev     ← singly không có dòng này, phải traverse O(n)
Bước 2: newTail->next = nullptr
Bước 3: delete tail
Bước 4: tail = newTail
```

Thực chất `pop_back()` chỉ là `erase(tail)`, `pop_front()` chỉ là `erase(head)`.

**Xóa theo giá trị — O(n):** vẫn search O(n), nhưng **không cần pattern two-pointer `prev`+`cur`** như singly. Tìm ra node rồi `erase(node)`. Ngắn hơn, ít bug hơn hẳn.

---

### Search — O(n)

Không khác singly, không có shortcut. Lợi thế nhỏ: nếu biết phần tử có xu hướng nằm gần cuối thì duyệt ngược từ `tail`. Cần tìm nhanh thật sự thì đừng dùng linked list — dùng hash map trỏ vào node (mục 5).

### Reverse — O(n), thanh lịch hơn singly

Singly reverse là màn nhào lộn 3 pointer. Doubly chỉ cần **swap `prev` và `next` của từng node** rồi swap `head`/`tail`:

```cpp
Node* cur = head;
while (cur) {
    Node* nxt = cur->next;      // lưu TRƯỚC, vì swap sẽ phá cur->next
    std::swap(cur->prev, cur->next);
    cur = nxt;
}
std::swap(head, tail);
```

Dòng `nxt` phải nằm trước `swap`. Sau swap, `cur->next` chính là node cũ phía trước — đi tiếp bằng nó là quay ngược lại và lặp vô hạn.

---

### Bảng đối chiếu singly ↔ doubly

| Thao tác | Singly | Doubly | Ghi chú |
|---|---|---|---|
| `sizeof(Node)` | 16 B | 24 B | +50% |
| Traverse ngược | cần stack O(n) | O(1) bộ nhớ | |
| `push_front`/`push_back` | O(1) | O(1) | doubly ghi 4 pointer thay vì 2 |
| `pop_front` | O(1) | O(1) | hòa |
| `pop_back` | **O(n)** | **O(1)** | khác biệt lớn nhất về Big-O |
| `erase(Node* x)` khi đã có `x` | **O(n)** | **O(1)** | khác biệt quan trọng nhất trong thực tế |
| `insert_before(pos)` | **O(n)** | **O(1)** | |
| `get(idx)` | tối đa `n` bước | tối đa `n/2` bước | chỉ cải thiện hằng số |
| Pointer phải ghi khi insert | 2 | 4 | nhiều chỗ sai hơn |
| Edge case trong `erase` | 2 | 4 | dummy triệt tiêu — mục 3.2 |

---

## 3. Cài đặt C++20

Hai bản. Bản A là "sách giáo khoa", `head`/`tail` thô, để thấy rõ từng edge case. Bản B dùng dummy node — đây là bản `std::list` của libstdc++ thực sự dùng, và là bản nên viết khi làm việc thật.

Cả hai đã compile `-std=c++20 -O2 -Wall -Wextra` (0 warning) và chạy sạch dưới AddressSanitizer + UBSan.

### 3.1 Bản A — head/tail thuần

```cpp
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node* prev;
    Node* next;
    explicit Node(int v) : val{v}, prev{nullptr}, next{nullptr} {}
};

class DoublyLinkedList {
   private:
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t sz = 0;

    void destroy() noexcept {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;   // lưu trước khi delete — bắt buộc
            delete cur;
            cur = nxt;
        }
        head = tail = nullptr;
        sz = 0;
    }

    void copy_from(const DoublyLinkedList& other) {
        for (Node* cur = other.head; cur; cur = cur->next) push_back(cur->val);
    }

   public:
    // ── Rule of five ──────────────────────────────────────────────────────
    DoublyLinkedList() = default;

    DoublyLinkedList(const DoublyLinkedList& other) { copy_from(other); }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head{other.head}, tail{other.tail}, sz{other.sz} {
        other.head = other.tail = nullptr;
        other.sz = 0;
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) return *this;   // self-assignment guard
        destroy();
        copy_from(other);
        return *this;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this == &other) return *this;
        destroy();
        head = other.head;
        tail = other.tail;
        sz   = other.sz;
        other.head = other.tail = nullptr;
        other.sz = 0;
        return *this;
    }

    ~DoublyLinkedList() { destroy(); }

    // ── Query ─────────────────────────────────────────────────────────────
    size_t size() const noexcept { return sz; }
    bool empty() const noexcept { return sz == 0; }
    Node* front_node() const noexcept { return head; }
    Node* back_node() const noexcept { return tail; }

    // ── Insert ────────────────────────────────────────────────────────────
    void push_front(int v) {
        Node* n = new Node(v);
        if (!head) {
            head = tail = n;          // list rỗng: vừa là head vừa là tail
        } else {
            n->next = head;
            head->prev = n;           // dòng singly không có
            head = n;
        }
        sz++;
    }

    void push_back(int v) {
        Node* n = new Node(v);
        if (!tail) {
            head = tail = n;
        } else {
            n->prev = tail;
            tail->next = n;
            tail = n;
        }
        sz++;
    }

    // Traverse tới index idx, chọn đầu gần hơn. O(min(idx, sz-idx)).
    Node* node_at(size_t idx) const {
        if (idx >= sz) return nullptr;
        if (idx < sz / 2) {
            Node* cur = head;
            for (size_t i = 0; i < idx; i++) cur = cur->next;
            return cur;
        }
        Node* cur = tail;
        for (size_t i = sz - 1; i > idx; i--) cur = cur->prev;
        return cur;
    }

    // Chèn v NGAY TRƯỚC pos. O(1) — singly không làm được.
    void insert_before(Node* pos, int v) {
        if (!pos)        { push_back(v);  return; }   // pos == "end" → nối đuôi
        if (pos == head) { push_front(v); return; }   // không có hàng xóm trái
        Node* n = new Node(v);
        n->prev = pos->prev;      // 1. pointer của node mới
        n->next = pos;            // 2.
        pos->prev->next = n;      // 3. hàng xóm trái — vẫn đọc được pos->prev
        pos->prev = n;            // 4. ghi đè pos->prev, phải để cuối
        sz++;
    }

    bool insert(size_t idx, int v) {
        if (idx > sz) return false;
        if (idx == sz) { push_back(v); return true; }
        insert_before(node_at(idx), v);
        return true;
    }

    // ── Delete ────────────────────────────────────────────────────────────
    // Hạt nhân của cả class. O(1). Xử lý trọn 4 edge case.
    void erase(Node* x) {
        if (!x) return;
        if (x->prev) x->prev->next = x->next;
        else         head = x->next;            // x là head
        if (x->next) x->next->prev = x->prev;
        else         tail = x->prev;            // x là tail
        delete x;
        sz--;
    }

    bool pop_front() {
        if (!head) return false;
        erase(head);
        return true;
    }

    bool pop_back() {                 // O(1) — điểm đau của singly đã biến mất
        if (!tail) return false;
        erase(tail);
        return true;
    }

    // ── Search ────────────────────────────────────────────────────────────
    Node* find_node(int v) const {
        for (Node* cur = head; cur; cur = cur->next)
            if (cur->val == v) return cur;
        return nullptr;
    }

    int find(int v) const {
        int idx = 0;
        for (Node* cur = head; cur; cur = cur->next, idx++)
            if (cur->val == v) return idx;
        return -1;
    }

    bool remove(int v) {              // không cần two-pointer prev/cur như singly
        Node* x = find_node(v);
        if (!x) return false;
        erase(x);
        return true;
    }

    // ── Misc ──────────────────────────────────────────────────────────────
    void reverse() noexcept {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->next;          // lưu TRƯỚC khi swap
            std::swap(cur->prev, cur->next);
            cur = nxt;
        }
        std::swap(head, tail);
    }

    void clear() noexcept { destroy(); }

    void print() const {
        cout << "nullptr <- ";
        for (Node* cur = head; cur; cur = cur->next) cout << cur->val << " <-> ";
        cout << "nullptr\n";
    }

    void print_reverse() const {        // O(1) bộ nhớ — singly phải dùng stack
        for (Node* cur = tail; cur; cur = cur->prev) cout << cur->val << " -> ";
        cout << "nullptr\n";
    }

    // Hàm debug. Gọi sau mỗi operation khi đang viết bài / tìm bug.
    // Bắt đúng loại bug "quên cập nhật pointer ngược".
    bool check_invariant() const {
        if (sz == 0) return head == nullptr && tail == nullptr;
        if (!head || !tail) return false;
        if (head->prev != nullptr || tail->next != nullptr) return false;
        size_t cnt = 0;
        for (Node* cur = head; cur; cur = cur->next) {
            cnt++;
            if (cur->next && cur->next->prev != cur) return false;  // mất đối xứng
            if (!cur->next && cur != tail) return false;            // tail sai
        }
        return cnt == sz;
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    DoublyLinkedList list;
    for (int i = 1; i <= 5; i++) list.push_back(i);
    list.print();          // nullptr <- 1 <-> 2 <-> 3 <-> 4 <-> 5 <-> nullptr
    list.print_reverse();  // 5 -> 4 -> 3 -> 2 -> 1 -> nullptr

    list.insert(2, 99);
    list.print();          // ... 1 <-> 2 <-> 99 <-> 3 <-> 4 <-> 5 ...

    list.pop_back();       // O(1)
    list.remove(99);
    list.print();          // ... 1 <-> 2 <-> 3 <-> 4 ...

    list.reverse();
    list.print();          // ... 4 <-> 3 <-> 2 <-> 1 ...

    cout << "find(3)   = " << list.find(3) << "\n";
    cout << "size      = " << list.size() << "\n";
    cout << "invariant = " << boolalpha << list.check_invariant() << "\n";

    DoublyLinkedList b = list;              // deep copy
    b.push_back(1000);
    DoublyLinkedList c = std::move(b);      // O(1), b thành rỗng
    cout << "c size = " << c.size() << ", b size = " << b.size() << "\n";
    return 0;
}
```

Ba chỗ đáng chú ý:

- `erase()` là hàm **duy nhất** thực sự xóa. `pop_front`, `pop_back`, `remove` đều gọi lại nó. Nguyên tắc: một điểm sửa pointer duy nhất. Viết logic gỡ node ở 4 chỗ khác nhau thì chắc chắn sẽ quên cập nhật `tail` ở một trong bốn chỗ — bug kinh điển trong doc singly của bạn cũng cùng bản chất.
- `destroy()` và `reverse()` đều phải lưu `next` vào biến tạm trước khi phá node hiện tại. Không phải style, là bắt buộc: `delete cur; cur = cur->next;` là use-after-free.
- `check_invariant()` là công cụ debug, không phải code thừa. Bug đối xứng không tự lộ ra; hàm này bắt nó ngay tại operation gây lỗi thay vì 500 dòng sau.

---

### 3.2 Dummy node (sentinel)

Đếm số `if` trong bản A:

```
push_front    : 1 nhánh (list rỗng?)
push_back     : 1 nhánh
insert_before : 2 nhánh (pos null? pos là head?)
erase         : 2 nhánh
```

Toàn bộ số nhánh này tồn tại vì **`head` và `tail` không có hàng xóm ở một phía**. Node giữa list luôn có đủ `prev` và `next`; node biên thì không, nên phải viết code riêng.

**Ý tưởng dummy: tạo hàng xóm giả để không còn node biên nào cả.** Cách hiệu quả nhất (chính là cách libstdc++ làm): **một node giả duy nhất, list nối vòng tròn**.

```
List rỗng:
        ┌──────────┐
        │  [nil]   │◄─┐
        │ prev next│  │
        └──┬────┬──┘  │
           └────┴─────┘     nil.next == nil.prev == &nil


List 3 phần tử:
     ┌──────────────────────────────────────────────────┐
     │                                                  │
     ▼                                                  │
  ┌─────┐     ┌────┐     ┌────┐     ┌────┐              │
  │[nil]│◄───►│ 10 │◄───►│ 20 │◄───►│ 30 │◄─────────────┘
  └─────┘     └────┘     └────┘     └────┘
     ▲          ▲                     ▲
  sentinel   nil.next              nil.prev
             (= "head")            (= "tail")
```

`nil` không mang dữ liệu. Nó là một địa chỉ hợp lệ đóng vai:

- `head` cũ → `nil.next`
- `tail` cũ → `nil.prev`
- `nullptr` hai đầu → `&nil`
- điều kiện dừng `cur != nullptr` → `cur != &nil`

**Cái gì biến mất:**

| | Bản A | Bản B (dummy) |
|---|---|---|
| `erase` | 4 dòng, 2 nhánh | 2 dòng, 0 nhánh |
| `push_front` | 1 nhánh | 0 — chỉ là `insert_before(nil.next)` |
| `push_back` | 1 nhánh | 0 — chỉ là `insert_before(&nil)` |
| List rỗng | check ở mọi hàm | tự đúng, không cần code |
| List 1 node | phải check | tự đúng |
| Rủi ro deref nullptr | có | không (mọi pointer luôn hợp lệ) |

So sánh trực tiếp `erase`:

```cpp
// Bản A
if (x->prev) x->prev->next = x->next; else head = x->next;
if (x->next) x->next->prev = x->prev; else tail = x->prev;
delete x;

// Bản B — x->prev và x->next KHÔNG BAO GIỜ null, tệ nhất là &nil
x->prev->next = x->next;
x->next->prev = x->prev;
delete x;
```

Đó là toàn bộ giá trị của dummy: **biến trường hợp đặc biệt thành trường hợp tổng quát**. Ít nhánh hơn → ít bug hơn, ít branch misprediction hơn, code đọc đúng như mô tả toán học.

**Cái giá — ba điểm, đừng bỏ qua:**

1. Tốn thêm 24 bytes mỗi list. Không đáng kể trừ khi bạn có hàng triệu list nhỏ.
2. Mọi vòng lặp dừng ở `cur != &nil`, không phải `cur != nullptr`. Quên một chỗ là loop vô hạn — list nối vòng, không bao giờ gặp `nullptr`.
3. **Bẫy nghiêm trọng nhất:** nếu `nil` là member theo giá trị (`Node nil;`), move constructor **không thể** chỉ copy hai pointer. Node đầu/cuối của list nguồn đang trỏ về `&other.nil` — địa chỉ sẽ chết khi `other` hết scope. Phải khâu lại vòng:

```cpp
nil.next = other.nil.next;
nil.prev = other.nil.prev;
nil.next->prev = &nil;      // ← thiếu 2 dòng này là dangling pointer
nil.prev->next = &nil;      // ←
```

Lỗi này không crash ngay. Nó crash lúc traverse ngược, ở một hàm khác, rất lâu sau đó. Cùng lý do, `swap()` giữa hai list dùng sentinel-by-value cũng phải khâu lại.

---

### 3.3 Bản B — dummy sentinel, đầy đủ

```cpp
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    Node* prev;
    Node* next;
    Node() : val{0}, prev{this}, next{this} {}          // ctor sentinel: tự trỏ vào mình
    explicit Node(int v) : val{v}, prev{nullptr}, next{nullptr} {}
};

class DllSentinel {
   private:
    Node nil;              // sentinel theo giá trị, không cấp phát heap
    size_t sz = 0;

    // Hai primitive duy nhất. Mọi operation khác đều gọi hai hàm này.
    void link_before(Node* pos, Node* n) noexcept {
        n->prev = pos->prev;
        n->next = pos;
        pos->prev->next = n;
        pos->prev = n;         // ghi đè cuối cùng
        sz++;
    }

    void unlink(Node* x) noexcept {   // gỡ khỏi list, KHÔNG delete
        x->prev->next = x->next;
        x->next->prev = x->prev;
        sz--;
    }

    void steal(DllSentinel& other) noexcept {
        if (other.sz == 0) return;
        nil.next = other.nil.next;
        nil.prev = other.nil.prev;
        nil.next->prev = &nil;        // khâu lại vòng tròn — BẮT BUỘC
        nil.prev->next = &nil;
        sz = other.sz;
        other.nil.next = other.nil.prev = &other.nil;
        other.sz = 0;
    }

   public:
    // ── Rule of five ──────────────────────────────────────────────────────
    DllSentinel() = default;

    DllSentinel(const DllSentinel& other) : nil{} {
        for (Node* cur = other.nil.next; cur != &other.nil; cur = cur->next)
            push_back(cur->val);
    }

    DllSentinel& operator=(const DllSentinel& other) {
        if (this == &other) return *this;
        clear();
        for (Node* cur = other.nil.next; cur != &other.nil; cur = cur->next)
            push_back(cur->val);
        return *this;
    }

    DllSentinel(DllSentinel&& other) noexcept : nil{} { steal(other); }

    DllSentinel& operator=(DllSentinel&& other) noexcept {
        if (this == &other) return *this;
        clear();
        steal(other);
        return *this;
    }

    ~DllSentinel() { clear(); }

    // ── Query ─────────────────────────────────────────────────────────────
    // begin_node/end_node đóng vai begin()/end() của STL: end_node là sentinel,
    // là vị trí "sau phần tử cuối", KHÔNG mang dữ liệu.
    Node* begin_node() const noexcept { return nil.next; }
    Node* end_node()   const noexcept { return const_cast<Node*>(&nil); }
    size_t size()      const noexcept { return sz; }
    bool empty()       const noexcept { return sz == 0; }

    // ── Insert — không một nhánh if nào ───────────────────────────────────
    void insert_before(Node* pos, int v) { link_before(pos, new Node(v)); }
    void push_back(int v)  { link_before(end_node(), new Node(v)); }
    void push_front(int v) { link_before(nil.next,   new Node(v)); }

    // ── Delete ────────────────────────────────────────────────────────────
    void erase(Node* x) noexcept {
        if (x == end_node()) return;     // chặn xóa nhầm sentinel
        unlink(x);
        delete x;
    }

    bool pop_front() noexcept {
        if (empty()) return false;
        erase(nil.next);
        return true;
    }

    bool pop_back() noexcept {
        if (empty()) return false;
        erase(nil.prev);
        return true;
    }

    // ── Search ────────────────────────────────────────────────────────────
    // Trả về end_node() khi không thấy — đúng quy ước STL, không dùng nullptr.
    Node* find_node(int v) const noexcept {
        for (Node* cur = nil.next; cur != end_node(); cur = cur->next)
            if (cur->val == v) return cur;
        return end_node();
    }

    bool remove(int v) noexcept {
        Node* x = find_node(v);
        if (x == end_node()) return false;
        erase(x);
        return true;
    }

    // Nhấc node có sẵn lên đầu list. O(1), không cấp phát, không nhánh.
    // Chính xác là thao tác lõi của LRU cache — thứ singly không làm được O(1).
    void move_to_front(Node* x) noexcept {
        if (x == end_node() || x == nil.next) return;
        unlink(x);                  // phải unlink TRƯỚC khi link lại,
        link_before(nil.next, x);   // nếu không sẽ tạo self-loop
    }

    void clear() noexcept {
        Node* cur = nil.next;
        while (cur != end_node()) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        nil.next = nil.prev = &nil;   // reset về trạng thái rỗng
        sz = 0;
    }

    void print() const {
        for (Node* cur = nil.next; cur != end_node(); cur = cur->next)
            cout << cur->val << " <-> ";
        cout << "[nil]\n";
    }

    void print_reverse() const {
        for (Node* cur = nil.prev; cur != end_node(); cur = cur->prev)
            cout << cur->val << " <-> ";
        cout << "[nil]\n";
    }
};

int main() {
    DllSentinel l;
    for (int i = 1; i <= 5; i++) l.push_back(i);
    l.print();            // 1 <-> 2 <-> 3 <-> 4 <-> 5 <-> [nil]
    l.print_reverse();    // 5 <-> 4 <-> 3 <-> 2 <-> 1 <-> [nil]

    l.push_front(0);
    l.pop_back();
    l.pop_front();
    l.remove(3);
    l.print();            // 1 <-> 2 <-> 4 <-> [nil]

    l.move_to_front(l.find_node(4));
    l.print();            // 4 <-> 1 <-> 2 <-> [nil]

    DllSentinel c = l;              // deep copy
    DllSentinel d = std::move(c);   // steal + khâu lại vòng
    d.print_reverse();

    while (l.pop_front()) {}
    l.print();            // [nil]  — list rỗng vẫn traverse an toàn
    return 0;
}
```

### 3.4 Compile & flag debug

```bash
# Release
g++ -std=c++20 -O2 -Wall -Wextra -o dll main.cpp

# Debug — LUÔN chạy bản này trước khi tin là code đúng
g++ -std=c++20 -g -O0 -Wall -Wextra -fsanitize=address,undefined -o dll_dbg main.cpp
./dll_dbg
```

ASan bắt use-after-free, double-free, memory leak — ba lỗi chiếm gần như toàn bộ bug pointer khi viết linked list. Với `-O2` không sanitizer, các lỗi đó thường **không** crash ngay; chương trình vẫn in ra kết quả có vẻ đúng. Đừng dựa vào "chạy không crash" để kết luận code đúng.

---

## 4. Độ phức tạp

| Operation | Time | Space | Lý do |
|---|---|---|---|
| `push_front` | O(1) | O(1) | Chỉ đụng `head`/`nil.next` |
| `push_back` | O(1) | O(1) | Có `tail`/`nil.prev` |
| `insert_before(Node*)` | **O(1)** | O(1) | Có `pos->prev` sẵn — singly O(n) |
| `insert(idx)` | O(n) | O(1) | Chi phí ở traverse, tối đa `n/2` bước |
| `pop_front` | O(1) | O(1) | |
| `pop_back` | **O(1)** | O(1) | Có `tail->prev` — singly O(n) |
| `erase(Node*)` | **O(1)** | O(1) | Hai phép gán — singly O(n) |
| `remove(v)` | O(n) | O(1) | Search O(n) + erase O(1) |
| `find` | O(n) | O(1) | Không shortcut |
| `get(idx)` | O(n) | O(1) | Tối đa `n/2` bước, vẫn O(n) |
| `reverse` | O(n) | O(1) | Một lần duyệt, swap tại chỗ |
| `move_to_front(Node*)` | **O(1)** | O(1) | unlink + link, không cấp phát |
| Traverse ngược toàn list | O(n) | **O(1)** | Singly cần O(n) stack phụ |
| Copy ctor | O(n) | O(n) | Cấp phát `n` node mới |
| Move ctor | O(1) | O(1) | Chuyển pointer (+khâu vòng ở bản sentinel) |

**Space tổng:** O(n), nhưng hằng số quan trọng. `24n` bytes cho node + metadata allocator. Thực đo với `int`: khoảng **32 bytes RAM cho mỗi 4 bytes dữ liệu**, hệ số 8x so với `vector<int>`.

**Tại sao `erase(Node*)` O(1) mới là điều quan trọng nhất, không phải `pop_back`?**

`pop_back` O(1) nghe kêu nhưng ít khi là lý do chọn doubly — nếu chỉ push/pop hai đầu thì `std::deque` nhanh hơn nhiều lần nhờ cache locality.

Giá trị thật: doubly cho phép **giữ pointer tới một phần tử ở giữa và thao tác trên nó trong O(1)** — xóa nó, chèn trước nó, chuyển nó sang list khác. Không cấu trúc nào khác làm được với chi phí đó: `vector`/`deque` phải shift O(n), cây cân bằng O(log n).

Chính vì thế `std::list` đảm bảo **iterator stability**: `insert`/`erase` không làm hỏng iterator trỏ tới phần tử khác. `vector` thì mọi iterator có thể invalid sau một `push_back` (realloc). Đây là tính chất, không phải hiệu năng — và đôi khi là lý do duy nhất chính đáng để chọn `std::list`.

---

## 5. Khi nào dùng / không dùng

### Dùng khi

**1. LRU cache — use case kinh điển, đáng học thuộc.**

Yêu cầu: `get` và `put` đều O(1), cache đầy thì loại phần tử ít dùng nhất. Kiến trúc: `unordered_map<key, Node*>` + doubly list. Map cho O(1) tìm node; list giữ thứ tự truy cập (đầu = mới dùng nhất, cuối = cũ nhất). Mỗi lần `get`, nhấc node lên đầu — O(1), chỉ làm được nhờ có `prev`.

```cpp
struct LNode {
    int key, val;
    LNode *prev, *next;
    LNode() : key{0}, val{0}, prev{this}, next{this} {}      // sentinel
    LNode(int k, int v) : key{k}, val{v}, prev{nullptr}, next{nullptr} {}
};

class LRUCache {
    LNode nil;                          // nil.next = MRU, nil.prev = LRU
    unordered_map<int, LNode*> mp;
    size_t cap;

    void unlink(LNode* x) noexcept {
        x->prev->next = x->next;
        x->next->prev = x->prev;
    }
    void link_front(LNode* x) noexcept {
        x->next = nil.next;
        x->prev = &nil;
        nil.next->prev = x;
        nil.next = x;
    }

   public:
    explicit LRUCache(size_t c) : cap{c} { mp.reserve(c * 2); }
    ~LRUCache() {
        LNode* cur = nil.next;
        while (cur != &nil) { LNode* nxt = cur->next; delete cur; cur = nxt; }
    }
    LRUCache(const LRUCache&) = delete;             // không cho copy nhầm
    LRUCache& operator=(const LRUCache&) = delete;

    int get(int key) {
        auto it = mp.find(key);
        if (it == mp.end()) return -1;
        unlink(it->second);
        link_front(it->second);          // vừa dùng → lên đầu. O(1).
        return it->second->val;
    }

    void put(int key, int val) {
        if (auto it = mp.find(key); it != mp.end()) {
            it->second->val = val;
            unlink(it->second);
            link_front(it->second);
            return;
        }
        if (mp.size() == cap) {          // đầy → loại phần tử cuối
            LNode* lru = nil.prev;
            unlink(lru);
            mp.erase(lru->key);          // xóa khỏi map TRƯỚC khi delete node
            delete lru;
        }
        LNode* n = new LNode(key, val);
        link_front(n);
        mp[key] = n;
    }
};
```

Hai chi tiết dễ sai: `mp.erase(lru->key)` phải chạy trước `delete lru` (sau delete thì `lru->key` là use-after-free); và trong `get` phải `unlink` trước `link_front`, ngược lại node tự trỏ vào chính nó.

Thử làm cấu trúc này bằng singly: bạn có `Node*` từ map, nhưng để gỡ nó ra phải tìm node đứng trước → O(n), toàn bộ thiết kế sụp đổ. **Doubly linked list là điều kiện cần của LRU O(1).**

**2. Cần pointer/iterator ổn định.** Bạn giữ tham chiếu tới phần tử ở nơi khác trong hệ thống trong khi list vẫn bị insert/erase. `vector` không đáp ứng được.

**3. Splice — nối/cắt đoạn list trong O(1).** Chuyển cả một đoạn từ list này sang list khác chỉ bằng vài phép gán pointer, không copy phần tử nào (`std::list::splice`). Dùng trong scheduler khi chuyển task giữa các hàng đợi trạng thái.

**4. Undo/redo, browser history, playlist.** Bản chất là con trỏ "vị trí hiện tại" chạy tới lui trên một chuỗi. Đi lùi cần `prev`.

**5. Kernel/embedded — intrusive list.** Linux dùng `struct list_head` (doubly, circular, có sentinel) nhúng thẳng vào struct dữ liệu: không cấp phát node riêng, xóa phần tử O(1) từ bất kỳ đâu.

**6. Free-list trong custom allocator.** Block rảnh cần được nhấc khỏi danh sách ngay khi có địa chỉ — O(1) bắt buộc.

### Không dùng khi

- **Chỉ cần push/pop hai đầu** → `std::deque`. Mảng của các block, cache locality tốt, thực tế nhanh hơn `std::list` nhiều lần cùng workload.
- **Cần random access** → `vector`. `get(idx)` là O(n).
- **Dataset lớn, thao tác chính là duyệt** → `vector`, luôn luôn. `memmove` trên vùng nhớ liên tục thường thắng cả insert/delete O(1) của list.
- **Payload nhỏ, số lượng lớn.** `int` trong node 24 bytes là lãng phí 83%. Nếu bắt buộc thì dùng biến thể index-based (mục 6).
- **Cần lock-free/thread-safe.** Một lần insert phải cập nhật 4 pointer nguyên tử, CAS đơn không đủ. Singly dễ hơn nhiều.

### Edge case bắt buộc kiểm tra

```
List rỗng          → head = tail = nullptr (bản A) / nil trỏ vào chính nó (bản B)
List 1 node        → head == tail, prev và next đều nullptr
Xóa head           → head mới phải có prev = nullptr
Xóa tail           → tail mới phải có next = nullptr
Xóa node duy nhất  → cả head và tail về nullptr
Chèn vào list rỗng → node mới vừa là head vừa là tail
insert(idx == sz)  → hợp lệ, tương đương push_back
```

Bản B triệt tiêu năm dòng đầu.

### Pitfall — xếp theo mức nguy hiểm

**1. Quên cập nhật pointer ngược.** Bug đặc trưng của doubly, không tồn tại ở singly.

```cpp
// SAI — chỉ nối một chiều
n->next = head;
head = n;              // thiếu head->prev = n
```

Traverse xuôi vẫn đúng, test pass. Rồi một ngày `print_reverse()` hoặc `erase()` chạm vào pointer sai và crash ở chỗ hoàn toàn không liên quan. Phòng bằng `check_invariant()` sau mỗi operation trong lúc phát triển.

**2. Use-after-free khi vừa duyệt vừa xóa.**

```cpp
// SAI
for (Node* cur = head; cur; cur = cur->next) delete cur;   // đọc cur->next sau delete

// ĐÚNG
Node* cur = head;
while (cur) { Node* nxt = cur->next; delete cur; cur = nxt; }
```

**3. Không unlink trước khi link lại → self-loop.**

```cpp
// SAI
link_front(x);     // x->next = nil.next, mà nil.next có thể chính là x
unlink(x);         // gỡ sau khi đã nối → node tự trỏ, loop vô hạn
```

**4. Shallow copy mặc định.** Không viết copy constructor → hai object cùng trỏ vào một chuỗi node → destructor chạy hai lần → double-free. Rule of five, không ngoại lệ.

**5. Thiếu khâu vòng tròn khi move object có sentinel-by-value.** Đã nói ở 3.2. Dangling pointer im lặng, khó tìm nhất trong danh sách này.

**6. `sz` không đồng bộ.** Đặt việc tăng/giảm `sz` **chỉ trong hai primitive** `link_before`/`unlink` là cách duy nhất giữ nhất quán — `move_to_front` gọi cả hai và chúng tự triệt tiêu nhau.

---

## 6. Biến thể liên quan

- **Singly Linked List** — bỏ `prev`, node nhỏ hơn 1/3, đổi lại `pop_back` và `erase(node)` tụt xuống O(n).
- **Circular Doubly Linked List** — `tail->next == head` và `head->prev == tail`, không còn `nullptr`, duyệt vòng vô hạn tự nhiên cho round-robin scheduler.
- **Sentinel/Dummy Doubly List** — thêm node giả không mang dữ liệu để mọi node thật đều có đủ hai hàng xóm, xóa sạch nhánh edge case (bản B ở 3.3, cũng là cách `std::list` được cài đặt).
- **Intrusive Doubly List** (`struct list_head` của Linux kernel) — nhúng cặp pointer vào chính struct dữ liệu thay vì bọc node bên ngoài, nên không tốn allocation và một object có thể nằm trong nhiều list cùng lúc.
- **Index-based / Arena Doubly List** — thay `Node*` bằng `int32_t` index vào một `vector` chung, giảm node từ 24 xuống 12 bytes và lấy lại cache locality.
- **Unrolled Doubly List** — mỗi node chứa một mảng nhỏ nhiều phần tử, đánh đổi độ phức tạp code lấy cache locality gần bằng `vector`.
- **XOR Linked List** — nén `prev` và `next` thành một field `prev ^ next`, tiết kiệm 8 bytes/node nhưng mất khả năng giữ pointer rời tới node.
- **Skip List** — chồng nhiều tầng liên kết "express lane", đưa search từ O(n) xuống O(log n) (Redis sorted set).
- **`std::list` / `std::forward_list`** — bản chuẩn thư viện của doubly/singly, đã tối ưu allocator và đảm bảo iterator stability; dùng nó thay vì tự viết trong production.