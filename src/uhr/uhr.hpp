/**
 * Copyright @leonardlover on GitHub
 * https://github.com/leonardlover/uhr
 * Modified for this project
 */

#pragma once

#include <chrono>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "uhr_utils.hpp"

/**
 * Run performance measurements.
 *
 * @tparam IntType Type of int to generate from rng.
 * @tparam Arg Type of argument the function to test accepts.
 * @tparam R Return type of the tested function.
 * @param filePath Path of the file where performance data will be written.
 * @param runs Number of runs per test case. Should be >= 32.
 * @param lower Range of test cases. Should be > 0.
 * @param upper Range of test cases. Should be > 0.
 * @param step Range of test cases. Should be > 0.
 * @param generator_min Min value the int_generator passed to setup_fn should generate.
 * @param generator_max Max value the int_generator passed to setup_fn should generate.
 * @param setup_fn Function to call on test setup.
 * @param fn_to_test Function to test.
 * @param control_fn Control function for to the test function.
 */
template <typename IntType, typename Arg, typename R>
void uhr(
    const std::string &filePath,
    const uint64_t runs,
    const uint64_t lower,
    const uint64_t upper,
    const uint64_t step,
    const int64_t generator_min,
    const int64_t generator_max,
    const std::function<Arg(uint64_t n, int_generator<IntType> &generator)> &setup_fn,
    const std::function<R(const Arg &)> &fn_to_test,
    const std::function<R(const Arg &)> &control_fn
) {
    validate_input(runs, lower, upper, step);

    const uint64_t total_runs_additive = runs * ((upper - lower) / step + 1);
    std::vector<double> times(runs);
    std::vector<double> q;
    std::chrono::duration<double, std::nano> elapsed_time{};

    // Set up random number generation
    // ReSharper disable once CppTooWideScope
    int_generator<IntType> generator(generator_min, generator_max);

    // File to write time data
    std::ofstream time_data(filePath);
    time_data << "n,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4\n";

    // Begin testing
    const std::string test_name = std::filesystem::path(filePath).stem().string();
    std::cout << "Running " << test_name << " tests...\n\n";
    uint64_t executed_runs = 0;

    for (uint64_t n = lower; n <= upper; n += step) {
        double mean_time = 0;
        double time_stdev = 0;

        // Test configuration goes here
        const auto &arg = setup_fn(n, generator);

        // Run to compute elapsed time
        for (size_t i = 0; i < runs; i++) {
            // Remember to change total depending on step type
            display_progress(++executed_runs, total_runs_additive);

            auto begin_time = std::chrono::high_resolution_clock::now();
            // Function to test goes here
            const R &result = fn_to_test(arg);
            auto end_time = std::chrono::high_resolution_clock::now();

            if (i == 0) {
                const R &expected = control_fn(arg);
                if (result != expected) {
                    std::cerr << "Test failed! Expected " << expected << " but got " << result << '\n';
                }
            }

            elapsed_time = end_time - begin_time;
            times[i] = elapsed_time.count();

            mean_time += times[i];
        }

        // Compute statistics
        mean_time /= runs;

        for (size_t i = 0; i < runs; i++) {
            const double dev = times[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= runs - 1; // Subtract 1 to get unbiased estimator
        time_stdev = std::sqrt(time_stdev);

        quartiles(times, q);

        time_data << n << ',' << mean_time << ',' << time_stdev << ',';
        time_data << q[0] << ',' << q[1] << ',' << q[2] << ',' << q[3] << ',' << q[4] << '\n';
    }

    // This is to keep loading bar after testing
    std::cout << "\n\n" << test_name << " done!" << std::endl;

    time_data.close();
}
