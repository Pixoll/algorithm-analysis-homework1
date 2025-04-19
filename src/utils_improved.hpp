#pragma once

#include "common.hpp"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

inline uint64_t get_pseudo_distance(const Point &p1, const Point &p2) {
    const uint64_t diff1 = (p1.x - p2.x) & COORD_MAX; // equivalent to std::abs (not inlined in -O0)
    const uint64_t diff2 = (p1.y - p2.y) & COORD_MAX; // equivalent to std::abs (not inlined in -O0)
    return diff1 * diff1 + diff2 * diff2;
}
