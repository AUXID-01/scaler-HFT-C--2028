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
│   ├── order_book_a.hpp      # Implementation A (Sorted Map & Hash Map)
│   ├── order_book_b.hpp      # Implementation B (Priority Queue + Hash Map) [Pending]
│   └── order_book_c.hpp      # Implementation C (Fixed-Size Array + Branchless Binary Search) [Pending]
├── src/
│   ├── main.cpp              # Unified benchmark & verification harness
│   └── order_book_a.cpp      # Implementation A logic
├── docs/
│   └── implementation_a_notes.md  # Detailed design notes for Approach A
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
g++ -O2 -std=c++17 src/main.cpp src/order_book_a.cpp -Iinclude -o bench.exe
```

---

## 4. How to Run Correctness Tests
To verify that the implementation correctly handles orders, cancellations, and BBO queries:

```bash
./bench.exe --verify
```

---

## 5. How to Run Benchmarks
The benchmark runs 1,000,000 mixed operations (70% adds, 20% cancels, 10% BBO queries, with depth queries every 100 ops) across 5 runs and reports the median:

```bash
./bench.exe --bench
```

---

## 6. Implementation A: Sorted Map & Hash Map (Baseline)

### Design Summary:
- **Per-Order Storage:** `std::unordered_map<uint64_t, OrderDetails>` for $O(1)$ average-time cancellation lookups.
- **Per-Price Storage:** Two `std::map` containers (bids sorted descending, asks sorted ascending) backed by Red-Black Trees.

### Empirical Benchmark Results (Median of 5 Runs - Linux/WSL):

| Metric | Implementation A | Implementation B | Implementation C |
| :--- | :--- | :--- | :--- |
| **Total time (ms)** | 165.40 ms | Pending | Pending |
| **ns / add** | 138.96 ns | Pending | Pending |
| **ns / cancel** | 234.32 ns | Pending | Pending |
| **ns / BBO** | 20.35 ns | Pending | Pending |
| **Task Clock** | 943.79 ms | Pending | Pending |
| **Page faults** | 22,774 | Pending | Pending |
| **IPC** | N/A (WSL vPMU off) | Pending | Pending |
| **Branch misses (%)** | N/A (WSL vPMU off) | Pending | Pending |
| **L1-dcache-load-misses** | N/A (WSL vPMU off) | Pending | Pending |
| **Cache misses** | N/A (WSL vPMU off) | Pending | Pending |

> *Note: Hardware PMU counters (cycles, instructions, cache/branch misses) require virtualized PMU support in hypervisor/WSL2; software metrics like task-clock and page-faults were captured via `perf stat`.*

### Hardware & Architectural Analysis:
- **`get_bbo` Performance (~20.35 ns):** Extremely fast because Red-Black trees maintain sorted keys, allowing `.begin()` to instantly reference the best bid/ask.
- **`cancel_order` Latency (~234.32 ns):** The most expensive operation because it requires a hash lookup, a tree traversal, quantity subtraction, and conditional tree node deletion/rebalancing.
- **Cache Inefficiency & Allocations:** Tree nodes and unordered map nodes are allocated dynamically on the heap (`new`/`malloc`), producing 22,774 page faults during operation runs and frequent cache misses due to pointer chasing.
