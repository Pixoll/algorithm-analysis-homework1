#include <filesystem>
#include <functional>
#include <string>

#include "args.hpp"
#include "brute_force/brute_force.hpp"
#include "brute_force/brute_force_improved.hpp"
#include "divide_and_conquer/divide_and_conquer.hpp"
#include "divide_and_conquer/divide_and_conquer_improved.hpp"
#include "uhr/uhr.hpp"

const std::string DATA_DIR = "../data";

inline points_t setup(const uint64_t n, int_generator<coord_t> &generator) {
    points_t points(n);

    for (size_t i = 0; i < n; i++) {
        points[i] = {generator(), generator()};
    }

    return points;
}

void run_uhr(
    const std::string &filePath,
    const std::function<double(const points_t &)> &fn_to_test,
    const ParsedArgs &args
) {
    uhr<coord_t, points_t, double>(
        filePath,
        args.runs,
        args.lower,
        *fn_to_test.target<double(*)(const points_t &)>() == brute_force ? MIN(args.upper, 768) : args.upper,
        args.step,
        args.gen_min,
        args.gen_max,
        setup,
        fn_to_test,
        brute_force_improved
    );
}

int main(const int argc, const char *const *const argv) {
    if (!std::filesystem::exists(DATA_DIR)) {
        std::filesystem::create_directory(DATA_DIR);
    }

    const ParsedArgs &args = parse_args(argc, argv);

    run_uhr(DATA_DIR + "/brute_force.csv", brute_force, args);
    run_uhr(DATA_DIR + "/brute_force_improved.csv", brute_force_improved, args);
    run_uhr(DATA_DIR + "/divide_and_conquer.csv", divide_and_conquer, args);
    run_uhr(DATA_DIR + "/divide_and_conquer_improved.csv", divide_and_conquer_improved, args);

    return 0;
}
