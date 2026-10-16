# Low-Latency Order Book (Capstone Project)

**Team Members:** Ayush Kumar and Nandani Kumari  
**Email:** ayush.34bcs10474@sst.scaler.com and nandani.24bcs10317@sst.scaler.com  

---

## 1. Project Overview
This project implements a single-instrument resting order book supporting bids and asks with tick sizes of `0.01` (represented internally as integers to avoid floating-point overhead). The goal is to design, implement, benchmark, and analyze three increasingly optimized data-structure architectures to understand low-latency C++ systems programming, cache locality, and hardware-level performance.

---

## 2. Project Structure
```text
low_latency_order_book/
├── include/
│   ├── order_book.hpp        # Common data structures (Side, Level, BBO)
│   ├── order_book_a.hpp      # Implementation A (Sorted Map & Hash Map)
│   ├── order_book_b.hpp      # Implementation B (Priority Queue + Hash Map)
│   └── order_book_c.hpp      # Implementation C (Fixed-Size Array + Branchless Binary Search) [Pending]
├── src/
│   ├── main.cpp              # Unified benchmark & verification harness
│   ├── order_book_a.cpp      # Implementation A logic
│   └── order_book_b.cpp      # Implementation B logic
├── docs/
│   ├── implementation_a_notes.md  # Detailed design notes for Approach A
│   └── implementation_b_notes.md  # Detailed design notes for Approach B
├── CMakeLists.txt             # Build configuration
└── README.md                  # Project documentation
```

---

## 3. How to Build the Project
Compiled with `-O2` optimization as required by the assignment rules:

```bash
# Navigate to the source directory
cd low_latency_order_book

# Compile using g++ directly
g++ -O2 -std=c++17 src/main.cpp src/order_book_a.cpp src/order_book_b.cpp -Iinclude -o bench
```

---

## 4. How to Run Correctness Tests
To verify that all implementations pass and maintain exact state parity:

```bash
./bench --verify
```

---

## 5. How to Run Benchmarks
The benchmark runs 1,000,000 mixed operations (70% adds, 20% cancels, 10% BBO queries, with depth queries every 100 ops) across 5 runs and reports the median:

```bash
# Benchmark Implementation A
./bench --impl A

# Benchmark Implementation B
./bench --impl B

# Capture hardware & OS performance counters via Linux perf
perf stat -r 5 \
-e task-clock,cycles,instructions,branches,branch-misses,\
cache-misses,L1-dcache-load-misses,dTLB-load-misses,page-faults \
./bench --impl B
```

---

## 6. Empirical Benchmark Results (Median of 5 Runs - Linux/WSL)

| Metric | Implementation A (Map + Hash) | Implementation B (Heap + Hash) | Implementation C (Array + Branchless) |
| :--- | :--- | :--- | :--- |
| **Total time (ms)** | **165.40 ms** | 6,115.08 ms *(~37x slower)* | Pending |
| **ns / add** | 138.96 ns | **131.48 ns** *(~5% faster)* | Pending |
| **ns / cancel** | **234.32 ns** | 308.03 ns | Pending |
| **ns / BBO** | **20.35 ns** | 48.67 ns | Pending |
| **Task Clock (5 runs)** | **943.79 ms** | 30,807.85 ms | Pending |
| **Page faults** | **22,774** | 33,700 *(+48%)* | Pending |
| **IPC** | N/A (WSL vPMU off) | N/A (WSL vPMU off) | Pending |
| **Branch misses (%)** | N/A (WSL vPMU off) | N/A (WSL vPMU off) | Pending |
| **L1-dcache-load-misses** | N/A (WSL vPMU off) | N/A (WSL vPMU off) | Pending |
| **Cache misses** | N/A (WSL vPMU off) | N/A (WSL vPMU off) | Pending |

> *Note: Hardware PMU counters (cycles, instructions, cache/branch misses) require hypervisor vPMU support in WSL2; software metrics like task-clock and page-faults were captured via `perf stat`.*

---

## 7. Implementation A: Sorted Map & Hash Map (Baseline)

### Design Summary:
- **Per-Order Storage:** `std::unordered_map<uint64_t, OrderDetails>` for $O(1)$ average-time cancellation lookups.
- **Per-Price Storage:** Two `std::map` containers (bids sorted descending, asks sorted ascending) backed by Red-Black Trees.

### Hardware & Architectural Analysis:
- **`get_bbo` Performance (~20.35 ns):** Extremely fast because Red-Black trees maintain sorted keys, allowing `.begin()` to instantly reference the best bid/ask.
- **`cancel_order` Latency (~234.32 ns):** Requires a hash lookup, a tree traversal, quantity subtraction, and conditional tree node deletion/rebalancing.
- **Cache Inefficiency & Allocations:** Tree nodes and unordered map nodes are allocated dynamically on the heap (`new`/`malloc`), producing 22,774 page faults during operation runs and frequent cache misses due to pointer chasing.

---

## 8. Implementation B: Priority Queue & Hash Map (Heap with Lazy Deletion)

### Design Summary:
- **Per-Order Storage:** `std::unordered_map<uint64_t, OrderInfo>` storing order metadata and `bool active` for $O(1)$ lazy cancellation.
- **Price Heaps:** `std::priority_queue` max-heap for bids and min-heap for asks.
- **Volume Tracking:** Two `std::unordered_map<int, long>` tracking aggregate volume per price level.
- **Lazy Top Cleaning:** Stale, cancelled orders at the top of the heaps are purged on-demand during `get_bbo()` and depth queries.

### Hardware & Architectural Analysis:
- **`add_order` Latency (~131.48 ns):** Pushing to a `std::priority_queue` (backed by a contiguous vector) provides better spatial cache locality than inserting a newly allocated node into a Red-Black tree.
- **Lazy Deletion Heap Bloat:** Cancelled orders remain in the heap until they naturally surface at the top. This causes heap size to grow unbounded over time, degrading cache locality and increasing page faults from 22,774 to 33,700 (+48%).
- **The Depth Query Bottleneck:** Every 100 ops, `get_bids(10)` and `get_asks(10)` are called. Because price quantities reside in unsorted hash maps, extracting the top 10 requires copying all active price levels into a vector and sorting them ($O(P \log P)$). This single bottleneck causes total runtime to balloon to **6,115.08 ms** (~37x slower than Implementation A).

### Pros & Cons:
- **Pros:** Fast $O(\log N)$ heap push with array cache locality; non-blocking $O(1)$ cancellation marking.
- **Cons:** Extremely slow market depth queries ($O(P \log P)$ sort); heap memory bloat from unpurged canceled orders; non-deterministic latency jitter in `get_bbo()`.

### Verdict: Is Implementation B Better or Worse?
**Worse overall for real-world trading.**  
While binary heaps offer a modest ~5% speedup on order entry (`add_order`), they severely compromise market depth retrieval, increase memory churn (+48% page faults), and make overall system throughput 37x slower. In electronic trading, disseminating L2/L3 market depth and maintaining deterministic latency are non-negotiable, making Implementation A superior as a complete order book.
