#pragma once

namespace pi_demo {

// Return zero for an invalid positive integer or invalid command line.
long long parse_n(const char* text);
long long read_n(int argc, char* argv[]);

// Sum rectangles rank, rank + size, rank + 2 * size, ... for positive n.
double partial_pi(long long n, int rank, int size);

void print_result(const char* version, long long n, int ranks,
                  double result, double seconds = -1.0, int group = -1);

} // namespace pi_demo
