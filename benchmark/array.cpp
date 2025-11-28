#include <benchmark/benchmark.h>

#include <vector>
#include "../core/arraylist.hpp"

// Benchmark push_back
static void BM_ArrayList_push(benchmark::State& state) {
    for (auto _ : state) {
        ArrayList<int> arr;
        for (int i = 0; i < state.range(0); ++i)
            arr.push(i);
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_ArrayList_push)->Range(1<<10, 1<<20);

static void BM_Vector_push(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec;
        for (int i = 0; i < state.range(0); ++i)
            vec.push_back(i);
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_Vector_push)->Range(1<<10, 1<<20);

// Benchmark with reserve
static void BM_ArrayList_pushWithreserve(benchmark::State& state) {
    for (auto _ : state) {
        ArrayList<int> arr;
        arr.reserve(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
            arr.push(i);
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_ArrayList_pushWithreserve)->Range(1<<10, 1<<20);

static void BM_Vector_pushWithreserve(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec;
        vec.reserve(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
            vec.push_back(i);
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_Vector_pushWithreserve)->Range(1<<10, 1<<20);

// Benchmark iteration
static void BM_ArrayList_Iterate(benchmark::State& state) {
    ArrayList<int> arr = 
      ArrayList<int>::with_capacity_initialize(state.range(0), 42).abort_on_error();
    for (auto _ : state) {
        long long sum = 0;
        for (size_t i = 0; i < arr.length(); ++i)
            sum += arr[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_ArrayList_Iterate)->Range(1<<10, 1<<20);

static void BM_Vector_Iterate(benchmark::State& state) {
    std::vector<int> vec(state.range(0), 42);
    for (auto _ : state) {
        long long sum = 0;
        for (size_t i = 0; i < vec.size(); ++i)
            sum += vec[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Vector_Iterate)->Range(1<<10, 1<<20);

BENCHMARK_MAIN();
