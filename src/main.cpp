#include <array>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "create_tracer.hpp"

namespace {
std::vector<unsigned> create_thread_counts() {
  auto hw_thds = std::thread::hardware_concurrency();
  std::cout << "std::thread::hardware_concurrency: " << hw_thds << std::endl;
  if (hw_thds == 0) {
    hw_thds = 4;
    std::cout
      << "std::thread::hardware_concurrency returned 0, defaulting to 4."
      << std::endl;
  }
  std::vector<unsigned> v(hw_thds * 10, 0);
  std::iota(v.begin(), v.end(), 1);
  return v;
}

template <int Rounds, class T>
std::chrono::microseconds profile(T lambda) {
  std::array<std::chrono::microseconds, Rounds> durations = {};

  for (int i = 0; i < Rounds; ++i) {
    const auto begin = std::chrono::steady_clock::now();
    lambda();
    const auto end = std::chrono::steady_clock::now();
    const auto diff
      = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    durations[i] = diff;
  }

  const auto sum = std::accumulate(durations.begin(), durations.end(),
                                   std::chrono::microseconds(0));
  const auto avg = sum / Rounds;
  return avg;
}
} // namespace

int main() {
  auto tracer = create_tracer("config.yml", "profile_tracer_service");
  const auto thread_counts = create_thread_counts();
  constexpr auto rounds = 10000;

  for (const auto current_thread_count : thread_counts) {
    std::vector<std::thread> thds;
    for (unsigned i = 0; i < current_thread_count; ++i) {
      thds.emplace_back([&tracer, i] {
        for (int cur_round = 0; cur_round < rounds; ++cur_round) {
          tracer->StartSpan(std::to_string(i));
        }
      });
    }

    const auto avg = profile<rounds>([&tracer, current_thread_count] {
      tracer->StartSpan("cnt" + std::to_string(current_thread_count));
    });

    for (auto& thd : thds) {
      thd.join();
    }

    std::cout << "Average time taken to create trace span: " << avg.count()
              << "µs\n"
              << "Current thread count                   : "
              << current_thread_count << '\n'
              << "Rounds                                 : " << rounds
              << std::endl;
  }
}
