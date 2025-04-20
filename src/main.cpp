#include <filesystem>
#include <functional>
#include <string>
#include <unordered_set>

#include "brute_force/brute_force.hpp"
#include "brute_force/brute_force_improved.hpp"
#include "divide_and_conquer/divide_and_conquer.hpp"
#include "divide_and_conquer/divide_and_conquer_improved.hpp"
#include "uhr/uhr.hpp"

constexpr auto RUNS = 100;
constexpr auto LOWER = 8;
constexpr auto UPPER = 2048;
constexpr auto STEP = 1;
constexpr auto GEN_MIN = 0;
constexpr auto GEN_MAX = 99;
const std::string DATA_DIR = "../data";

points_t setup(const uint64_t n, int_generator<uint64_t> &generator) {
    std::unordered_set<Point, point_hash> points_set(n);

    while (points_set.size() < n) {
        points_set.emplace(generator(), generator());
    }

    return {points_set.begin(), points_set.end()};
}

void run_uhr(const std::string &filePath, const std::function<double(const points_t &)> &fn_to_test) {
    uhr<uint64_t, points_t, double>(
        filePath,
        RUNS,
        LOWER,
        *fn_to_test.target<double(*)(const points_t &)>() == brute_force ? MIN(UPPER, 768) : UPPER,
        STEP,
        GEN_MIN,
        GEN_MAX,
        setup,
        fn_to_test,
        brute_force_improved
    );
}

int main() {
    if (!std::filesystem::exists(DATA_DIR)) {
        std::filesystem::create_directory(DATA_DIR);
    }

    run_uhr(DATA_DIR + "/brute_force.csv", brute_force);
    run_uhr(DATA_DIR + "/brute_force_improved.csv", brute_force_improved);
    run_uhr(DATA_DIR + "/divide_and_conquer.csv", divide_and_conquer);
    run_uhr(DATA_DIR + "/divide_and_conquer_improved.csv", divide_and_conquer_improved);

    return 0;
}
