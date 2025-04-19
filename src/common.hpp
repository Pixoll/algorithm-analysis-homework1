#pragma once
#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

typedef int64_t coord_t;

struct Point {
    coord_t x, y;

    auto operator<=>(const Point &) const noexcept = default;
};

typedef std::vector<Point> points_t;

inline constexpr coord_t COORD_MAX = std::numeric_limits<coord_t>::max();
inline constexpr double DOUBLE_MAX = std::numeric_limits<double>::max();

struct point_hash {
    size_t operator()(const Point &point) const noexcept {
        constexpr std::hash<coord_t> coord_hash;
        return coord_hash(point.x) ^ coord_hash(point.y);
    }
};

inline std::string points_to_string(const points_t &points) {
    std::string string = "[";

    for (size_t i = 0; i < points.size(); i++) {
        const auto &[x, y] = points[i];
        string += '(' + std::to_string(x) + ", " + std::to_string(y) + ')';

        if (i != points.size() - 1) {
            string += ", ";
        }
    }

    return string + ']';
}
