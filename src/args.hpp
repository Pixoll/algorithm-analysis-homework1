#pragma once

#include <array>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>

struct ArgValidationResult {
    const bool is_valid;
    const char *const message;
};

struct Arg {
    const char *name = "";
    int64_t value = 0;
    int64_t min = std::numeric_limits<int64_t>::min();
    int64_t max = std::numeric_limits<int64_t>::max();

    std::function<ArgValidationResult()> validator = []() -> ArgValidationResult {
        return {true, ""};
    };

    auto operator<=>(const Arg &other) const {
        return value <=> other.value;
    }
};

struct Args {
    Arg runs{
        .name = "-r",
        .value = 32,
        .min = 32,
    };
    Arg lower{
        .name = "-l",
        .value = 8,
        .min = 1,
    };
    Arg upper{
        .name = "-u",
        .value = 512,
        .min = 1,
        .validator = [this]() -> ArgValidationResult {
            return {upper > lower, "-u must be > -l"};
        },
    };
    Arg step{
        .name = "-s",
        .value = 1,
        .min = 1,
    };
    Arg gen_min{
        .name = "-gmin",
        .value = 0,
    };
    Arg gen_max{
        .name = "-gmax",
        .value = 99,
        .validator = [this]() -> ArgValidationResult {
            return {gen_max > gen_min, "-gmax must be > -gmin"};
        },
    };

private:
    typedef std::array<Arg *, 6> args_array_t;
    args_array_t m_args{&runs, &lower, &upper, &step, &gen_min, &gen_max};

public:
    args_array_t::iterator begin() {
        return m_args.begin();
    }

    args_array_t::iterator end() {
        return m_args.end();
    }

    [[nodiscard]] consteval size_t size() const {
        return m_args.size();
    }
};

struct ParsedArgs {
    int64_t runs;
    int64_t lower;
    int64_t upper;
    int64_t step;
    int64_t gen_min;
    int64_t gen_max;

    explicit ParsedArgs(const Args &args)
        : runs(args.runs.value),
          lower(args.lower.value),
          upper(args.upper.value),
          step(args.step.value),
          gen_min(args.gen_min.value),
          gen_max(args.gen_max.value) {
    }
};

inline auto usage_string = "Usage: algorithm_analysis_homework1 -r <int64:runs> -l <int64:lower> -u <int64:upper> "
        "-s <int64:step> -gmin <int64:gen_min> -gmax <int64:gen_max>";

inline ParsedArgs parse_args(const int argc, const char *const *const argv) {
    using std::cerr, std::endl, std::strcmp, std::exit;

    Args args;

    for (auto *arg_ptr: args) {
        auto &[name, value, min, max, validator] = *arg_ptr;

        int index = -1;
        bool found = false, is_valid = false;

        for (int _ = 0; _ < args.size(); ++_) {
            index += 2;

            if (index >= argc - 1)
                break;

            if (strcmp(name, argv[index]) == 0) {
                found = true;
                is_valid = index + 1 < argc;
                break;
            }
        }

        if (!found)
            continue;

        if (!is_valid) {
            cerr << usage_string << endl;
            exit(EXIT_FAILURE);
        }

        char *end;
        const char *string = argv[index + 1];
        const int64_t parsed_value = std::strtoll(string, &end, 10);

        if (string == end) {
            cerr << usage_string << endl;
            exit(EXIT_FAILURE);
        }

        value = parsed_value;

        if (value < min) {
            cerr << name << " must be >= " << min << ".\n"
                    << usage_string << endl;
            exit(EXIT_FAILURE);
        }

        if (value > max) {
            cerr << name << " must be <= " << max << ".\n"
                    << usage_string << endl;
            exit(EXIT_FAILURE);
        }
    }

    for (const auto *arg: args) {
        const auto &[is_valid, message] = arg->validator();

        if (!is_valid) {
            cerr << message << "\n" << usage_string << endl;
            exit(EXIT_FAILURE);
        }
    }

    return ParsedArgs(args);
}
