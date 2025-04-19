#pragma once

#include <cmath>

#include "../common.hpp"
#include "../utils.hpp"

inline double brute_force(const points_t &points) {
    double min_distance = DOUBLE_MAX;

    for (size_t i = 0; i < points.size(); i++) {
        const auto &p1 = points[i];
        for (size_t j = 0; j < points.size(); j++) {
            if (i != j) {
                min_distance = std::min(min_distance, get_distance(p1, points[j]));
            }
        }
    }

    return min_distance;
}

inline double brute_force_range(const points_t &points, const size_t low, const size_t high) {
    double min_distance = DOUBLE_MAX;

    for (size_t i = low; i < high; i++) {
        const auto &p1 = points[i];
        for (size_t j = low; j < high; j++) {
            if (i != j) {
                min_distance = std::min(min_distance, get_distance(p1, points[j]));
            }
        }
    }

    return min_distance;
}
