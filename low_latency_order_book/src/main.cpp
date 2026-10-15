#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include "order_book_a.hpp"

// Workload generator helper
struct Operation {
    int type; // 0: add, 1: cancel, 2: bbo
    uint64_t id;
    Side side;
    double price;
    long qty;
};

void run_benchmark() {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> op_dist(0, 99);
    std::uniform_int_distribution<int> side_dist(0, 1);
    std::uniform_int_distribution<long> qty_dist(1, 100);
    // Bids: 50.00 to 99.99 -> ticks 5000 to 9999
    // Asks: 100.01 to 150.00 -> ticks 10001 to 15000
    std::uniform_int_distribution<int> bid_price_dist(5000, 9999);
    std::uniform_int_distribution<int> ask_price_dist(10001, 15000);

    const int TOTAL_OPS = 1'000'000;
    
    // Pre-generate operations to ensure exact same workload
    std::vector<Operation> ops;
    ops.reserve(TOTAL_OPS);
    
    std::vector<uint64_t> active_ids;
    uint64_t next_id = 1;

    for (int i = 0; i < TOTAL_OPS; ++i) {
        int op_type = op_dist(rng); // 0-69: Add (70%), 70-89: Cancel (20%), 90-99: BBO (10%)
        if (op_type < 70 || active_ids.empty()) {
            // Add order
            Side side = (side_dist(rng) == 0) ? Side::Buy : Side::Sell;
            int tick = (side == Side::Buy) ? bid_price_dist(rng) : ask_price_dist(rng);
            double price = static_cast<double>(tick) / 100.0;
            long qty = qty_dist(rng);
            
            ops.push_back({0, next_id, side, price, qty});
            active_ids.push_back(next_id);
            next_id++;
        } else if (op_type < 90 && !active_ids.empty()) {
            // Cancel order
            std::uniform_int_distribution<size_t> idx_dist(0, active_ids.size() - 1);
            size_t idx = idx_dist(rng);
            uint64_t cancel_id = active_ids[idx];
            // Swap with back and pop for O(1) removal from tracking list
            active_ids[idx] = active_ids.back();
            active_ids.pop_back();

            ops.push_back({1, cancel_id, Side::Buy, 0.0, 0});
        } else {
            // BBO query
            ops.push_back({2, 0, Side::Buy, 0.0, 0});
        }
    }

    // Run benchmark 5 times and collect total times
    std::vector<double> run_times_ms;
    long long total_adds = 0, total_cancels = 0, total_bbos = 0;
    long long add_ns = 0, cancel_ns = 0, bbo_ns = 0;
    BBO final_bbo{false, 0, 0, false, 0, 0};

    std::cout << "Running Implementation A Benchmark (5 runs)..." << std::endl;

    for (int run = 0; run < 5; ++run) {
        OrderBookA book;
        long long add_count = 0, cancel_count = 0, bbo_count = 0;
        long long add_duration = 0, cancel_duration = 0, bbo_duration = 0;

        auto start_total = std::chrono::steady_clock::now();

        for (int i = 0; i < TOTAL_OPS; ++i) {
            const auto& op = ops[i];

            if (op.type == 0) {
                auto start = std::chrono::steady_clock::now();
                book.add_order(op.id, op.side, op.price, op.qty);
                auto end = std::chrono::steady_clock::now();
                add_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                add_count++;
            } else if (op.type == 1) {
                auto start = std::chrono::steady_clock::now();
                book.cancel_order(op.id);
                auto end = std::chrono::steady_clock::now();
                cancel_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                cancel_count++;
            } else {
                auto start = std::chrono::steady_clock::now();
                volatile auto bbo = book.get_bbo(); // volatile prevents optimization
                auto end = std::chrono::steady_clock::now();
                bbo_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                bbo_count++;
            }

            // Every 100 operations, call depth queries as requested
            if (i % 100 == 0) {
                volatile auto bids = book.get_bids(10);
                volatile auto asks = book.get_asks(10);
            }
        }

        auto end_total = std::chrono::steady_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(end_total - start_total).count();
        run_times_ms.push_back(elapsed_ms);

        if (run == 4) { // Save metrics from final run
            total_adds = add_count;
            total_cancels = cancel_count;
            total_bbos = bbo_count;
            add_ns = add_duration;
            cancel_ns = cancel_duration;
            bbo_ns = bbo_duration;
            final_bbo = book.get_bbo();
        }
    }

    // Median result reporting
    std::sort(run_times_ms.begin(), run_times_ms.end());
    double median_time_ms = run_times_ms[2];

    std::cout << "\n=== Implementation A Benchmark Results (Median of 5 Runs) ===" << std::endl;
    std::cout << "Total Execution Time: " << median_time_ms << " ms" << std::endl;
    std::cout << "Avg ns / add_order:   " << static_cast<double>(add_ns) / total_adds << " ns" << std::endl;
    std::cout << "Avg ns / cancel_order:" << static_cast<double>(cancel_ns) / total_cancels << " ns" << std::endl;
    std::cout << "Avg ns / get_bbo:     " << static_cast<double>(bbo_ns) / total_bbos << " ns" << std::endl;
    
    // Checksum to prevent compiler optimization
    long checksum = static_cast<long>(final_bbo.bid_price * 100) + static_cast<long>(final_bbo.ask_price * 100) 
                  + final_bbo.bid_qty + final_bbo.ask_qty;
    std::cout << "\nFinal BBO:" << std::endl;
    std::cout << "Bid: " << final_bbo.bid_price << " x " << final_bbo.bid_qty << std::endl;
    std::cout << "Ask: " << final_bbo.ask_price << " x " << final_bbo.ask_qty << std::endl;
    std::cout << "Checksum: " << checksum << std::endl;
}

void run_verify() {
    std::cout << "Running correctness test for Implementation A..." << std::endl;
    OrderBookA book;
    book.add_order(1, Side::Buy, 98.50, 10);
    book.add_order(2, Side::Sell, 101.50, 20);
    
    auto bbo = book.get_bbo();
    if (bbo.has_bid && bbo.bid_price == 98.50 && bbo.has_ask && bbo.ask_price == 101.50) {
        std::cout << "Implementation A: PASS" << std::endl;
    } else {
        std::cout << "Implementation A: FAIL" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--verify") {
        run_verify();
    } else if (argc > 1 && std::string(argv[1]) == "--bench") {
        run_benchmark();
    } else {
        std::cout << "Usage: ./bench [--verify | --bench]" << std::endl;
    }
    return 0;
}