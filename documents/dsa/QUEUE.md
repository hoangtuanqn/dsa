# `std::queue` — Hướng dẫn từ A–Z

---

## 1. Include & Khai báo

```cpp
#include <queue>
#include <iostream>
#include <string>

// Cú pháp tổng quát:
// std::queue<KiểuDữLiệu> tên;

std::queue<int>         q1;
std::queue<std::string> q2;
std::queue<double>      q3;

// Với struct tự định nghĩa:
struct Task {
    int id;
    std::string name;
};
std::queue<Task> taskQueue;
```

---

## 2. Toàn bộ API — từng hàm, từng chi tiết

### `push(val)` — thêm vào cuối

```cpp
std::queue<int> q;
q.push(10);
q.push(20);
q.push(30);
// queue: [10, 20, 30]  (10 ở front, 30 ở back)
```

Với object lớn, dùng **`emplace`** thay vì `push` để tránh copy:

```cpp
struct Point { int x, y; };
std::queue<Point> pq;

pq.push({1, 2});       // tạo Point tạm, rồi copy vào queue
pq.emplace(1, 2);      // construct trực tiếp trong queue — hiệu quả hơn
```

### `pop()` — xóa phần tử đầu, KHÔNG trả về giá trị

```cpp
q.pop();  // xóa 10
// queue: [20, 30]

// ❌ SAI — pop() trả về void
int x = q.pop();  // compile error

// ✅ ĐÚNG — front() trước, pop() sau
int val = q.front();
q.pop();
```

### `front()` & `back()` — xem không xóa

```cpp
std::queue<int> q;
q.push(10); q.push(20); q.push(30);

q.front();  // 10 — phần tử sẽ ra tiếp theo
q.back();   // 30 — phần tử mới nhất vừa push

// Cả hai trả về reference — có thể modify trực tiếp (ít dùng):
q.front() = 99;
// queue: [99, 20, 30]
```

### `empty()` & `size()`

```cpp
std::queue<int> q;

q.empty();   // true — queue rỗng
q.size();    // 0

q.push(5);
q.empty();   // false
q.size();    // 1
```

**Pattern phổ biến nhất** — luôn check trước khi access:

```cpp
// ❌ Nguy hiểm — undefined behavior nếu queue rỗng
int x = q.front();

// ✅ Đúng
if (!q.empty()) {
    int x = q.front();
    q.pop();
}
```

---

## 3. Không có iterator — đây là điểm khác biệt quan trọng

`std::queue` **cố tình không cho** duyệt phần tử giữa. Nó enforce FIFO nghiêm ngặt.

```cpp
// ❌ Không tồn tại:
for (auto& x : q) { }   // compile error
q[2];                    // compile error
q.begin();               // compile error
```

Nếu cần duyệt, phải drain ra:

```cpp
// Cách 1: drain tạm — destructive
while (!q.empty()) {
    std::cout << q.front() << "\n";
    q.pop();
}

// Cách 2: copy queue rồi drain bản copy — giữ nguyên original
std::queue<int> temp = q;
while (!temp.empty()) {
    std::cout << temp.front() << "\n";
    temp.pop();
}
```

Nếu thường xuyên cần duyệt giữa chừng → sai tool, dùng `std::deque` thay thế.

---

## 4. Swap & So sánh

```cpp
std::queue<int> a, b;
a.push(1); a.push(2);
b.push(9);

a.swap(b);
// a: [9], b: [1, 2]

// So sánh hai queue (lexicographic):
a == b;   // false
a < b;    // true (9 < 1? — so sánh từng phần tử theo thứ tự front→back)
```

---

## 5. Tình huống thực tế

### Tình huống 1: Xử lý request theo thứ tự đến

Web server nhận request, xử lý tuần tự FIFO:

```cpp
#include <queue>
#include <string>
#include <iostream>

struct Request {
    int         id;
    std::string endpoint;
    int         priority;  // ở đây chưa dùng, chỉ để minh họa struct
};

class RequestQueue {
    std::queue<Request> q_;

public:
    void receive(Request req) {
        q_.push(std::move(req));
        std::cout << "[IN]  Request #" << req.id
                  << " → " << req.endpoint << "\n";
    }

    void processAll() {
        while (!q_.empty()) {
            Request req = q_.front();
            q_.pop();
            std::cout << "[OUT] Xử lý #" << req.id
                      << " → " << req.endpoint << "\n";
        }
    }
};

int main() {
    RequestQueue server;
    server.receive({1, "/api/login",   0});
    server.receive({2, "/api/profile", 0});
    server.receive({3, "/api/logout",  0});
    server.processAll();
}
```

Output:
```
[IN]  Request #1 → /api/login
[IN]  Request #2 → /api/profile
[IN]  Request #3 → /api/logout
[OUT] Xử lý #1 → /api/login
[OUT] Xử lý #2 → /api/profile
[OUT] Xử lý #3 → /api/logout
```

---

### Tình huống 2: BFS duyệt cây theo từng tầng

Đây là ứng dụng thuật toán phổ biến nhất của queue:

```cpp
#include <queue>
#include <iostream>
#include <vector>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

//        1
//       / \
//      2   3
//     / \   \
//    4   5   6

void levelOrder(TreeNode* root) {
    if (!root) return;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        // Chụp lại size của tầng hiện tại TRƯỚC khi process
        // vì q.size() sẽ thay đổi khi ta push con vào
        int levelSize = q.size();

        std::cout << "Tầng: ";
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();

            std::cout << node->val << " ";

            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        std::cout << "\n";
    }
}

int main() {
    TreeNode* root  = new TreeNode(1);
    root->left      = new TreeNode(2);
    root->right     = new TreeNode(3);
    root->left->left  = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);

    levelOrder(root);
}
```

Output:
```
Tầng: 1
Tầng: 2 3
Tầng: 4 5 6
```

**Tại sao phải `int levelSize = q.size()` trước?** Vì trong vòng lặp ta vừa pop vừa push con — nếu check `q.size()` trực tiếp trong điều kiện for, số lượng sẽ sai.

---

### Tình huống 3: Sliding Window — tính trung bình cửa sổ k phần tử

```cpp
#include <queue>
#include <vector>
#include <iostream>

// Trung bình động của cửa sổ k phần tử gần nhất
std::vector<double> slidingAverage(const std::vector<int>& data, int k) {
    std::queue<int> window;
    std::vector<double> result;
    long long sum = 0;

    for (int x : data) {
        window.push(x);
        sum += x;

        // Cửa sổ vượt quá k → đẩy phần tử cũ nhất ra
        if ((int)window.size() > k) {
            sum -= window.front();
            window.pop();
        }

        // Chỉ ghi kết quả khi cửa sổ đủ k phần tử
        if ((int)window.size() == k) {
            result.push_back(static_cast<double>(sum) / k);
        }
    }
    return result;
}

int main() {
    std::vector<int> data = {1, 3, 5, 7, 9, 2, 4};
    auto avgs = slidingAverage(data, 3);
    for (double v : avgs) std::cout << v << " ";
    // Output: 3 5 7 6 5
}
```

---

## 6. Bài tập LeetCode ứng dụng

### 🟢 Easy — LC 225: Implement Stack using Queues

**Bài toán:** Implement Stack (LIFO) chỉ dùng Queue (FIFO).

**Ý tưởng:** Dùng 1 queue. Khi `push(x)`, đẩy x vào rồi rotate toàn bộ phần tử trước x ra sau — x sẽ nằm ở `front`.

```cpp
class MyStack {
    std::queue<int> q_;

public:
    void push(int x) {
        q_.push(x);
        // Rotate: đưa tất cả phần tử cũ ra sau x
        int n = q_.size() - 1;
        while (n--) {
            q_.push(q_.front());
            q_.pop();
        }
        // Bây giờ x đang ở front — hành xử như stack top
    }

    int pop() {
        int val = q_.front();
        q_.pop();
        return val;
    }

    int top() {
        return q_.front();
    }

    bool empty() {
        return q_.empty();
    }
};
```

**Trace** `push(1) → push(2) → push(3)`:
```
push(1): q = [1]
push(2): push 2 → [1,2], rotate 1 lần → [2,1]
push(3): push 3 → [2,1,3], rotate 2 lần → [3,2,1]
top() = 3  ✓
pop() = 3, còn [2,1]
top() = 2  ✓
```

---

### 🟡 Medium — LC 102: Binary Tree Level Order Traversal

**Bài toán:** Trả về mảng 2D — mỗi sub-array là một tầng của cây.

```cpp
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (!root) return result;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int sz = q.size();             // số node ở tầng hiện tại
        std::vector<int> level;

        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(std::move(level));
    }
    return result;
}
```

**Độ phức tạp:** O(n) time, O(n) space (queue lưu tối đa 1 tầng).

---

### 🟡 Medium — LC 994: Rotting Oranges

**Bài toán:** Grid chứa 0 (ô trống), 1 (cam tươi), 2 (cam thối). Mỗi phút cam thối lây sang 4 hướng. Hỏi bao nhiêu phút để tất cả cam thối?

**Đây là BFS multi-source** — khởi đầu từ tất cả cam thối cùng lúc:

```cpp
int orangesRotting(std::vector<std::vector<int>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    std::queue<std::pair<int,int>> q;
    int fresh = 0;

    // Bước 1: đẩy tất cả cam thối vào queue ngay từ đầu
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == 2) q.push({r, c});
            if (grid[r][c] == 1) ++fresh;
        }

    if (fresh == 0) return 0;  // không có cam tươi nào

    int minutes = 0;
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    // Bước 2: BFS theo từng "phút" (level)
    while (!q.empty() && fresh > 0) {
        int sz = q.size();
        ++minutes;

        for (int i = 0; i < sz; ++i) {
            auto [r, c] = q.front(); q.pop();

            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (grid[nr][nc] != 1) continue;  // không phải cam tươi

                grid[nr][nc] = 2;   // lây thối
                --fresh;
                q.push({nr, nc});
            }
        }
    }

    return fresh == 0 ? minutes : -1;
}
```

**Tại sao dùng BFS chứ không DFS?** DFS sẽ cho kết quả số phút sai vì không đảm bảo lây lan theo từng "lượt" đồng thời. BFS đảm bảo tất cả cam ở cùng khoảng cách được xử lý trong cùng 1 phút.

---

### 🟡 Medium — LC 239: Sliding Window Maximum

**Bài toán:** Mảng `nums`, cửa sổ kích thước k. Trả về max của từng cửa sổ.

Dùng **Monotonic Deque** (`std::deque`) — đây là bước tiến hóa tự nhiên từ queue:

```cpp
std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
    std::deque<int> dq;   // lưu INDEX, không lưu giá trị
    std::vector<int> result;

    for (int i = 0; i < (int)nums.size(); ++i) {
        // Loại index đã ra khỏi cửa sổ
        if (!dq.empty() && dq.front() < i - k + 1)
            dq.pop_front();

        // Loại index có giá trị nhỏ hơn phần tử hiện tại từ phía sau
        // — chúng sẽ không bao giờ là max khi nums[i] còn trong window
        while (!dq.empty() && nums[dq.back()] < nums[i])
            dq.pop_back();

        dq.push_back(i);

        // Bắt đầu ghi kết quả sau khi cửa sổ đủ k phần tử
        if (i >= k - 1)
            result.push_back(nums[dq.front()]);
    }
    return result;
}
```

---

## Tóm tắt chọn container

```
Cần FIFO đơn giản?              → std::queue
Cần FIFO + thêm/xóa cả 2 đầu?  → std::deque
Cần FIFO + ưu tiên?             → std::priority_queue
Cần FIFO + duyệt ngẫu nhiên?   → std::deque (không dùng queue)
BFS trên graph/tree?            → std::queue — lựa chọn mặc định
Sliding window min/max?         → std::deque (monotonic deque)
```