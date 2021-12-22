.PHONY: all

all: purecxx

purecxx: purecxx.cc
	g++ -O2 -std=c++17 -Wall -pthread -o $@ $< -lbenchmark

clean:
	rm -f purecxx
