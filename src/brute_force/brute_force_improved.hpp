#pragma once

#include <cmath>
#include <cstdint>

#include "../common.hpp"
#include "../utils_improved.hpp"

inline double brute_force_improved(const points_t &points) {
    const size_t size = points.size();
    uint64_t min_distance = UINT64_MAX;

    for (size_t i = 0; i < size - 1; i++) {
        const auto &p1 = points[i];
        for (size_t j = i + 1; j < size; j++) {
            const uint64_t distance = get_pseudo_distance(p1, points[j]);
            if (distance == 0)
                return 0;

            min_distance = MIN(min_distance, distance);
        }
    }

    return std::sqrt(min_distance);
}

inline uint64_t brute_force_range_improved(const points_t &points, const size_t low, const size_t high) {
    uint64_t min_distance = UINT64_MAX;

    for (size_t i = low; i < high - 1; i++) {
        const auto &p1 = points[i];
        for (size_t j = i + 1; j < high; j++) {
            const uint64_t distance = get_pseudo_distance(p1, points[j]);
            if (distance == 0)
                return 0;

            min_distance = MIN(min_distance, distance);
        }
    }

    return min_distance;
}
