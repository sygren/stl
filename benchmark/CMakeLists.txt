cmake_minimum_required(VERSION 3.31)
project(KaosCoreBenchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

find_package(benchmark QUIET)
if (NOT benchmark_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4  
  )
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(benchmark)
endif()

file(GLOB BENCHMARK_SRC "*.cpp")

foreach(benchmark_file ${BENCHMARK_SRC})
  get_filename_component(benchmark_name ${benchmark_file} NAME_WE)
  add_executable(${benchmark_name} ${benchmark_file})
  target_link_libraries(${benchmark_name} PRIVATE benchmark::benchmark)
endforeach()

