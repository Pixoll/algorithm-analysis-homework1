#pragma once

#include <cmath>

#include "common.hpp"

inline double get_distance(const Point &p1, const Point &p2) {
    const coord_t diff1 = p1.x - p2.x;
    const coord_t diff2 = p1.y - p2.y;
    return std::sqrt(diff1 * diff1 + diff2 * diff2);
}
