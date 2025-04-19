#include "divide_and_conquer.hpp"

#include <algorithm>
#include <cmath>
#include <ranges>

#include "../common.hpp"
#include "../utils.hpp"
#include "../brute_force/brute_force.hpp"

inline double divide_and_conquer_crossing(
    const points_t &points,
    const size_t low,
    const size_t high,
    const coord_t mid_x,
    const double distance_delta
) {
    const double range_left = mid_x - distance_delta;
    const double range_right = mid_x + distance_delta;

    points_t within_range;
    within_range.reserve((high - low) / 2);

    for (size_t i = low; i < high; i++) {
        const auto &p = points[i];
        if (range_left < p.x && p.x < range_right) {
            within_range.push_back(p);
        }
    }

    std::ranges::sort(within_range, {}, &Point::y);

    double min_distance = distance_delta;

    for (size_t i = 0; i < within_range.size(); i++) {
        const auto &p1 = within_range[i];
        for (size_t j = i + 1; j < i + 7 && j < within_range.size(); j++) {
            min_distance = std::min(min_distance, get_distance(p1, within_range[j]));
        }
    }

    return min_distance;
}

inline double divide_and_conquer_recursive( // NOLINT(*-no-recursion)
    const points_t &points,
    const size_t low,
    const size_t high
) {
    if (high - low <= 3) {
        return brute_force_range(points, low, high);
    }

    const size_t mid = low + (high - low) / 2;

    const double min_distance_left = divide_and_conquer_recursive(points, low, mid);
    const double min_distance_right = divide_and_conquer_recursive(points, mid, high);
    const double distance_delta = std::min(min_distance_left, min_distance_right);
    const double min_distance_crossing = divide_and_conquer_crossing(points, low, high, points[mid].x, distance_delta);

    return std::min(distance_delta, min_distance_crossing);
}

double divide_and_conquer(const points_t &points) {
    points_t sorted_points(points);
    std::ranges::sort(sorted_points, {}, &Point::x);

    return divide_and_conquer_recursive(sorted_points, 0, points.size());
}
