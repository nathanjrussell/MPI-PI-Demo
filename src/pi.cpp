#include "pi.hpp"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace pi_demo {

long long parse_n(const char* text) {
    if (text[0] < '0' || text[0] > '9') {
        return 0;
    }
    char* end;
    errno = 0;
    const long long n = std::strtoll(text, &end, 10);
    return errno == 0 && *end == '\0' && n > 0 ? n : 0;
}

long long read_n(int argc, char* argv[]) {
    const long long n = argc == 2 ? parse_n(argv[1]) : 0;
    if (n == 0) {
        std::fprintf(stderr, "Usage: %s N\nN must be a positive integer.\n", argv[0]);
    }
    return n;
}

double partial_pi(long long n, int rank, int size) {
    double sum = 0.0;
    for (long long i = rank; i < n; i += size) {
        const double x = (static_cast<double>(i) + 0.5) / n;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum / n;
}

void print_result(const char* version, long long n, int ranks,
                  double result, double seconds, int group) {
    std::printf("%s N=%lld ranks=%d pi=%.15f error=%.3e",
                version, n, ranks, result, std::abs(result - std::acos(-1.0)));
    if (seconds >= 0.0) {
        std::printf(" seconds=%.6f", seconds);
    }
    if (group >= 0) {
        std::printf(" group=%d", group);
    }
    std::printf("\n");
}

} // namespace pi_demo
