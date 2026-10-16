// main.cpp
// build:  g++ -O3 -march=native -std=c++17 -pthread main.cpp -o bench_naive
#include <cstdint>
#include <thread>
#include "spsc_queue.hpp"

std::uint64_t g_sink;   // global => compiler is NOT allowed to delete the pops

int main() {
    constexpr std::uint64_t N = 10'000'000;
    SPSCQueue<std::uint64_t, 4096> q;

    std::thread producer([&] {
        for (std::uint64_t i = 0; i < N; ++i)
            while (!q.push(i)) {}
    });

    std::thread consumer([&] {
        std::uint64_t v;
        std::uint64_t n = 0;
        while (n < N)
            if (q.pop(v)) { g_sink += v; ++n; }
    });

    producer.join();
    consumer.join();
}
