#include "divide_and_conquer_improved.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "../common.hpp"
#include "../utils_improved.hpp"
#include "../brute_force/brute_force_improved.hpp"

inline uint64_t divide_and_conquer_crossing_improved(
    const points_t &points,
    const coord_t mid_x,
    const uint64_t distance_delta
) {
    const double sqrt_distance_delta = std::sqrt(distance_delta);
    const double range_left = mid_x - sqrt_distance_delta;
    const double range_right = mid_x + sqrt_distance_delta;
    const size_t points_size = points.size();

    points_t within_range;
    within_range.reserve(points_size / 2);

    for (int i = 0; i < points_size; i++) {
        const auto &p = points[i];
        if (range_left < p.x && p.x < range_right) {
            within_range.push_back(p);
        }
    }

    const size_t range_size = within_range.size();
    uint64_t min_distance = distance_delta;

    for (size_t i = 0; i < range_size; i++) {
        const auto &p1 = within_range[i];
        for (size_t j = i + 1; j < i + 7 && j < range_size; j++) {
            const uint64_t distance = get_pseudo_distance(p1, within_range[j]);
            min_distance = MIN(min_distance, distance);
        }
    }

    return min_distance;
}

inline uint64_t divide_and_conquer_recursive_improved( // NOLINT(*-no-recursion)
    const points_t &sorted_x,
    const points_t &sorted_y,
    const size_t low,
    const size_t high
) {
    if (high - low <= 3) {
        return brute_force_range_improved(sorted_x, low, high);
    }

    const size_t mid = low + (high - low) / 2;
    const coord_t mid_x = sorted_x[mid].x;
    const size_t sorted_y_size = sorted_y.size();
    const size_t new_sorted_y_size = mid - low + (mid - low) / 2;

    points_t sorted_y_left, sorted_y_right;
    sorted_y_left.reserve(new_sorted_y_size);
    sorted_y_right.reserve(new_sorted_y_size);

    for (int i = 0; i < sorted_y_size; i++) {
        const auto &p = sorted_y[i];
        if (p.x <= mid_x) {
            sorted_y_left.push_back(p);
        } else {
            sorted_y_right.push_back(p);
        }
    }

    const uint64_t min_distance_left = divide_and_conquer_recursive_improved(sorted_x, sorted_y_left, low, mid);
    const uint64_t min_distance_right = divide_and_conquer_recursive_improved(sorted_x, sorted_y_right, mid, high);
    const uint64_t distance_delta = MIN(min_distance_left, min_distance_right);
    const uint64_t min_distance_crossing = divide_and_conquer_crossing_improved(sorted_y, mid_x, distance_delta);

    return MIN(distance_delta, min_distance_crossing);
}

double divide_and_conquer_improved(const points_t &points) {
    points_t sorted_x(points);
    points_t sorted_y(points);

    std::ranges::sort(sorted_x, {}, &Point::x);
    std::ranges::sort(sorted_y, {}, &Point::y);

    const uint64_t min_distance = divide_and_conquer_recursive_improved(sorted_x, sorted_y, 0, points.size());
    return std::sqrt(min_distance);
}
