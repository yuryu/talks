// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>

#include <benchmark/benchmark.h>

namespace {
constexpr long ITER = 1000000;
constexpr long SEED = 12345678;
constexpr long STRLEN = 20;
constexpr long MAX_INDEX = 1152921504606846975; // 2 ** 60 - 1
using value_t = std::deque<long>;
using value_ptr_t = std::unique_ptr<value_t>;
using hash_t = std::unordered_map<long, value_ptr_t>;

void do_hash_bench(benchmark::State& state) {
  std::uniform_int_distribution<long> random(0, MAX_INDEX);
  std::mt19937_64 mtgen;
  hash_t h;

  for (auto _ : state) {
    mtgen.seed(SEED);
    for (long i = 0; i < ITER; ++i) {
      const auto key = random(mtgen);
      const auto value = random(mtgen);
      auto p = h.try_emplace(key,
                              std::make_unique<value_t>(std::initializer_list<long>{value}));
      if (! p.second)
        p.first->second->push_back(value);
    }
    // verify
    mtgen.seed(SEED);
    for (long i = 0; i < ITER; ++i) {
      const auto key = random(mtgen);
      const auto value = random(mtgen);
      const auto& p = h.find(key);
      if (p->second->front() != value) {
        std::cerr << "Mismatch: key = [" << key << "], value = [" << p->second->front()
            << "] (expected: [" << value << "]" << std::endl;
        return;
      }
      p->second->pop_front();
    }
  }
}

} // namespace

BENCHMARK(do_hash_bench)->Unit(benchmark::kMillisecond);
BENCHMARK_MAIN();

