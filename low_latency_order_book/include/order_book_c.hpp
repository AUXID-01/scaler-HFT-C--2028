#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>

#include "order_book.hpp"

class OrderBookC {
private:
    struct OrderInfo {
        Side side;
        int price_tick;
        long qty;
    };

    // Fast order lookup for cancellations: Order ID -> OrderInfo
    std::unordered_map<uint64_t, OrderInfo> orders_;

    // Contiguous array/vector for price levels (indices 5000 to 15000)
    std::vector<long> price_quantities_;

    // Track Best Bid and Best Ask ticks for O(1) BBO retrieval
    int best_bid_tick_;
    int best_ask_tick_;

    static double tick_to_price(int tick) {
        return static_cast<double>(tick) / 100.0;
    }

public:
    OrderBookC();

    void add_order(uint64_t id, Side side, double price, long qty);
    bool cancel_order(uint64_t id);
    BBO get_bbo() const;
    std::vector<Level> get_bids(int depth = 10) const;
    std::vector<Level> get_asks(int depth = 10) const;
};