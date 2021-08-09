
all:
	g++ bench.cpp -Ofast -std=c++11 -g -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread -o bench