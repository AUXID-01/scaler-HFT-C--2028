// spsc_queue.hpp — lock-free SPSC ring buffer, NAIVE version.
//
// THE PLANTED BUG:
//   tail_ and head_ are declared back-to-back → 8 bytes apart →
//   SAME 64-byte cache line. Producer owns tail_, consumer owns head_,
//   so on EVERY operation both cores steal that one line from each other
//   (MESI ping-pong). perf c2c will show it as HITM traffic.
#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

template <typename T, std::size_t Capacity>
class SPSCQueue {
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be a power of 2");
    static constexpr std::size_t kMask = Capacity - 1;

    alignas(64) std::atomic<std::size_t> tail_{0};   // written ONLY by producer
    alignas(64) std::atomic<std::size_t> head_{0};   // written ONLY by consumer — 8 bytes from tail_!
    alignas(64) std::array<T, Capacity> slots_;  // buffer aligned, so ONLY the indices share a line

public:
    // producer thread only
    bool push(const T& v) {
        const std::size_t t = tail_.load(std::memory_order_relaxed);       // my own index: cheap
        if (t - head_.load(std::memory_order_acquire) == Capacity)         // full? (reads THEIR line)
            return false;
        slots_[t & kMask] = v;                         // plain write: I own this slot this generation
        tail_.store(t + 1, std::memory_order_release); // publish (pairs with acquire in pop)
        return true;
    }

    // consumer thread only
    bool pop(T& out) {
        const std::size_t h = head_.load(std::memory_order_relaxed);       // my own index: cheap
        if (h == tail_.load(std::memory_order_acquire))                    // empty? (reads THEIR line)
            return false;
        out = slots_[h & kMask];
        head_.store(h + 1, std::memory_order_release); // slot free for producer again
        return true;
    }

    // lab helper: byte distance between the two indices
    std::ptrdiff_t index_gap_bytes() const {
        return static_cast<std::ptrdiff_t>(
            reinterpret_cast<std::uintptr_t>(&head_) -
            reinterpret_cast<std::uintptr_t>(&tail_));
    }
};
