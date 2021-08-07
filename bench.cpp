#include <benchmark/benchmark.h>
#include <cstring>	
#include <fstream>

#define BUFF_SIZE 1024


static void BM_memcpy(benchmark::State& state) {
	uint8_t * src = new uint8_t[BUFF_SIZE];
	memset(src, 17, BUFF_SIZE);
	uint8_t * buffer = new uint8_t[BUFF_SIZE];

  for (auto _ : state)
    memcpy(buffer, src, BUFF_SIZE);

  delete[] src;
  delete[] buffer;
}
// Register the function as a benchmark
BENCHMARK(BM_memcpy);

// Define another benchmark
static void BM_IfstreamGetline(benchmark::State& state) {
  std::string filename = "data/ecoli_counts.txt";
	std::ifstream infile(filename);

  std::string line;
  for (auto _ : state)
  	std::getline(infile, line);

  infile.close();
}
BENCHMARK(BM_IfstreamGetline);

BENCHMARK_MAIN();
