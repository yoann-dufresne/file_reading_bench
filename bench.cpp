#include <benchmark/benchmark.h>

#include <cstring>	
#include <fstream>
#include <istream>
#include <iostream>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


#define BUFF_SIZE 1024


using namespace std;


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


static uint do_stuff(const char * txt, long size) {
  uint sum = 0;
  // std::cout << size << std::endl;
  for (uint i=0 ; i<size ; i++) {
    sum += (uint)txt[i];
    // cout << txt[i];
  }
  // cout << endl;

  return sum/10;
}

static uint do_stuff_mmap(char * & txt) {
  uint sum = 0;
  // std::cout << size << std::endl;
  do {
    sum += (uint)*txt;
    // cout << *txt;
  } while (*(++txt) != '\n');
  txt += 1;
  // cout << endl;

  return sum/10;
}

// Define another benchmark
static void BM_IfstreamGetline(benchmark::State& state) {
  std::string filename = "data/ecoli_counts.txt";
	std::ifstream infile(filename);

  uint global_sum = 0;

  std::string line;
  for (auto _ : state) {
  	std::getline(infile, line);
    uint sum = do_stuff(line.c_str(), line.length());
    global_sum += sum;
  }
  std::cout << "Global sum " << global_sum << std::endl;

  infile.close();
}
BENCHMARK(BM_IfstreamGetline);


// Define another benchmark
static void BM_IfstreamGetline_mmap(benchmark::State& state) {
  // Open file
  std::string filename = "data/ecoli_counts.txt";
  // Get file size
  std::ifstream fp(filename, std::ifstream::ate | std::ifstream::binary);
  std::ifstream::pos_type size = fp.tellg();
  fp.close();
  // std::cout << "size " << size << std::endl;
  // exit(0);

  // mmap file
  int fd = open(filename.c_str(), O_RDONLY);
  char *  addr = reinterpret_cast<char *>(mmap(NULL, size, PROT_READ, MAP_FILE | MAP_PRIVATE | MAP_POPULATE , fd, 0));
  if (addr == MAP_FAILED) {
    std::cout<<"Data can't be mapped???"<<std::endl;
    exit(1);
  }
  madvise(addr,size,MADV_SEQUENTIAL|MADV_WILLNEED);

  uint global_sum = 0;

  // Read lines
  std::string line;
  char * pointer = addr;
  for (auto _ : state) {
    // Do stuff
    uint sum = do_stuff_mmap(pointer);
    global_sum += sum;
  }
  std::cout << "Global sum " << global_sum << std::endl;

  // Close everything
  munmap(addr, size);
  close(fd);
}
BENCHMARK(BM_IfstreamGetline_mmap);

BENCHMARK_MAIN();
