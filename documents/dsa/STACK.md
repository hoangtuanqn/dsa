# Queue — Hàng đợi

---

## 1. Định nghĩa & Cấu trúc bộ nhớ

**Queue** là cấu trúc dữ liệu tuyến tính hoạt động theo nguyên tắc **FIFO** (First In, First Out) — phần tử nào vào trước thì ra trước. Nghĩ đến hàng người xếp chờ mua vé: người đứng đầu hàng được phục vụ trước, người mới đến xếp vào cuối.

Queue có **hai đầu cố định về vai trò**:
- **Front (head)** — nơi lấy phần tử ra
- **Rear (tail)** — nơi thêm phần tử vào

### Có hai cách implement phổ biến:

**Cách 1: Linked List–based Queue**

Mỗi node là một struct nằm ở địa chỉ bộ nhớ riêng biệt (heap), liên kết với nhau qua pointer:

```
Heap memory (địa chỉ ngẫu nhiên):

[0x1A00]        [0x3F20]        [0x2B44]
┌──────┬──────┐ ┌──────┬──────┐ ┌──────┬──────┐
│  10  │0x3F20│→│  20  │0x2B44│→│  30  │nullptr│
└──────┴──────┘ └──────┴──────┘ └──────┴──────┘
    ↑                                  ↑
  front                              rear
```

Queue object (thường nằm trên stack) chỉ lưu 2 pointer + size:

```
Stack:
┌─────────┬─────────┬───────┐
│ *front  │ *rear   │ size  │
│ 0x1A00  │ 0x2B44  │   3   │
└─────────┴─────────┴───────┘
```

**Cách 2: Circular Array–based Queue**

Dùng một mảng có kích thước cố định, dùng hai index `head` và `tail` dịch chuyển theo vòng tròn. Tránh được việc copy/shift mảng khi dequeue.

```
capacity = 6
arr: [ _ | 10 | 20 | 30 | _ | _ ]
           ↑              ↑
         head=1         tail=4
```

Khi `tail` chạy đến cuối mảng và `head` đã nhường chỗ phía đầu, `tail` wrap around về index 0 — đó là "circular".

---

## 2. Các Operation Cơ Bản

### Enqueue (thêm vào cuối)

Logic: tạo node mới, nối vào sau `rear`, cập nhật `rear`.

```
Trước: front→[10]→[20]→nullptr   rear→[20]
Enqueue(30):
  1. Tạo node mới chứa 30
  2. rear->next = newNode
  3. rear = newNode
Sau:  front→[10]→[20]→[30]→nullptr   rear→[30]
```

### Dequeue (lấy ra từ đầu)

Logic: lưu giá trị `front`, dịch `front` sang `front->next`, giải phóng node cũ.

```
Trước: front→[10]→[20]→[30]→nullptr   rear→[30]
Dequeue():
  1. value = front->data  (= 10)
  2. temp = front
  3. front = front->next  (→ [20])
  4. delete temp
  Trả về: 10
Sau:  front→[20]→[30]→nullptr   rear→[30]
```

**Edge case quan trọng**: nếu sau khi dequeue mà `front == nullptr` thì `rear` cũng phải được set về `nullptr`. Nếu không, `rear` sẽ trỏ vào vùng nhớ đã bị `delete` — undefined behavior.

### Peek / Front (xem đầu hàng, không xóa)

```
return front->data;  // O(1), không thay đổi gì
```

### IsEmpty

```
return front == nullptr;
// Hoặc: return size == 0;
```

### Traverse (duyệt toàn bộ)

Không remove, chỉ đọc từ front đến rear:

```
Trước: front→[10]→[20]→[30]→nullptr
  curr = front
  in 10 → curr = curr->next
  in 20 → curr = curr->next
  in 30 → curr = curr->next (nullptr → dừng)
```

---

## 3. Cài đặt C++20

```cpp
#include <iostream>
#include <stdexcept>
#include <concepts>
#include <format>

// Concept: chỉ chấp nhận type có thể in ra ostream
template<typename T>
concept Printable = requires(std::ostream& os, T val) {
    { os << val } -> std::same_as<std::ostream&>;
};

template<Printable T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;

        // Constructor — tận dụng perfect forwarding
        explicit Node(T&& val) : data(std::move(val)), next(nullptr) {}
        explicit Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head_;   // trỏ vào phần tử sẽ bị dequeue tiếp theo
    Node* tail_;   // trỏ vào phần tử mới nhất được enqueue
    std::size_t size_;

public:
    // ===== Constructor / Destructor =====

    Queue() : head_(nullptr), tail_(nullptr), size_(0) {}

    // Destructor — phải dọn sạch heap, không thì memory leak
    ~Queue() {
        while (head_ != nullptr) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    // Copy constructor — deep copy từng node
    Queue(const Queue& other) : head_(nullptr), tail_(nullptr), size_(0) {
        Node* curr = other.head_;
        while (curr != nullptr) {
            enqueue(curr->data);
            curr = curr->next;
        }
    }

    // Move constructor — chuyển ownership, không copy dữ liệu
    Queue(Queue&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
    }

    // Copy/Move assignment — dùng copy-and-swap idiom
    Queue& operator=(Queue other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
        return *this;
    }

    // ===== Core Operations =====

    // Enqueue: thêm vào cuối — O(1)
    void enqueue(const T& val) {
        Node* newNode = new Node(val);
        if (tail_ == nullptr) {
            // Queue đang rỗng: head và tail cùng trỏ vào node mới
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_;
    }

    // Enqueue với move semantics — tránh copy không cần thiết
    void enqueue(T&& val) {
        Node* newNode = new Node(std::move(val));
        if (tail_ == nullptr) {
            head_ = tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }
        ++size_;
    }

    // Dequeue: lấy và xóa phần tử đầu — O(1)
    T dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Dequeue từ queue rỗng");
        }
        Node* temp = head_;
        T value = std::move(temp->data);
        head_ = head_->next;

        // Quan trọng: nếu queue về rỗng, tail cũng phải reset
        if (head_ == nullptr) {
            tail_ = nullptr;
        }

        delete temp;
        --size_;
        return value;
    }

    // Peek: xem phần tử đầu mà không xóa — O(1)
    [[nodiscard]] const T& front() const {
        if (isEmpty()) {
            throw std::underflow_error("Front của queue rỗng");
        }
        return head_->data;
    }

    [[nodiscard]] const T& back() const {
        if (isEmpty()) {
            throw std::underflow_error("Back của queue rỗng");
        }
        return tail_->data;
    }

    // ===== Utility =====

    [[nodiscard]] bool isEmpty() const noexcept { return head_ == nullptr; }
    [[nodiscard]] std::size_t size() const noexcept { return size_; }

    // Traverse: in toàn bộ queue từ front → rear — O(n)
    void print() const {
        if (isEmpty()) {
            std::cout << "[Queue rỗng]\n";
            return;
        }
        std::cout << "front → ";
        Node* curr = head_;
        while (curr != nullptr) {
            std::cout << curr->data;
            if (curr->next != nullptr) std::cout << " → ";
            curr = curr->next;
        }
        std::cout << " ← rear\n";
    }
};

// ===== Demo =====
int main() {
    Queue<int> q;

    std::cout << "=== Enqueue 10, 20, 30 ===\n";
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.print();

    std::cout << std::format("\nFront: {}, Back: {}, Size: {}\n",
                              q.front(), q.back(), q.size());

    std::cout << "\n=== Dequeue ===\n";
    std::cout << std::format("Lấy ra: {}\n", q.dequeue());
    q.print();

    std::cout << "\n=== Enqueue 40 ===\n";
    q.enqueue(40);
    q.print();

    std::cout << "\n=== Drain toàn bộ ===\n";
    while (!q.isEmpty()) {
        std::cout << std::format("Dequeue: {}\n", q.dequeue());
    }
    q.print();

    return 0;
}
```

Compile:
```bash
g++ -std=c++20 -O2 -Wall -Wextra -o queue main.cpp && ./queue
```

---

## 4. Độ Phức Tạp

| Operation | Time | Space | Lý do |
|-----------|------|-------|-------|
| `enqueue` | O(1) | O(1) | Luôn nối vào `tail_`, có pointer sẵn, không cần duyệt |
| `dequeue` | O(1) | O(1) | Luôn lấy từ `head_`, không cần dịch phần tử nào |
| `front` / `back` | O(1) | O(1) | Dereference trực tiếp pointer |
| `isEmpty` / `size` | O(1) | O(1) | Biến được maintain liên tục |
| `traverse` | O(n) | O(1) | Phải đi qua tất cả n node, không cần bộ nhớ thêm |
| Khởi tạo (empty) | O(1) | O(1) | Chỉ khởi tạo 2 pointer |
| Destructor | O(n) | O(1) | Phải delete từng node một |
| **Tổng bộ nhớ** | — | O(n) | n node trên heap, mỗi node = data + 1 pointer |

**Tại sao enqueue/dequeue là O(1)?** Đây là điểm mấu chốt của thiết kế Queue. Khác với Array (shift O(n) khi xóa đầu), Queue với linked list duy trì **hai pointer cố định** — không bao giờ cần duyệt để tìm vị trí thêm/xóa.

---

## 5. Khi Nào Dùng / Không Dùng

### ✅ Dùng Queue khi:

- **BFS (Breadth-First Search)** — đây là use case kinh điển nhất. Level-order traversal cây, tìm đường ngắn nhất trong graph không có trọng số
- **Task scheduling** — OS process scheduler (round-robin), job queue trong background worker
- **Buffer** — network packet buffer, print spooler, I/O buffer
- **Rate limiting** — sliding window với queue lưu timestamp request
- **Producer–Consumer** — thread A đẩy job vào queue, thread B lấy ra xử lý (cần thread-safe queue)

### ❌ Không dùng Queue khi:

- **Cần truy cập random** — không có index, muốn lấy phần tử thứ k phải duyệt O(k)
- **Cần xử lý ưu tiên** — Queue thuần túy không biết "quan trọng hơn", cần Priority Queue
- **LIFO** — nếu logic cần "vào sau ra trước" thì dùng Stack
- **Dataset nhỏ, đơn giản** — overhead của pointer/heap allocation đôi khi thua `std::deque` hoặc circular buffer

### ⚠️ Pitfall & Edge Case:

**1. Dangling tail sau dequeue cuối:**
Nếu quên `tail_ = nullptr` khi `head_` thành `nullptr` sau dequeue, rồi enqueue tiếp → `tail_->next = newNode` sẽ write vào vùng nhớ đã freed. Bug âm thầm, crash khó debug.

**2. Enqueue vào queue đã move:**
Sau `Queue b = std::move(a)`, nếu tiếp tục gọi `a.enqueue(...)`, `a` đang ở trạng thái valid-but-empty, không crash nhưng cần chắc chắn move constructor set pointer về `nullptr`.

**3. Thread safety:**
Linked-list Queue này **không thread-safe**. Hai thread đồng thời enqueue có thể cùng đọc `tail_->next == nullptr` rồi cùng ghi — race condition. Cần mutex hoặc dùng lock-free queue (atomic pointer).

**4. Circular array queue — full vs empty:**
Cả hai trạng thái đều có thể cho `head == tail`. Cần thêm `size` counter hoặc để một slot trống làm sentinel để phân biệt.

---

## 6. Biến Thể Liên Quan

| Tên | Khác biệt cốt lõi |
|-----|-------------------|
| **Circular Queue** | Dùng mảng tĩnh với index wrap-around, tránh shift nhưng capacity cố định |
| **Deque (Double-Ended Queue)** | Cho phép enqueue/dequeue ở **cả hai đầu**, không phải chỉ một chiều |
| **Priority Queue** | Phần tử ra theo **priority**, không theo thứ tự vào; thường implement bằng heap |
| **Monotonic Queue** | Duy trì invariant tăng hoặc giảm, dùng để giải sliding window min/max trong O(1) amortized |
| **Blocking Queue** | Thread-safe, consumer block lại khi queue rỗng thay vì throw exception |
| **Lock-Free Queue** | Thread-safe không dùng mutex, dựa vào atomic CAS — throughput cao hơn nhưng cực kỳ phức tạp |