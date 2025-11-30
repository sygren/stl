#include <benchmark/benchmark.h>

#include <forward_list>
#include "../core/forward_list.hpp" // Your ForwardList header

// Benchmark push operations
static void BM_ForwardList_Push(benchmark::State& state) {
  for (auto _ : state) {
    ForwardList<int> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push(i);
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_StdForwardList_PushBack(benchmark::State& state) {
  for (auto _ : state) {
    std::forward_list<int> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push_front(i);
    }
    list.reverse(); // std::forward_list doesn't have push_back
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

// Benchmark push_front operations
static void BM_ForwardList_PushFront(benchmark::State& state) {
  for (auto _ : state) {
    ForwardList<int> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push_front(i);
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_StdForwardList_PushFront(benchmark::State& state) {
  for (auto _ : state) {
    std::forward_list<int> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push_front(i);
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

// Benchmark random access (get operations)
static void BM_ForwardList_RandomAccess(benchmark::State& state) {
  ForwardList<int> list;
  for (int i = 0; i < state.range(0); ++i) {
    list.push(i);
  }
  
  for (auto _ : state) {
    int sum = 0;
    for (int i = 0; i < state.range(0); ++i) {
      sum += list.get(i);
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_StdForwardList_RandomAccess(benchmark::State& state) {
  std::forward_list<int> list;
  for (int i = 0; i < state.range(0); ++i) {
    list.push_front(i);
  }
  list.reverse();
  
  for (auto _ : state) {
    int sum = 0;
    auto it = list.begin();
    for (int i = 0; i < state.range(0); ++i) {
      sum += *it;
      ++it;
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetComplexityN(state.range(0));
}

// Benchmark sequential iteration
static void BM_ForwardList_Sequential(benchmark::State& state) {
  ForwardList<int> list;
  for (int i = 0; i < state.range(0); ++i) {
    list.push(i);
  }
  
  for (auto _ : state) {
    int sum = 0;
    for (size_t i = 0; i < list.length(); ++i) {
      sum += list[i];
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_StdForwardList_Sequential(benchmark::State& state) {
  std::forward_list<int> list;
  for (int i = 0; i < state.range(0); ++i) {
    list.push_front(i);
  }
  list.reverse();
  
  for (auto _ : state) {
    int sum = 0;
    for (const auto& val : list) {
      sum += val;
    }
    benchmark::DoNotOptimize(sum);
  }
  state.SetComplexityN(state.range(0));
}

// Benchmark clear operation
static void BM_ForwardList_Clear(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    ForwardList<int> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push(i);
    }
    state.ResumeTiming();
    
    list.clear();
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_StdForwardList_Clear(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();
    std::forward_list<int> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push_front(i);
    }
    state.ResumeTiming();
    
    list.clear();
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

// Benchmark with larger objects
struct LargeObject {
  int data[64]; // 256 bytes
  LargeObject() { data[0] = 0; }
  LargeObject(int val) { data[0] = val; }
};

static void BM_ForwardList_LargeObjects(benchmark::State& state) {
  for (auto _ : state) {
    ForwardList<LargeObject> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push(LargeObject(i));
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

static void BM_StdForwardList_LargeObjects(benchmark::State& state) {
  for (auto _ : state) {
    std::forward_list<LargeObject> list;
    for (int i = 0; i < state.range(0); ++i) {
      list.push_front(LargeObject(i));
    }
    benchmark::DoNotOptimize(list);
  }
  state.SetComplexityN(state.range(0));
}

// Register benchmarks with different sizes
BENCHMARK(BM_ForwardList_Push)->Range(8, 8<<10)->Complexity();
BENCHMARK(BM_StdForwardList_PushBack)->Range(8, 8<<10)->Complexity();

BENCHMARK(BM_ForwardList_PushFront)->Range(8, 8<<10)->Complexity();
BENCHMARK(BM_StdForwardList_PushFront)->Range(8, 8<<10)->Complexity();

BENCHMARK(BM_ForwardList_RandomAccess)->Range(8, 8<<10)->Complexity();
BENCHMARK(BM_StdForwardList_RandomAccess)->Range(8, 8<<10)->Complexity();

BENCHMARK(BM_ForwardList_Sequential)->Range(8, 8<<10)->Complexity();
BENCHMARK(BM_StdForwardList_Sequential)->Range(8, 8<<10)->Complexity();

BENCHMARK(BM_ForwardList_Clear)->Range(8, 8<<10)->Complexity();
BENCHMARK(BM_StdForwardList_Clear)->Range(8, 8<<10)->Complexity();

BENCHMARK(BM_ForwardList_LargeObjects)->Range(8, 1<<10)->Complexity();
BENCHMARK(BM_StdForwardList_LargeObjects)->Range(8, 1<<10)->Complexity();

BENCHMARK_MAIN();
