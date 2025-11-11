#include <benchmark/benchmark.h>

#include <vector>
#include "../core/array.hpp"

// Benchmark push_back
static void BM_Array_Push(benchmark::State& state) {
    for (auto _ : state) {
        Array<int> arr;
        for (int i = 0; i < state.range(0); ++i)
            arr.Push(i);
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_Array_Push)->Range(1<<10, 1<<20);

static void BM_Vector_Push(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec;
        for (int i = 0; i < state.range(0); ++i)
            vec.push_back(i);
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_Vector_Push)->Range(1<<10, 1<<20);

// Benchmark with reserve
static void BM_Array_PushWithReserve(benchmark::State& state) {
    for (auto _ : state) {
        Array<int> arr;
        arr.Reserve(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
            arr.Push(i);
        benchmark::DoNotOptimize(arr);
    }
}
BENCHMARK(BM_Array_PushWithReserve)->Range(1<<10, 1<<20);

static void BM_Vector_PushWithReserve(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec;
        vec.reserve(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
            vec.push_back(i);
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_Vector_PushWithReserve)->Range(1<<10, 1<<20);

// Benchmark iteration
static void BM_Array_Iterate(benchmark::State& state) {
    Array<int> arr(state.range(0), 42);
    for (auto _ : state) {
        long long sum = 0;
        for (size_t i = 0; i < arr.Length(); ++i)
            sum += arr[i];
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Array_Iterate)->Range(1<<10, 1<<20);

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
